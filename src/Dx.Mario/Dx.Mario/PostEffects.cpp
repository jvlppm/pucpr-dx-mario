#include "PostEffects.h"
#include "Resources.h"

using namespace mario;
using namespace games;
using namespace mage;
using namespace std;

struct PostEffects::private_implementation {
    private_implementation() {
        D3DXMatrixIdentity(&identity);
        technique = "Default";
    }

    ~private_implementation() {
    }

    void begin(IDirect3DDevice9* device) {
        if (this->device != device) {
            this->device = device;
            this->effect = Resources::getEffect(device, "postEffects.fx");

            this->toonDiffuser = Resources::getTexture(device, "ToonLightTexture.bmp");
            this->effect->setTexture("gToonLightTexture", this->toonDiffuser.get());
        }

        this->effect->setTechnique(technique);
    }

    void end() {
    }

    void setScene(shared_ptr<Scene> scene) {
    }

    void setCamera(shared_ptr<Camera> camera) {
    }

    void setObject(shared_ptr<BaseObject> object) {
    }

    void setTexture(shared_ptr<IDirect3DTexture9> texture) {
        effect->setTexture("gTexture", texture.get());
    }

    void setMaterial(D3DMATERIAL9 material) {
    }

    void execute(function<void(IDirect3DDevice9*)> drawFunction) {
        effect->commit();
        effect->execute(drawFunction);
    }

    void setEffect(const string& name) {
        technique = name;
    }

private:
    string technique;
    IDirect3DDevice9* device;
    shared_ptr<Effect> effect;
    shared_ptr<IDirect3DTexture9> toonDiffuser;
    D3DXMATRIX identity;
};

PostEffects::PostEffects() : pImpl(new PostEffects::private_implementation())
{
}

PostEffects::~PostEffects()
{
}

void PostEffects::begin(IDirect3DDevice9* device)
{
    pImpl->begin(device);
}

void PostEffects::end()
{
    pImpl->end();
}

void PostEffects::setScene(shared_ptr<Scene> scene)
{
    pImpl->setScene(scene);
}

void PostEffects::setCamera(shared_ptr<Camera> camera)
{
    pImpl->setCamera(camera);
}

void PostEffects::setObject(shared_ptr<BaseObject> object)
{
    pImpl->setObject(object);
}

void PostEffects::setTexture(shared_ptr<IDirect3DTexture9> texture)
{
    pImpl->setTexture(texture);
}

void PostEffects::setMaterial(D3DMATERIAL9 material)
{
    pImpl->setMaterial(material);
}

void PostEffects::execute(function<void(IDirect3DDevice9*)> drawFunction)
{
    pImpl->execute(drawFunction);
}

void PostEffects::setEffect(const string& name)
{
    pImpl->setEffect(name);
}
