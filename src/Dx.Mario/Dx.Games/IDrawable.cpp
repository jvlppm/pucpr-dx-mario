#include "IDrawable.h"
#include "GameObject.h"

using namespace games;

struct IDrawable::private_implementation {
	private_implementation() {
		isVisible = true;
		couldRender = true;
	}

	IDrawable* self;
	bool isVisible;
	bool couldRender;

	void disableChildRendering()
	{
		if (auto obj = dynamic_cast<GameObject*>(self))
		{
			for (auto child : obj->findChildren<IDrawable>())
			{
				if (!child->pImpl->couldRender)
					continue;
				child->pImpl->couldRender = false;
				child->pImpl->disableChildRendering();
			}
		}
	}

	void enableChildRendering()
	{
		if (auto obj = dynamic_cast<GameObject*>(self))
		{
			for (auto child : obj->findChildren<IDrawable>())
			{
				child->pImpl->couldRender = true;
				if (!child->pImpl->isVisible)
					continue;
				child->pImpl->enableChildRendering();
			}
		}
	}

	bool canRender()
	{
		return couldRender && isVisible;
	}

	void setVisible(bool visible)
	{
		isVisible = visible;
		if (!couldRender)
			return;

		if (!visible)
			disableChildRendering();
		else
			enableChildRendering();
	}
};


IDrawable::IDrawable() : pImpl(new IDrawable::private_implementation())
{
	pImpl->self = this;
}

IDrawable::~IDrawable()
{
}

bool IDrawable::canRender() {
	return pImpl->canRender();
}

void IDrawable::setVisible(bool visible)
{
	pImpl->setVisible(visible);
}

void Test()
{
	GameObject a;
	auto b = a.findChildren<IDrawable>();
}