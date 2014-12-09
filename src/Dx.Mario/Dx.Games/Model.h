#pragma once
#include <memory>
#include <d3dx9.h>
#include "GameObject.h"
#include "IDrawable.h"
#include "IShader.h"
#include "Mesh.h"

namespace games {
    class Model :
        public GameObject<Model>,
        public IDrawable
    {
    public:
        Model();
        Model(IDirect3DDevice9* device, const std::string& mesh, const std::string& shader);
        ~Model();
        virtual D3DXVECTOR3 worldPosition();
        virtual D3DXMATRIX world();

        std::shared_ptr<Model> setShader(std::shared_ptr<IShader> shader);
        std::shared_ptr<Model> setMesh(std::shared_ptr<Mesh> mesh);

        void draw(IDirect3DDevice9* device, std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);

    private:
        struct private_implementation;
        std::unique_ptr<private_implementation> pImpl;
    };
}
