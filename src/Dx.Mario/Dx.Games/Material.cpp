#include "Material.h"

using namespace games;
using namespace mage;

struct Material::private_implementation {
	private_implementation(std::string technique)
		: technique(technique)
	{
	}

	std::string technique;

	void activate(std::shared_ptr<Effect> effect) {
		effect->setTechnique(technique);
	}
};

Material::Material(std::string technique)
	: pImpl(new Material::private_implementation(technique))
{
}

Material::~Material()
{
}

void Material::activate(std::shared_ptr<Effect> effect)
{
	pImpl->activate(effect);
}
