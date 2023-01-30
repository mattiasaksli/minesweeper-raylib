#include "timer.h"
#include "raylib_cpp.h"
#include "resources_singleton.h"
#include "settings.h"

#if defined DEBUG
#include <iostream>
#endif


//	####################
//	# Public functions #
//	####################


Timer::Timer(const Vec2<int>& screenPos, const int& cellSize)
	: AGameObject(screenPos, cellSize)
{
}

void Timer::Update()
{
	if (!timerStarted || previousTimeElapsed == 999)
	{
		return;
	}

	const auto timeElapsed = static_cast<int>(GetTime() - gameStartTime);

#if (defined DEBUG)
	if (timeElapsed - previousTimeElapsed >= 1)
	{
		std::cout << "Time elapsed: " << timeElapsed << "\n";
	}
#endif

	previousTimeElapsed = timeElapsed;
}

void Timer::Draw() const
{
	const int hundreds = previousTimeElapsed / 100 % 10;

	RaylibCpp::DrawTexturePro(
		ResourcesSingleton::GetInstance().GetDigitsAtlasTexture(),
		Settings::GetDigitSprite(hundreds),
		screenPositionPixels,
		Vec2<int>{cellSizePixels, cellSizePixels * 2},
		Vec2<int>{0, 0},
		0,
		WHITE
	);

	const int tens = previousTimeElapsed / 10 % 10;

	RaylibCpp::DrawTexturePro(
		ResourcesSingleton::GetInstance().GetDigitsAtlasTexture(),
		Settings::GetDigitSprite(tens),
		Vec2<int>{screenPositionPixels.x + cellSizePixels, screenPositionPixels.y},
		Vec2<int>{cellSizePixels, cellSizePixels * 2},
		Vec2<int>{0, 0},
		0,
		WHITE
	);

	const int ones = previousTimeElapsed % 10;

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

void Timer::Reset()
{
	gameStartTime = 0;
	previousTimeElapsed = 0;
	timerStarted = false;
}


//	#####################
//	# Private functions #
//	#####################


void Timer::OnGameStarted()
{
	timerStarted = true;
	gameStartTime = GetTime();
}
