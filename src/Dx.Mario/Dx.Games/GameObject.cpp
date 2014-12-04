#include "GameObject.h"
#include <vector>
#include "Scene.h"
#include "ImmutableList.h"

using namespace games;

GameObject::GameObject()
{
	dirtyTransform = false;
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
	child->setParent(shared_from_this());
	children = children.add(child);
}

void GameObject::remove(std::shared_ptr<GameObject> child)
{
	child->parent_ref.reset();
	children = children.remove(child);
}

void GameObject::setParent(std::weak_ptr<GameObject> parent)
{
	parent_ref = parent;
	invalidateTransform();
}

D3DXVECTOR3 GameObject::worldPosition()
{
	auto worldMatrix = world();
	return D3DXVECTOR3(worldMatrix._41, worldMatrix._42, worldMatrix._43);
}

D3DXMATRIX GameObject::world()
{
	if (dirtyTransform)
		updateTransform();

	return globalTransform;
}

void games::GameObject::updateTransform()
{
	if (auto parent = parent_ref.lock())
		globalTransform = parent->world() * localTransform;
	else
		globalTransform = localTransform;

	dirtyTransform = false;
}

void GameObject::invalidateTransform() {
	dirtyTransform = true;
	for (auto c : *children)
		c->invalidateTransform();
}

void GameObject::translate(float x, float y, float z)
{
	transform([x, y, z](D3DXMATRIX& m) { D3DXMatrixTranslation(&m, x, y, z); });
}

void GameObject::rotateX(float angle)
{
	transform([angle](D3DXMATRIX& m) { D3DXMatrixRotationX(&m, angle); });
}

void GameObject::rotateY(float angle)
{
	transform([angle](D3DXMATRIX& m) { D3DXMatrixRotationY(&m, angle); });
}

void GameObject::rotateZ(float angle)
{
	transform([angle](D3DXMATRIX& m) { D3DXMatrixRotationZ(&m, angle); });
}