#pragma once

#include <d3dx9.h>
#include "GameObject.h"
#include "Scene.h"
#include <memory>
#include "mage/Effect.h"
#include "IShader.h"

namespace games {

    class IShader;
    class Scene;

	class Camera : public GameObject<Camera>
	{
	public:
		Camera();
        Camera(IDirect3DDevice9* device, const std::string& shader);
		~Camera();

		std::shared_ptr<Camera> setPerspective(float fov, float near, float far);
        std::shared_ptr<Camera> setShader(std::shared_ptr<IShader> shader);
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