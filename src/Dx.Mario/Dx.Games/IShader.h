#pragma once
#include <functional>
#include <d3dx9.h>
#include <memory>
#include "Scene.h"

namespace games {
    class IShader
    {
    public:
        virtual void begin(IDirect3DDevice9* device) = 0;
        virtual void end() = 0;
        virtual void setScene(std::shared_ptr<Scene> scene) = 0;
        virtual void setCamera(std::shared_ptr<Camera> camera) = 0;
        virtual void setObject(std::shared_ptr<BaseObject> object) = 0;
        virtual void setTexture(std::shared_ptr<IDirect3DTexture9> texture) = 0;
        virtual void setMaterial(D3DMATERIAL9 material) = 0;
        virtual void execute(std::function<void(IDirect3DDevice9*)> drawFunction) = 0;
    };
}
