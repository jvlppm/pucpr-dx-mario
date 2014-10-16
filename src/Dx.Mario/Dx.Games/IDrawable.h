#pragma once
#include <memory>
#include <d3dx9math.h>
#include <vector>
#include "Material.h"
#include "mage\Effect.h"

namespace games {
	class IDrawable
	{
	public:
		virtual bool sortedRendering() = 0;
		virtual void draw(IDirect3DDevice9* device) = 0;
		virtual D3DXVECTOR3 globalPosition() = 0;

		bool canRender();
		void setVisible(bool visible);

		std::shared_ptr<Material> material;

	protected:
		IDrawable();
		~IDrawable();
	private:
		struct private_implementation;
		std::unique_ptr<private_implementation> pImpl;
	};
}
