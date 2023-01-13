#include "Renderer.h"

Renderer::Renderer()
	: m_VAO(0)
{

}

void Renderer::Setup()
{
	unsigned int VBO;

	float vertices[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glCreateVertexArrays(1, &m_VAO);
	glCreateBuffers(1, &VBO);
	glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(m_VAO, 0);
	glVertexArrayAttribBinding(m_VAO, 0, 0);
	glVertexArrayAttribFormat(m_VAO, 0, 4, GL_FLOAT, GL_FALSE, 0);

	glVertexArrayVertexBuffer(m_VAO, 0, VBO, 0, 4 * sizeof(float));
}

void Renderer::Draw(Shader& shader, Texture2D& texture, glm::mat4& model)
{
	shader.Use();
	texture.BindTexture();
	glBindVertexArray(m_VAO);
	shader.SetMatrix4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
