
#include "MenuState.h"

#include "..\Math\Math.h"

#include "..\ECS\Entity.h"

#include "..\Platform\Game.h"

MenuState::MenuState(Game& game) : 
	game(game),
	startButton(Entity::null()),
	quitButton(Entity::null())
{}

void MenuState::OnEnter()
{
	SpawnRandomAsteroids();
	SpawnMenuButtons();
}


void MenuState::OnExit()
{
	for (Entity asteroid : floatingAsteroids)
	{
		game.entities.Destroy(asteroid);
	}
	game.entities.Destroy(startButton);
}

void MenuState::Update()
{

}

void MenuState::Render()
{

}


void MenuState::SpawnMenuButtons()
{
	Vector2 screenCenter = game.gameField.max * 0.5f;
	Vector2 buttonSize(300.0f, 100.0f);

	AABB playButton(screenCenter - (buttonSize * 0.5f), screenCenter + (buttonSize * 0.5f));

	
	std::function<void()> test = [&]() -> void {game.ChangeState<MenuState>(); };
	startButton = game.create.UIButton(playButton, SpriteID::SHITTY_LOGO, test);
}

void MenuState::SpawnRandomAsteroids()
{
	std::vector<Vector2> asteroidPositions;

	int numAsteroids = 16;
	floatingAsteroids.reserve(numAsteroids);


	for (auto i = 0; i < numAsteroids; i++)
	{
		bool isValidPosition = false;
		int attempts = 0;
		while (!isValidPosition)
		{
			if (attempts > 2048)
			{
				// Yea I know. It's temporary, ok?
				return;
			}

			constexpr float asteroidRadius = ColliderRadius::Large;
			Vector2 startPos = Vector2{ Math::RandomRange(asteroidRadius, game.gameField.max.x - asteroidRadius),
				Math::RandomRange(asteroidRadius, game.gameField.max.y - asteroidRadius) };

			isValidPosition = true;

			Circle newAsteroid(startPos, asteroidRadius);
			for (auto& existingAsteroid : asteroidPositions)
			{
				Circle existing(existingAsteroid, asteroidRadius);
				if (existing.Overlaps(newAsteroid))
				{
					isValidPosition = false;
					break;
				}
			}

			if (isValidPosition)
			{
				float startRot = (float)(rand() % 360);
				Vector2 startVel = Vector2{ Math::RandomRange(-40.0f, 40.0f), Math::RandomRange(-40.0f, 40.0f) };
				float rotVel = Math::RandomRange(-45.0f, 45.0f);

				asteroidPositions.push_back(startPos);

				floatingAsteroids.push_back(game.create.Asteroid(startPos, startRot, startVel, rotVel, Create::AsteroidType::LARGE));
			}
			++attempts;
		}
	}
}