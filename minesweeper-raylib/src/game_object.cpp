#include "a_game_object.h"

AGameObject::AGameObject(const Vec2<int>& screenPos, const int& cellSize)
	: screenPositionPixels(screenPos),
	  cellSizePixels(cellSize)
{
}
