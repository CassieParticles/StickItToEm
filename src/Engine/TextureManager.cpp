#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>

std::vector<std::string> TextureManager::paths{};
std::vector<texture> TextureManager::textures{};

texture TextureManager::getTexturePtr(std::string filePath)
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
	for (int i = 0; i < textures.size(); i++)
	{
		glDeleteTextures(1, &textures.at(i).textureID);
	}
}

texture TextureManager::addTexture(std::string filePath)
{
	unsigned int newTextureID;
	

	int width, height, channels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	if (data)
	{
		glGenTextures(1, &newTextureID);	//Create the new texture
		glBindTexture(GL_TEXTURE_2D, newTextureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB + channels - 3, width, height, 0, GL_RGB + channels - 3, GL_UNSIGNED_BYTE, data);	//Add the image data to the texture
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Error in loading texture located at " << filePath << " maybe the file path is wrong?\n";
		return {};
	}

	texture newTexture = { newTextureID,width,height,channels };

	paths.push_back(filePath);
	textures.push_back(newTexture);

	return textures.at(textures.size()-1);
}
