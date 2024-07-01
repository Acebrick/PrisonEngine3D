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

	// On a mouse press event
	TShared<PEvents<>> OnMousePress;

	// On a mouse release event
	TShared<PEvents<>> OnMouseRelease;

private:
	// Weak pointer to the window to not count as a reference
	// so that the input doesn't control if the window gets destroyed or not
	TWeak<PWindow> m_Window;
};