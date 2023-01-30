#pragma once

#include "vec2.h"

#include <raylib.h>

namespace Settings
{
	// Window settings
	constexpr int FPS = 60;

	// Game settings
	enum Difficulty
	{
		INVALID,
		BEGINNER,
		INTERMEDIATE,
		EXPERT,
		CUSTOM
	};

	constexpr Vec2<int> BEGINNER_BOARD_DIMENSIONS{9};
	constexpr int BEGINNER_MINES_TOTAL = 10;

	constexpr Vec2<int> INTERMEDIATE_BOARD_DIMENSIONS{16};
	constexpr int INTERMEDIATE_MINES_TOTAL = 40;

	constexpr Vec2<int> EXPERT_BOARD_DIMENSIONS{30, 16};
	constexpr int EXPERT_MINES_TOTAL = 99;

	static int GetDifficultyBoardWidth(const Difficulty difficulty)
	{
		if (difficulty == BEGINNER)
		{
			return BEGINNER_BOARD_DIMENSIONS.x;
		}
		if (difficulty == INTERMEDIATE)
		{
			return INTERMEDIATE_BOARD_DIMENSIONS.x;
		}
		return EXPERT_BOARD_DIMENSIONS.x;
	};

	static int GetDifficultyBoardHeight(const Difficulty difficulty)
	{
		if (difficulty == BEGINNER)
		{
			return BEGINNER_BOARD_DIMENSIONS.y;
		}
		if (difficulty == INTERMEDIATE)
		{
			return INTERMEDIATE_BOARD_DIMENSIONS.y;
		}
		return EXPERT_BOARD_DIMENSIONS.y;
	};

	static int GetDifficultyBoardMaxMines(const Difficulty difficulty)
	{
		if (difficulty == BEGINNER)
		{
			return BEGINNER_MINES_TOTAL;
		}
		if (difficulty == INTERMEDIATE)
		{
			return INTERMEDIATE_MINES_TOTAL;
		}
		return EXPERT_MINES_TOTAL;
	};


	// Utils
	static Vec2<int> GetBoardScreenPosition(const int cellSizePixels, const int borderThicknessPixels)
	{
		return Vec2<int>{
			borderThicknessPixels,
			cellSizePixels * 4
		};
	}

	static Vec2<int> GetFlagCounterScreenPosition(const int cellSizePixels)
	{
		return Vec2<int>{cellSizePixels};
	}

	static Vec2<int> GetFaceButtonScreenPosition(const int cellSizePixels,
	                                             const int borderThicknessPixels,
	                                             const int boardWidthCells)
	{
		return Vec2<int>{
			boardWidthCells * cellSizePixels / 2 - cellSizePixels + borderThicknessPixels,
			cellSizePixels
		};
	}

	static Vec2<int> GetTimerScreenPosition(const int cellSizePixels, const int boardWidthCells)
	{
		return Vec2<int>{
			boardWidthCells * cellSizePixels - cellSizePixels * 3,
			cellSizePixels
		};
	}

	static Rectangle GetDigitSprite(const int& digit) // Used in Timer and FlagCounter, corresponds to sprites in digits_atlas.png
	{
		return Rectangle{static_cast<float>(digit) * 32, 0, 32, 64};
	}
}
