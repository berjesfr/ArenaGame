#include "Weapon.h"


float lerp(float a, float b, float t)
{
	return a + ((b - a) * t);

}

Weapon::Weapon()
{}

Weapon::Weapon(Shader& shader, Texture2D& texture)
	: Entity({0.f, 0.f}, {32.f, 32.f}, 0.0f, shader, texture)
{}

void Weapon::setup(Entity& player)
{
	p_Shader.SetInt("numberOfRows", p_Texture.NumberOfRows);
	p_Shader.SetInt("numberOfColumns", p_Texture.NumberOfColumns);
	p_Position = player.p_Position + glm::vec2(32.f, 0.0f);
	m_RotationPoint = dynamic_cast<Player&>(player).p_Collider.center;
	m_AttackState = AttackStates::NEUTRAL;
	p_Collider.Update(this);
}


void Weapon::move(Entity& player, float dt, float elapsedTime)
{
	p_Position = player.p_Position + glm::vec2(32.f, 0.0f);
	m_RotationPoint = dynamic_cast<Player&>(player).p_Collider.center;
}


void Weapon::look_at(glm::vec2 mousePosition)
{
	if (!p_Attacking && m_AttackState == AttackStates::NEUTRAL) {
		float opposite = mousePosition.y - p_Position.y;
		float adjacent = mousePosition.x - p_Position.x;
		float toa = opposite / adjacent;
		p_Rotation = (atan2(opposite, adjacent) * 180.f) / 3.14f;
		m_PreviousRotation = p_Rotation;
	}
}


void Weapon::attack(float dt)
{
	m_AttackStateCounter++;
	if (m_AttackState == AttackStates::NEUTRAL && p_Attacking) {
		m_AttackState = AttackStates::SWING_ONE;
		m_AttackStateCounter = 0;
	}

	if (m_AttackState == AttackStates::SWING_ONE) {
		p_Rotation = lerp(p_Rotation, m_PreviousRotation + 45.f, m_AttackStateCounter / 20.f);
	} else if (m_AttackState == AttackStates::SWING_TWO) {
		p_Rotation = lerp(p_Rotation, m_PreviousRotation - 45.f, m_AttackStateCounter / 20.f);
	}

	if (m_AttackStateCounter >= 20.0f) {
		if (m_AttackState == AttackStates::SWING_ONE) {
			m_AttackState = AttackStates::SWING_TWO;
			m_AttackStateCounter = 0;
		}
		else if (m_AttackState == AttackStates::SWING_TWO) {
			m_AttackState = AttackStates::NEUTRAL;
			m_AttackStateCounter = 0;
			p_Attacking = false;
		}
	}
}

glm::vec2 Weapon::get_offset() const
{
	float column = m_TileID % p_Texture.NumberOfColumns;
	float xOffset = column / p_Texture.NumberOfColumns;

	float row = floor(m_TileID / p_Texture.NumberOfRows);
	float yOffset = row / p_Texture.NumberOfRows;
	return {xOffset, yOffset};
}

void Weapon::fix_position()
{
	float xnew = ((p_Position.x + p_Size.x / 2) - m_RotationPoint.x) * cos(glm::radians(p_Rotation)) - ((p_Position.y + p_Size.y / 2) - m_RotationPoint.y) * sin(glm::radians(p_Rotation));
	float ynew = ((p_Position.x + p_Size.x / 2) - m_RotationPoint.x) * sin(glm::radians(p_Rotation)) + ((p_Position.y + p_Size.y / 2) - m_RotationPoint.y) * cos(glm::radians(p_Rotation));

	p_Position.x = (xnew - p_Size.x / 2) + m_RotationPoint.x;
	p_Position.y = (ynew - p_Size.y / 2) + m_RotationPoint.y;

}

void Weapon::build_model_matrix()
{
	p_Model = glm::mat4(1.0f);
	p_Model = glm::translate(p_Model, glm::vec3(p_Position, 0.0f));

	p_Model = glm::translate(p_Model, glm::vec3(.5f * p_Size.x, .5f * p_Size.y, 0.0f));
	p_Model = glm::rotate(p_Model, glm::radians(p_Rotation + 90.0f), { 0.0f, 0.0f, 1.0f });
	p_Model = glm::translate(p_Model, glm::vec3(-.5f * p_Size.x, -.5f * p_Size.y, 0.0f));

	p_Model = glm::scale(p_Model, glm::vec3(p_Size, 1.f));
}

void Weapon::draw(Renderer& renderer)
{
	glm::vec2 offset = get_offset();
	p_Shader.SetVec2F("offset", offset.x, offset.y);
	p_Shader.SetBoolean("hit", false);
	fix_position();
	build_model_matrix();
	p_Collider.Update(this);
	renderer.Draw(p_Shader, p_Texture, p_Model);
}


void Weapon::reset(const Player& player)
{
	p_Position = player.p_Position + glm::vec2(32.f, 0.0f);
	m_RotationPoint = player.p_Collider.center;
	m_AttackState = AttackStates::NEUTRAL;
	p_Rotation = 0;
	p_Collider.Update(this);
}