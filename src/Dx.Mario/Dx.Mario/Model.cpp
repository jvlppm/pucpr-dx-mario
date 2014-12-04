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
	shared_ptr<Effect> effect;
	shared_ptr<Mesh> mesh;

	void setEffect(IDirect3DDevice9* device, const string& file)
	{
		effect = Resources::getEffect(device, file);
	}

	void setMesh(IDirect3DDevice9* device, const string& file)
	{
		mesh = Resources::getMesh(device, file);
	}

	void draw(IDirect3DDevice9* device, Scene* scene, Camera* camera) {
		effect->setTechnique("PhongTech");

		effect->setMatrix("gWorld", ((GameObject*)self)->world());

		for (unsigned int j = 0; j < mesh->materials.size(); j++) {
			//Se tiver a textura, usa. Caso contrário usa a default
			auto texture = mesh->textures[j];
			if (!texture)
				continue;

			// Valores da Cena
			effect->setVector("gLightDir", scene->lightDir);
			effect->setVector("gAmbientColor", scene->ambientColor);
			effect->setVector("gDiffuseColor", scene->diffuseColor);
			effect->setVector("gSpecularColor", scene->specularColor);

			// Valores da Câmera
			effect->setMatrix("gView", camera->view);
			effect->setVector("gCameraPos", camera->worldPosition());
			effect->setMatrix("gProjection", camera->projection);

			//Ajusta as propriedades do material
			effect->setColor("gAmbientMaterial", mesh->materials[j].Ambient);
			effect->setColor("gDiffuseMaterial", mesh->materials[j].Diffuse);
			effect->setColor("gSpecularMaterial", mesh->materials[j].Specular);
			effect->setFloat("gSpecularPower", mesh->materials[j].Power);

			effect->setTexture("gTexture", texture.get());
			effect->commit();
			effect->execute([this, j](LPDIRECT3DDEVICE9 device)
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

Model::Model(IDirect3DDevice9* device, const string& mesh, const string& effect) : Model()
{
	setMesh(device, mesh);
	setEffect(device, effect);
}

Model::~Model()
{
}

void Model::draw(IDirect3DDevice9* device, Scene* scene, Camera* camera)
{
	pImpl->draw(device, scene, camera);
}

void Model::setEffect(IDirect3DDevice9* device, const string& file)
{
	pImpl->setEffect(device, file);
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
