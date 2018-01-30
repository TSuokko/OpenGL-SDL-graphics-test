#pragma once
#include "GLTexture.h"
#include <string>
namespace DevyEngine
{
	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};
}
