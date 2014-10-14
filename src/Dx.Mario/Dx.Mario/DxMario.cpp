#include "DxMario.h"
#include "Scene.h"
#include "Camera.h"
#include "MeshRenderer.h"

using namespace mario;
using namespace games;

struct DxMario::private_implementation {
	std::shared_ptr<Scene> scene;

	void setup(IDirect3DDevice9* device)
	{
		scene = std::make_shared<Scene>();
		scene->add(std::make_shared<Camera>());
		scene->add(std::make_shared<MeshRenderer>());
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
