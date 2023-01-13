#include "Menu.h"

Menu::Menu(unsigned int width, unsigned int height)
{
	m_Width = width;
	m_Height = height;
	m_ButtonShader = new Shader();
	m_ButtonShader->GetShaderSources("./src/shaders/button_vertex.glsl", "./src/shaders/button_fragment.glsl");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
	m_ButtonShader->SetMatrix4("projection", projection);
	m_PlayButton.size = glm::vec2(256.f, 82.f);
	m_PlayButton.position = glm::vec2((width / 2.f) - (m_PlayButton.size.x / 2), (height / 2.f) - (m_PlayButton.size.y / 2));
	m_PlayButton.color = glm::vec4(.9f, .6f, .6f, .5f);
	m_PlayButton.text = "Play";

	m_ExitButton.size = glm::vec2(128.f, 56.f);
	m_ExitButton.position = glm::vec2((width / 2.f) - (m_ExitButton.size.x / 2), m_PlayButton.position.y + 128.f);
	m_ExitButton.color = glm::vec4(.9f, .6f, .6f, .5f);
	m_ExitButton.text = "Exit";
	m_PlayButton.init();
	m_ExitButton.init(); 
}

ClickAction Menu::check_for_clicks(glm::vec2 mousePosition)
{
	if (m_PlayButton.check_for_hovering(mousePosition)) {
		m_PlayButton.color.w = 1.0f;
		if (p_MouseClick) {
			return ClickAction::START;
		}
	} else {
		m_PlayButton.color.w = 0.5f;
	}

	if (m_ExitButton.check_for_hovering(mousePosition)) {
		m_ExitButton.color.w = 1.0f;
		if (p_MouseClick) {
			return ClickAction::EXIT;
		}
	} else {
		m_ExitButton.color.w = 0.5f;
	}
	return ClickAction::NONE;
}

void Menu::draw_buttons(TextRenderer& textRenderer)
{
	m_PlayButton.draw(*m_ButtonShader, textRenderer);
	m_ExitButton.draw(*m_ButtonShader, textRenderer);
	textRenderer.render_text("Arena", m_Width / 2.f - textRenderer.p_FontSize * 5, 200.f, 3.0f, { 1.0f, 1.0f, 1.0f });
}