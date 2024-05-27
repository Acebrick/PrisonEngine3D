#include "PWindow.h"

// External Libs
#include <SDL/SDL.h>

PWindow::PWindow()
{
	m_SDLWindow = nullptr;
	m_ShouldClose = false;

	std::cout << "Window created" << std::endl;
}

PWindow::~PWindow()
{
	// If the SDL window exists, destroy it
	if (m_SDLWindow)
		SDL_DestroyWindow(m_SDLWindow);

	std::cout << "Window destroyed" << std::endl;
}

bool PWindow::CreateWindow(const PSWindowParams& params)
{
	// Enabling opengl in our SDL window
	unsigned int windowFlags = SDL_WINDOW_OPENGL;

	// Assigning parameters to the member for the window
	m_Params = params;

	// Add vsync flag if it's selected
	if (m_Params.vsync)
		windowFlags += SDL_WINDOW_ALLOW_HIGHDPI;

	// Set fullscreen if it's set
	if (m_Params.fullscreen)
		windowFlags += SDL_WINDOW_FULLSCREEN_DESKTOP; // Fullscreen borderless
	else
		windowFlags += SDL_WINDOW_SHOWN; // Windowed mode

	// Create the SDL window
	m_SDLWindow = SDL_CreateWindow(
		m_Params.title.c_str(),
		m_Params.x,
		m_Params.y,
		m_Params.w,
		m_Params.h,
		windowFlags
	);

	// Check if SDL window was created
	if (!m_SDLWindow)
	{
		std::cout << "SDL failed to create window: " << SDL_GetError() << std::endl;
		CloseWindow();
		return false;
	}

	

	return true;
}
