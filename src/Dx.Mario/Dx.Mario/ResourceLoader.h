#pragma once
#include "Resources.h"
#include <d3dx9.h>
#include "mage/TString.h"
#include "mesh.h"
#include "mage/Effect.h"

namespace mario {
	class ResourceLoader :
		public games::IResourceLoader
	{
	public:
		std::shared_ptr<IDirect3DTexture9> loadTexture(IDirect3DDevice9* device, const std::string& name);
		std::shared_ptr<games::Mesh> loadMesh(IDirect3DDevice9* device, const std::string& name);
		std::shared_ptr<mage::Effect> loadEffect(IDirect3DDevice9* device, const std::string& name);
        std::shared_ptr<games::IShader> loadShader(IDirect3DDevice9* device, const std::string& name);
	};
}
