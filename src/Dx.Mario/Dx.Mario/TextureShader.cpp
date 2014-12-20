#include "TextureShader.h"
#include "Resources.h"

using namespace mario;
using namespace games;
using namespace mage;
using namespace std;

struct TextureShader::private_implementation {
    private_implementation() {
        D3DXMatrixIdentity(&identity);
    }

    ~private_implementation() {
    }

    void begin(IDirect3DDevice9* device) {
        if (this->device != device) {
            this->device = device;
            this->effect = Resources::getEffect(device, "texture.fx");
        }

        this->effect->setTechnique("PhongTech");
    }

    void end() {
        effect->setMatrix("gWorld", identity);
        effect->setMatrix("gView", identity);
        effect->setMatrix("gProjection", identity);
        effect->setColor("gAmbientColor", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        effect->setColor("gDiffuseColor", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        effect->setColor("gSpecularColor", D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    }

    void setScene(shared_ptr<Scene> scene) {
        effect->setVector("gLightDir", scene->lightDir);
        effect->setVector("gAmbientColor", scene->ambientColor);
        effect->setVector("gDiffuseColor", scene->diffuseColor);
        effect->setVector("gSpecularColor", scene->specularColor);
    }

    void setCamera(shared_ptr<Camera> camera) {
        effect->setMatrix("gView", camera->view);
        effect->setVector("gCameraPos", camera->worldPosition());
        effect->setMatrix("gProjection", camera->projection);
    }

    void setObject(shared_ptr<BaseObject> object) {
        effect->setMatrix("gWorld", object->world());
    }

    void setTexture(shared_ptr<IDirect3DTexture9> texture) {
        effect->setTexture("gTexture", texture.get());
    }

    void setMaterial(D3DMATERIAL9 material) {
        effect->setColor("gAmbientMaterial", material.Ambient);
        effect->setColor("gDiffuseMaterial", material.Diffuse);
        effect->setColor("gSpecularMaterial", material.Specular);
        effect->setFloat("gSpecularPower", material.Power);
    }

    void execute(function<void(IDirect3DDevice9*)> drawFunction) {
        effect->commit();
        effect->execute(drawFunction);
    }

private:
    IDirect3DDevice9* device;
    shared_ptr<Effect> effect;
    D3DXMATRIX identity;
};

TextureShader::TextureShader() : pImpl(new TextureShader::private_implementation())
{
}

TextureShader::~TextureShader()
{
}

void TextureShader::begin(IDirect3DDevice9* device)
{
    pImpl->begin(device);
}

void TextureShader::end()
{
    pImpl->end();
}

void TextureShader::setScene(shared_ptr<Scene> scene)
{
    pImpl->setScene(scene);
}

void TextureShader::setCamera(shared_ptr<Camera> camera)
{
    pImpl->setCamera(camera);
}

void TextureShader::setObject(shared_ptr<BaseObject> object)
{
    pImpl->setObject(object);
}

void TextureShader::setTexture(shared_ptr<IDirect3DTexture9> texture)
{
    pImpl->setTexture(texture);
}

void TextureShader::setMaterial(D3DMATERIAL9 material)
{
    pImpl->setMaterial(material);
}

void TextureShader::execute(function<void(IDirect3DDevice9*)> drawFunction)
{
    pImpl->execute(drawFunction);
}
