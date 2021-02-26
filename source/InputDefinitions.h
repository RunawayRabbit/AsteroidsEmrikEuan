#pragma once

enum class InputOneShot
{
	NONE = 0,

	Bomb,
};

enum class InputToggle
{
	NONE = 0,

	MoveForward,
	RotateLeft,
	RotateRight,

	// Questionable controls :thinking:
	StrafeLeft,
	StrafeRight,

	Shoot,
};
