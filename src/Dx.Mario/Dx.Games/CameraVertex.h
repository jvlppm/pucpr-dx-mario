#pragma once
#include <memory>
#include <d3dx9.h>
#include <vector>

namespace games {
	class CameraVertex
	{
	private:
		static std::shared_ptr<IDirect3DVertexDeclaration9> declaration;
	public:
		D3DXVECTOR2 position;
		D3DXVECTOR2 texture;

		static std::shared_ptr<IDirect3DVertexDeclaration9> getDeclaration(IDirect3DDevice9* device);
		static std::shared_ptr<IDirect3DVertexBuffer9> createVertexBuffer(IDirect3DDevice9* device,
			const std::vector<CameraVertex>& vertices, D3DPOOL pool = D3DPOOL_MANAGED,
			DWORD usage = 0, HANDLE* pSharedHandle = nullptr);

		CameraVertex(const D3DXVECTOR2& position, const D3DXVECTOR2& texCoord);
		CameraVertex(float x, float y, float u, float v);
	};
}
