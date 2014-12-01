#include "ResourceLoader.h"
#include <d3dx9.h>

using namespace mario;
using namespace std;

ResourceLoader::ResourceLoader()
{
}


ResourceLoader::~ResourceLoader()
{
}

IDirect3DTexture9* mario::ResourceLoader::loadTexture(IDirect3DDevice9* device, string name)
{
	if (name == "default")
		name = "whitetex.dds";

	IDirect3DTexture9* tex = nullptr;
	std::string fileName = "Resources/" + name;

	auto error = D3DXCreateTextureFromFileA(device, fileName.c_str(), &tex);
	if (error)
		throw std::exception("Error loading texture: " + error);
	return tex;
}
