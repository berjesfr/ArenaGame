#include "EnemySpawner.h"

EnemySpawner::EnemySpawner()
{}

void EnemySpawner::SpawnEnemy(Texture2D& texture, Shader& shader, Map& map)
{
	Enemy enemy = Enemy(shader, texture);
	enemy.setup(map);
	m_Enemies.push_back(enemy);
}

void EnemySpawner::SpawnEnemies(Texture2D& texture, Shader& shader, Map& map, int number)
{
	for (int i = 0; i < number; i++) {
		SpawnEnemy(texture, shader, map);
	}
}

void EnemySpawner::Respawn(Enemy& enemy, Map& map)
{
	enemy.p_Health = 100;
	p_KilledEnemies++;
	enemy.setup(map);
}

void EnemySpawner::MoveEnemies(Player& player, Map& map, float dt)
{
	for (Enemy& enemy : m_Enemies) {
		if (enemy.p_Health > 0) {
			enemy.move(player, dt, 0u);
		} else {
			Respawn(enemy, map);
		}
	}
}

void EnemySpawner::DrawEnemies(Renderer& renderer)
{
	for (Enemy& enemy : m_Enemies) {
		enemy.draw(renderer);
	}
}

void EnemySpawner::CheckEnemyAttackedCollisions(CollisionHandler& collisionHandler, Weapon& playerWeapon)
{
	for (Enemy& enemy : m_Enemies) {
		collisionHandler.CheckAttackCollision(playerWeapon, enemy);
	}
}

void EnemySpawner::CheckCollisions(CollisionHandler& collisionHandler, Player& player, Map& map)
{
	player.p_Hit = false;
	for (Enemy& enemy : m_Enemies) {
		collisionHandler.CheckEnemyCollisions(enemy, player);
		collisionHandler.CheckEnemyWallCollisions(map, enemy);
	} 
}

bool EnemySpawner::some_enemy_got_hit() const
{
	for (const Enemy& enemy : m_Enemies) {
		if (enemy.p_Hit) {
			return enemy.p_Hit;
		}
	}
	return false;
}

void EnemySpawner::reset()
{
	m_Enemies.clear();
	p_KilledEnemies = 0;
}
