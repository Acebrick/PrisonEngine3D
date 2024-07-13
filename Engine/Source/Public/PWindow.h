#pragma once

// System Libs
#include "EngineTypes.h"
#include "Math/PSTransform.h"

class PGraphicsEngine;
class PInput;
struct PSCamera;
class PModel;

struct PSWindowParams
{
	// Default constructor
	PSWindowParams()
	{
		title = "Prison Engine Window";
		x = y = 0;
		w = 1280;
		h = 720;
		vsync = false;
		fullscreen = false;
	}

	// Settings constructor
	PSWindowParams(PString title, int x, int y, unsigned int w, unsigned int h) :
		title(title),
		x(x), y(y),
		w(w), h(h),
		vsync(false),
		fullscreen(false) {}

	// Title of the window
	PString title;

	// Position of the window
	int x, y;

	// Width and height of the window
	unsigned int w, h;

	// VSync enable
	bool vsync;

	// Fullscreen enable
	bool fullscreen;
};

struct SDL_Window;

class PWindow 
{
public:
	PWindow();
	~PWindow();

	// Create the window with parameters
	bool CreateWindow(const PSWindowParams& params);

	// Close the window
	void CloseWindow() { m_ShouldClose = true; }

	// Check if the window has been set to closed
	bool IsPendingClose() { return m_ShouldClose; }

	// Listen for input
	void RegisterInput(const TShared<PInput>& m_Input);

	// Render the graphics engine
	void Render();

private:
	// A ref to the window in sdl
	SDL_Window* m_SDLWindow;

	// Window parameters
	PSWindowParams m_Params;

	// Determine if the window should close
	bool m_ShouldClose;

	// Store the graphics engine
	TUnique<PGraphicsEngine> m_GraphicsEngine;

	// Direction to move the camera
	glm::vec3 m_CameraDirection;

	// Amount to rotate the camera
	glm::vec3 m_CameraRotation;

	// Can zoom
	bool m_CanZoom;

	// Is the user in input mode
	bool m_InputMode;

	TShared<PSCamera> cameraRef;
	TShared<PModel> tankRef;

	glm::vec3 m_TankDirection;
};