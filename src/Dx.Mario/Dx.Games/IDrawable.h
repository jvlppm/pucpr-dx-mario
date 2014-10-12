#pragma once
#include <d3d9.h>

namespace games {
	class IDrawable
	{
	public:
		virtual void draw(IDirect3DDevice9* device) = 0;
		virtual D3DXVECTOR3 globalPosition() = 0;
	};
}
