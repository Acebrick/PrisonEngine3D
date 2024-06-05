#include "PWindow.h"
#include "Graphics/PGraphicsEngine.h"
#include "Debug/PDebug.h"

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

	m_GraphicsEngine = std::make_unique<PGraphicsEngine>();

	// Initialise the graphics engine and test if it failed
	if (!m_GraphicsEngine->InitEngine(m_SDLWindow, m_Params.vsync))
	{
		PDebug::Log("Window failed to initialise graphics engine", LT_ERROR);
		m_GraphicsEngine = nullptr;
		return false;
	}

	return true;
}

void PWindow::Render()
{
	// Render the graphics engine if one exists
	if (m_GraphicsEngine)
	{
		m_GraphicsEngine->Render(m_SDLWindow);
	}
}
