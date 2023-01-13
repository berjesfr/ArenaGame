#include "Map.h"

Map::Map()
{}

Map::Map(Shader& shader, Texture2D& texture)
	: Entity({0.0f, 0.0f}, {0.0f, 0.0f}, 0.0f, shader, texture)
{}

void Map::load_map(const char* mapFilePath)
{
	tmx::Map map;
	map.load(mapFilePath);
	p_Shader.SetInt("numberOfRows", p_Texture.NumberOfRows);
	p_Shader.SetInt("numberOfColumns", p_Texture.NumberOfColumns);
	const auto& layers = map.getLayers();
	const auto& tilesets = map.getTilesets();
	const auto& mapSize = map.getTileCount();
	for (const auto& layer : layers) {
		const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
		const auto& tileIDs = tileLayer.getTiles();
		MapTile tile;
		for (auto y = 0u; y < mapSize.y; ++y) {
			for (auto x = 0u; x < mapSize.x; ++x) {
				auto idx = y * mapSize.x + x;
				if (tileIDs[idx].ID != 0) {
					int tileID = (tileIDs[idx].ID - 1);
					if (tileID == 57) {
						tileID = 45;
					} else if (tileID == 58) {
						tileID = 46;
					} else if (tileID == 59) {
						tileID = 47;
					}
					float column = tileID % p_Texture.NumberOfColumns;
					float xOffset = column / p_Texture.NumberOfColumns;

					float row = floor(tileID / p_Texture.NumberOfRows);
					float yOffset = row / p_Texture.NumberOfRows;

					tile.id = tileID;
					tile.position = glm::vec2(x * 32, y * 32);
					tile.rotation = 0.0f;
					if ((tileIDs[idx].flipFlags & FLIP_VERTICAL) != 0) {
						tile.rotation += -90.0f;
					}
					if ((tileIDs[idx].flipFlags & FLIP_HORIZONTAL) != 0) {
						tile.rotation += -90.0f;

					}
					tile.size = glm::vec2(32.f, 32.f);
					tile.offset = glm::vec2(xOffset, yOffset);
					if (strcmp(layer->getName().c_str(), "Objects") == 0) {
						m_ObjectTiles.push_back(tile);
					} else {
						m_MapTiles.push_back(tile);
						const auto& tileset = tilesets[0];
						if (strcmp(tileset.getTile(tileIDs[idx].ID)->Class.c_str(), "wall") == 0) {
							tile.collider = AABBCollider(tile.position, tile.size);
							p_Walls.push_back(tile);
						}
					}
				}
			}
		}
	}
}

void Map::draw(Renderer & renderer)
{
	p_Shader.SetBoolean("hit", false);
	for (const MapTile& tile : m_MapTiles) {
		if (tile.active) {
			p_Shader.SetVec2F("offset", tile.offset.x, tile.offset.y);
			p_Model = glm::mat4(1.0f);
			p_Model = glm::translate(p_Model, glm::vec3(tile.position, 0.0f));
			p_Model = glm::translate(p_Model, glm::vec3(.5f * tile.size.x, .5f * tile.size.y, 0.0f));
			p_Model = glm::rotate(p_Model, glm::radians(tile.rotation), { 0.0f, 0.0f, 1.0f });
			p_Model = glm::translate(p_Model, glm::vec3(-.5f * tile.size.x, -.5f * tile.size.y, 0.0f));
			p_Model = glm::scale(p_Model, glm::vec3(tile.size, 1.f));
			renderer.Draw(p_Shader, p_Texture, p_Model);
		}
	}
	for (const MapTile& objectTile : m_ObjectTiles) {
		if (objectTile.active) {
			p_Shader.SetVec2F("offset", objectTile.offset.x, objectTile.offset.y);
			p_Model = glm::mat4(1.0f);
			p_Model = glm::translate(p_Model, glm::vec3(objectTile.position, 0.0f));
			p_Model = glm::translate(p_Model, glm::vec3(.5f * objectTile.size.x, .5f * objectTile.size.y, 0.0f));
			p_Model = glm::rotate(p_Model, glm::radians(objectTile.rotation), { 0.0f, 0.0f, 1.0f });
			p_Model = glm::translate(p_Model, glm::vec3(-.5f * objectTile.size.x, -.5f * objectTile.size.y, 0.0f));
			p_Model = glm::scale(p_Model, glm::vec3(objectTile.size, 1.f));
			renderer.Draw(p_Shader, p_Texture, p_Model);
		}
	}
}

glm::vec2 Map::get_player_spawn_position()
{
	for (MapTile& objectTile : m_ObjectTiles) {
		if (objectTile.id == 60) {
			objectTile.active = false;
			return objectTile.position;
		}
	}
	return {0.0f, 0.0f};
}


glm::vec2 Map::get_enemy_a_spawn_position()
{
	std::vector<glm::vec2> positions;
	for (MapTile& objectTile : m_ObjectTiles) {
		if (objectTile.id == 61) {
			objectTile.active = false;
			positions.push_back(objectTile.position);
		}
	}
	return positions.at((rand() % positions.size()));
}