#pragma once
#include <iostream>

typedef void* SDL_GLContext;
struct SDL_Window;
class PShaderProgram;

class PGraphicsEngine
{
public:
	PGraphicsEngine() = default;
	~PGraphicsEngine() = default;

	//  Initialise the graphics engine
	bool InitEngine(SDL_Window* sdlWindow, const bool& vsync);

	// Render the graphics engine
	void Render(SDL_Window* sdlWindow);

private:
	// Storing memory location for open gl context
	SDL_GLContext m_SDLGLContext;

	// Store the shader for trhe engine
	std::shared_ptr<PShaderProgram> m_Shader;

	void SetTriangleData();
	void SetSquareData();
	void SetScleraData();
	void SetIrisData();
	void SetLightningData();
};