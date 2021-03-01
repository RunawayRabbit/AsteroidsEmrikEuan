#pragma once

#include "..\ECS\Entity.h"
#include "..\Math\Vector2.h"

#include "..\Input\InputBuffer.h"

class Ship : public Entity
{
public:
	Ship(const SpriteAtlas& spriteAtlas, const Rect* gameField, Vector2 initialPosition, float initialRotationDeg);
	Ship() = delete;

	void ProcessInput(float deltaTime, const InputBuffer& inputBuffer);
	void Update(float deltaTime);

private:
	Vector2 velocity;
	Vector2 forward; // @TODO: Roll this into transform

	static constexpr float rotVelocity = 2.3f; // in radians
	static constexpr float fwdAccel = 200.0f; // acceleration speed
	static constexpr float strafeAccel = 80.0f; // acceleration speed
};