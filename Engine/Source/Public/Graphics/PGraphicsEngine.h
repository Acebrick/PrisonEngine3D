#pragma once
#include "EngineTypes.h"
#include "Graphics/PSMaterial.h"

typedef void* SDL_GLContext;
struct SDL_Window;
class PShaderProgram;
struct PSCamera;
struct PSLight;
struct PSPointLight;
struct PSDirLight;
class PModel;

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

	// Create a point light and return a weak pointer
	TWeak<PSPointLight> CreatePointLight();

	// Create a directional light and return a weak pointer
	TWeak<PSDirLight> CreateDirLight();

	// Import a model and return a weak pointer
	TWeak<PModel> ImportModel(const PString& path);

	// Create a material for the engine
	TShared<PSMaterial> CreateMaterial();

private:
	// Storing memory location for open gl context
	SDL_GLContext m_SDLGLContext;

	// Store the shader for trhe engine
	TShared<PShaderProgram> m_Shader;

	// Store the camera
	TShared<PSCamera> m_Camera;

	TArray<TShared<PSLight>> m_Lights;

	// Stores all of the models in the engine
	TArray<TShared<PModel>> m_Models;
};