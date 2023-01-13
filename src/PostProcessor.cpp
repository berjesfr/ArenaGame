#include "PostProcessor.h"

PostProcessor::PostProcessor(Shader shader, unsigned int width, unsigned int height)
	: p_Shader(shader), p_Texture(), p_Width(width), p_Height(height), p_Shake(false)
{
	glCreateFramebuffers(1, &m_MSFBO);
	glCreateRenderbuffers(1, &m_RBO);

	glNamedRenderbufferStorageMultisample(m_RBO, 4, GL_RGB, width, height);
	glNamedFramebufferRenderbuffer(m_MSFBO, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_RBO);
	if (glCheckNamedFramebufferStatus(m_MSFBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;

	glCreateFramebuffers(1, &m_FBO);
	p_Texture.Generate(width, height);

	glNamedFramebufferTexture(m_FBO, GL_COLOR_ATTACHMENT0, p_Texture.ID, 0);
	auto status = glCheckNamedFramebufferStatus(m_FBO, GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	init();
	p_Shader.SetInt("scene", 0);
}

void PostProcessor::init()
{
	unsigned int VBO;
	float vertices[] = {
		// pos        // tex
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};
	
	glCreateVertexArrays(1, &m_VAO);
	glCreateBuffers(1, &VBO);

	glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexArrayAttrib(m_VAO, 0);
	glVertexArrayAttribBinding(m_VAO, 0, 0);
	glVertexArrayAttribFormat(m_VAO, 0, 4, GL_FLOAT, GL_FALSE, 0);

	glVertexArrayVertexBuffer(m_VAO, 0, VBO, 0, 4 * sizeof(float));
}

void PostProcessor::begin_render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO);
	glClearColor(0.46f, 0.23f, 0.21f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
void PostProcessor::end_render()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
	glBlitFramebuffer(0, 0, p_Width, p_Height, 0, 0, p_Width, p_Height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::render(float time)
{
	p_Shader.Use();
	p_Shader.SetFloat("time", time);
	p_Shader.SetInt("shake", p_Shake);
	// render textured quad
	p_Texture.BindTexture();
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}