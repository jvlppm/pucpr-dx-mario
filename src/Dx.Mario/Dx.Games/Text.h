#pragma once
#include <memory>
#include <d3dx9.h>
#include "GameObject.h"
#include "IDrawable.h"

namespace games {
    class Text :
        public GameObject<Text>,
        public IDrawable
    {
    public:
        Text(const std::string& text, int fontSize);
        ~Text();

        virtual D3DXVECTOR3 worldPosition();
        virtual D3DXMATRIX world();

        virtual void draw(IDirect3DDevice9* device, std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);

    private:
        struct private_implementation;
        std::unique_ptr<private_implementation> pImpl;
    };
}
