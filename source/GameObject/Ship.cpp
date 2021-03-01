
#include "Ship.h"
#include "Math.h" // @TODO: for random impulse on create REMOVE LATER



Ship::Ship(const SpriteAtlas& spriteAtlas, const Rect* gameField, Vector2 initialPosition, float initialRotationDeg) :
	Entity(spriteAtlas.shipSprite, gameField, initialPosition, initialRotationDeg)
{
	float x = Math::RandomRange(-10.0f, 10.0f);
	float y = Math::RandomRange(-10.0f, 10.0f);

	velocity = { x, y };
	forward = { 0.0f, -1.0f }; // up..?
}

void Ship::ProcessInput(float deltaTime, const InputBuffer& inputBuffer) // @TODO: This should take the player input struct?
{
	// forward

	if (inputBuffer.Contains(InputToggle::MoveForward))
	{
		velocity += forward * fwdAccel * deltaTime;
	}

	// strafe

	if (inputBuffer.Contains(InputToggle::StrafeLeft))
	{
		velocity += forward.Rot90CCW() * strafeAccel * deltaTime;
	}
	if (inputBuffer.Contains(InputToggle::StrafeRight))
	{
		velocity += forward.Rot90CW() * strafeAccel * deltaTime;
	}

	// rotate

	if (inputBuffer.Contains(InputToggle::RotateLeft))
	{
		float rot = -rotVelocity * deltaTime;
		forward = forward.Rotate(rot);

		// @TODO: Sprite. None of this should stay, this all gets
		// bundled into our Transform code.
		rotation += rot * Math::RAD2DEG;

	}
	if (inputBuffer.Contains(InputToggle::RotateRight))
	{
		float rot = rotVelocity * deltaTime;
		forward = forward.Rotate(rot);

		// @TODO: Sprite. None of this should stay, this all gets
		// bundled into our Transform code.
		rotation += rot * Math::RAD2DEG;
	}
}

void Ship::Update(float deltaTime)
{
	Displace(velocity * deltaTime);
}