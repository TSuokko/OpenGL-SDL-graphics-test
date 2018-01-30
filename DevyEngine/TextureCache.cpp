#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>
namespace DevyEngine
{
	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}

	GLTexture TextureCache::getTexture(std::string texturePath)
	{
		//look up texture if it's in tha map
		auto mit = _textureMap.find(texturePath);
		//check if its not in the map
		if (mit == _textureMap.end())
		{
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			_textureMap.insert(make_pair(texturePath, newTexture));
			std::cout << "Loaded cached texture \n";
			return newTexture;
		}
		std::cout << "Loaded texture\n";
		return mit->second;

	}
}