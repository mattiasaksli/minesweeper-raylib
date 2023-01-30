#include "i_observers.h"

#include "board.h"

#include <cassert>
#include <chrono>
#include <random>

#if defined DEBUG
#include <iostream>
#endif


//	####################
//	# Public functions #
//	####################


Board::Board(const Vec2<int> screenPos,
             const int cellSize,
             const int boardWidthCells,
             const int boardHeightCells,
             const int maxMinesAmount)
	: AGameObject(screenPos, cellSize),
	  width(boardWidthCells),
	  height(boardHeightCells),
	  maxMinesOnBoard(maxMinesAmount)
{
	assert(width > 0 && height > 0 && "The board width or height cannot be smaller than 1");
	assert(cellSizePixels > 0 && "The board cell size cannot be smaller than 1");

	cellsLeftToReveal = width * height - maxMinesOnBoard;
	cellsLeftToFlag = maxMinesOnBoard;

	cells.reserve(static_cast<size_t>(width) * height);
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			cells.emplace_back(Vec2<int>(x, y));
		}
	}
}

Board::~Board()
{
	while (!observers.empty())
	{
		Board::Detach(observers.front());
	}
}

void Board::Update()
{
	if (!IsCursorOnScreen())
	{
		return;
	}

	ResetCellsSpriteState();

	auto [mouseX, mouseY] = GetMousePosition();
	const Vec2<int> mousePos =
			Vec2<int>(static_cast<int>(mouseX), static_cast<int>(mouseY))
			- screenPositionPixels;

	// Check if the mouse is within the board area
	if (mousePos.x <= 0 || mousePos.x >= width * cellSizePixels
		|| mousePos.y <= 0 || mousePos.y >= height * cellSizePixels)
	{
		return;
	}

	const Vec2<int> selectedCellPos = GetCellPositionUnderMouse(mousePos);
	Cell& newlySelectedCell = cells[selectedCellPos.y * width + selectedCellPos.x];

	mouseSelectedCell = &newlySelectedCell;

	ProcessInput();
}

void Board::Draw() const
{
	for (const auto& cell : cells)
	{
		cell.Draw(
			screenPositionPixels,
			cellSizePixels,
			IsGameWon(),
			IsGameLost()
		);
	}
}

void Board::Reset()
{
	for (Cell& cell : cells)
	{
		cell.Reset();
	}

	cellsLeftToReveal = width * height - maxMinesOnBoard;
	cellsLeftToFlag = maxMinesOnBoard;
	boardState = LAYOUT_UNINITIALIZED;
}

void Board::Attach(IObserver* observer)
{
	observers.push_back(observer);
}

void Board::Detach(IObserver* observer)
{
	observers.remove(observer);
}

void Board::NotifyFlagToggleObservers()
{
	for (IObserver* observer : observers)
	{
		if (auto* specificObserver = dynamic_cast<IObserverFlagToggle*>(observer); specificObserver != nullptr)
		{
			specificObserver->OnToggledCellFlag(cellsLeftToFlag);
		}
	}
}


//	#####################
//	# Private functions #
//	#####################


void Board::NotifyGameStartObservers()
{
	for (IObserver* observer : observers)
	{
		if (auto* specificObserver = dynamic_cast<IObserverGameStart*>(observer); specificObserver != nullptr)
		{
			specificObserver->OnGameStarted();
		}
	}
}

void Board::NotifyBoardMouseButtonDownObservers()
{
	for (IObserver* observer : observers)
	{
		if (auto* specificObserver = dynamic_cast<IObserverBoardMouseButtonDown*>(observer); specificObserver != nullptr)
		{
			specificObserver->OnBoardMouseButtonDown();
		}
	}
}

void Board::NotifyGameWonObservers()
{
	for (IObserver* observer : observers)
	{
		if (auto* specificObserver = dynamic_cast<IObserverGameWon*>(observer); specificObserver != nullptr)
		{
			specificObserver->OnGameWon();
		}
	}
}

void Board::NotifyGameLostObservers()
{
	for (IObserver* observer : observers)
	{
		if (auto* specificObserver = dynamic_cast<IObserverGameLost*>(observer); specificObserver != nullptr)
		{
			specificObserver->OnGameLost();
		}
	}
}

