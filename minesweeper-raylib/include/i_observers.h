#pragma once

class IObserver
{
protected:
	virtual ~IObserver() = default;
};

class IObserverGameStart : public virtual IObserver
{
public:
	virtual void OnGameStarted() = 0;
};

class IObserverGameRestart : public virtual IObserver
{
public:
	virtual void OnGameRestarted() = 0;
};

class IObserverGameWon : public virtual IObserver
{
public:
	virtual void OnGameWon() = 0;
};

class IObserverGameLost : public virtual IObserver
{
public:
	virtual void OnGameLost() = 0;
};

class IObserverFlagToggle : public virtual IObserver
{
public:
	virtual void OnToggledCellFlag(const int& minesLeftToFlag) = 0;
};

class IObserverBoardMouseButtonDown : public virtual IObserver
{
public:
	virtual void OnBoardMouseButtonDown() = 0;
};
