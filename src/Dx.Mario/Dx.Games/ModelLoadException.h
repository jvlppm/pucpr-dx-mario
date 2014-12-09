#pragma once
#include "HResultException.h"

namespace games {
	class ModelLoadException : public HResultException
	{
	public:
		ModelLoadException(const std::string& message, HRESULT hr) : HResultException(message, hr) {}
		~ModelLoadException() { }
	};
}
