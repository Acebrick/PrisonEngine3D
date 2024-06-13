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

	// Tell SDL that we'll be rendering in open gl version 460 or 4.60
	// 4 is the major version
	// .60 is the minor version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	// Make sure if open gl is using a version not in SDL we run SDL in compatibility mode
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	// Set the bit depth for each colour
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);

	m_Window = std::make_unique<PWindow>();
	
	// Creating an SDL window
	if (!m_Window->CreateWindow({"Game Window", 
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		720, 720}))
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

		// Render the window
		m_Window->Render();
	}

	// Clean up the engine
	Cleanup();

	return 0;
}