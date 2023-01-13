#pragma once

#include <vector>
#include "Enemy.h"
#include "Shader.h"
#include "Texture.h"
#include "Map.h"
#include "Weapon.h"
#include "CollisionHandler.h"

class EnemySpawner
{
public:
	int p_KilledEnemies = 0;
public:
	EnemySpawner();
	void SpawnEnemy(Texture2D& texture, Shader& shader, Map& map);
	void SpawnEnemies(Texture2D& texture, Shader& shader, Map& map, int number);
	void Respawn(Enemy& enemy, Map& map);
	void MoveEnemies(Player& player, Map& map, float dt);
	void DrawEnemies(Renderer& renderer);
	void CheckEnemyAttackedCollisions(CollisionHandler& collisionHandler, Weapon& playerWeapon);
	void CheckCollisions(CollisionHandler& collisionHandler, Player& player, Map& map);
	bool some_enemy_got_hit() const;
	void reset();
private:
	std::vector<Enemy> m_Enemies;
};