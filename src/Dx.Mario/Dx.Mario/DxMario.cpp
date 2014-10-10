#include "DxMario.h"
#include "Scene.h"
#include "Camera.h"

using namespace mario;
using namespace games;

struct DxMario::private_implementation {
	std::shared_ptr<Scene> scene;
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
	return true;
}

void DxMario::setup(IDirect3DDevice9* device)
{
	pImpl->scene = std::make_shared<Scene>();
	pImpl->scene->add(std::make_shared<Camera>());
	pImpl->scene->init();
}
