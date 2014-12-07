#pragma once

#include <d3dx9.h>
#include "GameObject.h"
#include "Scene.h"
#include <memory>
#include "mage/Effect.h"

namespace games {
	class Camera : public GameObject<Camera>
	{
	public:
		Camera();
		~Camera();

		std::shared_ptr<Camera> setPerspective(float fov, float near, float far);
		std::shared_ptr<Camera> lookAt(D3DXVECTOR3 target);

		void begin(IDirect3DDevice9* device, std::shared_ptr<Scene> scene);
		void end(IDirect3DDevice9* device);
		void draw(IDirect3DDevice9* device);

		D3DXMATRIX view;
		D3DXMATRIX projection;
	private:
		struct private_implementation;
		std::unique_ptr<private_implementation> pImpl;
	};
}