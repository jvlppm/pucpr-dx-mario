#pragma once
#include <memory>
#include <d3dx9.h>
#include "Scene.h"
#include "Camera.h"

namespace games {
	class IDrawable
	{
	public:
		IDrawable();
		~IDrawable();

		virtual void draw(IDirect3DDevice9* device, std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera) = 0;
		virtual D3DXVECTOR3 worldPosition() = 0;
		virtual D3DXMATRIX world() = 0;

        virtual bool sortedRendering();
		bool canRender();
		void setVisible(bool visible);
	private:
		struct private_implementation;
		std::unique_ptr<private_implementation> pImpl;
	};
}
