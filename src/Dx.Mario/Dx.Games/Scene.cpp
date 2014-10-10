#include "Scene.h"

#include <vector>
#include "Camera.h"
#include "Utils.h"

using namespace games;

struct Scene::private_implementation {
	private_implementation() : cameras(std::make_shared<std::vector<std::shared_ptr<Camera>>>()) {

	}

	std::shared_ptr<std::vector<std::shared_ptr<Camera>>> cameras;
};

Scene::Scene() : pImpl(new Scene::private_implementation())
{
}


Scene::~Scene()
{
}

void Scene::draw(IDirect3DDevice9* device)
{
}

void games::Scene::registerItem(std::shared_ptr<GameObject> obj)
{
	if (auto cam = std::dynamic_pointer_cast<Camera>(obj))
		pImpl->cameras = Utils::add(pImpl->cameras, cam);
}

void games::Scene::unregisterItem(std::shared_ptr<GameObject> obj)
{
	if (auto cam = std::dynamic_pointer_cast<Camera>(obj))
		pImpl->cameras = Utils::remove(pImpl->cameras, cam);
}
