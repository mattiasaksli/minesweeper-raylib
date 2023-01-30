#pragma once

class IObserver;

class ISubject
{
protected:
	virtual ~ISubject() = default;

	virtual void Attach(IObserver* observer) = 0;
	virtual void Detach(IObserver* observer) = 0;
};

class ISubjectGameStart : public ISubject
{
protected:
	virtual void NotifyGameStartObservers() = 0;
};

class ISubjectGameRestart : public ISubject
{
protected:
	virtual void NotifyGameRestartObservers() = 0;
};

class ISubjectGameWin : public ISubject
{
protected:
	virtual void NotifyGameWonObservers() = 0;
};

class ISubjectGameLose : public ISubject
{
protected:
	virtual void NotifyGameLostObservers() = 0;
};

class ISubjectFlagCell : public ISubject
{
protected:
	virtual void NotifyFlagToggleObservers() = 0;
};

class ISubjectBoardMouseButtonDown : public ISubject
{
protected:
	virtual void NotifyBoardMouseButtonDownObservers() = 0;
};
