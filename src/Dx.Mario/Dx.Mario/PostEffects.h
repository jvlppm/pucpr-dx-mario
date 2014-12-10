#pragma once
#include "IShader.h"

namespace mario {
    class PostEffects :
        public games::IShader
    {
    public:
        PostEffects();
        ~PostEffects();

        // Inherited via IShader
        virtual void begin(IDirect3DDevice9 * device) override;
        virtual void end() override;
        virtual void setScene(std::shared_ptr<games::Scene> scene) override;
        virtual void setCamera(std::shared_ptr<games::Camera> camera) override;
        virtual void setObject(std::shared_ptr<games::BaseObject> object) override;
        virtual void setTexture(std::shared_ptr<IDirect3DTexture9> texture) override;
        virtual void setMaterial(D3DMATERIAL9 material) override;
        virtual void execute(std::function<void(IDirect3DDevice9*)> drawFunction) override;

    private:
        struct private_implementation;
        std::unique_ptr<private_implementation> pImpl;
    };
}
