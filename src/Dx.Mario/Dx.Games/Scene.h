#pragma once
#include <d3d9.h>
#include <memory>
#include "GameObject.h"
#include "Fog.h"

namespace games {
	class Scene : public GameObject<Scene>
	{
	public:
		Scene();
		~Scene();
		void draw(IDirect3DDevice9* device);
		void update(float time);
		void registerItem(std::shared_ptr<BaseObject> obj);
		void unregisterItem(std::shared_ptr<BaseObject> obj);

		D3DXCOLOR clearColor;
		D3DXVECTOR3 lightDir;
		D3DXVECTOR3 ambientColor;
		D3DXVECTOR3 diffuseColor;
		D3DXVECTOR3 specularColor;
        Fog fog;
	private:
		struct private_implementation;
		std::unique_ptr<private_implementation> pImpl;
	};
}
