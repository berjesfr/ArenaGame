#pragma once

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

class Texture2D
{
public:
	unsigned int ID;
	int NumberOfRows = 11;
	int NumberOfColumns = 12;
public:
	Texture2D();
	void LoadTexture(const char* texturePath);
	void BindTexture();
	void Generate(unsigned int width, unsigned int height);
};