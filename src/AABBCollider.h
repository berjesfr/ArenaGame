#pragma once

#include "Entity.h"

class AABBCollider
{
public:
	glm::vec2 max;
	glm::vec2 min;
	glm::vec2 halfExtents;
	glm::vec2 center;
public:
	AABBCollider();
	AABBCollider(glm::vec2 position, glm::vec2 size);
	bool IsColliding(AABBCollider& other);
	void Update(Entity* entity);
};