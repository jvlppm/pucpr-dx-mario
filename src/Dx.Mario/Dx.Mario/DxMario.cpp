#include "DxMario.h"
#include "mage/TString.h"
#include <memory>
#include "Resources.h"
#include "ResourceLoader.h"
#include "Model.h"
#include "TextureShader.h"

using namespace mario;
using namespace games;
using namespace mage;
using namespace std;

struct DxMario::private_implementation {
	std::shared_ptr<Scene> scene;

	void setup(IDirect3DDevice9* device)
	{
		Resources::setLoader(make_shared<ResourceLoader>());

		scene = std::make_shared<Scene>();
		scene->diffuseColor = D3DXVECTOR3(1.0f, 1.0f, 0.8f);
		scene->ambientColor = D3DXVECTOR3(0.4f, 0.4f, 0.4f);

		auto camera = scene->add<Camera>();
		camera->translate(0.0f, 0.0f, -20.0f);
		camera->setPerspective(60, 1, 5000);
		camera->lookAt(D3DXVECTOR3(0, 0, 0));

		scene->add<Model>(device, "skullocc.x", "texture.fx");
		scene->add<Model>(device, "Dwarf.x", "texture.fx")
			->scale(8)
			->translate(1.0f, -1.0f, 0.0f);

		scene->init();
	}
};

DxMario::DxMario() : pImpl(new DxMario::private_implementation())
{

}

void DxMario::processEvent(const mage::WindowsEvent& evt)
{
}

void DxMario::shutDown(IDirect3DDevice9* device)
{

}

void DxMario::paint(IDirect3DDevice9* device)
{
	pImpl->scene->draw(device);
}

bool DxMario::process(float time)
{
	pImpl->scene->update(time);
	return true;
}

void DxMario::setup(IDirect3DDevice9* device)
{
	pImpl->setup(device);
}
