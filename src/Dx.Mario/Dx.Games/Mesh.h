#pragma once
#include <memory>
#include <vector>
#include <d3d9.h>

namespace games {
	typedef struct Mesh {
		std::shared_ptr<ID3DXMesh> data;
		std::vector<D3DMATERIAL9> materials = {};
		std::vector<std::shared_ptr<IDirect3DTexture9>> textures = {};
	} Mesh;
}
