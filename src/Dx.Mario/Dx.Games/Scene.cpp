#include "Scene.h"

#include <d3dx9math.h>
#include <vector>
#include "Camera.h"
#include "Utils.h"
#include "IDrawable.h"
#include "cpplinq.hpp"

using namespace games;

struct Scene::private_implementation {
	private_implementation() :
		cameras(std::make_shared<std::vector<std::shared_ptr<Camera>>>()),
		drawables(std::make_shared<std::vector<std::shared_ptr<IDrawable>>>())
	{

	}

	std::shared_ptr<std::vector<std::shared_ptr<Camera>>> cameras;
	std::shared_ptr<std::vector<std::shared_ptr<IDrawable>>> drawables;
};

Scene::Scene() : pImpl(new Scene::private_implementation())
{
}

Scene::~Scene()
{
}

void Scene::draw(IDirect3DDevice9* device)
{
	using namespace cpplinq;

	if (auto cameras = pImpl->cameras)
	{
		for (auto camera : *cameras)
		{
			device->SetViewport(&camera->viewport);
			//auto vp = camera->view * camera->projection;
			if (auto drawables = pImpl->drawables)
			{
				auto sorted_items = from_iterators(drawables->begin(), drawables->end())
					>> orderby_descending([camera](std::shared_ptr<IDrawable> i) {
						auto dist = camera->globalPosition() - i->globalPosition();
						return D3DXVec3LengthSq(&dist);
					})
					>> to_list();

				for (auto drawable : sorted_items)
					drawable->draw(device);
			}
		}
	}
}

void games::Scene::registerItem(std::shared_ptr<GameObject> obj)
{
	if (auto cam = std::dynamic_pointer_cast<Camera>(obj))
		pImpl->cameras = Utils::add(pImpl->cameras, cam);
	if (auto drawable = std::dynamic_pointer_cast<IDrawable>(obj))
		pImpl->drawables = Utils::add(pImpl->drawables, drawable);
}

void games::Scene::unregisterItem(std::shared_ptr<GameObject> obj)
{
	if (auto cam = std::dynamic_pointer_cast<Camera>(obj))
		pImpl->cameras = Utils::remove(pImpl->cameras, cam);
}
