#include "DxMario.h"
#include "mage/TString.h"
#include <memory>
#include "Resources.h"
#include "ResourceLoader.h"
#include "Model.h"
#include "Container.h"
#include "PostEffects.h"

using namespace mario;
using namespace games;
using namespace mage;
using namespace std;

struct DxMario::private_implementation {
    shared_ptr<Scene> scene;
    shared_ptr<Model> castle;
    shared_ptr<Container> mario;
    shared_ptr<Container> luigi;
    shared_ptr<Camera> camera;

    D3DXVECTOR3 toMove = D3DXVECTOR3(0, 0, 0);
    D3DXVECTOR3 toMoveCam = D3DXVECTOR3(0, 0, 0);

    void setup(IDirect3DDevice9* device)
    {
        Resources::setLoader(make_shared<ResourceLoader>());

        createScene(device);

        auto postEffects = make_shared<PostEffects>();
        //postEffects->setEffect("Borders");
        postEffects->setEffect("Toon");
        camera = scene->add<Camera>()
            ->setPerspective(60, 1, 5000)
            ->translate(-48, 11, -67)
            ->lookAt(mario->worldPosition())
            ->setShader(postEffects);

        scene->lightDir = mario->worldPosition() - camera->worldPosition();
        scene->lightDir.x += 1;
        D3DXVec3Normalize(&scene->lightDir, &scene->lightDir);

        scene->init();
    }

    void createScene(IDirect3DDevice9* device) {
        scene = std::make_shared<Scene>();
        scene->diffuseColor *= 1.5;
        scene->specularColor /= 20;

        castle = scene->add<Model>(device, "Peach's Castle.x", "texture.fx")
            ->scale(0.01f);

        luigi = scene->add<Container>()
            ->translate(-56, 8.25, -68)
            ->rotateY(1);
        luigi->add<Model>(device, "Luigi.x", "texture.fx")
            ->scale(0.2f);

        mario = scene->add<Container>()
            ->translate(-56, 8, -64)
            ->rotateY(-1.2);
        mario->add<Model>(device, "Mario.x", "texture.fx")
            ->scale(0.2f);
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
                case 'A':
                    toMove.x += 1;
                    break;
                case 'D':
                    toMove.x -= 1;
                    break;
                case 'W':
                    toMove.z -= 1;
                    break;
                case 'S':
                    toMove.z += 1;
                    break;
                case VK_SPACE:
                    toMove.y += 1;
                    toMoveCam.y += 1;
                    break;
                case VK_CONTROL:
                    toMove.y -= 1;
                    toMoveCam.y -= 1;
                    break;

                case VK_UP:
                    toMoveCam.x += 1;
                    break;
                case VK_DOWN:
                    toMoveCam.x -= 1;
                    break;
                case VK_LEFT:
                    toMoveCam.z -= 1;
                    break;
                case VK_RIGHT:
                    toMoveCam.z += 1;
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
