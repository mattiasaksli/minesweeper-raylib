#pragma once

#include "a_game_object.h"
#include "i_observers.h"

#include "vec2.h"

class Timer final : AGameObject,
                    public IObserverGameStart
{
public:
	Timer(const Vec2<int>& screenPos, const int& cellSize);

	void Update() override;
	void Draw() const override;
	void Reset() override;

private:
	double gameStartTime = 0;
	int previousTimeElapsed = 0;
	bool timerStarted = false;

	void OnGameStarted() override;
};
