#pragma once
#include <memory>
#include <d3d9.h>
#include <string>

namespace mario {
	static class Texture
	{
	public:
		std::shared_ptr<IDirect3DTexture9> fromName(std::string name);
	};
}
