#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Texture.h"


class Renderer
{
public:
	Renderer();
	void Setup();
	void Draw(Shader& shader, Texture2D& texture, glm::mat4& model);
private:
	unsigned int m_VAO;
};