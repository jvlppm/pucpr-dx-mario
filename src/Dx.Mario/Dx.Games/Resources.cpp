#include "Resources.h"

using namespace mage;
using namespace games;

struct Resources::private_implementation {
	private_implementation() : textures(), meshes(), effects() {
		loader = nullptr;
	}
	std::shared_ptr<IResourceLoader> loader;

	typedef std::map<std::string, std::shared_ptr<IDirect3DTexture9>> TexMap;
	TexMap textures;
	typedef std::map<std::string, std::shared_ptr<Mesh>> MeshMap;
	MeshMap meshes;
	typedef std::map<std::string, std::shared_ptr<mage::Effect>> EffectMap;
	EffectMap effects;

	std::shared_ptr<IResourceLoader> getLoader() {
		std::shared_ptr<IResourceLoader> loader = this->loader;
		if (!loader)
			throw std::exception("Resource loader not initialized");
		return loader;
	}

	std::shared_ptr<IDirect3DTexture9> getTexture(IDirect3DDevice9* device, const std::string& name) {
		auto loader = getLoader();
		return getData<IDirect3DTexture9>(name, textures, [loader, device](const std::string& name) {
			return loader->loadTexture(device, name);
		});
	}
	
	std::shared_ptr<Mesh> getMesh(IDirect3DDevice9* device, std::string name) {
		auto loader = getLoader();
		return getData<Mesh>(name, meshes, [loader, device](const std::string& name) {
			return loader->loadMesh(device, name);
		});
	}

	std::shared_ptr<mage::Effect> getEffect(IDirect3DDevice9* device, const std::string& name) {
		auto loader = getLoader();
		return getData<mage::Effect>(name, effects, [loader, device](const std::string& name) {
			return loader->loadEffect(device, name);
		});
	}
};

void Resources::setLoader(std::shared_ptr<IResourceLoader> loader)
{
	pImpl->loader = loader;
}

std::shared_ptr<IDirect3DTexture9> Resources::getTexture(IDirect3DDevice9* device, const std::string& name)
{
	return pImpl->getTexture(device, name);
}

std::shared_ptr<Mesh> Resources::getMesh(IDirect3DDevice9* device, std::string name)
{
	return pImpl->getMesh(device, name);
}

std::shared_ptr<mage::Effect> Resources::getEffect(IDirect3DDevice9* device, const std::string& name)
{
	return pImpl->getEffect(device, name);
}

std::unique_ptr<struct Resources::private_implementation> Resources::pImpl(new struct Resources::private_implementation());
