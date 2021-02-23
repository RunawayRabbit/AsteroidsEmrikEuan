#pragma once

#include <string>

#include <SDL_render.h> // Move out into Renderer

class Game
{
public:
	Game(std::string windowName, int width, int height);
	Game() = delete;
	Game(Game&) = delete;

	~Game();

	bool IsRunning() const;

	void ProcessInput();
	void Update();
	void Render();
	void Quit();

private:
	// Stuff to move out into Renderer
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* spritesheet;

	SDL_Texture* LoadSpritemap(const std::string path) const;


	bool isRunning;

	void ExitWithSDLError(const std::string errorMessage) const;
};