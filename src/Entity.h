#pragma once


#include <glad/glad.h>
#include "Renderer.h"

class Entity
{
public:
	glm::vec2 p_Position;
	glm::vec2 p_Size;
	glm::mat4 p_Model;
	float p_Rotation;
	Shader p_Shader;
	Texture2D p_Texture;
public:
	Entity();
	Entity(glm::vec2 pos, glm::vec2 size, float rotation, Shader& shader, Texture2D& texture);
	virtual void move(Entity& entity, float dt, float elapsed_time);
	virtual void setup(Entity& entity);
	virtual void draw(Renderer& renderer);
};