#include "Scene.h"

#include <d3dx9math.h>
#include <vector>
#include "Camera.h"
#include "ImmutableList.h"
#include "IDrawable.h"
#include "cpplinq.hpp"
#include "IUpdateable.h"
#include <memory>

using namespace std;
using namespace games;

struct Scene::private_implementation {
	private_implementation() {
		backBufferClearColor = D3DXCOLOR(0, 0, 0, 1);
	}

	void draw(IDirect3DDevice9* device, shared_ptr<Scene> scene) {
		using namespace cpplinq;
		IDirect3DSurface9* pBackBuffer;
		device->GetRenderTarget(0, &pBackBuffer);

		for (auto camera : *cameras)
		{
			camera->begin(device, scene);

			for (auto drawable : *fastDrawables)
				drawable->draw(device, scene.get(), camera.get());

			if (auto lockDrawables = drawables)
			{
				auto sorted_items = from_iterators(lockDrawables->begin(), lockDrawables->end())
					>> orderby_descending([camera](shared_ptr<IDrawable> i) {
					auto dist = camera->worldPosition() - i->worldPosition();
					return D3DXVec3LengthSq(&dist);
				})
					>> to_list();

				for (auto drawable : sorted_items)
					drawable->draw(device, scene.get(), camera.get());
			}

			camera->end(device);
		}

		device->SetRenderTarget(0, pBackBuffer);

		device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, backBufferClearColor, 1.0f, 0);
		device->BeginScene();
		for (auto camera : *cameras)
			camera->render(device);
		device->EndScene();
		device->Present(nullptr, nullptr, nullptr, nullptr);
	}

	void update(float time) {
		for (auto updateable : *updateables)
			updateable->update(time);
	}

	void registerItem(shared_ptr<BaseObject> obj)
	{
		if (auto item = dynamic_pointer_cast<Camera>(obj))
			cameras = cameras.add(item);

		if (auto item = dynamic_pointer_cast<IDrawable>(obj)) {
			if (item->sortedRendering())
				drawables = drawables.add(item);
			else
				fastDrawables = fastDrawables.add(item);
		}

		if (auto item = dynamic_pointer_cast<IUpdateable>(obj))
			updateables = updateables.add(item);
	}

	void unregisterItem(shared_ptr<BaseObject> obj) {
		if (auto item = dynamic_pointer_cast<Camera>(obj))
			cameras = cameras.remove(item);

		if (auto item = dynamic_pointer_cast<IDrawable>(obj)) {
			drawables = drawables.remove(item);
			fastDrawables = fastDrawables.remove(item);
		}

		if (auto item = dynamic_pointer_cast<IUpdateable>(obj))
			updateables = updateables.remove(item);
	}

private:
	D3DXCOLOR backBufferClearColor;
	ImmutableList<shared_ptr<Camera>> cameras;
	ImmutableList<shared_ptr<IDrawable>> fastDrawables;
	ImmutableList<shared_ptr<IDrawable>> drawables;
	ImmutableList<shared_ptr<IUpdateable>> updateables;
};

Scene::Scene() : pImpl(new Scene::private_implementation())
{
	clearColor = D3DXCOLOR(101 / 255.f, 156 / 255.f, 239 / 255.f, 1);
	ambientColor = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
	diffuseColor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	specularColor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	lightDir = D3DXVECTOR3(-1.0f, -1.0f, 4.0f);
	D3DXVec3Normalize(&lightDir, &lightDir);
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
	pImpl->draw(device, shared_from_this());
}

void games::Scene::registerItem(shared_ptr<BaseObject> obj)
{
	pImpl->registerItem(obj);
}

void games::Scene::unregisterItem(shared_ptr<BaseObject> obj)
{
	pImpl->unregisterItem(obj);
}
