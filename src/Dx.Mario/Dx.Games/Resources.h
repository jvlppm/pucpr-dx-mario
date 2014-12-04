#pragma once
#include <d3dx9.h>
#include <memory>
#include <string>
#include <map>
#include "mage/TString.h"
#include "mesh.h"
#include <functional>
#include "mage/Effect.h"

namespace games {
	class IResourceLoader {
	public:
		virtual std::shared_ptr<IDirect3DTexture9> loadTexture(IDirect3DDevice9* device, const std::string& name) = 0;
		virtual std::shared_ptr<Mesh> loadMesh(IDirect3DDevice9* device, const std::string& name) = 0;
		virtual std::shared_ptr<mage::Effect> loadEffect(IDirect3DDevice9* device, const std::string& name) = 0;
	};

	class Resources
	{
	private:
		static std::shared_ptr<IResourceLoader> loader;

		typedef std::map<std::string, std::shared_ptr<IDirect3DTexture9>> TexMap;
		static TexMap textures;
		typedef std::map<std::string, std::shared_ptr<Mesh>> MeshMap;
		static MeshMap meshes;
		typedef std::map<std::string, std::shared_ptr<mage::Effect>> EffectMap;
		static EffectMap effects;
		
	public:
		static void setLoader(std::shared_ptr<IResourceLoader> loader) {
			Resources::loader = loader;
		}

		template <class T>
		static std::shared_ptr<T> getData(const std::string& name, std::map<std::string, std::shared_ptr<T>> cache, std::function<std::shared_ptr<T>(const std::string&)> loadData) {
			auto savedData = cache[name];
			if (savedData)
				return savedData;

			if (!loader)
				throw std::exception("Resource loader not initialized");

			auto data = loadData(name);
			cache[name] = data;
			return data;
		}

		static std::shared_ptr<IDirect3DTexture9> getTexture(IDirect3DDevice9* device, const std::string& name) {
			return getData<IDirect3DTexture9>(name, textures, [device](const std::string& name) {
				return loader->loadTexture(device, name);
			});
		}

		static std::shared_ptr<Mesh> getMesh(IDirect3DDevice9* device, std::string name) {
			return getData<Mesh>(name, meshes, [device](const std::string& name) {
				return loader->loadMesh(device, name);
			});
		}

		static std::shared_ptr<mage::Effect> getEffect(IDirect3DDevice9* device, const std::string& name) {
			return getData<mage::Effect>(name, effects, [device](const std::string& name) {
				return loader->loadEffect(device, name);
			});
		}
	};
}