void Board::ResetCellsSpriteState() const
{
	// Resets old cell highlighting
	if (mouseSelectedCell != nullptr)
	{
		mouseSelectedCell->Highlight(false);
	}

	// Resets chorded cells preview
	for (Cell* cell : previewChordedCells)
	{
		cell->SetPreviewingChord(false);
	}

	// Resets cell click preview
	if (mouseSelectedCell != nullptr
		&& mouseSelectedCell->IsPreviewingClick())
	{
		mouseSelectedCell->SetPreviewingClick(false);
	}
}

void Board::ProcessInput()
{
	if (IsGameLost() || IsGameWon())
	{
		return;
	}

	mouseSelectedCell->Highlight(true);

	// Chording
	if (IsMouseButtonDown(RaylibCpp::MOUSE_LEFT_CLICK) && IsMouseButtonReleased(RaylibCpp::MOUSE_RIGHT_CLICK)
		|| IsMouseButtonDown(RaylibCpp::MOUSE_RIGHT_CLICK) && IsMouseButtonReleased(RaylibCpp::MOUSE_LEFT_CLICK)
		|| IsMouseButtonReleased(RaylibCpp::MOUSE_MIDDLE_CLICK))
	{
		ChordClickedCell(true);
	}
	// Preview chorded cells
	else if (IsMouseButtonDown(RaylibCpp::MOUSE_LEFT_CLICK) && IsMouseButtonDown(RaylibCpp::MOUSE_RIGHT_CLICK)
		|| IsMouseButtonDown(RaylibCpp::MOUSE_MIDDLE_CLICK))
	{
		ChordClickedCell(false);
		mouseSelectedCell->SetPreviewingClick(true);
		NotifyBoardMouseButtonDownObservers();
	}
	// Revealing a cell
	else if (IsMouseButtonReleased(RaylibCpp::MOUSE_LEFT_CLICK))
	{
		if (mouseSelectedCell->IsFlagged())
		{
			return;
		}

		if (boardState == LAYOUT_UNINITIALIZED)
		{
			GenerateBoardLayout();
		}

		OpenClickedCell(*mouseSelectedCell);
	}
	// Preview click
	else if (IsMouseButtonDown(RaylibCpp::MOUSE_LEFT_CLICK))
	{
		if (mouseSelectedCell->IsFlagged())
		{
			return;
		}

		mouseSelectedCell->SetPreviewingClick(true);
		NotifyBoardMouseButtonDownObservers();
	}
	// Flagging a cell
	else if (IsMouseButtonReleased(RaylibCpp::MOUSE_RIGHT_CLICK))
	{
		if (mouseSelectedCell->ToggleFlag())
		{
			cellsLeftToFlag--;
		}
		else
		{
			cellsLeftToFlag++;
		}

		NotifyFlagToggleObservers();

#if defined DEBUG
		std::cout << "Cells left to flag: " << cellsLeftToFlag << "\n";
#endif
	}
}

Vec2<int> Board::GetCellPositionUnderMouse(const Vec2<int>& mousePosPixels) const
{
	const Vec2<int> cellPos = {
		mousePosPixels.x / cellSizePixels,
		mousePosPixels.y / cellSizePixels
	};

	assert(cellPos.x < width && cellPos.y < height && "Trying to access cell position out of bounds");

	return cellPos;
}

void Board::OpenClickedCell(Cell& currentCell)
{
	if (currentCell.HasMine())
	{
		currentCell.ExplodeMine();

		LoseGame();

		return;
	}

	const int cellsRevealed = currentCell.Reveal();

	cellsLeftToReveal -= cellsRevealed;

	if (cellsLeftToReveal == 0)
	{
		WinGame();
	}
}

