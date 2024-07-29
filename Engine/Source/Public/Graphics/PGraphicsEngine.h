#pragma once
#include "EngineTypes.h"

typedef void* SDL_GLContext;
struct SDL_Window;
class PShaderProgram;
struct PSCamera;
struct PSLight;

class PGraphicsEngine
{
public:
	PGraphicsEngine() = default;
	~PGraphicsEngine() = default;

	//  Initialise the graphics engine
	bool InitEngine(SDL_Window* sdlWindow, const bool& vsync);

	// Render the graphics engine
	void Render(SDL_Window* sdlWindow);

	// Return a weak version of the camera
	TWeak<PSCamera> GetCamera() { return m_Camera; }

private:
	// Storing memory location for open gl context
	SDL_GLContext m_SDLGLContext;

	// Store the shader for trhe engine
	TShared<PShaderProgram> m_Shader;

	// Store the camera
	TShared<PSCamera> m_Camera;

	TArray<TShared<PSLight>> m_Lights;
};