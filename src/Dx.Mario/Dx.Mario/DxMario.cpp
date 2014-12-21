#include "DxMario.h"
#include "mage/TString.h"
#include <memory>
#include "Resources.h"
#include "ResourceLoader.h"
#include "Model.h"
#include "PostEffects.h"

using namespace mario;
using namespace games;
using namespace mage;
using namespace std;

struct DxMario::private_implementation {
    shared_ptr<Scene> scene;
    shared_ptr<Model> mario;
    shared_ptr<Model> luigi;

    void setup(IDirect3DDevice9* device)
    {
        Resources::setLoader(make_shared<ResourceLoader>());

        scene = std::make_shared<Scene>();
        scene->diffuseColor *= 1.5;
        D3DXVec3Normalize(&scene->lightDir, &scene->lightDir);
        scene->specularColor /= 20;

        auto postEffects = make_shared<PostEffects>();
        postEffects->setEffect("Toon");

		auto camera = scene->add<Camera>()
			->setPerspective(60, 1, 5000)
			->translate(0.0f, 6.0f, -10.0f)
			->lookAt(D3DXVECTOR3(0, 2, 0))
            ->setShader(postEffects)
			;

		
        luigi = scene->add<Model>(device, "Luigi.x", "texture.fx")
             ->scale(0.4f)
             ->translate(-6, -2, 0);

        mario = scene->add<Model>(device, "Mario.x", "texture.fx")
            ->scale(0.4f)
            ->translate(6, -2, 0);

        scene->init();
    }

    void update(float time) {
		if (luigi)
			luigi->rotateY(time);
        if (mario)
            mario->rotateY(-time);
        scene->update(time);
    }
};

DxMario::DxMario() : pImpl(new DxMario::private_implementation())
{

}

void DxMario::processEvent(const mage::WindowsEvent& evt)
{
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
