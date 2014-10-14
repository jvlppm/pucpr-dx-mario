#pragma once

#include "IDrawable.h"
#include "GameObject.h"

namespace games {
	class MeshRenderer :
		public GameObject,
		public IDrawable
	{
	public:
		MeshRenderer();
		~MeshRenderer();

		virtual bool sortedRendering();
		virtual void draw(IDirect3DDevice9* device);
		virtual D3DXVECTOR3 globalPosition();

	private:
		struct private_implementation;
		std::unique_ptr<private_implementation> pImpl;
	};
}
