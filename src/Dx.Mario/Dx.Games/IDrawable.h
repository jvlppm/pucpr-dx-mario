#pragma once
#include <memory>
#include <d3dx9math.h>
#include <vector>

namespace games {
	class IDrawable
	{
	public:
		virtual bool sortedRendering() = 0;
		virtual void draw(IDirect3DDevice9* device) = 0;
		virtual D3DXVECTOR3 globalPosition() = 0;

		bool canRender();
		void setVisible(bool visible);

	protected:
		IDrawable();
		~IDrawable();
	private:
		struct private_implementation;
		std::unique_ptr<private_implementation> pImpl;
	};
}
