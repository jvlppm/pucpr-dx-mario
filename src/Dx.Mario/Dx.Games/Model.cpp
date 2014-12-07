#include "Model.h"

using namespace games;
using namespace std;

struct Model::private_implementation {
    private_implementation() {
    }

    ~private_implementation() {
    }

    void draw(IDirect3DDevice9* device, shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<Model> self) {
        shader->begin(device);
        shader->setScene(scene);
        shader->setCamera(camera);
        shader->setObject(static_pointer_cast<BaseObject>(self));

        for (unsigned int j = 0; j < mesh->materials.size(); j++) {
            //Se tiver a textura, usa. Caso contrário usa a default
            auto texture = mesh->textures[j];
            if (!texture)
                continue;

            shader->setMaterial(mesh->materials[j]);
            shader->setTexture(texture);
            shader->execute([this, j](IDirect3DDevice9* device)
            {
                mesh->data->DrawSubset(j);
            });
        }

        shader->end();
    }
private:
    std::shared_ptr<IShader> shader;
};

Model::Model() : pImpl(new Model::private_implementation())
{
}

Model::~Model()
{
}

void Model::draw(IDirect3DDevice9* device, shared_ptr<Scene> scene, shared_ptr<Camera> camera)
{
    pImpl->draw(device, scene, camera, shared_from_this());
}
