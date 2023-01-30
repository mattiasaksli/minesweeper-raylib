#include "board.h"
#include "resources_singleton.h"


//	####################
//	# Public functions #
//	####################


Board::Cell::Cell(const Vec2<int>&& pos)
	: cellPosition(pos)
{
}

void Board::Cell::Initialize(const bool& placeMine, const uint8_t& adjacentMines, const std::vector<Cell*>& neighborCells)
{
	hasMine = placeMine;
	minesAdjacent = adjacentMines;
	adjacentCells = neighborCells;
}

bool Board::Cell::ToggleFlag()
{
	if (!isOpen)
	{
		isFlagged = !isFlagged;
	}

	return isFlagged;
}

void Board::Cell::Highlight(const bool& isActive)
{
	if (!isOpen)
	{
		isHighlighted = isActive;
	}
}

// DFS traversal of all adjacent cells until it hits a cell that is open, contains a mine, or is flagged.
// Returns how many cells were revealed.
int Board::Cell::Reveal()
{
	// Base case
	if (isOpen || hasMine || isFlagged)
	{
		return 0;
	}

	isOpen = true;

	if (minesAdjacent > 0)
	{
		return 1;
	}

	int cellsRevealed = 1;

	// We don't need to worry about the adjacent cell we came from since that was opened in the previous function call
	// and it will be handled by the base case in the next call
	for (Cell* adjacentCell : adjacentCells)
	{
		cellsRevealed += adjacentCell->Reveal();
	}

	return cellsRevealed;
}

void Board::Cell::Draw(const Vec2<int>& boardOriginPixels,
                       const int& cellSizePixels,
                       const bool& isGameWon,
                       const bool& isGameLost) const
{
	const Vec2<int> topLeft = boardOriginPixels + (cellPosition * cellSizePixels);
	const Rectangle spriteRect = GetCellSprite(isGameWon, isGameLost);

	RaylibCpp::DrawTexturePro(
		ResourcesSingleton::GetInstance().GetCellAtlasTexture(),
		spriteRect,
		topLeft,
		Vec2<int>{cellSizePixels},
		Vec2<int>{0, 0},
		0,
		WHITE
	);
}

void Board::Cell::Reset()
{
	isOpen = false;
	hasMine = false;
	hasExplodedMine = false;
	isHighlighted = false;
	isFlagged = false;
	isPreviewingChord = false;
	isPreviewingClick = false;
	minesAdjacent = 0;
	// An alternative is to not reset adjacentCells and add extra checks to Board::GenerateBoardLayout(),
	// so that we don't do extra processing to find adjacent cells, but that would make the generation code more unreadable
	adjacentCells = {};
}


//	#####################
//	# Private functions #
//	#####################


Rectangle Board::Cell::GetCellSprite(const bool& isGameWon, const bool& isGameLost) const
{
	if (isGameWon)
	{
		if (!isOpen && !isFlagged)
		{
			return BoardSprite::FLAGGED;
		}
	}

	if (isGameLost)
	{
		if (isFlagged && !hasMine)
		{
			return BoardSprite::FLAGGED_WRONG;
		}

		if (hasMine)
		{
			if (hasExplodedMine)
			{
				return BoardSprite::EXPLODED_MINE;
			}

			return BoardSprite::MINE;
		}
	}

	if (!isOpen)
	{
		if (isPreviewingChord || isPreviewingClick)
		{
			return BoardSprite::OPEN_EMPTY;
		}

		if (isFlagged)
		{
			if (isHighlighted && !isGameWon)
			{
				return BoardSprite::FLAGGED_HIGHLIGHTED;
			}

			return BoardSprite::FLAGGED;
		}

		if (isHighlighted && !isGameLost)
		{
			return BoardSprite::CLOSED_HIGHLIGHTED;
		}
	}
	else
	{
		switch (minesAdjacent)
		{
		case 1:
			return BoardSprite::OPEN_1;
		case 2:
			return BoardSprite::OPEN_2;
		case 3:
			return BoardSprite::OPEN_3;
		case 4:
			return BoardSprite::OPEN_4;
		case 5:
			return BoardSprite::OPEN_5;
		case 6:
			return BoardSprite::OPEN_6;
		case 7:
			return BoardSprite::OPEN_7;
		case 8:
			return BoardSprite::OPEN_8;
		default:
			return BoardSprite::OPEN_EMPTY;
		}
	}

	return BoardSprite::CLOSED;
}
