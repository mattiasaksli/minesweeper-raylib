#pragma once

#include "vec2.h"

class AGameObject
{
protected:
	AGameObject(const Vec2<int>& screenPos, const int& cellSize);
	virtual ~AGameObject() = default;

	virtual void Update() = 0;
	virtual void Draw() const = 0;
	virtual void Reset() = 0;

	Vec2<int> screenPositionPixels;
	const int cellSizePixels;
};
