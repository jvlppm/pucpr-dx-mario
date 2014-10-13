#pragma once
#include <d3d9.h>
#include <memory>
#include "GameObject.h"

namespace games {
	class Scene : public GameObject
	{
	public:
		Scene();
		~Scene();
		void draw(IDirect3DDevice9* device);
		void update(float time);
		void registerItem(std::shared_ptr<GameObject> obj);
		void unregisterItem(std::shared_ptr<GameObject> obj);
	private:
		struct private_implementation;
		std::unique_ptr<private_implementation> pImpl;
	};
}
