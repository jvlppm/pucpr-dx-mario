#include "GameObject.h"
#include <vector>
#include "Scene.h"
#include "ImmutableList.h"

using namespace games;
using namespace std;

BaseObject::BaseObject()
{
	dirtyTransform = false;
	D3DXMatrixIdentity(&globalTransform);
	D3DXMatrixIdentity(&localTransform);
}

BaseObject::~BaseObject()
{
}

void BaseObject::init()
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

void BaseObject::destroy()
{
	auto toRemove = shared_from_this();

	auto scene = getAncestor<Scene>();
	if (scene && scene != toRemove)
		scene->unregisterItem(toRemove);

	if (auto parent = parent_ref.lock())
		parent->remove(toRemove);
}

void BaseObject::add(shared_ptr<BaseObject> child)
{
	child->setParent(shared_from_this());
	children = children.add(child);
}

void BaseObject::remove(shared_ptr<BaseObject> child)
{
	child->parent_ref.reset();
	children = children.remove(child);
}

void BaseObject::setParent(weak_ptr<BaseObject> parent)
{
	parent_ref = parent;
	invalidateTransform();
}

shared_ptr<BaseObject> BaseObject::getParent()
{
	return parent_ref.lock();
}

D3DXVECTOR3 BaseObject::worldPosition()
{
	auto worldMatrix = world();
	return D3DXVECTOR3(worldMatrix._41, worldMatrix._42, worldMatrix._43);
}

D3DXMATRIX BaseObject::world()
{
	if (dirtyTransform)
		updateTransform();

	return globalTransform;
}

void BaseObject::updateTransform()
{
	if (auto parent = parent_ref.lock())
		globalTransform = parent->world() * localTransform;
	else
		globalTransform = localTransform;

	dirtyTransform = false;
}

void BaseObject::invalidateTransform() {
	dirtyTransform = true;
	for (auto c : *children)
		c->invalidateTransform();
}
