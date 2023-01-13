#include "Enemy.h"

Enemy::Enemy() {}

Enemy::Enemy(Shader& shader, Texture2D& texture)
	: Entity({ 0.0f, 0.0f }, { 32.0f, 32.0f }, 0.0f, shader, texture), p_Collider(AABBCollider({ 400.0f, 320.f }, { 32.f, 32.f }))
{}

void Enemy::setup(Entity& map)
{
	p_Shader.SetInt("numberOfRows", p_Texture.NumberOfRows);
	p_Shader.SetInt("numberOfColumns", p_Texture.NumberOfColumns);
	p_Velocity = { 0.0f, 0.0f };
	p_Health = 100;
	p_Rotation = 0;
	m_Speed = (rand() % 64) + 64.f;
	p_Position = dynamic_cast<Map&>(map).get_enemy_a_spawn_position();
	p_Collider.Update(this);
}

glm::vec2 Enemy::get_offset() const
{
	float column = m_TileID % p_Texture.NumberOfColumns;
	float xOffset = column / p_Texture.NumberOfColumns;

	float row = floor(m_TileID / p_Texture.NumberOfRows);
	float yOffset = row / p_Texture.NumberOfRows;
	return { xOffset, yOffset };
}

void Enemy::move(Entity& player, float dt, float elapsed_time)
{
	if (p_Health > 0.f) {
		if (!p_Damaged) {
			glm::vec2 direction = player.p_Position - p_Position;
			float sumOfSquares = direction.x * direction.x + direction.y * direction.y;
			float directionLength = std::abs(sqrtf(sumOfSquares));
			if (directionLength > 32.0f)
				p_Position += (direction / directionLength) * m_Speed * dt;
		}
		else {
			m_Hitframes = m_Hitframes > 0 ? m_Hitframes - 1 : 0;
			p_Position += p_Velocity * dt;
			if (m_Hitframes == 0) {
				p_Damaged = false;
				p_Velocity = { 0.0f, 0.0f };
			}
		}
		p_Collider.Update(this);
	}
}

void Enemy::damage(float dmg)
{
	if (!p_Damaged) {
		p_Damaged = true;
		m_Hitframes = 120;
		p_Health -= dmg;
		if (p_Health <= 0) {
			p_Rotation = 90.0f;
		}
	}
}


void Enemy::draw(Renderer& renderer)
{
	glm::vec2 offset = get_offset();
	p_Shader.SetVec2F("offset", offset.x, offset.y);
	p_Shader.SetBoolean("hit", got_hit());
	p_Model = glm::mat4(1.0f);
	p_Model = glm::translate(p_Model, { p_Position, 0.0f });
	p_Model = glm::translate(p_Model, { .5f * p_Size.x, .5f * p_Size.y, 0.0f });
	p_Model = glm::rotate(p_Model, glm::radians(p_Rotation), { 0.0f, 0.0f, 1.0f });
	p_Model = glm::translate(p_Model, { -.5f * p_Size.x, -.5f * p_Size.y, 0.0f });
	p_Model = glm::scale(p_Model, { p_Size, 1.0f });
	renderer.Draw(p_Shader, p_Texture, p_Model);
}

