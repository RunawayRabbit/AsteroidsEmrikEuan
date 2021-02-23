#pragma once

#include <string>

class Game
{
public:
	Game(std::string windowName, int width, int height);
	Game() = delete;
	Game(Game&) = delete;

	bool IsRunning() const;

	void ProcessInput();
	void Update();
	void Render();
	void Quit();

private:
	bool isRunning;

	void ExitWithSDLError(const std::string errorMessage) const;
};