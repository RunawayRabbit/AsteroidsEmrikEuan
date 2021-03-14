
#include "..\Platform\Game.h"

#include "PlayState.h"

#include "..\Math\Math.h"

PlayState::PlayState(Game& game) : 
	game(game),
	player(game.entities, game.rigidbodies, game.xforms, game.create, game.physics),
	playerEntity(Entity::null())
{}


void PlayState::OnEnter()
{
	SpawnFreshAsteroids(6, 20.0f, 25.0f);
	SpawnPlayer();
}

void PlayState::OnExit()
{
	// STUB
}

void PlayState::Update(const InputBuffer& inputBuffer, const float& deltaTime)
{
	player.Update(inputBuffer, deltaTime);
}

void PlayState::Render()
{
}

void PlayState::SpawnPlayer()
{
	player.Spawn(game.gameField.max * 0.5f, 0);
}

void PlayState::SpawnFreshAsteroids(const int& count, const float& minVelocity, const float& maxVelocity)
{
	currentAsteroids.reserve(count);

	int leftRightCount = count / 3;
	float yBucketWidth = (game.gameField.max.y - ColliderRadius::Large) / leftRightCount;
	for (int i = 0; i < leftRightCount; ++i)
	{
		// Static in X, variable in Y
		Vector2 startPos{};
		startPos.x = 0;
		startPos.y = Math::RandomRange(ColliderRadius::Large + (yBucketWidth)*i, (yBucketWidth)*i + 1);

		float startRot = (float)(rand() % 360);

		Vector2 velDir = Vector2::Forward().RotateDeg(Math::RandomRange(0.0f, 360.0f));
		Vector2 vel = velDir * Math::RandomRange(minVelocity, maxVelocity);
		float rotVel = Math::RandomRange(-45.0f, 45.0f);

		currentAsteroids.push_back(game.create.Asteroid(startPos, startRot, vel, rotVel, Create::AsteroidType::LARGE));
	}

	int topBottomCount = count - leftRightCount;
	float xBucketWidth = (game.gameField.max.x - ColliderRadius::Large) / topBottomCount;
	for (int i = 0; i < topBottomCount; ++i)
	{
		// Static in Y, variable in X
		Vector2 startPos{};
		startPos.x = Math::RandomRange(ColliderRadius::Large + (xBucketWidth)*i, (xBucketWidth)*i + 1);
		startPos.y = 0;
		
		float startRot = (float)(rand() % 360);
		
		Vector2 velDir = Vector2::Forward().RotateDeg(Math::RandomRange(0.0f, 360.0f));
		Vector2 vel = velDir * Math::RandomRange(minVelocity, maxVelocity);
		float rotVel = Math::RandomRange(-45.0f, 45.0f);

		currentAsteroids.push_back(game.create.Asteroid(startPos, startRot, vel, rotVel, Create::AsteroidType::LARGE));
	}	
}