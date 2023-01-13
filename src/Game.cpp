#include "Game.h"

Game::Game()
	: m_ElapsedTime(0), m_GameState(GameState::MENU)
{}

void Game::init(unsigned int width, unsigned int height)
{
	m_Width = width;
	m_Height = height;
	m_GameState = GameState::MENU;
	m_Menu = new Menu(width, height);
	m_MapShader = Shader();
	m_PlayerShader = Shader();
	m_HudShader = Shader();
	m_PostprocessingShader = Shader();
	m_TextRendererShader = Shader();
	m_Texture = Texture2D();
	mousePosition = { 0.0f, 0.0f };

	m_MapShader.GetShaderSources("./src/shaders/basic_vertex.glsl", "./src/shaders/basic_fragment.glsl");
	m_PlayerShader.GetShaderSources("./src/shaders/basic_vertex.glsl", "./src/shaders/basic_fragment.glsl");
	m_HudShader.GetShaderSources("./src/shaders/hud_vertex.glsl", "./src/shaders/hud_fragment.glsl");
	m_PostprocessingShader.GetShaderSources("./src/shaders/postprocessor_vertex.glsl", "./src/shaders/postprocessor_fragment.glsl");
	m_TextRendererShader.GetShaderSources("./src/shaders/text_vertex.glsl", "./src/shaders/text_fragment.glsl");
	m_Texture.LoadTexture("./Tilemap/tilemap_packed.png");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
	m_MapShader.SetMatrix4("projection", projection);
	m_PlayerShader.SetMatrix4("projection", projection);
	m_HudShader.SetMatrix4("projection", projection);
	m_MapShader.SetInt("tex", 0);
	m_PlayerShader.SetInt("tex", 0);

	map = new Map(m_MapShader, m_Texture);
	m_Renderer = new Renderer();
	player = new Player(m_PlayerShader, m_Texture);
	playerWeapon = new Weapon(m_PlayerShader, m_Texture);
	enemySpawner = new EnemySpawner();
	postProcessor = new PostProcessor(m_PostprocessingShader, width, height);
	textRenderer = new TextRenderer(m_TextRendererShader, width, height);
	textRenderer->load("fonts/ocraext.TTF", 16.f);
	m_Renderer->Setup();
	map->load_map("assets/sampleMap.tmx");
	playerWeapon->setup(*player);
	player->setup(*map);
	enemySpawner->SpawnEnemies(m_Texture, m_PlayerShader, *map, 10);

	m_LifeBar = new HudElement(&(player->p_Health), {12.f, height - 60.f}, {256.f, 16.f}, m_HudShader, m_Texture);

	p_SoundEngine = irrklang::createIrrKlangDevice();
	if (!p_SoundEngine) {
		std::cout << "Error initializing sound engine" << std::endl;
		p_Finished = true;
	}
	m_WooshSound = p_SoundEngine->play2D("./sounds/woosh.ogg", true, true);
	m_HitSound = p_SoundEngine->addSoundSourceFromFile("./sounds/hit.ogg");
	m_SoundTrack = p_SoundEngine->addSoundSourceFromFile("./sounds/soundtrack.ogg");
	m_WooshSound->setVolume(0.07f);
	m_HitSound->setDefaultVolume(0.07f);
	m_SoundTrack->setDefaultVolume(0.07f);

}

void Game::process_input()
{
	if (m_GameState == GameState::RUNNING) {
		if (!player->got_hit()) {
			if (keys[GLFW_KEY_W]) {
				player->p_Velocity.y = -100.f;
			} else if (keys[GLFW_KEY_S]) {
				player->p_Velocity.y = 100.f;
			} else if (!keys[GLFW_KEY_W] && !keys[GLFW_KEY_S]) {
				player->p_Velocity.y = 0.0f;
			}

			if (keys[GLFW_KEY_A]) {
				player->p_Velocity.x = -100.f;
			} else if (keys[GLFW_KEY_D]) {
				player->p_Velocity.x = 100.f;
			} else if (!keys[GLFW_KEY_A] && !keys[GLFW_KEY_D]) {
				player->p_Velocity.x = 0.0f;
			}

			if (mouseButton[GLFW_MOUSE_BUTTON_LEFT]) {
				playerWeapon->p_Attacking = true;
			}
		}
	} else if (m_GameState == GameState::END) {
		if (keys[GLFW_KEY_SPACE]) {
			restart();
		}
	} else if (m_GameState == GameState::MENU) {
		if (mouseButton[GLFW_MOUSE_BUTTON_LEFT]) {
			m_Menu->p_MouseClick = true;
		} else {
			m_Menu->p_MouseClick = false;
		}
	}
}

void Game::update(float dt)
{
	if (m_GameState == GameState::RUNNING) {
		m_ElapsedTime += dt;
		m_WooshSound->setIsPaused(true);
		player->move(*player, dt, m_ElapsedTime);
		playerWeapon->move(*player, dt, m_ElapsedTime);
		enemySpawner->MoveEnemies(*player, *map, dt);
		if (playerWeapon->p_Attacking) {
			playerWeapon->attack(dt);
			enemySpawner->CheckEnemyAttackedCollisions(collisionHandler, *playerWeapon);
			if (enemySpawner->some_enemy_got_hit()) {
				p_SoundEngine->play2D(m_HitSound);
			} else {
				m_WooshSound->setIsPaused(false);
			}
		} 
		playerWeapon->look_at(mousePosition);
		collisionHandler.CheckWallCollisions(*map, *player);
		enemySpawner->CheckCollisions(collisionHandler, *player, *map);
		if (player->p_Hit) {
			p_SoundEngine->play2D(m_HitSound);
		}
		postProcessor->p_Shake = player->got_hit();
		if (player->p_Health < 0) m_GameState = GameState::END;
	} else if (m_GameState == GameState::MENU) {
		ClickAction action = m_Menu->check_for_clicks(mousePosition);
		if (action == ClickAction::START) {
			m_GameState = GameState::RUNNING;
			p_SoundEngine->play2D(m_SoundTrack, true, false);
		} else if(action == ClickAction::EXIT) {
			p_Finished = true;
		}
	}
}

void Game::render(float dt)
{
	if (m_GameState == GameState::RUNNING) {
		postProcessor->begin_render();
		map->draw(*m_Renderer);
		player->draw(*m_Renderer);
		enemySpawner->DrawEnemies(*m_Renderer);
		if (player->p_Health > 0) {
			playerWeapon->draw(*m_Renderer);
		}
		m_LifeBar->draw(*m_Renderer);
		textRenderer->render_text(std::to_string(player->p_Health), 16.f, m_Height - 58, 1.f);
		textRenderer->render_text("Score: " + std::to_string(enemySpawner->p_KilledEnemies), m_Width - 120.f, m_Height - 58, 1.f);
		postProcessor->end_render();
		postProcessor->render(m_ElapsedTime);
	} else if (m_GameState == GameState::END) {
		textRenderer->render_text("Game Over", (m_Width / 2.f) - 144.f, m_Height / 2.f, 3.f);
		textRenderer->render_text("Press SPACEBAR to restart", (m_Width / 2.f) - 256.f, (m_Height / 2.f) + 42.f, 2.f);
	} else {
		m_Menu->draw_buttons(*textRenderer);
	}
}

void Game::restart()
{
	m_GameState = GameState::RUNNING;
	player->reset(*map);
	enemySpawner->reset();
	enemySpawner->SpawnEnemies(m_Texture, m_PlayerShader, *map, 10);
	playerWeapon->reset(*player);
}