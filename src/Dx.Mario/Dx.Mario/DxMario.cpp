#include "DxMario.h"
#include "Scene.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "mage/TString.h"
#include <memory>
#include "Resources.h"
#include "ResourceLoader.h"

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
		auto camera = scene->add<Camera>();
		camera->transform([](D3DXMATRIX& m) { D3DXMatrixTranslation(&m, 1.0f, 0.8f, 20.0f); });
		camera->setPerspective(60, 1, 5000);
		camera->lookAt(D3DXVECTOR3(0, 0.8, 0));

		auto meshRenderer = scene->add<MeshRenderer>();
		meshRenderer->setShaderFile(device, _T("Resources/texture.fx"));
		meshRenderer->setModel(device, _T("Resources/skullocc.x"));
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
