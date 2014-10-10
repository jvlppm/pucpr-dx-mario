#pragma once

#include <d3dx9.h>
#include "GameObject.h"

namespace games {
	class Camera : public GameObject
	{
	public:
		D3DXMATRIX view;
		D3DXMATRIX projection;

		Camera();
		~Camera();
	};
}