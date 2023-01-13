#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Texture.h"
#include "Renderer.h"
#include "Shader.h"

class PostProcessor
{
public:
	Shader p_Shader;
	Texture2D p_Texture;
	unsigned int p_Width, p_Height;
	bool p_Shake;
public:
	PostProcessor(Shader shader, unsigned int width, unsigned int height);
	void begin_render();
	void end_render();
	void render(float time);
private:
	unsigned int m_MSFBO = 0, m_FBO = 0;
	unsigned int m_RBO = 0;
	unsigned int m_VAO = 0;
private:
	void init();
};
