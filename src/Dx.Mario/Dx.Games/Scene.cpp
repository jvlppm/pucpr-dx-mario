#include "Scene.h"

#include <d3dx9math.h>
#include <vector>
#include "Camera.h"
#include "ImmutableList.h"
#include "IDrawable.h"
#include "cpplinq.hpp"
#include "IUpdateable.h"

using namespace games;

struct Scene::private_implementation {
	private_implementation() {

	}

	Scene* self;
	std::shared_ptr<Shader> shader;
	ImmutableList<std::shared_ptr<Camera>> cameras;
	ImmutableList<std::shared_ptr<IDrawable>> fastDrawables;
	ImmutableList<std::shared_ptr<IDrawable>> drawables;
	ImmutableList<std::shared_ptr<IUpdateable>> updateables;

	void update(float time)
	{
		if (auto lock = updateables)
		{
			for (auto updateable : *lock)
				updateable->update(time);
		}
	}

	void draw(IDirect3DDevice9* device)
	{
		using namespace cpplinq;
		device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, self->clearColor, 1.0f, 0);

		if (auto lockCameras = cameras)
		{
			for (auto camera : *lockCameras)
			{
				//auto vp = camera->view * camera->projection;

				device->SetViewport(&camera->viewport);
				if (auto lockDrawables = fastDrawables)
				{
					for (auto drawable : *lockDrawables)
						shader->draw(device, drawable);
				}

				if (auto lockDrawables = drawables)
				{
					auto sorted_items = from_iterators(lockDrawables->begin(), lockDrawables->end())
						>> orderby_descending([camera](std::shared_ptr<IDrawable> i) {
							auto dist = camera->globalPosition() - i->globalPosition();
							return D3DXVec3LengthSq(&dist);
						})
						>> to_list();

					for (auto drawable : sorted_items)
						shader->draw(device, drawable);
				}
			}
		}
	}

	void registerItem(std::shared_ptr<GameObject> obj)
	{
		if (auto item = std::dynamic_pointer_cast<Camera>(obj))
			cameras = cameras.add(item);

		if (auto item = std::dynamic_pointer_cast<IDrawable>(obj)) {
			if (item->sortedRendering())
				drawables = drawables.add(item);
			else
				fastDrawables = fastDrawables.add(item);
		}

		if (auto item = std::dynamic_pointer_cast<IUpdateable>(obj))
			updateables = updateables.add(item);
	}

	void unregisterItem(std::shared_ptr<GameObject> obj)
	{
		if (auto item = std::dynamic_pointer_cast<Camera>(obj))
			cameras = cameras.remove(item);

		if (auto item = std::dynamic_pointer_cast<IDrawable>(obj)) {
			drawables = drawables.remove(item);
			fastDrawables = fastDrawables.remove(item);
		}

		if (auto item = std::dynamic_pointer_cast<IUpdateable>(obj))
			updateables = updateables.remove(item);
	}
};

Scene::Scene() : pImpl(new Scene::private_implementation())
{
	pImpl->self = this;
}

Scene::~Scene()
{
}

void games::Scene::update(float time)
{
	pImpl->update(time);
}

void Scene::draw(IDirect3DDevice9* device)
{
	pImpl->draw(device);
}

void games::Scene::registerItem(std::shared_ptr<GameObject> obj)
{
	pImpl->registerItem(obj);
}

void games::Scene::unregisterItem(std::shared_ptr<GameObject> obj)
{
	pImpl->unregisterItem(obj);
}
