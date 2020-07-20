#include "ComputeShaders.h"
#include "GL/glew.h"

namespace Graph
{
	ComputeShader::ComputeShader(const char* computePath)
	{
		//Retrive the code from the shader file provided
		std::string computeCode;
		std::ifstream cShaderFile;

		// ensure ifstream object can throw exceptions
		cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			//open the file
			cShaderFile.open(computePath);
			std::stringstream cShaderStream;

			//read the file's buffer contents in to streams
			cShaderStream << cShaderFile.rdbuf();

			//close the file stream
			cShaderFile.close();

			//Convert to string
			computeCode = cShaderStream.str();

		}
		catch ([[maybe_unused]] std::ifstream::failure& e )
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* cShaderCode = computeCode.c_str();

		//Compile the shader
		unsigned int compute;

		compute = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(compute, 1, &cShaderCode, NULL);
		glCompileShader(compute);

		CheckCompileErrors(compute, "COMPUTE");

		mID = glCreateProgram();
		glAttachShader(mID, compute);
		glLinkProgram(mID);

		CheckCompileErrors(mID, "PROGRAM");

		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(compute);

	}


	void ComputeShader::Init()
	{
		glUseProgram(mID);
		int buffer[3];
		glGetProgramiv(mID, GL_COMPUTE_WORK_GROUP_SIZE, buffer);
		workGroupSizeX = buffer[0];
		workGroupSizeY = buffer[1];

	}

	void ComputeShader::Use()
	{
		glUseProgram(mID);
	}

	void ComputeShader::SetBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
	}

	void ComputeShader::SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(mID, name.c_str()), value);
	}

	void ComputeShader::SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(mID, name.c_str()), value);
	}

	void ComputeShader::SetFloat3(const std::string& name, float v1, float v2, float v3) const
	{
		glUniform3f(glGetUniformLocation(mID, name.c_str()), v1, v2, v3);
	}

	void ComputeShader::CheckCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
			else
			{
				std::cout << type << " Compiled Successfully" << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

}