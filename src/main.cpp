
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>
#include "Game.h"

const uint32_t WIDTH = 1024;
const uint32_t HEIGHT = 720;

Game game;
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	game.mousePosition.x = xpos;
	game.mousePosition.y = ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		game.keys[GLFW_KEY_W] = true;
	} else if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		game.keys[GLFW_KEY_W] = false;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		game.keys[GLFW_KEY_S] = true;
	} else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		game.keys[GLFW_KEY_S] = false;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		game.keys[GLFW_KEY_A] = true;
	} else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
		game.keys[GLFW_KEY_A] = false;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		game.keys[GLFW_KEY_D] = true;
	} else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		game.keys[GLFW_KEY_D] = false;
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		game.keys[GLFW_KEY_SPACE] = true;
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
		game.keys[GLFW_KEY_SPACE] = false;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		game.mouseButton[GLFW_MOUSE_BUTTON_LEFT] = true;
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		game.mouseButton[GLFW_MOUSE_BUTTON_LEFT] = false;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		game.mouseButton[GLFW_MOUSE_BUTTON_RIGHT] = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		game.mouseButton[GLFW_MOUSE_BUTTON_RIGHT] = false;
	}
}

int main()
{
	if (!glfwInit()) {
		std::cout << "FAILED TO INIT GLFW" << std::endl;
		return -1;
	}
	srand(time(NULL));
	GLFWwindow* window;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Dungeon Crawler", NULL, NULL);
	if (!window) {
		glfwTerminate();
		std::cout << "FAILED TO INIT GLFW WINDOW" << std::endl;
		return -1;
	}
	
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "FAILED TO INIT GLAD" << std::endl;
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	//stbi_set_flip_vertically_on_load(true);
	glViewport(0, 0, WIDTH, HEIGHT);

	game.init(WIDTH, HEIGHT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float currentTime, previousTime, deltaTime;
	currentTime = 0.0f;
	previousTime = 0.0f;
	deltaTime = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		glfwPollEvents();
		//INPUT HANDLE;
		game.process_input();
		//UPDATE;
		game.update(deltaTime);
		//RENDER;
		glClearColor(0.46f, 0.23f, 0.21f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		game.render(deltaTime);
		glfwSwapBuffers(window);
		if (game.p_Finished) {
			glfwSetWindowShouldClose(window, true);
		}
	}
	return 1;
}