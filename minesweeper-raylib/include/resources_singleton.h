#pragma once

#include <raylib.h>

class ResourcesSingleton
{
public:
	ResourcesSingleton(const ResourcesSingleton&) = delete;
	ResourcesSingleton& operator=(const ResourcesSingleton&) = delete;
	ResourcesSingleton(const ResourcesSingleton&&) = delete;
	ResourcesSingleton& operator=(const ResourcesSingleton&&) = delete;

	static ResourcesSingleton& GetInstance();

	Texture2D& GetCellAtlasTexture();
	Texture2D& GetDigitsAtlasTexture();
	Texture2D& GetFaceAtlasTexture();

private:
	Texture2D cellAtlasTexture{};
	Texture2D digitsAtlasTexture{};
	Texture2D faceAtlasTexture{};

	ResourcesSingleton();
	~ResourcesSingleton();
};
