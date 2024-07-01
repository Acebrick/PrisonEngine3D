#include "PWindow.h"
#include "Graphics/PGraphicsEngine.h"
#include "Debug/PDebug.h"
#include "Listeners/PInput.h"
#include "Graphics/PSCamera.h"

// External Libs
#include <SDL/SDL.h>

PWindow::PWindow()
{
	m_SDLWindow = nullptr;
	m_ShouldClose = false;
	m_CameraDirection = glm::vec3(0.0f);

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

void PWindow::RegisterInput(const TShared<PInput>& m_Input)
{
	

	m_Input->OnKeyPress->Bind([this](const SDL_Scancode& key)
	{
		if (key == SDL_SCANCODE_W)
		{
			m_CameraDirection.z += 1.0f;
		}

		if (key == SDL_SCANCODE_S)
		{
			m_CameraDirection.z += -1.0f;
		}

		if (key == SDL_SCANCODE_A)
		{
			m_CameraDirection.x += 1.0f;
		}

		if (key == SDL_SCANCODE_D)
		{
			m_CameraDirection.x += -1.0f;
		}

		if (key == SDL_SCANCODE_E)
		{
			m_CameraDirection.y += 1.0f;
		}

		if (key == SDL_SCANCODE_Q)
		{
			m_CameraDirection.y += -1.0f;
		}
	});

	m_Input->OnKeyRelease->Bind([this](const SDL_Scancode& key)
	{
		if (key == SDL_SCANCODE_W)
		{
			m_CameraDirection.z += -1.0f;
		}

		if (key == SDL_SCANCODE_S)
		{
			m_CameraDirection.z += 1.0f;
		}

		if (key == SDL_SCANCODE_A)
		{
			m_CameraDirection.x += -1.0f;
		}

		if (key == SDL_SCANCODE_D)
		{
			m_CameraDirection.x += 1.0f;
		}

		if (key == SDL_SCANCODE_E)
		{
			m_CameraDirection.y += -1.0f;
		}

		if (key == SDL_SCANCODE_Q)
		{
			m_CameraDirection.y += 1.0f;
		}
	});
}

void PWindow::Render()
{
	// Render the graphics engine if one exists
	if (m_GraphicsEngine)
	{
		// Test if there is a camera
		if (const auto& camRef = m_GraphicsEngine->GetCamera().lock())
		{
			// If a camera exists move it based on the camera direction input
			camRef->transform.position += m_CameraDirection * 0.1f;
		}
		m_GraphicsEngine->Render(m_SDLWindow);
	}
}
