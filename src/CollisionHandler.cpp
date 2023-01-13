#include "CollisionHandler.h"

CollisionHandler::CollisionHandler()
{}


float CollisionHandler::clamp(float value, float min, float max) const
{
	return std::max(min, std::min(max, value));
}

Direction CollisionHandler::collisionDirection(glm::vec2 target) const
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};

	float max = 0.0f;
	unsigned int bestMatch = -1;

	for (unsigned int i = 0; i < 4; i++) {
		float dotProduct = glm::dot(glm::normalize(target), compass[i]);
		if (dotProduct > max) {
			max = dotProduct;
			bestMatch = i;
		}
	}
	return (Direction)bestMatch;
}

void CollisionHandler::CheckWallCollisions(Map& map, Player& player)
{
	for (MapTile& wall : map.p_Walls) {
		if (player.p_Collider.IsColliding(wall.collider)) {
			glm::vec2 D = player.p_Collider.center - wall.collider.center;
			glm::vec2 P = wall.collider.center + glm::vec2(clamp(D.x, -wall.collider.halfExtents.x, wall.collider.halfExtents.x), clamp(D.y, -wall.collider.halfExtents.y, wall.collider.halfExtents.y));
			D = P - player.p_Collider.center;
			Direction playerCollisionDirection = collisionDirection(D);
			float yAmount = player.p_Collider.halfExtents.y - std::abs(D.y);
			float xAmount = player.p_Collider.halfExtents.x - std::abs(D.x);

			if (playerCollisionDirection == Direction::UP) {
				player.p_Position = glm::vec2(player.p_Position.x, player.p_Position.y - yAmount);
			} else if (playerCollisionDirection == Direction::DOWN) {
				player.p_Position = glm::vec2(player.p_Position.x, player.p_Position.y + yAmount);
			} else if (playerCollisionDirection == Direction::LEFT) {
				player.p_Position = glm::vec2(player.p_Position.x + xAmount, player.p_Position.y);
			} else {
				player.p_Position = glm::vec2(player.p_Position.x - xAmount, player.p_Position.y);
			}
		}
	}
}

void CollisionHandler::CheckEnemyCollisions(Enemy& enemy, Player& player)
{
	if (player.p_Collider.IsColliding(enemy.p_Collider) && !player.got_hit()) {
		player.p_Hit = true;
		glm::vec2 D = player.p_Collider.center - enemy.p_Collider.center;
		glm::vec2 P = enemy.p_Collider.center + glm::vec2(clamp(D.x, -enemy.p_Collider.halfExtents.x, enemy.p_Collider.halfExtents.x), clamp(D.y, -enemy.p_Collider.halfExtents.y, enemy.p_Collider.halfExtents.y));
		D = P - player.p_Collider.center;
		Direction playerCollisionDirection = collisionDirection(D);
		float yAmount = player.p_Collider.halfExtents.y - std::abs(D.y);
		float xAmount = player.p_Collider.halfExtents.x - std::abs(D.x);

		if (playerCollisionDirection == Direction::UP) {
			player.p_Position = glm::vec2(player.p_Position.x, player.p_Position.y - yAmount);
			player.p_Velocity.y = -100.f;
			enemy.p_Position = glm::vec2(enemy.p_Position.x, enemy.p_Position.y + yAmount);
		} else if (playerCollisionDirection == Direction::DOWN) {
			player.p_Position = glm::vec2(player.p_Position.x, player.p_Position.y + yAmount);
			player.p_Velocity.y = 100.f;
			enemy.p_Position = glm::vec2(enemy.p_Position.x, enemy.p_Position.y - yAmount);
		} else if (playerCollisionDirection == Direction::LEFT) {
			player.p_Position = glm::vec2(player.p_Position.x + xAmount, player.p_Position.y);
			player.p_Velocity.x = 100.f;
			enemy.p_Position = glm::vec2(enemy.p_Position.x - xAmount, enemy.p_Position.y);
		} else {
			player.p_Position = glm::vec2(player.p_Position.x - xAmount, player.p_Position.y);
			player.p_Velocity.x = -100.f;
			enemy.p_Position = glm::vec2(enemy.p_Position.x + xAmount, enemy.p_Position.y);
		}
		player.damage(20);
	}
}

void CollisionHandler::CheckEnemyWallCollisions(Map& map, Enemy& enemy)
{
	for (MapTile& wall : map.p_Walls) {
		if (enemy.p_Collider.IsColliding(wall.collider)) {
			glm::vec2 D = enemy.p_Collider.center - wall.collider.center;
			glm::vec2 P = wall.collider.center + glm::vec2(clamp(D.x, -wall.collider.halfExtents.x, wall.collider.halfExtents.x), clamp(D.y, -wall.collider.halfExtents.y, wall.collider.halfExtents.y));
			D = P - enemy.p_Collider.center;
			Direction enemyCollisionDirection = collisionDirection(D);
			float yAmount = enemy.p_Collider.halfExtents.y - std::abs(D.y);
			float xAmount = enemy.p_Collider.halfExtents.x - std::abs(D.x);

			if (enemyCollisionDirection == Direction::UP) {
				enemy.p_Position = glm::vec2(enemy.p_Position.x, enemy.p_Position.y - yAmount);
			}
			else if (enemyCollisionDirection == Direction::DOWN) {
				enemy.p_Position = glm::vec2(enemy.p_Position.x, enemy.p_Position.y + yAmount);
			}
			else if (enemyCollisionDirection == Direction::LEFT) {
				enemy.p_Position = glm::vec2(enemy.p_Position.x + xAmount, enemy.p_Position.y);
			}
			else {
				enemy.p_Position = glm::vec2(enemy.p_Position.x - xAmount, enemy.p_Position.y);
			}
		}
	}
}

void CollisionHandler::CheckAttackCollision(Weapon& weapon, Enemy& enemy)
{
	enemy.p_Hit = false;
	if (weapon.p_Attacking) {
		if (weapon.p_Collider.IsColliding(enemy.p_Collider) && !enemy.got_hit()) {
			enemy.p_Hit = true;
			glm::vec2 D = weapon.p_Collider.center - enemy.p_Collider.center;
			glm::vec2 P = enemy.p_Collider.center + glm::vec2(clamp(D.x, -enemy.p_Collider.halfExtents.x, enemy.p_Collider.halfExtents.x), clamp(D.y, -enemy.p_Collider.halfExtents.y, enemy.p_Collider.halfExtents.y));
			D = P - weapon.p_Collider.center;
			Direction weaponCollisionDirection = collisionDirection(D);
			float yAmount = weapon.p_Collider.halfExtents.y - std::abs(D.y);
			float xAmount = weapon.p_Collider.halfExtents.x - std::abs(D.x);

			if (weaponCollisionDirection == Direction::UP) {
				enemy.p_Position = glm::vec2(enemy.p_Position.x, enemy.p_Position.y + yAmount);
				enemy.p_Velocity.y = 100.f;
			} else if (weaponCollisionDirection == Direction::DOWN) {
				enemy.p_Position = glm::vec2(enemy.p_Position.x, enemy.p_Position.y - yAmount);
				enemy.p_Velocity.y = -100.f;
			} else if (weaponCollisionDirection == Direction::LEFT) {
				enemy.p_Position = glm::vec2(enemy.p_Position.x - xAmount, enemy.p_Position.y);
				enemy.p_Velocity.x = -100.f;
			} else {
				enemy.p_Position = glm::vec2(enemy.p_Position.x + xAmount, enemy.p_Position.y);
				enemy.p_Velocity.x = 100.f;
			}
			enemy.damage(50);
		}
	}
}