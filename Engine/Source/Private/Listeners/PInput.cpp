#include "Listeners/PInput.h"
#include "PWindow.h"

PInput::PInput()
{
	OnKeyPress = TMakeShared<PEvents<SDL_Scancode>>();
	OnKeyRelease = TMakeShared<PEvents<SDL_Scancode>>();
	OnMousePress = TMakeShared<PEvents<>>();
	OnMousePress = TMakeShared<PEvents<>>();
}

void PInput::InitInput(const TShared<PWindow>& window)
{
	// To assign a weak pointer, just plug in the shared pointer
	m_Window = window;
}

void PInput::UpdateInputs()
{
	// TO DO: Game loop
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			if (const auto& windowRef = m_Window.lock())
				windowRef->CloseWindow();
		}

		// Run different events depending on the type of input
		switch (e.type)
		{
		case SDL_KEYDOWN:
			// Is the key not already being held down
			if (e.key.repeat == 0)
				OnKeyPress->Run(e.key.keysym.scancode);
			break;
		case SDL_KEYUP:
			if (e.key.repeat == 0)
				OnKeyRelease->Run(e.key.keysym.scancode);
			break;
		default:
			break;
		}
	}
}
