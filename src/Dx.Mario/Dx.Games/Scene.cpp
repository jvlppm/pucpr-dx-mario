#include "Scene.h"

#include <d3dx9math.h>
#include <vector>
#include "Camera.h"
#include "Utils.h"
#include "IDrawable.h"
#include "cpplinq.hpp"
#include "IUpdateable.h"

using namespace games;

struct Scene::private_implementation {
	private_implementation() {
	}

	ImmutableList<std::shared_ptr<Camera>> cameras;
	ImmutableList<std::shared_ptr<IDrawable>> drawables;
	ImmutableList<std::shared_ptr<IUpdateable>> updateables;
};

Scene::Scene() : pImpl(new Scene::private_implementation())
{
}

Scene::~Scene()
{
}

void games::Scene::update(float time)
{
	if (auto updateables = pImpl->updateables)
	{
		for (auto updateable : *updateables)
		{
			updateable->update(time);
		}
	}
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
	if (auto item = std::dynamic_pointer_cast<Camera>(obj))
		pImpl->cameras = pImpl->cameras.add(item);
	if (auto item = std::dynamic_pointer_cast<IDrawable>(obj))
		pImpl->drawables = pImpl->drawables.add(item);
	if (auto item = std::dynamic_pointer_cast<IUpdateable>(obj))
		pImpl->updateables = pImpl->updateables.add(item);
}

void games::Scene::unregisterItem(std::shared_ptr<GameObject> obj)
{
	if (auto item = std::dynamic_pointer_cast<Camera>(obj))
		pImpl->cameras = pImpl->cameras.remove(item);
	if (auto item = std::dynamic_pointer_cast<IDrawable>(obj))
		pImpl->drawables = pImpl->drawables.remove(item);
	if (auto item = std::dynamic_pointer_cast<IUpdateable>(obj))
		pImpl->updateables = pImpl->updateables.remove(item);
}
