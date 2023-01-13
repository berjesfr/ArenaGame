#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <irrklang/irrKlang.h>
#include <iostream>
#include "Renderer.h"
#include "Shader.h"
#include "CollisionHandler.h"
#include "Texture.h"
#include "Map.h"
#include "Player.h"
#include "Weapon.h"
#include "EnemySpawner.h"
#include "Menu.h"
#include "HudElement.h"
#include "PostProcessor.h"
#include "TextRenderer.h"

enum class GameState
{
	MENU,
	RUNNING,
	END,
};


class Game
{
public:
	bool keys[1024];
	bool mouseButton[3];
	glm::vec2 mousePosition;
	bool p_Finished;
	irrklang::ISoundEngine* p_SoundEngine;
public:
	Game();
	~Game()
	{
		delete m_Renderer;
		delete map;
		delete player;
		delete playerWeapon;
		delete enemySpawner;
		delete postProcessor;
		delete p_SoundEngine;
	}
	void init(unsigned int width, unsigned int height);
	void process_input();
	void update(float dt);
	void render(float dt);
	void restart();

private:
	GameState m_GameState;
	Renderer* m_Renderer;
	Menu* m_Menu;
	HudElement* m_LifeBar;
	Map* map;
	PostProcessor* postProcessor;
	TextRenderer* textRenderer;
	Player* player;
	Weapon* playerWeapon;
	EnemySpawner* enemySpawner;
	irrklang::ISound* m_WooshSound;
	irrklang::ISoundSource* m_HitSound;
	irrklang::ISoundSource* m_SoundTrack;
	Shader m_MapShader;
	Shader m_PlayerShader;
	Shader m_HudShader;
	Shader m_PostprocessingShader;
	Shader m_TextRendererShader;
	Texture2D m_Texture;
	CollisionHandler collisionHandler;
	unsigned int m_Width, m_Height;
	float m_ElapsedTime;
};