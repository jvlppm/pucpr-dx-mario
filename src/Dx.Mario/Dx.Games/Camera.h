#pragma once

#include <d3dx9.h>
#include "GameObject.h"

namespace games {
	class Camera : public GameObject<Camera>
	{
	public:
		Camera();
		~Camera();

		std::shared_ptr<Camera> setPerspective(float fov, float near, float far);
		std::shared_ptr<Camera> lookAt(D3DXVECTOR3 target);

		D3DVIEWPORT9 viewport;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
}