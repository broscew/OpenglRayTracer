#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//Wrapper class for managing shaders for EZEngine
namespace Graph
{
	class Shader
	{
	public:
		Shader(const char* vertexPath, const char* fragmentPath);
		void Use();
		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int  value) const;
		void SetFloat(const std::string& name, float value) const;

		unsigned int mID;
	private:
		void CheckCompileErrors(unsigned int shader, std::string type);

	};
}