#pragma once
#include "mage\Effect.h"
#include <memory>
namespace games {
	// "gMaterial", color
	class Material
	{
	public:
		Material(std::string technique);
		~Material();

		void activate(std::shared_ptr<mage::Effect> effect);
	private:
		struct private_implementation;
		std::unique_ptr<private_implementation> pImpl;
	};
}
