#pragma once
#include <memory>
#include <d3dx9math.h>
#include <vector>
#include "Material.h"
#include "Scene.h"
#include "Camera.h"
#include "mage\Effect.h"

namespace games {
	class IDrawable
	{
	public:
		IDrawable();
		~IDrawable();

		virtual bool sortedRendering() = 0;
		virtual void draw(IDirect3DDevice9* device, Scene* scene, Camera* camera) = 0;
		virtual D3DXVECTOR3 worldPosition() = 0;
		virtual D3DXMATRIX world() = 0;

		bool canRender();
		void setVisible(bool visible);

		std::shared_ptr<Material> material;
	private:
		struct private_implementation;
		std::unique_ptr<private_implementation> pImpl;
	};
}
