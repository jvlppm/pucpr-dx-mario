#include "MeshRenderer.h"

using namespace games;

struct MeshRenderer::private_implementation {
	private_implementation() {
	}
};

MeshRenderer::MeshRenderer() : pImpl(new MeshRenderer::private_implementation())
{
}


MeshRenderer::~MeshRenderer()
{
}

bool games::MeshRenderer::sortedRendering()
{
	return false;
}

void games::MeshRenderer::draw(IDirect3DDevice9* device)
{
	
}

D3DXVECTOR3 games::MeshRenderer::globalPosition()
{
	throw std::logic_error("The method or operation is not implemented.");
}
