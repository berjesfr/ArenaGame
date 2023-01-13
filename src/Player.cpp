#include "Player.h"

Player::Player()
{}

Player::Player(Shader & shader, Texture2D & texture)
	: Entity({0.f, 0.f}, {32.f, 32.f}, 0.0f, shader, texture), p_Velocity({0.0f, 0.0f}), m_WalkState(0), p_Collider(AABBCollider({400.0f, 320.f}, {32.f, 32.f}))
{
}

void Player::setup(Entity& map)
{
	p_Shader.SetInt("numberOfRows", p_Texture.NumberOfRows);
	p_Shader.SetInt("numberOfColumns", p_Texture.NumberOfColumns);
	p_Position = dynamic_cast<Map&>(map).get_player_spawn_position();
	p_Collider.Update(this);
}


void Player::move(Entity& entity, float dt, float elapsedTime)
{
	p_Position += p_Velocity * dt;
	p_Collider.Update(this);
	if (p_Velocity.x != 0 || p_Velocity.y != 0) {
		animate_walk(dt);
	} else {
		p_Rotation = 0;
		m_WalkState = 0;
	}
	m_HitFrames = m_HitFrames > 0 ? m_HitFrames - 1 : 0;
	if (m_Damaged && m_HitFrames == 0) {
		m_Damaged = false;
		p_Velocity = { 0.0f, 0.0f };
	}
}

glm::vec2 Player::get_offset() const
{
	float column = m_TileID % p_Texture.NumberOfColumns;
	float xOffset = column / p_Texture.NumberOfColumns;

	float row = floor(m_TileID / p_Texture.NumberOfRows);
	float yOffset = row / p_Texture.NumberOfRows;
	return {xOffset, yOffset};
}

void Player::animate_walk(float dt)
{
	if (m_WalkState == 0) {
		m_WalkState = -1;
	}
	if (p_Rotation > 5.f) {
		m_WalkState = -1;
	}
	if (p_Rotation < -5.f) {
		m_WalkState = 1;
	}
	p_Rotation += m_WalkState * 100.0f * dt;
}

void Player::draw(Renderer & renderer)
{
	glm::vec2 offset = get_offset();
	p_Shader.SetVec2F("offset", offset.x, offset.y);
	p_Shader.SetBoolean("hit", got_hit());
	p_Model = glm::mat4(1.0f);
	p_Model = glm::translate(p_Model, glm::vec3(p_Position, 0.0f));
	p_Model = glm::translate(p_Model, glm::vec3(.5f * p_Size.x, .5f * p_Size.y, 0.0f));
	p_Model = glm::rotate(p_Model, glm::radians(p_Rotation), { 0.0f, 0.0f, 1.0f });
	p_Model = glm::translate(p_Model, glm::vec3(-.5f * p_Size.x, -.5f * p_Size.y, 0.0f));
	p_Model = glm::scale(p_Model, glm::vec3(p_Size, 1.f));
	renderer.Draw(p_Shader, p_Texture, p_Model);
}

void Player::damage(float dmg)
{
	if (!m_Damaged) {
		m_Damaged = true;
		m_HitFrames = 120;
		p_Health -= dmg;
		if (p_Health <= 0) {
			p_Rotation = 90.0f;
		}
	}
}

void Player::reset(Map& map)
{
	m_WalkState = 0;
	p_Health = 100.0f;
	p_Velocity = { 0.0f, 0.0f };
	p_Rotation = 0;
	m_Damaged = false;
	m_HitFrames = 0;
	p_Position = map.get_player_spawn_position();
	p_Collider.Update(this);
}
