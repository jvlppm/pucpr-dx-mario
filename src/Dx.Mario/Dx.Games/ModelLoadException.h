#pragma once
#include <string>
#include <winerror.h>

namespace games {
	class ModelLoadException
	{
	public:
		const std::string message;
		const HRESULT result;

		ModelLoadException(const std::string& message, HRESULT result) : message(message), result(result) {}
		~ModelLoadException() { }

		const char* what() const throw() { return message.c_str(); }
	};
}
