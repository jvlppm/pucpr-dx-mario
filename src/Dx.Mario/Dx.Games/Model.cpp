#include "Model.h"
#include "Mesh.h"
#include "Resources.h"
#include "mage/Effect.h"

using namespace games;
using namespace mage;
using namespace std;

struct Model::private_implementation {
    private_implementation() {
    }

    ~private_implementation() {
    }

    void setShader(shared_ptr<IShader> shader)
    {
        this->shader = shader;
    }

    void setMesh(shared_ptr<Mesh> mesh)
    {
        this->mesh = mesh;
    }

    void draw(IDirect3DDevice9* device, shared_ptr<Scene> scene, shared_ptr<Camera> camera, shared_ptr<Model> self) {
        shader->begin(device);
        shader->setScene(scene);
        shader->setCamera(camera);
        shader->setObject(static_pointer_cast<BaseObject>(self));

        for (unsigned int j = 0; j < mesh->materials.size(); j++) {
            auto texture = mesh->textures[j];
            if (!texture)
                continue;

            shader->setMaterial(mesh->materials[j]);
            shader->setTexture(texture);
            shader->execute([this, j](IDirect3DDevice9* device) {
                mesh->data->DrawSubset(j);
            });
        }

        shader->end();
    }
private:
    std::shared_ptr<IShader> shader;
    shared_ptr<Mesh> mesh;
};

Model::Model() : pImpl(new Model::private_implementation())
{
}

Model::Model(IDirect3DDevice9* device, const std::string& mesh, const std::string& shader)
    : Model()
{
    pImpl->setMesh(Resources::getMesh(device, mesh));
    pImpl->setShader(Resources::getShader(device, shader));
}

Model::~Model()
{
}

void Model::draw(IDirect3DDevice9* device, shared_ptr<Scene> scene, shared_ptr<Camera> camera)
{
    pImpl->draw(device, scene, camera, shared_from_this());
}

D3DXMATRIX Model::world()
{
    return BaseObject::world();
}

D3DXVECTOR3 Model::worldPosition()
{
    return BaseObject::worldPosition();
}

std::shared_ptr<Model> Model::setShader(shared_ptr<IShader> shader)
{
    pImpl->setShader(shader);
    return shared_from_this();
}

std::shared_ptr<Model> Model::setMesh(shared_ptr<Mesh> mesh)
{
    pImpl->setMesh(mesh);
    return shared_from_this();
}
