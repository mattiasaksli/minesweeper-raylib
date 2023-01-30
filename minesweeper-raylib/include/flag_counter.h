#pragma once

#include "a_game_object.h"
#include "i_observers.h"

#include "settings.h"

class FlagCounter final : AGameObject,
                          public IObserverFlagToggle
{
public:
	FlagCounter(const Vec2<int>& screenPos, const int& cellSize);

	void Update() override;
	void Draw() const override;
	void Reset() override;

private:
	int unflaggedMines = 0;

	void OnToggledCellFlag(const int& minesLeftToFlag) override;
};
