#pragma once

#include <d3d9.h>

namespace games {
	class Scene
	{
	public:
		Scene();
		~Scene();
		void draw(IDirect3DDevice9* device);
	};
}
