
// Used to catch out-of-range from std::map.
#include <stdexcept> 

#include "InputContext.h"

InputContext::InputContext()
{
	//DEFAULT INPUTS
	// @TODO: This defeats the purpose of the nice context system you made.
	//   Try to get remapping support in before you ship!

	AddToggle(SDLK_w, InputToggle::MoveForward);
	AddToggle(SDLK_a, InputToggle::RotateLeft);
	AddToggle(SDLK_d, InputToggle::RotateRight);
	
	AddToggle(SDLK_q, InputToggle::StrafeLeft);
	AddToggle(SDLK_e, InputToggle::StrafeRight);

	AddToggle(SDLK_SPACE, InputToggle::Shoot);
}

InputContext::~InputContext()
{
	//
}

InputOneShot InputContext::ContainsOneShot(const SDL_Keycode key) const
{
	auto it = _oneShots.find(key);
	if (it != _oneShots.end())
		return it->second;
	else
		return InputOneShot::NONE;
	
}

InputToggle InputContext::ContainsToggle(const SDL_Keycode key) const
{
	auto it = _toggles.find(key);
	if (it != _toggles.end())
		return it->second;
	else
		return InputToggle::NONE;
}


void InputContext::AddOneShot(const SDL_Keycode key, InputOneShot oneShot)
{
	_oneShots.insert(std::pair<SDL_Keycode, InputOneShot>(key, oneShot));
}
void InputContext::AddToggle(const SDL_Keycode key, InputToggle toggle)
{
	_toggles.insert(std::pair<SDL_Keycode, InputToggle>(key, toggle));
}