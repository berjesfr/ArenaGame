#include "Entity.h"

Entity::Entity()
	: p_Position(0.0f, 0.0f), p_Size(0.0f, 0.0f), p_Rotation(0.0f), p_Shader(), p_Texture()
{}

Entity::Entity(glm::vec2 pos, glm::vec2 size, float rotation, Shader & shader, Texture2D & texture)
	: p_Position(pos), p_Size(size), p_Rotation(rotation), p_Shader(shader), p_Texture(texture)
{}

void Entity::move(Entity& entity, float dt, float elapsedTime)
{
	p_Position += dt;
}

void Entity::setup(Entity& entity)
{
	p_Shader.SetInt("numberOfRows", p_Texture.NumberOfRows);
	p_Shader.SetInt("numberOfColumns", p_Texture.NumberOfColumns);
	p_Position = entity.p_Position;
	p_Rotation = 0;
}

void Entity::draw(Renderer& renderer)
{
	p_Model = glm::mat4(1.0f);
	renderer.Draw(p_Shader, p_Texture, p_Model);
}