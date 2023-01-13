#pragma once

#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Map.h"
#include "AABBCollider.h"


enum class Direction
{
	UP, RIGHT, DOWN, LEFT
};


class CollisionHandler
{
public:
	CollisionHandler();
	void CheckWallCollisions(Map& map, Player& player);
	void CheckEnemyCollisions(Enemy& enemy, Player& player);
	void CheckEnemyWallCollisions(Map& map, Enemy& enemy);
	void CheckAttackCollision(Weapon& weapon, Enemy& enemy);
private:
	float clamp(float value, float min, float max) const;
	Direction collisionDirection(glm::vec2 target) const;
};