#include <SDL_events.h>
#include <iostream>

#include "InputHandler.h"
#include "InputContext.h"

#include "..\Platform\Game.h"

InputHandler::InputHandler(Game& ourGame) :
	_ourGame(ourGame)
{
	AddContext("Default", InputContext(true), true);
}


void InputHandler::Dispatch() const
{
	for (auto& callback : _callbacks)
	{
		callback.second(_currentBuffer);
	}
}

void InputHandler::Clear()
{
	_currentBuffer.oneShots.clear();

	// Toggles get cleared explicitly in ProcessKey(). They need to persist across frames.
	// _currentBuffer.toggles.clear(); 
}

void InputHandler::AddContext(const std::string name, InputContext context, bool activated)
{
	auto pair = std::pair<const std::string, InputContext>(name, context);
	if (activated)
	{
		_active.insert(pair);
	}
	else
	{
		_inactive.insert(pair);
	}
}

void InputHandler::AddCallback(InputCallback callback, int priority = 1)
{
	auto pair = std::pair<int, InputCallback>(priority, callback);
	_callbacks.insert(pair);
}


void InputHandler::ActivateContext(const std::string name)
{
	// Icky horrible exceptions.
	try
	{
		auto context = _inactive.at(name);
		auto pair = std::pair<std::string, InputContext>(name, context);
		_active.insert(pair);
	}
	catch (const std::out_of_range&)
	{
		return;
	}
}

void InputHandler::DeactivateContext(const std::string name)
{
	// Icky horrible copy/pasted exceptions.
	try
	{
		auto context = _active.at(name);
		auto pair = std::pair<std::string, InputContext>(name, context);
		_inactive.insert(pair);
	}
	catch (const std::out_of_range&)
	{
		return;
	}
}

void InputHandler::ProcessInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		bool wasDown = false;
		bool isDown = false;
		switch (event.type)
		{

			// KEYBOARD //

		case SDL_KEYDOWN:
			isDown = true;
			wasDown = event.key.repeat;

			ProcessKey(event.key.keysym.sym, isDown, wasDown);
			break;

		case SDL_KEYUP:
			isDown = false;
			wasDown = true;

			ProcessKey(event.key.keysym.sym, isDown, wasDown);
			break;


			// MOUSE BUTTON //

		case SDL_MOUSEBUTTONDOWN:
			ProcessMouseButton(event.button.button, true);
			break;

		case SDL_MOUSEBUTTONUP:
			ProcessMouseButton(event.button.button, false);
			break;


			// MOUSE WHEEL //

		case SDL_MOUSEWHEEL:
			// @STUB:
			//event.wheel.y?

			break;


			// CURSOR MOVEMENT //

		case SDL_MOUSEMOTION:
			_currentBuffer.mouseX = event.motion.x;
			_currentBuffer.mouseY = event.motion.y;
			break;

		case SDL_QUIT:
			_ourGame.Quit();
			break;
		}
	}
}

void InputHandler::ProcessKey(const SDL_Keycode key, const bool isDown, const bool wasDown)
{
	// @TODO: @ROBUSTNESS:
	//  If a key is bound to both a OneShot *and* a Toggle, the
	//  OneShot will always consume the input before the Toggle
	//  sees it, regardless of context order or priority. This
	//  behaviour feels fine for now, but *might* be something
	// that I want to fix later on.

	// One-shots
	if (isDown && !wasDown)
	{
		for (auto& pair : _active)
		{
			auto context = pair.second;
			InputOneShot oneShot = context.ContainsOneShot(key);
			if (oneShot != InputOneShot::NONE)
			{
				_currentBuffer.oneShots.insert(oneShot);
				return;
			}
		}
	}

	// Toggles
	if (isDown)
	{
		for (auto& pair : _active)
		{
			auto context = pair.second;
			InputToggle toggle = context.ContainsToggle(key);
			if (toggle != InputToggle::NONE)
			{
				_currentBuffer.toggles.insert(toggle);
				return;
			}
		}
	}
	else
	{
		for (auto& pair : _active)
		{
			auto context = pair.second;
			InputToggle toggle = context.ContainsToggle(key);
			if (toggle != InputToggle::NONE)
			{
				_currentBuffer.toggles.erase(toggle);
				return;
			}
		}
	}

}

void InputHandler::ProcessMouseButton(const uint8_t button, const bool isDown)
{
	// @TODO: @STUB:
	//  Mouse input is a whole special case that I haven't really thought
	//  through in great detail. Single and Double-clicks ought to be OneShot
	//  events, but dragging should be a Toggle. This can all be handled right
	//  here in this method, but I haven't done *any* of it.

	return;
}

InputBuffer InputHandler::GetBuffer() const
{
	return _currentBuffer;
}