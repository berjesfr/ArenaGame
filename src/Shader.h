#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;
public:
	Shader();
	void GetShaderSources(const char* vertexShaderPath, const char* fragmentShaderPath);
	unsigned int Use();
	void SetMatrix4(const char* name, glm::mat4& matrix);
	void SetInt(const char* name, int value);
	void SetVec2F(const char* name, float x, float y);
	void SetVec3F(const char* name, float x, float y, float z);
	void SetVec3F(const char* name, glm::vec3& value);
	void SetVec4F(const char* name, glm::vec4& value);
	void SetBoolean(const char* name, bool value);
	void SetFloat(const char* name, float value);
private:
	void CompileShaders(const char* vertexShaderSource, const char* fragmentShaderSource);
};