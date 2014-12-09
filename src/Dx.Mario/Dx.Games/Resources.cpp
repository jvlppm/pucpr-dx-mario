#include "Resources.h"

using namespace mage;
using namespace games;
using namespace std;

struct Resources::private_implementation {
	private_implementation() : textures(), meshes(), effects() {
		loader = nullptr;
	}
	shared_ptr<IResourceLoader> loader;

	typedef map<string, shared_ptr<IDirect3DTexture9>> TexMap;
	TexMap textures;
	typedef map<string, shared_ptr<Mesh>> MeshMap;
	MeshMap meshes;
	typedef map<string, shared_ptr<mage::Effect>> EffectMap;
	EffectMap effects;
    typedef map<string, shared_ptr<IShader>> ShaderMap;
    ShaderMap shaders;

	shared_ptr<IResourceLoader> getLoader() {
		shared_ptr<IResourceLoader> loader = this->loader;
		if (!loader)
			throw exception("Resource loader not initialized");
		return loader;
	}

	shared_ptr<IDirect3DTexture9> getTexture(IDirect3DDevice9* device, const string& name) {
		auto loader = getLoader();
		return getData<IDirect3DTexture9>(name, textures, [loader, device](const string& name) {
			return loader->loadTexture(device, name);
		});
	}
	
	shared_ptr<Mesh> getMesh(IDirect3DDevice9* device, string name) {
		auto loader = getLoader();
		return getData<Mesh>(name, meshes, [loader, device](const string& name) {
			return loader->loadMesh(device, name);
		});
	}

	shared_ptr<IShader> getShader(IDirect3DDevice9* device, const string& name) {
        auto loader = getLoader();
        return getData<IShader>(name, shaders, [loader, device](const string& name) {
            return loader->loadShader(device, name);
        });
    }

    shared_ptr<mage::Effect> getEffect(IDirect3DDevice9* device, const string& name) {
        auto loader = getLoader();
        return getData<mage::Effect>(name, effects, [loader, device](const string& name) {
            return loader->loadEffect(device, name);
        });
    }
};

void Resources::setLoader(shared_ptr<IResourceLoader> loader)
{
	pImpl->loader = loader;
}

shared_ptr<IDirect3DTexture9> Resources::getTexture(IDirect3DDevice9* device, const string& name)
{
	return pImpl->getTexture(device, name);
}

shared_ptr<Mesh> Resources::getMesh(IDirect3DDevice9* device, string name)
{
	return pImpl->getMesh(device, name);
}

shared_ptr<mage::Effect> Resources::getEffect(IDirect3DDevice9* device, const string& name)
{
	return pImpl->getEffect(device, name);
}

shared_ptr<IShader> Resources::getShader(IDirect3DDevice9* device, const string& name)
{
    return pImpl->getShader(device, name);
}

unique_ptr<struct Resources::private_implementation> Resources::pImpl(new struct Resources::private_implementation());
