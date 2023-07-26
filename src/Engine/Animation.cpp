#include "Animation.h"

#include "TextureManager.h"

Animation::Animation(const std::string& spriteSheetPath, int frameCount)
{
	spriteSheet = TextureManager::getTexturePtr(spriteSheetPath);
}

Animation::~Animation()
{
}
