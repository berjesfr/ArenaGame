#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Shader.h"
#include "TextRenderer.h"

struct Button
{
	std::string text;
	glm::vec2 position;
	glm::vec2 size;
	glm::vec4 color;
	float rotation = 0;
	unsigned int VAO = 0;

	void init()
	{
		unsigned int VBO;
		float vertices[] = {
			0.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		};

		glCreateVertexArrays(1, &VAO);
		glCreateBuffers(1, &VBO);
		glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexArrayAttrib(VAO, 0);
		glVertexArrayAttribBinding(VAO, 0, 0);
		glVertexArrayAttribFormat(VAO, 0, 2, GL_FLOAT, GL_FALSE, 0);

		glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 2 * sizeof(float));
	}

	bool check_for_hovering(glm::vec2 mousePosition)
	{
		bool mouseXHovering = mousePosition.x >= position.x && mousePosition.x <= position.x + size.x;
		bool mouseYHovering = mousePosition.y >= position.y && mousePosition.y <= position.y + size.y;

		return mouseXHovering && mouseYHovering;
	}

	void draw(Shader& shader, TextRenderer& textRenderer)
	{
		shader.Use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, {position, 0.0f});
		model = glm::translate(model, { 0.5f * size.x, 0.5f * size.y, 0.f });
		model = glm::rotate(model, glm::radians(rotation), { 0.0f, 0.0f, 1.0f });
		model = glm::translate(model, { -0.5f * size.x, -0.5f * size.y, 0.f });
		model = glm::scale(model, { size, 1.0f });
		glBindVertexArray(VAO);
		shader.SetMatrix4("model", model);
		shader.SetVec4F("color", color);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		textRenderer.render_text(text, position.x + (size.x / 2) - (textRenderer.p_FontSize * 1.f) - text.length(),
										position.y + (size.y / 2) - 8.f, 
										1.f, { 1.0f, 1.0f, 1.0f });
	}
};

enum class ClickAction
{
	NONE,
	START,
	EXIT
};

class Menu
{
public:
	bool p_MouseClick = false;
public:
	Menu(unsigned int width, unsigned int height);
	~Menu()
	{
		delete m_ButtonShader;
	}
	ClickAction check_for_clicks(glm::vec2 mousePosition);
	void draw_buttons(TextRenderer& textRenderer);
private:
	Shader* m_ButtonShader;
	Button m_PlayButton;
	Button m_ExitButton;
	unsigned int m_Width;
	unsigned int m_Height;
};