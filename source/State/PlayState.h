#pragma once


#include "IState.h"

class Game;

class PlayState : public IState
{
public:
	PlayState(Game& game);
	PlayState() = delete;

	virtual void OnEnter() override;
	virtual void OnExit() override;

	virtual void Update() override;
	virtual void Render() override;

private:
	Game& game;
};