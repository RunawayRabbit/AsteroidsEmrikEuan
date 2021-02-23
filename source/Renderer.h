#pragma once

#include <string>
#include <SDL_render.h>

class Renderer
{
public:
	Renderer(std::string windowName, int width, int height);
	Renderer() = delete;
	Renderer(Renderer&) = delete;
	~Renderer();

	void Clear();
	void Flip();

	SDL_Texture* GetSpritesheet() const { return spritesheet; }
	SDL_Renderer* GetRenderer() const { return renderer; }

private:
	void ExitWithSDLError(const std::string errorMessage) const;
	SDL_Texture* LoadSpritemap(const std::string path) const;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* spritesheet;
};
