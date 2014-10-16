#pragma once
#include <memory>
#include <d3dx9.h>
#include "Material.h"
#include "IDrawable.h"

namespace games {
	class Shader
	{
		//void setAmbientLight(D3DXCOLOR color);
		//void setDirectionalLight(D3DXCOLOR color, D3DXVECTOR3 direction);
		//void setMaterial(Material material);
	public:
		virtual void draw(IDirect3DDevice9* device, std::shared_ptr<IDrawable> drawable) const = 0;
	};
}
