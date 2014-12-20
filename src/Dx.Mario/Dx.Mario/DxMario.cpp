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
    shared_ptr<Model> skull;
    shared_ptr<Model> dwarf;

    void setup(IDirect3DDevice9* device)
    {
        Resources::setLoader(make_shared<ResourceLoader>());

        scene = std::make_shared<Scene>();
        scene->diffuseColor = D3DXVECTOR3(1.0f, 1.0f, 0.8f);
        scene->ambientColor = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
        scene->specularColor /= 20;

        auto postEffects = make_shared<PostEffects>();
        postEffects->setEffect("Toon");

		auto camera = scene->add<Camera>()
			->setPerspective(60, 1, 5000)
			->translate(0.0f, 4.0f, -10.0f)
			->lookAt(D3DXVECTOR3(0, 6, 0))
            ->setShader(postEffects)
			;

		skull = scene->add<Model>(device, "skullocc.x", "texture.fx")
            ->translate(3, 2, 0)
			->scale(0.6f);
        dwarf = scene->add<Model>(device, "Dwarf.x", "texture.fx")
            ->translate(-3, 2, 0)
            ->scale(4);

        scene->init();
    }

    void update(float time) {
		if (skull)
			skull->rotateY(time);
		if (dwarf)
			dwarf->rotateY(-time);
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
