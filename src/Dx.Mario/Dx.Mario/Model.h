#pragma once
#include "GameObject.h"
#include "IDrawable.h"

namespace mario {
	class Model :
		public games::GameObject<Model>,
		public games::IDrawable
	{
	public:
		Model();
		Model(IDirect3DDevice9* device, const std::string& mesh, const std::string& effect);
		~Model();

		virtual void draw(IDirect3DDevice9* device, games::Scene* scene, games::Camera* camera);
		virtual D3DXVECTOR3 worldPosition();
		virtual D3DXMATRIX world();
		void setEffect(IDirect3DDevice9* device, const std::string& file);
		void setMesh(IDirect3DDevice9* device, const std::string& file);
	private:
		struct private_implementation;
		std::unique_ptr<private_implementation> pImpl;
	};
}
