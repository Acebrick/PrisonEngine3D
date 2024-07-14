#include "PWindow.h"
#include "Graphics/PGraphicsEngine.h"
#include "Debug/PDebug.h"
#include "Listeners/PInput.h"
#include "Graphics/PSCamera.h"

// External Libs
#include <SDL/SDL.h>

// DEBUG
#include "Graphics/PModel.h"

float tankRotationSpeed = 1.5f;

PWindow::PWindow()
{
	m_SDLWindow = nullptr;
	m_ShouldClose = false;
	m_CameraDirection = glm::vec3(0.0f);
	m_CameraRotation = glm::vec3(0.0f);
	m_CanZoom = false;
	m_InputMode = false;
	m_TankDirection = glm::vec3(0.0f);

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

	cameraRef = m_GraphicsEngine->GetCamera().lock();
	tankRef = m_GraphicsEngine->GetTank().lock();

	return true;
}

void PWindow::RegisterInput(const TShared<PInput>& m_Input)
{
	// Hide the cursor and set relative mouse mode
	m_Input->ShowCursor(false);

	m_Input->OnKeyPress->Bind([this, m_Input](const SDL_Scancode& key)
		{
			// Quick exit for debug
			if (key == SDL_SCANCODE_ESCAPE)
				CloseWindow();

			// Toggle the cursor visibility
			if (key == SDL_SCANCODE_PERIOD)
			{
				m_Input->ShowCursor(m_Input->IsCursorHidden());

				// Set the game to input mode if the cursor is visible
				m_InputMode = !m_Input->IsCursorHidden();
			}

			// Toggle third person mode
			if (key == SDL_SCANCODE_COMMA)
			{
				// Turn on/off third person mode
				cameraRef->ToggleThirdPerson(); 

				cameraRef->transform.position.x = tankRef->GetTransform().position.x;
				cameraRef->transform.position.y = tankRef->GetTransform().position.y + 5.0f;
				cameraRef->transform.position.z = tankRef->GetTransform().position.z - 15.0f;

				PDebug::Log(std::to_string(cameraRef->thirdPerson));
			}

			// Controls for the tank (third person mode)
			if (cameraRef->thirdPerson)
			{
				if (key == SDL_SCANCODE_W) //  Forward
				{
					m_TankDirection.z += 1.0f;
				}

				if (key == SDL_SCANCODE_S) //  Backward
				{
					m_TankDirection.z += -1.0f;
				}

				if (key == SDL_SCANCODE_A) // Left
				{
					tankRef->GetTransform().rotation.y += tankRotationSpeed;
					cameraRef->transform.rotation.y += tankRotationSpeed;

					cameraRef->RotateAroundObject(
						tankRef->GetTransform().position.x,
						tankRef->GetTransform().position.z,
						-tankRotationSpeed);
				}

				if (key == SDL_SCANCODE_D) // Right
				{
					tankRef->GetTransform().rotation.y -= tankRotationSpeed;
					cameraRef->transform.rotation.y -= tankRotationSpeed;

					cameraRef->RotateAroundObject(
						tankRef->GetTransform().position.x,
						tankRef->GetTransform().position.z,
						tankRotationSpeed);
				}
			}

			if (key == SDL_SCANCODE_W) //  Forward
			{
				m_CameraDirection.z += 1.0f;
			}

			if (key == SDL_SCANCODE_S) //  Backward
			{
				m_CameraDirection.z += -1.0f;
			}

			if (key == SDL_SCANCODE_A) // Left
			{
				m_CameraDirection.x += -1.0f;
			}

			if (key == SDL_SCANCODE_D) // Right
			{
				m_CameraDirection.x += 1.0f;
			}

			if (key == SDL_SCANCODE_Q) // Down
			{
				m_CameraDirection.y += -1.0f;
			}

			if (key == SDL_SCANCODE_E) // Up
			{
				m_CameraDirection.y += 1.0f;
			}
		});

	m_Input->OnKeyRelease->Bind([this](const SDL_Scancode& key)
		{
			// Controls for the tank (third person mode)
			if (cameraRef->thirdPerson)
			{
				if (key == SDL_SCANCODE_W) //  Forward
				{
					m_TankDirection.z -= 1.0f;
				}

				if (key == SDL_SCANCODE_S) //  Backward
				{
					m_TankDirection.z += 1.0f;
				}
			}
			if (key == SDL_SCANCODE_W) // Forward
			{
				m_CameraDirection.z += -1.0f;
			}

			if (key == SDL_SCANCODE_S) // Backward
			{
				m_CameraDirection.z += 1.0f;
			}

			if (key == SDL_SCANCODE_A) // Left
			{
				m_CameraDirection.x += 1.0f;
			}

			if (key == SDL_SCANCODE_D) // Right
			{
				m_CameraDirection.x += -1.0f;
			}

			if (key == SDL_SCANCODE_Q) // Down
			{
				m_CameraDirection.y += 1.0f;
			}

			if (key == SDL_SCANCODE_E) // Up
			{
				m_CameraDirection.y += -1.0f;
			}
		});

	m_Input->OnKeyHeld->Bind([this](const SDL_Scancode& key)
		{
			// Controls for the tank (third person mode)
			if (cameraRef->thirdPerson)
			{
				if (key == SDL_SCANCODE_A) // Left
				{
					tankRef->GetTransform().rotation.y += tankRotationSpeed;
					cameraRef->transform.rotation.y += tankRotationSpeed;

					cameraRef->RotateAroundObject(
						tankRef->GetTransform().position.x,
						tankRef->GetTransform().position.z,
						-tankRotationSpeed);
				}

				if (key == SDL_SCANCODE_D) // Right
				{
					tankRef->GetTransform().rotation.y -= tankRotationSpeed;
					cameraRef->transform.rotation.y -= tankRotationSpeed;

					cameraRef->RotateAroundObject(
						tankRef->GetTransform().position.x,
						tankRef->GetTransform().position.z,
						tankRotationSpeed);
				}
			}
		});

	// On mouse move rotate the camera input
	m_Input->OnMouseMove->Bind([this](const float& x, const float& y,
		const float& xrel, const float& yrel)
		{
				m_CameraRotation.y = -xrel;
				m_CameraRotation.x = -yrel;
		});

	m_Input->OnMouseScroll->Bind([this](const float& delta)
		{
			if (m_CanZoom)
			{
				cameraRef->Zoom(delta);
			}
		});

	m_Input->OnMousePress->Bind([this](const PUi8& button)
		{
			if (button == SDL_BUTTON_RIGHT)
			{
				m_CanZoom = true;
			}
		});

	m_Input->OnMouseRelease->Bind([this](const PUi8& button)
		{
			if (button == SDL_BUTTON_RIGHT)
			{
				m_CanZoom = false;
				cameraRef->ResetZoom();
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
			if (!m_InputMode)
			{
				if (cameraRef->thirdPerson)
				{
					// Move the camera according to the tanks direction
					cameraRef->Translate(m_TankDirection);
					tankRef->Translate(m_TankDirection);
				}
					
				else
				{
					// Translate the camera based on input direction
					cameraRef->Translate(m_CameraDirection);

					// Rotate the camera based on input direction
					// glm::abs = absolute (force a positive value)
					camRef->Rotate(m_CameraRotation, glm::abs(m_CameraRotation));
				}
			}
		}
		m_GraphicsEngine->Render(m_SDLWindow);
	}
} 