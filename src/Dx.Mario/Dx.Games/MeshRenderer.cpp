#include "MeshRenderer.h"

#include "mage\Effect.h"
#include "ShaderCompileException.h"
#include "ModelLoadException.h"
#include "Vertex.h"

using namespace games;
using namespace mage;
using namespace std;

struct MeshRenderer::private_implementation {
	private_implementation() {
	}

	shared_ptr<Effect> effect;
	unique_ptr<ID3DXMesh> mesh;
	vector<D3DMATERIAL9> materials;
	vector<shared_ptr<IDirect3DTexture9>> textures;

	void setShaderFile(IDirect3DDevice9* device, const TString& file)
	{
		effect = make_shared<Effect>(file);
		string error = effect->compile(device);
		if (!error.empty())
			throw ShaderCompileException("Failed to compile shader file.\n" + error);
	}

	void setModel(IDirect3DDevice9* device, const TString& file)
	{
		if (mesh) {
			mesh->Release();
			mesh = nullptr;
			materials.clear();
			textures.clear();
		}

		// Passo 1: Carregar o .x na memória do sistema
		ID3DXMesh* meshSys = nullptr;
		ID3DXBuffer* adjBuffer = nullptr;
		ID3DXBuffer* mtrlBuffer = nullptr;
		DWORD numMaterials;

		HR(D3DXLoadMeshFromX(file.c_str(), D3DXMESH_SYSTEMMEM,
			device, &adjBuffer, &mtrlBuffer, 0, &numMaterials, &meshSys));

		//Passo 2: Descobrir se a mesh tem normais de luz
		D3DVERTEXELEMENT9 elems[MAX_FVF_DECL_SIZE];
		HR(meshSys->GetDeclaration(elems));

		bool hasNormals = false;
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

		//Passo 3: Alterar o formato da mesh para o do nosso vertice
		D3DVERTEXELEMENT9 elements[64];
		UINT numElements = 0;
		Vertex::getDeclaration(device)->GetDeclaration(elements, &numElements);

		ID3DXMesh* temp = nullptr;
		HR(meshSys->CloneMesh(D3DXMESH_SYSTEMMEM, elements, device, &temp));
		meshSys->Release();
		meshSys = temp;

		//Psso 4: Se não tiver normals, gera.
		if (!hasNormals)
			HR(D3DXComputeNormals(meshSys, 0));

		//Passo 5: Otimização da mesh
		ID3DXMesh* meshOut;
		HR(meshSys->Optimize(D3DXMESH_MANAGED |
			D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT |
			D3DXMESHOPT_VERTEXCACHE,
			(DWORD*)adjBuffer->GetBufferPointer(),
			0, 0, 0, &meshOut));
		meshSys->Release();
		adjBuffer->Release();

		mesh = unique_ptr<ID3DXMesh>(meshOut);

		//Passo 6: Extrair os materiais e carregar as texturas
		if (mtrlBuffer != nullptr && numMaterials > 0)
		{
			D3DXMATERIAL* d3dxmtrls = (D3DXMATERIAL*)mtrlBuffer->GetBufferPointer();
			for (DWORD i = 0; i < numMaterials; ++i)
			{
				//Salva o i-esimo material. E define o componente ambiente para
				//ser igual ao difuso
				d3dxmtrls[i].MatD3D.Ambient = d3dxmtrls[i].MatD3D.Diffuse;
				materials.push_back(d3dxmtrls[i].MatD3D);

				//Verifica se há uma textura associada na face
				if (d3dxmtrls[i].pTextureFilename != nullptr)
				{
					IDirect3DTexture9* tex = nullptr;
					char* texFN = d3dxmtrls[i].pTextureFilename;
					HR(D3DXCreateTextureFromFileA(device, texFN, &tex));
					textures.push_back(shared_ptr<IDirect3DTexture9>(tex));
				}
				else
				{
					textures.push_back(nullptr);
				}
			}
		}
	}
};

MeshRenderer::MeshRenderer() : pImpl(new MeshRenderer::private_implementation())
{
}

MeshRenderer::~MeshRenderer()
{
}

bool MeshRenderer::sortedRendering()
{
	return false;
}

void MeshRenderer::draw(IDirect3DDevice9* device)
{

}

D3DXVECTOR3 MeshRenderer::globalPosition()
{
	throw logic_error("The method or operation is not implemented.");
}

void MeshRenderer::setShaderFile(IDirect3DDevice9* device, TString file)
{
	pImpl->setShaderFile(device, file);
}

void MeshRenderer::setModel(IDirect3DDevice9* device, TString file)
{
	pImpl->setModel(device, file);
}
