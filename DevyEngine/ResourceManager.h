#pragma once
#include <string>
#include "TextureCache.h"
namespace DevyEngine
{
	class ResourceManager
	{
	public:
		static GLTexture getTexture(std::string texturePath);

	private:
		static TextureCache _textureCache;
	};
}
