#pragma once

#include "a_game_object.h"
#include "i_subjects.h"

#include "raylib_cpp.h"
#include "vec2.h"

#include <vector>
#include <functional>

// Rectangles correspond to sprites in cell_atlas.png
namespace BoardSprite
{
	// Closed states
	constexpr Rectangle CLOSED{0, 0, 32, 32};
	constexpr Rectangle CLOSED_HIGHLIGHTED{32, 0, 32, 32};
	constexpr Rectangle FLAGGED{64, 0, 32, 32};
	constexpr Rectangle FLAGGED_HIGHLIGHTED{96, 0, 32, 32};

	// Opened states
	constexpr Rectangle OPEN_EMPTY{32, 96, 32, 32};
	constexpr Rectangle OPEN_1{32, 32, 32, 32};
	constexpr Rectangle OPEN_2{64, 32, 32, 32};
	constexpr Rectangle OPEN_3{96, 32, 32, 32};
	constexpr Rectangle OPEN_4{0, 64, 32, 32};
	constexpr Rectangle OPEN_5{32, 64, 32, 32};
	constexpr Rectangle OPEN_6{64, 64, 32, 32};
	constexpr Rectangle OPEN_7{96, 64, 32, 32};
	constexpr Rectangle OPEN_8{0, 96, 32, 32};

	// Game end states
	constexpr Rectangle MINE{64, 96, 32, 32};
	constexpr Rectangle EXPLODED_MINE{96, 96, 32, 32};
	constexpr Rectangle FLAGGED_WRONG{0, 32, 32, 32};
}

class Board final : AGameObject,
                    ISubjectGameStart, ISubjectFlagCell, ISubjectBoardMouseButtonDown, ISubjectGameWin, ISubjectGameLose
{
private:
	class Cell
	{
	public:
		explicit Cell(const Vec2<int>&& pos);
		~Cell() = default;
		Cell(Cell&) = delete;
		Cell& operator=(Cell&) = delete;
		Cell(Cell&&) = default;
		Cell& operator=(Cell&&) = default;

		[[nodiscard]] bool IsOpen() const { return isOpen; }
		[[nodiscard]] bool HasMine() const { return hasMine; }
		[[nodiscard]] bool HasExplodedMine() const { return hasExplodedMine; }
		[[nodiscard]] bool IsHighlighted() const { return isHighlighted; }
		[[nodiscard]] bool IsFlagged() const { return isFlagged; }
		[[nodiscard]] Vec2<int> GetCellPosition() const { return cellPosition; }
		[[nodiscard]] uint8_t GetAdjacentMinesAmount() const { return minesAdjacent; }
		[[nodiscard]] bool IsPreviewingClick() const { return isPreviewingClick; }

		void Initialize(const bool& placeMine, const uint8_t& adjacentMines, const std::vector<Cell*>& neighborCells);
		void Highlight(const bool& isActive);
		[[nodiscard]] bool ToggleFlag();
		void SetPreviewingChord(const bool& previewChord) { isPreviewingChord = previewChord; }
		void SetPreviewingClick(const bool& previewClick) { isPreviewingClick = previewClick; }
		int Reveal();
		void ExplodeMine() { hasExplodedMine = true; }

		void Draw(const Vec2<int>& boardOriginPixels,
		          const int& cellSizePixels,
		          const bool& isGameWon,
		          const bool& isGameLost) const;
		void Reset();

	private:
		bool isOpen = false;
		bool hasMine = false;
		bool hasExplodedMine = false;
		bool isHighlighted = false;
		bool isFlagged = false;
		bool isPreviewingChord = false;
		bool isPreviewingClick = false;

		Vec2<int> cellPosition = {};
		uint8_t minesAdjacent = 0;
		std::vector<Cell*> adjacentCells = {};

		[[nodiscard]] Rectangle GetCellSprite(const bool& isGameWon, const bool& isGameLost) const;
	};

	typedef std::function<void (Cell& currentCell, Cell& adjacentCell)> CallbackOnAdjacentCells;

public:
	Board(Vec2<int> screenPos, int cellSize, int boardWidthCells, int boardHeightCells, int maxMinesAmount);
	~Board() override;

	void Update() override;
	void Draw() const override;
	void Reset() override;

	void Attach(IObserver* observer) override;
	void Detach(IObserver* observer) override;
	void NotifyFlagToggleObservers() override; // We need to call this in the Game class constructor and when resetting the game

private:
	enum BoardState
	{
		LAYOUT_UNINITIALIZED,
		IN_PROGRESS,
		GAME_LOST,
		GAME_WON
	};

	BoardState boardState = LAYOUT_UNINITIALIZED;
	std::vector<Cell> cells = {};
	std::vector<Cell*> previewChordedCells = {};
	Cell* mouseSelectedCell = nullptr;
	const int width;
	const int height;
	const int maxMinesOnBoard;
	int cellsLeftToReveal;
	int cellsLeftToFlag;

	std::list<IObserver*> observers = {};

	void NotifyGameStartObservers() override;
	void NotifyBoardMouseButtonDownObservers() override;
	void NotifyGameWonObservers() override;
	void NotifyGameLostObservers() override;

	void ResetCellsSpriteState() const;
	void ProcessInput();
	[[nodiscard]] Vec2<int> GetCellPositionUnderMouse(const Vec2<int>& mousePosPixels) const;
	void OpenClickedCell(Cell& currentCell);
	void ChordClickedCell(const bool& openCells);

	void GenerateBoardLayout();
	void DetermineInitialFreeCellPositions(std::vector<Vec2<int>>& freeCellPositions);
	void PlaceMines(std::vector<Vec2<int>>& freeCellPositions);
	void InitializeNonMinedCells();

	void ProcessCellNeighbors(Cell& cellToProcess, const CallbackOnAdjacentCells& callback);
	void WinGame();
	void LoseGame();
	[[nodiscard]] bool IsGameWon() const { return boardState == GAME_WON; }
	[[nodiscard]] bool IsGameLost() const { return boardState == GAME_LOST; }
};
