#pragma once

// System Libs
#include <iostream>
#include <string>

class PGraphicsEngine;

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
	PSWindowParams(std::string title, int x, int y, unsigned int w, unsigned int h) :
		title(title),
		x(x), y(y),
		w(w), h(h),
		vsync(false),
		fullscreen(false) {}

	// Title of the window
	std::string title;

	// Position of the window
	int x, y;

	// Width and hright of the window
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
	std::unique_ptr<PGraphicsEngine> m_GraphicsEngine;
};