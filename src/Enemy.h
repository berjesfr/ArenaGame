#pragma once

#include "Entity.h"
#include "Player.h"

class Enemy : public Entity 
{
public:
	glm::vec2 p_Velocity;
	AABBCollider p_Collider;
	float p_Health = 100.f;
	bool p_Damaged = false;
	bool p_Hit = false;
public:
	Enemy();
	Enemy(Shader& shader, Texture2D& texture);
	void setup(Entity& entity) override;
	void move(Entity& player, float dt, float elapsedTime) override;
	void draw(Renderer& renderer) override;
	void damage(float dmg);
	bool got_hit() const { return p_Damaged; }
private:
	unsigned int m_TileID = 109;
	int m_Hitframes = 0;
	float m_Speed = 0.f;
private:
	glm::vec2 get_offset() const;
};