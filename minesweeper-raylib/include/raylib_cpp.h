#pragma once

#include "vec2.h"

#include <raylib.h>

namespace RaylibCpp
{
	constexpr int MOUSE_LEFT_CLICK = 0;
	constexpr int MOUSE_RIGHT_CLICK = 1;
	constexpr int MOUSE_MIDDLE_CLICK = 2;

	void DrawLineEx(const Vec2<int>& startPos, const Vec2<int>& endPos, int lineThick, const Color& color);
	void DrawRectangleLinesEx(Vec2<int> pos, Vec2<int> widthHeight, int lineThick, Color color);
	void DrawTexturePro(const Texture& texture,
	                    const Rectangle& spriteRect,
	                    const Vec2<int>& destTopLeft,
	                    const Vec2<int>& destWidthHeight,
	                    const Vec2<int>& origin,
	                    float rotation,
	                    const Color& color);
}
