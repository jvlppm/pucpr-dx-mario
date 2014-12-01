#include "GameObject.h"
#include <vector>
#include "Scene.h"
#include "ImmutableList.h"

using namespace games;

GameObject::GameObject()
{
	D3DXMatrixIdentity(&globalTransform);
	D3DXMatrixIdentity(&localTransform);
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

D3DXVECTOR3 GameObject::worldPosition()
{
	return D3DXVECTOR3(globalTransform._41, globalTransform._42, globalTransform._43);
}

D3DXMATRIX GameObject::world()
{
	return globalTransform;
}

void games::GameObject::updateTransform()
{
	if (auto parent = parent_ref.lock())
		globalTransform = parent->globalTransform * localTransform;
	else
		globalTransform = localTransform;

	for (auto c : *children)
		c->updateTransform();
}
