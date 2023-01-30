#pragma once

#include "board.h"
#include "timer.h"
#include "flag_counter.h"
#include "face_button.h"

#include <string>

class Game final : public IObserverGameRestart, public IObserverGameWon, public IObserverGameLost
{
public:
	Game(int fps, const std::string& title, int cellSize, int boardWidth, int boardHeight, int maxMinesAmount);
	~Game() noexcept override;
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(const Game&&) = delete;
	Game& operator=(const Game&&) = delete;

	[[nodiscard]] bool GameShouldClose() const;
	void Tick();

private:
	const int cellSizePixels;
	const int borderThicknessPixels;
	const int boardWidthCells;
	const int boardHeightCells;
	Board board;
	FlagCounter flagCounter;
	FaceButton faceButton;
	Timer timer;

	bool isGameWon = false;
	bool isGameLost = false;

	void Update();
	void Draw() const;

	void OnGameRestarted() override;
	void OnGameWon() override;
	void OnGameLost() override;
};
