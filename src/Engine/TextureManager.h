#pragma once
#include <vector>
#include <string>

struct texture
{
	unsigned int textureID;
	int width;
	int height;
	int channels;
};

class TextureManager
{
public:
	static texture getTexturePtr(std::string filePath);	//Checks if texture has been loaded before


	static void cleanup();
protected:
	TextureManager() {}	//Class won't be initialised, so these don't matter
	~TextureManager() {}

	static std::vector<std::string> paths;
	static std::vector<texture> textures;

	static texture addTexture(std::string filePath);
};