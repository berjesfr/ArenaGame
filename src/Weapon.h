#pragma once

#include "Entity.h"
#include "Player.h"


enum class AttackStates {
	NEUTRAL,
	SWING_ONE,
	SWING_TWO,
};


class Weapon : public Entity
{
public:
	bool p_Attacking = false;
	AABBCollider p_Collider;

public:
	Weapon();
	Weapon(Shader& shader, Texture2D& texture);
	void setup(Entity& player) override;
	void move(Entity& player, float dt, float elapsedTime) override;
	void look_at(glm::vec2 mousePosition);
	void attack(float dt);
	void draw(Renderer& renderer) override;
	void reset(const Player& player);
private:
	unsigned int m_TileID = 95;
	float m_PreviousRotation = 0;
	glm::vec2 m_RotationPoint = { 0.0f, 0.0f };
	AttackStates m_AttackState = AttackStates::NEUTRAL;
	int m_AttackStateCounter = 0;
private:
	glm::vec2 get_offset() const;
	void build_model_matrix();
	void fix_position();
};