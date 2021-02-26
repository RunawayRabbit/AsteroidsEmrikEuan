#pragma once


#include <map>
#include <SDL_keycode.h>

#include "InputDefinitions.h"

class InputContext
{
public:
	InputContext();
	~InputContext();

	InputOneShot ContainsOneShot(const SDL_Keycode key) const;
	InputToggle ContainsToggle(const SDL_Keycode key) const;

	void AddOneShot(const SDL_Keycode key, InputOneShot oneShot);
	void AddToggle(const SDL_Keycode key, InputToggle toggle);

private:
	std::map<SDL_Keycode, InputOneShot> _oneShots;
	std::map<SDL_Keycode, InputToggle> _toggles;
};