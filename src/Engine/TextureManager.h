#pragma once
#include <vector>
#include <string>

class TextureManager
{
public:
	static unsigned int getTexturePtr(std::string filePath);	//Checks if texture has been loaded before


	static void cleanup();
protected:
	TextureManager() {}	//Class won't be initialised, so these don't matter
	~TextureManager() {}

	static std::vector<std::string> paths;
	static std::vector<unsigned int> textures;

	static unsigned int addTexture(std::string filePath);
};