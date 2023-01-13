#include "Texture.h"

Texture2D::Texture2D()
{}

void Texture2D::LoadTexture(const char* texturePath)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

	if (data) {
		glCreateTextures(GL_TEXTURE_2D, 1, &this->ID);
		glTextureParameteri(this->ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(this->ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(this->ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(this->ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureStorage2D(this->ID, 1, GL_RGBA32F, width, height);
		glTextureSubImage2D(this->ID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateTextureMipmap(this->ID);
	} else {
		std::cout << "FAILURE LOADING IMAGE" << std::endl;
	}
	stbi_image_free(data);
}

void Texture2D::BindTexture()
{
	glBindTextureUnit(0, this->ID);
}

void Texture2D::Generate(unsigned int width, unsigned int height)
{
	// create Texture
	glGenTextures(1, &this->ID);
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	// set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateTextureMipmap(this->ID);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

}
