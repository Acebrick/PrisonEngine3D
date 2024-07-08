#pragma once
#include "EngineTypes.h"
#include "Listeners/PEvents.h"

// External libs
#include <SDL/SDL_keycode.h>
#include <SDL/SDL_events.h>

class PWindow;

class PInput
{
public:
	PInput();

	//  Initialising the window
	void InitInput(const TShared<PWindow>& window);

	// Updating the inputs
	void UpdateInputs();

	// On a key press event
	TShared < PEvents <SDL_Scancode>> OnKeyPress;

	// On a key release event
	TShared<PEvents<SDL_Scancode>> OnKeyRelease;


	// On mouse movement
	// @params 1: mouse pos x
	// @params 2: mouse pos y
	// @params 3: x relative
	// @params 4: y relative
	TShared<PEvents<float, float, float, float>> OnMouseMove;

	// When the mouse scrolls pass the delta
	TShared<PEvents<float>> OnMouseScroll;

	// Listern for mouse pressed events
	TShared<PEvents<PUi8>> OnMousePress;

	// Listen for mouse released events
	TShared<PEvents<PUi8>> OnMouseRelease;

	// Hide mouse
	// This will also make the mouse is centered to the screen when moving
	void ShowCursor(const bool& enable);

	bool IsCursorHidden() const;

private:
	// Weak pointer to the window to not count as a reference
	// so that the input doesn't control if the window gets destroyed or not
	TWeak<PWindow> m_Window;

	// Last mouse motion event
	SDL_MouseMotionEvent m_LastMotion;
};