#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>

std::vector<std::string> TextureManager::paths{};
std::vector<unsigned int> TextureManager::textures{};

unsigned int TextureManager::getTexturePtr(std::string filePath)
{
	for (int i = 0; i < paths.size(); i++)	//See if texture has already been loaded
	{
		if (filePath == paths.at(i))
		{
			return textures.at(i);
		}
	}
	//Texture hasn't been loaded before, generate a new one
	return addTexture(filePath);
}

void TextureManager::cleanup()
{
	glDeleteTextures(textures.size(), textures.data());	//Since std::vector contains an array of the unsigned ints, you can free them all in one
}

unsigned int TextureManager::addTexture(std::string filePath)
{
	unsigned int newTexture;
	

	int width, height, channels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	if (data)
	{
		glGenTextures(1, &newTexture);	//Create the new texture
		glBindTexture(GL_TEXTURE_2D, newTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	//Add the image data to the texture
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Error in loading texture located at " << filePath << " maybe the file path is wrong?\n";
	}

	paths.push_back(filePath);
	textures.push_back(newTexture);

	return newTexture;
}
