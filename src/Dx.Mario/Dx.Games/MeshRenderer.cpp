#include "MeshRenderer.h"

#include "ShaderCompileException.h"
#include "mage\Effect.h"

using namespace games;
using namespace mage;

struct MeshRenderer::private_implementation {
	private_implementation() {
	}

	std::shared_ptr<Effect> effect;

	void setShaderFile(IDirect3DDevice9* device, TString file)
	{
		effect = std::make_shared<Effect>(file);
		std::string error = effect->compile(device);
		if (!error.empty())
			throw ShaderCompileException("Failed to compile shader file.\n" + error);
	}
};

MeshRenderer::MeshRenderer() : pImpl(new MeshRenderer::private_implementation())
{
}

MeshRenderer::~MeshRenderer()
{
}

bool MeshRenderer::sortedRendering()
{
	return false;
}

void MeshRenderer::draw(IDirect3DDevice9* device)
{
	
}

D3DXVECTOR3 MeshRenderer::globalPosition()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void MeshRenderer::setShaderFile(IDirect3DDevice9* device, TString file)
{
	pImpl->setShaderFile(device, file);
}
