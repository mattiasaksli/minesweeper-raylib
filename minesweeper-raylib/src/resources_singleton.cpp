#include "resources_singleton.h"

#include <cassert>

ResourcesSingleton& ResourcesSingleton::GetInstance()
{
	static ResourcesSingleton instance;

	return instance;
}

Texture2D& ResourcesSingleton::GetCellAtlasTexture()
{
	return cellAtlasTexture;
}

Texture2D& ResourcesSingleton::GetDigitsAtlasTexture()
{
	return digitsAtlasTexture;
}

Texture2D& ResourcesSingleton::GetFaceAtlasTexture()
{
	return faceAtlasTexture;
}

ResourcesSingleton::ResourcesSingleton()
{
	assert(GetWindowHandle() && "Cannot load texture data, since window doesn't exist");

	const Image cellAtlasImage = LoadImage("resources/cell_atlas.png");
	cellAtlasTexture = LoadTextureFromImage(cellAtlasImage);
	UnloadImage(cellAtlasImage);

	const Image digitsAtlasImage = LoadImage("resources/digits_atlas.png");
	digitsAtlasTexture = LoadTextureFromImage(digitsAtlasImage);
	UnloadImage(digitsAtlasImage);

	const Image faceAtlasImage = LoadImage("resources/face_atlas.png");
	faceAtlasTexture = LoadTextureFromImage(faceAtlasImage);
	UnloadImage(faceAtlasImage);
}

ResourcesSingleton::~ResourcesSingleton()
{
	UnloadTexture(cellAtlasTexture);
	UnloadTexture(digitsAtlasTexture);
	UnloadTexture(faceAtlasTexture);
}
