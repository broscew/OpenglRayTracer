#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//Wrapper class for managing compute shaders for EZEngine
namespace Graph
{
	class ComputeShader
	{
	public:
		ComputeShader(const char* computePath);
		void Init();
		void Use();

		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int  value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetFloat3(const std::string& name, float v1, float v2, float v3) const;

		unsigned int mID;
		int workGroupSizeX, workGroupSizeY;
	private:
		void CheckCompileErrors(unsigned int shader, std::string type);

	};
}