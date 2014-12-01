#pragma once
#include "Resources.h"
#include <d3dx9.h>
#include "mage/TString.h"

namespace mario {
	class ResourceLoader :
		public games::ResourceLoader
	{
	public:
		ResourceLoader();
		~ResourceLoader();

		IDirect3DTexture9* loadTexture(IDirect3DDevice9* device, std::string name);
	};
}
