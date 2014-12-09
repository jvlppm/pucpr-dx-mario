#pragma once
#include <d3dx9.h>
#include <memory>
#include <string>
#include <map>
#include "mage/TString.h"
#include "mesh.h"
#include <functional>
#include "mage/Effect.h"
#include "IShader.h"

namespace games {
	class IResourceLoader {
	public:
		virtual std::shared_ptr<IDirect3DTexture9> loadTexture(IDirect3DDevice9* device, const std::string& name) = 0;
		virtual std::shared_ptr<Mesh> loadMesh(IDirect3DDevice9* device, const std::string& name) = 0;
		virtual std::shared_ptr<mage::Effect> loadEffect(IDirect3DDevice9* device, const std::string& name) = 0;
        virtual std::shared_ptr<IShader> loadShader(IDirect3DDevice9* device, const std::string& name) = 0;
	};

	class Resources
	{
	public:
		static void setLoader(std::shared_ptr<IResourceLoader> loader);

		static std::shared_ptr<IDirect3DTexture9> getTexture(IDirect3DDevice9* device, const std::string& name);
		static std::shared_ptr<Mesh> getMesh(IDirect3DDevice9* device, std::string name);
        static std::shared_ptr<IShader> getShader(IDirect3DDevice9* device, const std::string& name);
		static std::shared_ptr<mage::Effect> getEffect(IDirect3DDevice9* device, const std::string& name);
	private:
		struct private_implementation;
		static std::unique_ptr<private_implementation> pImpl;

		template <class T>
		static std::shared_ptr<T> getData(const std::string& name, std::map<std::string, std::shared_ptr<T>> cache, std::function<std::shared_ptr<T>(const std::string&)> loadData) {
			auto savedData = cache[name];
			if (savedData)
				return savedData;

			auto data = loadData(name);
			cache[name] = data;
			return data;
		}
	};
}
