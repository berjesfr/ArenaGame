#pragma once

#include "Entity.h"
#include "Map.h"
#include "AABBCollider.h"

class Player : public Entity
{
public: 
	glm::vec2 p_Velocity;
	AABBCollider p_Collider;
	float p_Health = 100.f;
	bool p_Hit = false;
public:
	Player();
	Player(Shader& shader, Texture2D& texture);
	void setup(Entity& map) override;
	void move(Entity& entity, float dt, float elapsedTime) override;
	void draw(Renderer& renderer) override;
	void damage(float dmg);
	bool got_hit() const { return m_Damaged; }
	void reset(Map& map);
private:
	unsigned int m_TileID = 87;
	int m_WalkState = 0;
	bool m_Damaged = false;
	int m_HitFrames = 0;
private:
	glm::vec2 get_offset() const;
	void animate_walk(float dt);
};