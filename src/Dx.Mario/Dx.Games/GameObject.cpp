#include "GameObject.h"
#include <vector>
#include "Scene.h"
#include "ImmutableList.h"

using namespace games;

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::init()
{
	auto scene = getAncestor<Scene>();
	if (scene && scene != shared_from_this())
		scene->registerItem(shared_from_this());

	if (auto childrenLock = children)
	{
		for (auto child : *childrenLock)
			child->init();
	}
}

void GameObject::destroy()
{
	auto toRemove = shared_from_this();

	auto scene = getAncestor<Scene>();
	if (scene && scene != toRemove)
		scene->unregisterItem(toRemove);

	if (auto parent = parent_ref.lock())
		parent->remove(toRemove);
}

void GameObject::add(std::shared_ptr<GameObject> child)
{
	child->parent_ref = shared_from_this();
	children = children.add(child);
}

void GameObject::remove(std::shared_ptr<GameObject> child)
{
	child->parent_ref.reset();
	children = children.remove(child);
}

D3DXVECTOR3 GameObject::globalPosition()
{
	return D3DXVECTOR3();
}
