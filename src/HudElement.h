#pragma once

#include "Entity.h"

class HudElement : public Entity
{
public:
	float *p_Value;
public:
	HudElement();
	HudElement(float *value, glm::vec2 position, glm::vec2 size, Shader& shader, Texture2D& texture);
	~HudElement()
	{
		delete p_Value;
	}
	void draw(Renderer& renderer) override;
};