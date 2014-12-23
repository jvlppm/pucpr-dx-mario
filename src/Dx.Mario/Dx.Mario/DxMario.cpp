#include "DxMario.h"
#include "mage/TString.h"
#include <memory>
#include "Resources.h"
#include "ResourceLoader.h"
#include "Model.h"
#include "Text.h"
#include "Container.h"
#include "PostEffects.h"

using namespace mario;
using namespace games;
using namespace mage;
using namespace std;

struct DxMario::private_implementation {
    shared_ptr<PostEffects> postEffects;
    shared_ptr<Scene> scene;
    shared_ptr<Model> castle;
    shared_ptr<Container> mario;
    shared_ptr<Container> luigi;
    shared_ptr<Camera> camera;
    IDirect3DDevice9* device;
    int effectCount = 0;
    int modelCount = 0;

    D3DXVECTOR3 toMove = D3DXVECTOR3(0, 0, 0);
    D3DXVECTOR3 toMoveCam = D3DXVECTOR3(0, 0, 0);

    void setup(IDirect3DDevice9* device)
    {
        this->device = device;
        Resources::setLoader(make_shared<ResourceLoader>());

        createScene(device);

        postEffects = make_shared<PostEffects>();
        applyEffect(0);

        camera = scene->add<Camera>()
            ->setPerspective(60, 1, 5000)
            ->translate(-48, 11, -67)
            ->lookAt(mario->worldPosition() + D3DXVECTOR3(0, 2, 0))
            ->setShader(postEffects);

        scene->lightDir = mario->worldPosition() - camera->worldPosition();
        scene->lightDir.x += 1;
        D3DXVec3Normalize(&scene->lightDir, &scene->lightDir);

        scene->init();
        selectModel(0);
    }

    void createScene(IDirect3DDevice9* device) {
        scene = std::make_shared<Scene>();
        scene->specularColor /= 20;

        castle = scene->add<Model>(device, "Peach's Castle.x", "texture.fx")
            ->scale(0.01f);

        luigi = scene->add<Container>()
            ->translate(-56, 8.30, -68)
            ->rotateY(1);
        luigi->add<Model>(device, "dwarf.x", "texture.fx")
            ->scale(2);

        mario = scene->add<Container>()
            ->translate(-56, 8, -64)
            ->rotateY(-1.2);

        scene->add<Text>("Change model: RIGHT\nChange effect: UP", 23);
    }

    void update(float time) {
        if (luigi)
            luigi->rotateY(time);
        if (mario) {
            mario->rotateY(time);
            toMove.x = toMove.y = toMove.z = 0;
            toMoveCam.x = toMoveCam.y = toMoveCam.z = 0;
        }

        scene->update(time);
    }

    void processEvent(const mage::WindowsEvent& evt) {
        switch (evt.type) {
        case WM_KEYDOWN:
            switch (evt.wParam) {
                case VK_UP:
                    applyEffect(effectCount + 1);
                    break;
                case VK_DOWN:
                    applyEffect(effectCount - 1);
                    break;
                case VK_RIGHT:
                    selectModel(modelCount + 1);
                    break;

                case VK_LEFT:
                    selectModel(modelCount - 1);
                    break;
            }
            break;
        }

        if (toMove.x || toMove.y || toMove.z) {
            D3DXVec3Normalize(&toMove, &toMove);
        }

        if (toMoveCam.x || toMoveCam.y || toMoveCam.z) {
            D3DXVec3Normalize(&toMoveCam, &toMoveCam);
        }
    }

    void applyEffect(int effect) {
        effectCount = (effect + 5) % 5;

        switch (effectCount) {
        case 0:
            scene->diffuseColor = D3DXVECTOR3(1.5f, 1.5f, 1.5f);
            postEffects->setEffect("Toon");
            break;
        case 1:
            scene->diffuseColor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
            postEffects->setEffect("Default");
            break;
        case 2:
            scene->diffuseColor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
            postEffects->setEffect("Borders");
            break;
        case 3:
            scene->diffuseColor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
            postEffects->setEffect("Grayscale");
            break;
        case 4:
            scene->diffuseColor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
            postEffects->setEffect("Inverse");
            break;
        }
    }

    void selectModel(int model) {
        modelCount = (model + 4) % 4;

        auto oldModel = mario->findChildren<Model>();
        if(oldModel.size())
            oldModel[0]->destroy();

        switch (modelCount) {
        case 0:
            mario->add<Model>(device, "Mario.x", "texture.fx")
                ->scale(0.2f)->init();
            break;
        case 1:
            mario->add<Model>(device, "Luigi.x", "texture.fx")
                ->scale(0.2f)->init();
            break;
        case 2:
            mario->add<Model>(device, "skullocc.x", "texture.fx")
                ->scale(0.3)->init();
            break;
        case 3:
            mario->add<Model>(device, "Dwarf.x", "texture.fx")
                ->scale(4)->init();
            break;
        }
    }
};

DxMario::DxMario() : pImpl(new DxMario::private_implementation())
{

}

void DxMario::processEvent(const mage::WindowsEvent& evt)
{
    pImpl->processEvent(evt);
}

void DxMario::shutDown(IDirect3DDevice9* device)
{

}

void DxMario::paint(IDirect3DDevice9* device)
{
    pImpl->scene->draw(device);
}

bool DxMario::process(float time)
{
    pImpl->update(time);
    return true;
}

void DxMario::setup(IDirect3DDevice9* device)
{
    pImpl->setup(device);
}
