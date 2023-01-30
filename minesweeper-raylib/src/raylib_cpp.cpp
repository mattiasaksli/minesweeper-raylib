#include "raylib_cpp.h"

#include <cassert>

void RaylibCpp::DrawLineEx(const Vec2<int>& startPos, const Vec2<int>& endPos, const int lineThick, const Color& color)
{
	assert(startPos.x < GetScreenWidth()
		&& startPos.y < GetScreenHeight()
		&& "Trying to draw line starting position outside of the screen");
	assert(endPos.x < GetScreenWidth()
		&& endPos.y < GetScreenHeight()
		&& "Trying to draw line ending position outside of the screen");
	assert(lineThick > 0 && "Line thickness cannot be less than 1");

	DrawLineEx(Vector2{static_cast<float>(startPos.x), static_cast<float>(startPos.y)},
	           Vector2{static_cast<float>(endPos.x), static_cast<float>(endPos.y)},
	           static_cast<float>(lineThick),
	           color);
}

void RaylibCpp::DrawRectangleLinesEx(const Vec2<int> pos,
                                     const Vec2<int> widthHeight,
                                     const int lineThick,
                                     const Color color)
{
	assert(pos.x < GetScreenWidth()
		&& pos.y < GetScreenHeight()
		&& "Trying to draw outside of the screen");
	assert(lineThick > 0 && "Line thickness cannot be less than 1");

	DrawRectangleLinesEx(Rectangle{
		                     static_cast<float>(pos.x),
		                     static_cast<float>(pos.y),
		                     static_cast<float>(widthHeight.x),
		                     static_cast<float>(widthHeight.y)
	                     },
	                     static_cast<float>(lineThick),
	                     color
	);
}

void RaylibCpp::DrawTexturePro(const Texture& texture,
                               const Rectangle& spriteRect,
                               const Vec2<int>& destTopLeft,
                               const Vec2<int>& destWidthHeight,
                               const Vec2<int>& origin,
                               const float rotation,
                               const Color& color)
{
	assert(destTopLeft.x < GetScreenWidth()
		&& destTopLeft.y < GetScreenHeight()
		&& "Trying to draw texture outside of the screen");

	DrawTexturePro(texture,
	               spriteRect,
	               Rectangle{
		               static_cast<float>(destTopLeft.x),
		               static_cast<float>(destTopLeft.y),
		               static_cast<float>(destWidthHeight.x),
		               static_cast<float>(destWidthHeight.y)
	               },
	               Vector2{static_cast<float>(origin.x), static_cast<float>(origin.y)},
	               rotation,
	               color
	);
}
