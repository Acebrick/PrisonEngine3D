// System Libs
#include <iostream>

// External Libs
#include <SDL/SDL.h>

// Engine Libs
#include "PWindow.h"

// Smart pointers delete themselves when there is no reference
// Shared pointer = Shares ownership across all references
// Unique pointer = Does not share ownership with anything
// Weak pointer = this has no ownership over any references

// Source variables
std::unique_ptr<PWindow> m_Window = nullptr;

// Source functions
bool Initialise()
{
	// Initialise the components of SDL that we need
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
	{
		std::cout << "Failed to init SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	m_Window = std::make_unique<PWindow>();
	
	// Creating an SDL window
	if (!m_Window->CreateWindow({"Game Window", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1280, 720}))
		return false;

	return true;
}

void Cleanup()
{
	// Exit SDL and deallocate all initialised SDL components
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	// Initialise the engine
	if (!Initialise())
	{
		Cleanup();
		return -1;
	}

	// Keep the game open as long as the window is open
	while (!m_Window->IsPendingClose())
	{
		// TO DO: Game loop
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				m_Window->CloseWindow();
			}
		}
	}

	// Clean up the engine
	Cleanup();

	return 0;
}