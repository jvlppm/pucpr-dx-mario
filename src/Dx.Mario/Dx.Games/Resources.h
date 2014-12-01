#pragma once
#include <d3dx9.h>
#include <memory>
#include <string>
#include <map>
#include "mage/TString.h"

namespace games {
	class ResourceLoader {
	public:
		virtual IDirect3DTexture9* loadTexture(IDirect3DDevice9* device, std::string name) = 0;
	};

	class Resources
	{
	private:
		static std::shared_ptr<ResourceLoader> loader;

		typedef std::map<std::string, IDirect3DTexture9*> TexMap;
		static TexMap textures;
		
	public:
		static void setLoader(std::shared_ptr<ResourceLoader> loader) {
			Resources::loader = loader;
		}

		static IDirect3DTexture9* getTexture(IDirect3DDevice9* device, std::string name) {
			auto savedTex = textures[name];
			if (savedTex)
				return savedTex;

			if (!loader)
				throw std::exception("Resource loader not initialized");

			auto tex = loader->loadTexture(device, name);
			textures[name] = tex;
			return tex;
		}
	};
}
