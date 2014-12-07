#include "CameraVertex.h"

using namespace games;
using namespace std;

shared_ptr<IDirect3DVertexDeclaration9> CameraVertex::declaration = nullptr;

using namespace std;

CameraVertex::CameraVertex(const D3DXVECTOR2& position, const D3DXVECTOR2& texCoord)
	: position(position), texture(texCoord)
{
}

CameraVertex::CameraVertex(float x, float y, float u, float v)
	: position(x, y), texture(u, v)
{
}

shared_ptr<IDirect3DVertexDeclaration9> CameraVertex::getDeclaration(IDirect3DDevice9* device)
{
	if (declaration)
		return declaration;

	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	IDirect3DVertexDeclaration9* vertexDeclaration;
	if (device->CreateVertexDeclaration(decl, &vertexDeclaration) != S_OK)
		throw runtime_error("Could not create camera vertex declaration");

	declaration = shared_ptr<IDirect3DVertexDeclaration9>(vertexDeclaration, [](IDirect3DVertexDeclaration9* declaration) {
		declaration->Release();
	});

	return declaration;
}

shared_ptr<IDirect3DVertexBuffer9> CameraVertex::createVertexBuffer(IDirect3DDevice9* device, const std::vector<CameraVertex>& vertices, D3DPOOL pool, DWORD usage, HANDLE* pSharedHandle)
{
	IDirect3DVertexBuffer9* vertexBuffer;

	if (device->CreateVertexBuffer(
		sizeof(CameraVertex) * vertices.size(), usage, 0, pool,
		&vertexBuffer, pSharedHandle) != S_OK)
		throw runtime_error("Could not create camera vertex buffer");

	CameraVertex* v = 0;
	if (vertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&v), 0) != S_OK)
		throw runtime_error("Could not lock camera vertex buffer");
	for (unsigned i = 0; i < vertices.size(); ++i)
		v[i] = vertices[i];
	if(vertexBuffer->Unlock() != S_OK)
		throw runtime_error("Could not release camera vertex buffer");

	return shared_ptr<IDirect3DVertexBuffer9>(vertexBuffer, [](IDirect3DVertexBuffer9* vertexBuffer) {
		vertexBuffer->Release();
	});
}
