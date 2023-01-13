
#include "AABBCollider.h"

AABBCollider::AABBCollider()
{}

AABBCollider::AABBCollider(glm::vec2 position, glm::vec2 size)
	: max({position.x + size.x, position.y + size.y}), min({position.x, position.y}),
	halfExtents({size.x / 2, size.y / 2}), center({position.x + size.x / 2, position.y + size.y / 2})
{}

bool AABBCollider::IsColliding(AABBCollider& other)
{
	bool collisionX = this->min.x < other.max.x && this->max.x > other.min.x;
	bool collisionY = this->min.y < other.max.y && this->max.y > other.min.y;
	return collisionX && collisionY;
}

void AABBCollider::Update(Entity* entity)
{	
	
	max.x = entity->p_Position.x + entity->p_Size.x;
	max.y = entity->p_Position.y + entity->p_Size.y;
	min.x = entity->p_Position.x;
	min.y = entity->p_Position.y;
	halfExtents = {entity->p_Size.x / 2, entity->p_Size.y / 2};
	center = {entity->p_Position.x + entity->p_Size.x / 2, entity->p_Position.y + entity->p_Size.y / 2};
}