void Board::ChordClickedCell(const bool& openCells)
{
	// Check how many adjacent cells are flagged and preview the rest as chorded
	int flaggedAdjacentCells = 0;
	const CallbackOnAdjacentCells checkAdjacentFlaggedCells = [&flaggedAdjacentCells, this](Cell&, Cell& adjacentCell)
	{
		if (adjacentCell.IsFlagged())
		{
			flaggedAdjacentCells++;
		}
		else
		{
			adjacentCell.SetPreviewingChord(true);
			this->previewChordedCells.push_back(&adjacentCell);
		}
	};
	ProcessCellNeighbors(*mouseSelectedCell, checkAdjacentFlaggedCells);

	// Exit function if previewing chord or chording action is invalid for this cell
	if (!openCells
		|| !mouseSelectedCell->IsOpen()
		|| flaggedAdjacentCells == 0
		|| flaggedAdjacentCells != mouseSelectedCell->GetAdjacentMinesAmount())
	{
		return;
	}

	// Open the adjacent cells
	const CallbackOnAdjacentCells openAdjacentClosedCells = [this](Cell&, Cell& adjacentCell)
	{
		if (!adjacentCell.IsOpen()
			&& !adjacentCell.IsFlagged())
		{
			this->OpenClickedCell(adjacentCell);
		}
	};
	ProcessCellNeighbors(*mouseSelectedCell, openAdjacentClosedCells);
}

void Board::GenerateBoardLayout()
{
	std::vector<Vec2<int>> freeCellPositions = {};

	// Simple case if the are no mines
	if (maxMinesOnBoard == 0)
	{
		for (Cell& cell : cells)
		{
			// We still need to determine the adjacent cells to open up the entire board in one click
			std::vector<Cell*> adjacentCells = {};
			CallbackOnAdjacentCells getAdjacentCellsOnly = [&adjacentCells](Cell&, Cell& adjacentCell)
			{
				adjacentCells.push_back(&adjacentCell);
			};
			ProcessCellNeighbors(cell, getAdjacentCellsOnly);

			cell.Initialize(false, 0, adjacentCells);
		}
	}
	// Simple case if every cell has a mine
	else if (maxMinesOnBoard == static_cast<int>(cells.size()))
	{
		for (Cell& cell : cells)
		{
			cell.Initialize(true, 8, {});
		}
	}
	// Normal case
	else
	{
		DetermineInitialFreeCellPositions(freeCellPositions);
		PlaceMines(freeCellPositions);
		InitializeNonMinedCells();
	}

	boardState = IN_PROGRESS;
	NotifyGameStartObservers();
}

void Board::DetermineInitialFreeCellPositions(std::vector<Vec2<int>>& freeCellPositions)
{
	// Add the position of every cell on the board to the list of candidates for mines
	freeCellPositions.reserve(cells.size());
	for (const Cell& cell : cells)
	{
		if (&cell != mouseSelectedCell)
		{
			freeCellPositions.push_back(cell.GetCellPosition());
		}
	}

	// Determine if we can reserve the initial adjacent cells,
	// so that the initial area around the clicked cell would not contain any mines. If not, then exit the function.
	int adjacentCellsAmount = 0;
	const CallbackOnAdjacentCells getInitialAdjacentCellsAmount = [&adjacentCellsAmount](Cell&, const Cell&)
	{
		adjacentCellsAmount++;
	};
	ProcessCellNeighbors(*mouseSelectedCell, getInitialAdjacentCellsAmount);

	if (static_cast<int>(cells.size()) - adjacentCellsAmount < maxMinesOnBoard)
	{
		return;
	}

	// Remove the cells around the initial clicked cell from the candidates list
	const CallbackOnAdjacentCells removeInitialAdjacentCells = [&freeCellPositions, this](Cell&, const Cell& adjacentCell)
	{
		const auto positionIterator = std::ranges::find(freeCellPositions,
		                                                adjacentCell.GetCellPosition());
		freeCellPositions.erase(positionIterator);
	};
	ProcessCellNeighbors(*mouseSelectedCell, removeInitialAdjacentCells);
}

