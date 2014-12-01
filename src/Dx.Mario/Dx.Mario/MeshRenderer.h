#pragma once

#include "IDrawable.h"
#include "GameObject.h"
#include "mage\TString.h"
#include "Scene.h"
#include "Camera.h"

namespace mario {
	class MeshRenderer :
		public games::GameObject,
		public games::IDrawable
	{
	public:
		MeshRenderer();
		~MeshRenderer();

		virtual bool sortedRendering();
		virtual void draw(IDirect3DDevice9* device, games::Scene* scene, games::Camera* camera);
		virtual D3DXVECTOR3 worldPosition();
		virtual D3DXMATRIX world();
		void setShaderFile(IDirect3DDevice9* device, mage::TString file);
		void setModel(IDirect3DDevice9* device, mage::TString file);
	private:
		struct private_implementation;
		std::unique_ptr<private_implementation> pImpl;
	};
}
