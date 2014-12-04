#include "Model.h"

#include "mage\Effect.h"
#include "ShaderCompileException.h"
#include "ModelLoadException.h"
#include "Vertex.h"
#include "Resources.h"
#include <string.h>
#include <string>

using namespace games;
using namespace mario;
using namespace mage;
using namespace std;

struct Model::private_implementation {
	private_implementation(Model* self) {
		this->self = self;
	}

	~private_implementation() {
	}

	Model* self;
	shared_ptr<Effect> shader;
	shared_ptr<Mesh> mesh;

	void setShader(IDirect3DDevice9* device, const string& file)
	{
		shader = Resources::getEffect(device, file);
	}

	void setMesh(IDirect3DDevice9* device, const string& file)
	{
		mesh = Resources::getMesh(device, file);
	}

	void draw(IDirect3DDevice9* device, Scene* scene, Camera* camera) {
		shader->setTechnique("PhongTech");

		shader->setMatrix("gWorld", ((GameObject*)self)->world());

		for (unsigned int j = 0; j < mesh->materials.size(); j++) {
			//Se tiver a textura, usa. Caso contrário usa a default
			auto texture = mesh->textures[j];
			if (!texture)
				continue;

			// Valores da Cena
			shader->setVector("gLightDir", scene->lightDir);
			shader->setVector("gAmbientColor", scene->ambientColor);
			shader->setVector("gDiffuseColor", scene->diffuseColor);
			shader->setVector("gSpecularColor", scene->specularColor);

			// Valores da Câmera
			shader->setMatrix("gView", camera->view);
			shader->setVector("gCameraPos", camera->worldPosition());
			shader->setMatrix("gProjection", camera->projection);

			//Ajusta as propriedades do material
			shader->setColor("gAmbientMaterial", mesh->materials[j].Ambient);
			shader->setColor("gDiffuseMaterial", mesh->materials[j].Diffuse);
			shader->setColor("gSpecularMaterial", mesh->materials[j].Specular);
			shader->setFloat("gSpecularPower", mesh->materials[j].Power);

			shader->setTexture("gTexture", texture.get());
			shader->commit();
			shader->execute([this, j](LPDIRECT3DDEVICE9 device)
			{
				mesh->data->DrawSubset(j);
			});
			texture = nullptr;
		}
	}
};

Model::Model() : pImpl(new Model::private_implementation(this))
{
}

Model::~Model()
{
}

void Model::draw(IDirect3DDevice9* device, Scene* scene, Camera* camera)
{
	pImpl->draw(device, scene, camera);
}

void Model::setShader(IDirect3DDevice9* device, const string& file)
{
	pImpl->setShader(device, file);
}

void Model::setMesh(IDirect3DDevice9* device, const string& file)
{
	pImpl->setMesh(device, file);
}

D3DXMATRIX mario::Model::world()
{
	return GameObject::world();
}

D3DXVECTOR3 mario::Model::worldPosition()
{
	return GameObject::worldPosition();
}
