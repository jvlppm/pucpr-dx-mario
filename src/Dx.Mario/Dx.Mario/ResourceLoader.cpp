#include "ResourceLoader.h"
#include <d3dx9.h>
#include <memory>
#include "Vertex.h"
#include "ModelLoadException.h"
#include "mage/HandleError.h"
#include "ShaderCompileException.h"
#include "TextureShader.h"
#include "PostEffects.h"

using namespace mario;
using namespace games;
using namespace mage;
using namespace std;

shared_ptr<Effect> ResourceLoader::loadEffect(IDirect3DDevice9* device, const std::string& name)
{
	string fileName = "Resources/" + name;
	wstring wideFileName(fileName.length(), L' ');
	copy(fileName.begin(), fileName.end(), wideFileName.begin());

	auto effect = make_shared<Effect>(wideFileName);
	string error = effect->compile(device);
	if (!error.empty())
		throw ShaderCompileException("Failed to compile shader file.\n" + error);

	return effect;
}

shared_ptr<IDirect3DTexture9> ResourceLoader::loadTexture(IDirect3DDevice9* device, const string& name)
{
	IDirect3DTexture9* tex = nullptr;
	string fileName = "Resources/" + name;

	auto error = D3DXCreateTextureFromFileA(device, fileName.c_str(), &tex);
	if (error)
		throw exception((static_cast<string>("Error loading texture \"") + name + "\": " + to_string(error)).c_str());
	return shared_ptr<IDirect3DTexture9>(tex,  [](IDirect3DTexture9* tex) {
		tex->Release();
	});
}

shared_ptr<Mesh> ResourceLoader::loadMesh(IDirect3DDevice9* device, const string& name)
{
	auto mesh = make_shared<Mesh>();

	string fileName = "Resources/" + name;
	wstring wideFileName(fileName.length(), L' ');
	copy(fileName.begin(), fileName.end(), wideFileName.begin());

	// Passo 1: Carregar o .x na memória do sistema
	ID3DXMesh* meshSys = nullptr;
	ID3DXBuffer* adjBuffer = nullptr;
	ID3DXBuffer* mtrlBuffer = nullptr;
	DWORD numMaterials;

    if (int loadError = D3DXLoadMeshFromX(wideFileName.c_str(), D3DXMESH_SYSTEMMEM,
        device, &adjBuffer, &mtrlBuffer, 0, &numMaterials, &meshSys)) {
        HR(loadError);
        throw ModelLoadException("Failed to load mesh from X.", loadError);
    }

	//Passo 2: Descobrir se a mesh tem normais de luz
	D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
	bool hasNormals = false;
	if (meshSys->GetDeclaration(elems) == S_OK) {
		for (int i = 0; i < MAX_FVF_DECL_SIZE; ++i)
		{
			//Se atingirmos o elemento D3DDECL_END() para a iteração
			if (elems[i].Stream == 0xFF)
				break;

			if (elems[i].Type == D3DDECLTYPE_FLOAT3 &&
				elems[i].Usage == D3DDECLUSAGE_NORMAL &&
				elems[i].UsageIndex == 0) {
				hasNormals = true;
				break;
			}
		}
	}

	//Passo 3: Alterar o formato da mesh para o do nosso vertice
	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	Vertex::getDeclaration(device)->GetDeclaration(elements, &numElements);

	ID3DXMesh* temp = nullptr;
	if (meshSys->CloneMesh(D3DXMESH_SYSTEMMEM, elements, device, &temp) == S_OK) {
		meshSys->Release();
		meshSys = temp;

		//Passo 4: Se não tiver normals, gera.
		if (!hasNormals)
			HR(D3DXComputeNormals(meshSys, 0));

		//Passo 5: Otimização da mesh
		ID3DXMesh* meshOut;
		if (meshSys->Optimize(D3DXMESH_MANAGED |
			D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT |
			D3DXMESHOPT_VERTEXCACHE,
			(DWORD*)adjBuffer->GetBufferPointer(),
			0, 0, 0, &meshOut) == S_OK) {
			meshSys->Release();
			meshSys = meshOut;
		}
	}
	adjBuffer->Release();

	mesh->data = shared_ptr<ID3DXMesh>(meshSys, [](ID3DXMesh* m){ m->Release(); });

	//Passo 6: Extrair os materiais e carregar as texturas
	if (mtrlBuffer != nullptr && numMaterials > 0)
	{
		D3DXMATERIAL* d3dxmtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
		for (DWORD i = 0; i < numMaterials; ++i)
		{
			//Salva o i-esimo material. E define o componente ambiente para
			//ser igual ao difuso
			d3dxmtrls[i].MatD3D.Ambient = d3dxmtrls[i].MatD3D.Diffuse;
			mesh->materials.push_back(d3dxmtrls[i].MatD3D);

			//Verifica se há uma textura associada na face
			if (d3dxmtrls[i].pTextureFilename != nullptr)
			{
				char* texFN = d3dxmtrls[i].pTextureFilename;
				auto tex = Resources::getTexture(device, texFN);
				mesh->textures.push_back(tex);
			}
			else
			{
				auto tex = Resources::getTexture(device, "whitetex.dds");
				mesh->textures.push_back(tex);
			}
		}
	}
	return mesh;
}

std::shared_ptr<games::IShader> mario::ResourceLoader::loadShader(IDirect3DDevice9 * device, const std::string & name)
{
    if (name == "texture.fx") {
        return make_shared<TextureShader>();
    }
    if (name == "postEffects.fx") {
        return make_shared<PostEffects>();
    }
    throw logic_error("Unknown shader " + name);
}
