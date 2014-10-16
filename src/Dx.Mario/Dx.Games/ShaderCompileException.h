#pragma once
#include <string>
namespace games {
	class ShaderCompileException
	{
	public:
		std::string message;

		ShaderCompileException(std::string message) : message(message) { }
		~ShaderCompileException()	{ }

		const char* what() const throw() { return message.c_str(); }
	};
}
