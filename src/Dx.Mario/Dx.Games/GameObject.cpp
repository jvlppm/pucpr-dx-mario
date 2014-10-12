#include "GameObject.h"
#include <vector>
#include "Scene.h"
#include "Utils.h"

using namespace games;

struct GameObject::private_implementation {
	private_implementation() : children(std::make_shared<std::vector<std::shared_ptr<GameObject>>>()) {

	}

	std::shared_ptr<std::vector<std::shared_ptr<GameObject>>> children;
};

GameObject::GameObject() : pImpl(new GameObject::private_implementation())
{
}


GameObject::~GameObject()
{
}

void games::GameObject::init()
{
	auto scene = getAncestor<Scene>();
	if (scene && scene != shared_from_this())
		scene->registerItem(shared_from_this());

	if (auto children = pImpl->children)
	{
		for (auto child : *children)
			child->init();
	}
}

void games::GameObject::destroy()
{
	auto toRemove = shared_from_this();

	auto scene = getAncestor<Scene>();
	if (scene && scene != toRemove)
		scene->unregisterItem(toRemove);

	if (auto parent = parent_ref.lock())
		parent->remove(toRemove);
}

void games::GameObject::add(std::shared_ptr<GameObject> child)
{
	child->parent_ref = shared_from_this();
	pImpl->children = Utils::add(pImpl->children, child);
}

void games::GameObject::remove(std::shared_ptr<GameObject> child)
{
	child->parent_ref.reset();
	pImpl->children = Utils::remove(pImpl->children, child);
}

D3DXVECTOR3 games::GameObject::globalPosition()
{
	return D3DXVECTOR3();
}
