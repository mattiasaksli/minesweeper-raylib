#pragma once

#include "a_game_object.h"
#include "i_subjects.h"
#include "i_observers.h"

#include "raylib_cpp.h"

#include <list>

// Rectangles correspond to sprites in face_atlas.png
namespace FaceSprite
{
	constexpr Rectangle CLOSED_SMILE{0, 0, 64, 64};
	constexpr Rectangle OPEN_SMILE{64, 0, 64, 64};
	constexpr Rectangle SURPRISED{128, 0, 64, 64};
	constexpr Rectangle DEAD{192, 0, 64, 64};
	constexpr Rectangle COOL{256, 0, 64, 64};
}

class FaceButton final : AGameObject,
                         ISubjectGameRestart,
                         public IObserverBoardMouseButtonDown, public IObserverGameWon, public IObserverGameLost
{
public:
	FaceButton(const Vec2<int>& screenPos, const int& cellSize);

	void Update() override;
	void Draw() const override;
	void Reset() override;

	void Attach(IObserver* observer) override;
	void Detach(IObserver* observer) override;

private:
	enum FaceButtonState
	{
		CLOSED_SMILE,
		OPEN_SMILE,
		SURPRISED,
		DEAD,
		COOL
	};

	std::list<IObserver*> observers = {};
	FaceButtonState faceButtonState = CLOSED_SMILE;
	bool isGameWon = false;
	bool isGameLost = false;
	const int widthPixels;
	const int heightPixels;

	void ResetFaceButtonSpriteState();
	void ProcessInput();
	[[nodiscard]] Rectangle GetFaceSprite() const;

	void NotifyGameRestartObservers() override;

	void OnBoardMouseButtonDown() override;
	void OnGameWon() override;
	void OnGameLost() override;
};
