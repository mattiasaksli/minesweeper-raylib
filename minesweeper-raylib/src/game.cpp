#include "game.h"
#include "raylib_cpp.h"
#include "resources_singleton.h"

#include <cassert>
#if defined DEBUG
#include <iostream>
#endif

using namespace Settings;

Game::Game(const int fps,
           const std::string& title,
           const int cellSize,
           const int boardWidth,
           const int boardHeight,
           const int maxMinesAmount)
	: cellSizePixels(cellSize),
	  borderThicknessPixels(cellSize / 2),
	  boardWidthCells(boardWidth),
	  boardHeightCells(boardHeight),

	  board(GetBoardScreenPosition(cellSizePixels, borderThicknessPixels),
	        cellSizePixels,
	        boardWidthCells,
	        boardHeightCells,
	        maxMinesAmount),
	  flagCounter(GetFlagCounterScreenPosition(cellSizePixels), cellSizePixels),
	  faceButton(GetFaceButtonScreenPosition(cellSizePixels, borderThicknessPixels, boardWidth), cellSizePixels),
	  timer(GetTimerScreenPosition(cellSizePixels, boardWidth), cellSizePixels)
{
	assert(!GetWindowHandle() && "Cannot initialize window, since window is already open");

	// Initialize window
	SetTargetFPS(fps);

	const int windowWidth = boardWidthCells * cellSizePixels + cellSizePixels;
	const int windowHeight = boardHeightCells * cellSizePixels + cellSizePixels * 4 + borderThicknessPixels;

	InitWindow(windowWidth, windowHeight, title.c_str());

	const Image gameIcon = LoadImage("resources/icon.png");
	SetWindowIcon(gameIcon);

	// Setup events
	board.Attach(&flagCounter);
	board.Attach(&faceButton);
	board.Attach(&timer);
	board.Attach(this); // For winning/losing the game

	// We need to make sure that flagCounter is attached to board,
	// so that the initial notification can trigger successfully once the board object is constructed
	board.NotifyFlagToggleObservers();

	faceButton.Attach(this); // For restarting the game
}

Game::~Game() noexcept
{
	assert(GetWindowHandle() && "Cannot close window, since window is already closed");
	CloseWindow();
}

bool Game::GameShouldClose() const
{
	return WindowShouldClose();
}

void Game::Tick()
{
	BeginDrawing();
	Update();
	Draw();
	EndDrawing();
}

void Game::Update()
{
	faceButton.Update();

	if (isGameWon || isGameLost)
	{
		return;
	}

	board.Update();
	timer.Update();
}

void Game::Draw() const
{
	ClearBackground(Color{50, 50, 50, 255});

	// Draw borders
	RaylibCpp::DrawRectangleLinesEx(
		Vec2<int>{0},
		Vec2{
			boardWidthCells * cellSizePixels,
			boardHeightCells * cellSizePixels + cellSizePixels * 3 + borderThicknessPixels
		} + cellSizePixels,
		borderThicknessPixels,
		DARKGRAY
	);
	RaylibCpp::DrawLineEx(
		Vec2<int>{borderThicknessPixels, cellSizePixels * 4 - borderThicknessPixels / 2},
		Vec2<int>{
			boardWidthCells * cellSizePixels + borderThicknessPixels,
			cellSizePixels * 4 - borderThicknessPixels / 2
		},
		borderThicknessPixels,
		DARKGRAY
	);

	board.Draw();
	timer.Draw();
	flagCounter.Draw();
	faceButton.Draw();
}

void Game::OnGameRestarted()
{
	isGameWon = false;
	isGameLost = false;

	board.Reset();
	timer.Reset();
	flagCounter.Reset();
	faceButton.Reset();

	board.NotifyFlagToggleObservers();

#if defined DEBUG
	std::cout << "GAME RESTARTED!\n";
#endif
}

void Game::OnGameWon()
{
	isGameWon = true;
}

void Game::OnGameLost()
{
	isGameLost = true;
}
