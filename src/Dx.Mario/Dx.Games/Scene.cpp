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
	private_implementation() { }
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
	for (auto updateable : *pImpl->updateables)
		updateable->update(time);
}

void Scene::draw(IDirect3DDevice9* device)
{
	using namespace cpplinq;
	device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clearColor, 1.0f, 0);

	device->BeginScene();

	for (auto camera : *pImpl->cameras)
	{
		device->SetViewport(&camera->viewport);

		for (auto drawable : *pImpl->fastDrawables)
			drawable->draw(device, shared_from_this().get(), camera.get());

		if (auto lockDrawables = pImpl->drawables)
		{
			auto sorted_items = from_iterators(lockDrawables->begin(), lockDrawables->end())
				>> orderby_descending([camera](shared_ptr<IDrawable> i) {
				auto dist = camera->worldPosition() - i->worldPosition();
				return D3DXVec3LengthSq(&dist);
			})
				>> to_list();

			for (auto drawable : sorted_items)
				drawable->draw(device, shared_from_this().get(), camera.get());
		}
	}

	device->EndScene();
	device->Present(nullptr, nullptr, nullptr, nullptr);
}

void games::Scene::registerItem(shared_ptr<BaseObject> obj)
{
	if (auto item = dynamic_pointer_cast<Camera>(obj))
		pImpl->cameras = pImpl->cameras.add(item);

	if (auto item = dynamic_pointer_cast<IDrawable>(obj)) {
		if (item->sortedRendering())
			pImpl->drawables = pImpl->drawables.add(item);
		else
			pImpl->fastDrawables = pImpl->fastDrawables.add(item);
	}

	if (auto item = dynamic_pointer_cast<IUpdateable>(obj))
		pImpl->updateables = pImpl->updateables.add(item);
}

void games::Scene::unregisterItem(shared_ptr<BaseObject> obj)
{
	if (auto item = dynamic_pointer_cast<Camera>(obj))
		pImpl->cameras = pImpl->cameras.remove(item);

	if (auto item = dynamic_pointer_cast<IDrawable>(obj)) {
		pImpl->drawables = pImpl->drawables.remove(item);
		pImpl->fastDrawables = pImpl->fastDrawables.remove(item);
	}

	if (auto item = dynamic_pointer_cast<IUpdateable>(obj))
		pImpl->updateables = pImpl->updateables.remove(item);
}
