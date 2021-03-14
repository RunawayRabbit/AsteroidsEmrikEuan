#pragma once

#include <vector>

#include "IState.h"

#include "..\GameObject\Create.h"
#include "..\ECS\Entity.h"

class Game;

class MenuState : public IState
{
public:
	MenuState(Game& game);
	MenuState() = delete;

	virtual void OnEnter() override;
	virtual void OnExit() override;
	
	virtual void Update() override;
	virtual void Render() override;

private:
	void SpawnMenuButtons();
	void SpawnRandomAsteroids();

	Game& game;

	std::vector<Entity> floatingAsteroids;
	Entity startButton;
	Entity quitButton;

};