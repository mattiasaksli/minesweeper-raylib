#include "i_observers.h"

#include "face_button.h"
#include "resources_singleton.h"


//	####################
//	# Public functions #
//	####################


FaceButton::FaceButton(const Vec2<int>& screenPos, const int& cellSize)
	: AGameObject(screenPos, cellSize),
	  widthPixels(2 * cellSize),
	  heightPixels(2 * cellSize)
{
}

void FaceButton::Update()
{
	if (!IsCursorOnScreen())
	{
		return;
	}

	ResetFaceButtonSpriteState();

	auto [mouseX, mouseY] = GetMousePosition();
	const Vec2<int> mousePos =
		Vec2<int>(static_cast<int>(mouseX), static_cast<int>(mouseY))
		- screenPositionPixels;

	// Check if the mouse is within the face button area
	if (mousePos.x <= 0 || mousePos.x >= widthPixels
		|| mousePos.y <= 0 || mousePos.y >= heightPixels)
	{
		return;
	}

	ProcessInput();
}

void FaceButton::Draw() const
{
	RaylibCpp::DrawTexturePro(
		ResourcesSingleton::GetInstance().GetFaceAtlasTexture(),
		GetFaceSprite(),
		screenPositionPixels,
		Vec2<int>{widthPixels, heightPixels},
		Vec2<int>{0, 0},
		0,
		WHITE
	);
}

void FaceButton::Reset()
{
	faceButtonState = CLOSED_SMILE;
	isGameWon = false;
	isGameLost = false;
}

void FaceButton::Attach(IObserver* observer)
{
	observers.push_back(observer);
}

void FaceButton::Detach(IObserver* observer)
{
	observers.remove(observer);
}


//	#####################
//	# Private functions #
//	#####################


void FaceButton::ResetFaceButtonSpriteState()
{
	if (isGameWon && faceButtonState != COOL)
	{
		faceButtonState = COOL;
	}
	else if (isGameLost && faceButtonState != DEAD)
	{
		faceButtonState = DEAD;
	}
	else if (!isGameWon && !isGameLost && faceButtonState != CLOSED_SMILE)
	{
		faceButtonState = CLOSED_SMILE;
	}
}

void FaceButton::ProcessInput()
{
	if (IsMouseButtonReleased(RaylibCpp::MOUSE_LEFT_CLICK))
	{
		NotifyGameRestartObservers();
	}
	else if (IsMouseButtonDown(RaylibCpp::MOUSE_LEFT_CLICK))
	{
		if (faceButtonState != OPEN_SMILE)
		{
			faceButtonState = OPEN_SMILE;
		}
	}
}

Rectangle FaceButton::GetFaceSprite() const
{
	switch (faceButtonState)
	{
	case CLOSED_SMILE:
		return FaceSprite::CLOSED_SMILE;
	case OPEN_SMILE:
		return FaceSprite::OPEN_SMILE;
	case SURPRISED:
		return FaceSprite::SURPRISED;
	case DEAD:
		return FaceSprite::DEAD;
	case COOL:
		return FaceSprite::COOL;
	}

	return FaceSprite::CLOSED_SMILE;
}

void FaceButton::NotifyGameRestartObservers()
{
	for (IObserver* observer : observers)
	{
		if (auto* specificObserver = dynamic_cast<IObserverGameRestart*>(observer); specificObserver != nullptr)
		{
			specificObserver->OnGameRestarted();
		}
	}
}

void FaceButton::OnBoardMouseButtonDown()
{
	if (faceButtonState != SURPRISED)
	{
		faceButtonState = SURPRISED;
	}
}

void FaceButton::OnGameWon()
{
	if (!isGameWon)
	{
		isGameWon = true;
		faceButtonState = COOL;
	}
}

void FaceButton::OnGameLost()
{
	if (!isGameLost)
	{
		isGameLost = true;
		faceButtonState = DEAD;
	}
}
