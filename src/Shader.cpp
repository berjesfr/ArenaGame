#include "Shader.h"

Shader::Shader()
	: ID(0)
{}

void Shader::GetShaderSources(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(vertexShaderPath);
		fShaderFile.open(fragmentShaderPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	} catch (std::ifstream::failure e) {
		std::cout << "SHADER LOAD FAILURE" << std::endl;
	}

	CompileShaders(vertexCode.c_str(), fragmentCode.c_str());
}

void Shader::CompileShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	unsigned int vertex, fragment;

	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR SHADER VERTEX" << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR SHADER FRAGMENT" << std::endl;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR SHADER PROGRAM LINKING FAILED" << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

unsigned int Shader::Use()
{
	glUseProgram(ID);
	return ID;
}

void Shader::SetMatrix4(const char* name, glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(this->Use(), name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetInt(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(this->Use(), name), value);
}

void Shader::SetVec2F(const char* name, float x, float y)
{
	glUniform2f(glGetUniformLocation(this->Use(), name), x, y);
}

void Shader::SetVec3F(const char* name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(this->Use(), name), x, y, z);
}

void Shader::SetVec3F(const char* name, glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(this->Use(), name), 1, glm::value_ptr(value));
}

void Shader::SetVec4F(const char* name, glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(this->Use(), name), 1, glm::value_ptr(value));
}

void Shader::SetBoolean(const char* name, bool value)
{
	glUniform1i(glGetUniformLocation(this->Use(), name), value);
}

void Shader::SetFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(this->Use(), name), value);
}