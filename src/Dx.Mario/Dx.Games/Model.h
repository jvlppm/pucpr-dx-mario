#pragma once
#include <memory>
#include "GameObject.h"
#include "IDrawable.h"
#include "IShader.h"

namespace games {
    class Model :
        public GameObject<Model>,
        public IDrawable
    {
    public:
        Model();
        ~Model();

        void draw(IDirect3DDevice9* device, shared_ptr<Scene> scene, shared_ptr<Camera> camera);

    private:
        struct private_implementation;
        std::unique_ptr<private_implementation> pImpl;
    };
}
