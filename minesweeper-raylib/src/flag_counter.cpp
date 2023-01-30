#include "flag_counter.h"
#include "raylib_cpp.h"
#include "resources_singleton.h"
#include "settings.h"

#include <cmath>


//	####################
//	# Public functions #
//	####################


FlagCounter::FlagCounter(const Vec2<int>& screenPos, const int& cellSize)
	: AGameObject(screenPos, cellSize)
{
}

void FlagCounter::Update()
{
	// Not used
}

void FlagCounter::Draw() const
{
	const int hundreds = unflaggedMines >= 0
		                     ? unflaggedMines / 100 % 10
		                     : 11; // Dash location in the atlas texture

	RaylibCpp::DrawTexturePro(
		ResourcesSingleton::GetInstance().GetDigitsAtlasTexture(),
		Settings::GetDigitSprite(hundreds),
		screenPositionPixels,
		Vec2<int>{cellSizePixels, cellSizePixels * 2},
		Vec2<int>{0, 0},
		0,
		WHITE
	);

	const int tens = std::abs(unflaggedMines) / 10 % 10;

	RaylibCpp::DrawTexturePro(
		ResourcesSingleton::GetInstance().GetDigitsAtlasTexture(),
		Settings::GetDigitSprite(tens),
		Vec2<int>{screenPositionPixels.x + cellSizePixels, screenPositionPixels.y},
		Vec2<int>{cellSizePixels, cellSizePixels * 2},
		Vec2<int>{0, 0},
		0,
		WHITE
	);

	const int ones = std::abs(unflaggedMines) % 10;

	RaylibCpp::DrawTexturePro(
		ResourcesSingleton::GetInstance().GetDigitsAtlasTexture(),
		Settings::GetDigitSprite(ones),
		Vec2<int>{screenPositionPixels.x + cellSizePixels * 2, screenPositionPixels.y},
		Vec2<int>{cellSizePixels, cellSizePixels * 2},
		Vec2<int>{0, 0},
		0,
		WHITE
	);
}

void FlagCounter::Reset()
{
	unflaggedMines = 0;
}


//	#####################
//	# Private functions #
//	#####################


void FlagCounter::OnToggledCellFlag(const int& minesLeftToFlag)
{
	this->unflaggedMines = minesLeftToFlag;
}
