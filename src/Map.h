#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <vector>
#include <map>
#include "Entity.h"
#include "AABBCollider.h"

struct MapTile
{
	int id;
	glm::vec2 position;
	glm::vec2 size;
	float rotation;
	glm::vec2 offset;
	AABBCollider collider;
	bool active = true;
};

constexpr uint8_t FLIP_HORIZONTAL = 8u;
constexpr uint8_t FLIP_VERTICAL = 4u;
constexpr uint8_t FLIP_DIAGONAL = 2u;

class Map : public Entity
{
public:
	std::vector<MapTile> p_Walls;
public:
	Map();
	Map(Shader& shader, Texture2D& texture);
	void load_map(const char* mapFilePath);
	void draw(Renderer& renderer);
	glm::vec2 get_player_spawn_position();
	glm::vec2 get_enemy_a_spawn_position();
private:
	std::vector<MapTile> m_MapTiles;
	std::vector<MapTile> m_ObjectTiles;
};