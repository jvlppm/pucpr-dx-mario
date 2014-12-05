#pragma once

#include <d3dx9.h>
#include "GameObject.h"

namespace games {
	class Camera : public GameObject<Camera>
	{
	public:
		D3DVIEWPORT9 viewport;
		D3DXMATRIX view;
		D3DXMATRIX projection;

		std::shared_ptr<Camera> setPerspective(float fov, float near, float far);
		std::shared_ptr<Camera> lookAt(D3DXVECTOR3 target);

		Camera();
		~Camera();
	};
}