#include "HudElement.h"

HudElement::HudElement()
{
	p_Value = nullptr;
}

HudElement::HudElement(float* value, glm::vec2 position, glm::vec2 size, Shader& shader, Texture2D& texture)
	: Entity(position, size, 0.f, shader, texture), p_Value(value)
{

}


void HudElement::draw(Renderer& renderer)
{

	glm::vec2 updateSize = { p_Size.x * (*p_Value / 100.f), p_Size.y };
	p_Model = glm::mat4(1.0f);
	p_Model = glm::translate(p_Model, glm::vec3(p_Position, 0.0f));

	p_Model = glm::translate(p_Model, glm::vec3(.5f * updateSize.x, .5f * updateSize.y, 0.0f));
	p_Model = glm::rotate(p_Model, glm::radians(p_Rotation), { 0.0f, 0.0f, 1.0f });
	p_Model = glm::translate(p_Model, glm::vec3(-.5f * updateSize.x, -.5f * updateSize.y, 0.0f));

	p_Model = glm::scale(p_Model, glm::vec3(updateSize, 1.f));
	p_Shader.SetFloat("value", *p_Value);
	renderer.Draw(p_Shader, p_Texture, p_Model);
}