#include "game.h"
#include "settings.h"

#include <fstream>
#include <filesystem>

void CreateGameConfigFile(const std::string& filePath)
{
	std::ofstream file{filePath};

	file << "# The difficulty can be set to either 'beginner', 'intermediate', 'expert', or 'custom'. Invalid values get reset to 'intermediate'.\n";
	file << "difficulty = intermediate\n\n";
	file << "# The minimum cell size is 16 pixels. Invalid values get set to the minimum.\n";
	file << "cell_size_pixels = 64\n\n";
	file << "##########################################################################\n";
	file << "# The settings below are only used if the difficulty is set to 'custom'. #\n";
	file << "##########################################################################\n\n";
	file << "# The minimum width is 8 and minimum height is 1. Values less than that get set to the minimum.\n";
	file << "board_width = 16\n";
	file << "board_height = 16\n\n";
	file << "# The minimum amount of mines is 0 and the maximum amount is the number of cells on the board. Values under 0 get set to 0 and values over the maximum fill the entire board with mines.\n";
	file << "board_max_mines = 40";
}

void LoadGameConfigFromFile(Settings::Difficulty& difficulty,
                            int& cellSize,
                            int& boardWidthCells,
                            int& boardHeightCells,
                            int& maxMines)
{
	const std::string filePath = "config.ini";

	// Check if the config file exists, if not, then we need to create it for future use
	if (!std::filesystem::exists(filePath))
	{
		CreateGameConfigFile(filePath);

		difficulty = Settings::INTERMEDIATE;
		cellSize = 64;

		return;
	}

	std::ifstream file{filePath};
	std::string inputToken;

	while (file.good())
	{
		// We don't need to parse more of the config file if we've already got a standard difficulty and cell size
		if (difficulty != Settings::INVALID && difficulty != Settings::CUSTOM && cellSize != -1)
		{
			return;
		}

		file >> inputToken;
		std::ranges::transform(inputToken, inputToken.begin(), tolower);

		if (inputToken == "difficulty")
		{
			std::string inputDifficulty;
			file >> inputToken >> inputDifficulty;

			std::ranges::transform(inputDifficulty, inputDifficulty.begin(), tolower);

			if (inputDifficulty == "beginner")			{ difficulty = Settings::BEGINNER; }
			else if (inputDifficulty == "intermediate")	{ difficulty = Settings::INTERMEDIATE; }
			else if (inputDifficulty == "expert")		{ difficulty = Settings::EXPERT; }
			else if (inputDifficulty == "custom")		{ difficulty = Settings::CUSTOM; }
		}
		else if (inputToken == "cell_size_pixels")
		{
			file >> inputToken >> cellSize;
		}
		else if (inputToken == "board_width")
		{
			file >> inputToken >> boardWidthCells;
		}
		else if (inputToken == "board_height")
		{
			file >> inputToken >> boardHeightCells;
		}
		else if (inputToken == "board_max_mines")
		{
			file >> inputToken >> maxMines;
		}

		// This makes sure we parse the file one line at a time and ignore any tokens after a setting's value
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void ValidateGameConfig(Settings::Difficulty& difficulty,
                        int& cellSize,
                        int& boardWidthCells,
                        int& boardHeightCells,
                        int& maxMines)
{
	if (cellSize < 16)
	{
		cellSize = 16;
	}

	if (difficulty == Settings::INVALID)
	{
		difficulty = Settings::INTERMEDIATE;
	}

	if (difficulty != Settings::CUSTOM)
	{
		boardWidthCells = GetDifficultyBoardWidth(difficulty);
		boardHeightCells = GetDifficultyBoardHeight(difficulty);
		maxMines = GetDifficultyBoardMaxMines(difficulty);
	}
	else
	{
		if (boardWidthCells < 8)
		{
			boardWidthCells = 8;
		}

		if (boardHeightCells < 1)
		{
			boardHeightCells = 1;
		}

		if (maxMines < 0)
		{
			maxMines = 0;
		}
		else if (maxMines > boardWidthCells * boardHeightCells)
		{
			maxMines = boardWidthCells * boardHeightCells;
		}
	}
}

int main()
{
	Settings::Difficulty difficulty = Settings::INVALID;
	int cellSize = -1;
	int boardWidthCells = -1;
	int boardHeightCells = -1;
	int maxMines = -1;

	LoadGameConfigFromFile(difficulty, cellSize, boardWidthCells, boardHeightCells, maxMines);

	ValidateGameConfig(difficulty, cellSize, boardWidthCells, boardHeightCells, maxMines);

	Game game{
		Settings::FPS,
		"Minesweeper",
		cellSize,
		boardWidthCells,
		boardHeightCells,
		maxMines,
	};

	while (!game.GameShouldClose())
	{
		game.Tick();
	}

	return 0;
}