void Board::PlaceMines(std::vector<Vec2<int>>& freeCellPositions)
{
	const uint32_t currentTime = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::steady_clock::now().time_since_epoch()).count());
	auto mersenne = std::mt19937(currentTime);

	int minesPlaced = 0;
	while (minesPlaced < maxMinesOnBoard)
	{
		std::uniform_int_distribution<int> distribution{0, static_cast<int>(freeCellPositions.size()) - 1};
		const int randomIndex = distribution(mersenne);
		const Vec2<int> randomCellPos = freeCellPositions[randomIndex];

		Cell& randomCell = cells[randomCellPos.y * width + randomCellPos.x];

		std::vector<Cell*> adjacentCells = {};
		CallbackOnAdjacentCells getMineAdjacentCells = [&adjacentCells](Cell&, Cell& adjacentCell)
		{
			adjacentCells.push_back(&adjacentCell);
		};
		ProcessCellNeighbors(randomCell, getMineAdjacentCells);

		// If a cell is already surrounded by mines and there is space to put the mine somewhere else,
		// then remove that cell from the candidates list, so that we can have an 8 cell there instead
		if (randomCell.GetAdjacentMinesAmount() == adjacentCells.size() && minesPlaced != static_cast<int>(freeCellPositions.size()))
		{
		}
		// Normal case
		else
		{
			randomCell.Initialize(true, 0, adjacentCells); // We don't care how many mines are adjacent, since this cell has a mine
			minesPlaced++;
		}

		freeCellPositions.erase(freeCellPositions.begin() + randomIndex);
	}
}

void Board::InitializeNonMinedCells()
{
	for (Cell& cell : cells)
	{
		if (!cell.HasMine())
		{
			std::vector<Cell*> adjacentCells = {};
			uint8_t adjacentMines = 0;
			CallbackOnAdjacentCells getNormalCellAdjacentCellsData = [&adjacentCells, &adjacentMines](Cell&, Cell& adjacentCell)
			{
				adjacentCells.push_back(&adjacentCell);
				if (adjacentCell.HasMine())
				{
					adjacentMines++;
				}
			};
			ProcessCellNeighbors(cell, getNormalCellAdjacentCellsData);

			cell.Initialize(false, adjacentMines, adjacentCells);
		}
	}
}

void Board::ProcessCellNeighbors(Cell& cellToProcess, const CallbackOnAdjacentCells& callback)
{
	Cell* adjacentCell;
	auto [x, y] = cellToProcess.GetCellPosition();

	//[o] []  []
	//[]  x   []
	//[]  []  []
	x -= 1;
	y -= 1;
	if (x >= 0 && y >= 0)
	{
		adjacentCell = &cells[y * width + x];
		callback(cellToProcess, *adjacentCell);
	}

	//[]  [o] []
	//[]  x   []
	//[]  []  []
	x += 1;
	if (y >= 0)
	{
		adjacentCell = &cells[y * width + x];
		callback(cellToProcess, *adjacentCell);
	}

	//[]  []  [o]
	//[]  x   []
	//[]  []  []
	x += 1;
	if (x < width && y >= 0)
	{
		adjacentCell = &cells[y * width + x];
		callback(cellToProcess, *adjacentCell);
	}

	//[]  []  []
	//[o] x   []
	//[]  []  []
	x -= 2;
	y += 1;
	if (x >= 0)
	{
		adjacentCell = &cells[y * width + x];
		callback(cellToProcess, *adjacentCell);
	}

	//[]  []  []
	//[]  x   [o]
	//[]  []  []
	x += 2;
	if (x < width)
	{
		adjacentCell = &cells[y * width + x];
		callback(cellToProcess, *adjacentCell);
	}

	//[]  []  []
	//[]  x   []
	//[o] []  []
	x -= 2;
	y += 1;
	if (x >= 0 && y < height)
	{
		adjacentCell = &cells[y * width + x];
		callback(cellToProcess, *adjacentCell);
	}

	//[]  []  []
	//[]  x   []
	//[]  [o] []
	x += 1;
	if (y < height)
	{
		adjacentCell = &cells[y * width + x];
		callback(cellToProcess, *adjacentCell);
	}

	//[]  []  []
	//[]  x   []
	//[]  []  [o]
	x += 1;
	if (x < width && y < height)
	{
		adjacentCell = &cells[y * width + x];
		callback(cellToProcess, *adjacentCell);
	}
}

void Board::WinGame()
{
	boardState = GAME_WON;
	cellsLeftToFlag = 0;

	NotifyFlagToggleObservers();
	NotifyGameWonObservers();

#if defined DEBUG
	std::cout << "GAME WON!\n";
#endif
}

void Board::LoseGame()
{
	boardState = GAME_LOST;

	NotifyGameLostObservers();

#if defined DEBUG
	std::cout << "GAME LOST!\n";
#endif
}
