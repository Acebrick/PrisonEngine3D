#include "Graphics/PGraphicsEngine.h"
#include "Debug/PDebug.h"
#include "Graphics/PMesh.h"

// External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

std::vector<PSVertexData> triangle1Vertex;
std::vector<PSVertexData> triangle2Vertex;
std::vector<uint32_t> indexData;
// Test mesh for debug
std::unique_ptr<PMesh> m_Mesh;
std::unique_ptr<PMesh> m_Mesh2;


bool PGraphicsEngine::InitEngine(SDL_Window* sdlWindow, const bool& vsync)
{
	if (sdlWindow == nullptr)
	{
		PDebug::Log("SDL window was null", LT_ERROR);
		PDebug::Log("Graphics engine failed to initialise", LT_ERROR);
		return false;
	}

	// Create an open gl context
	m_SDLGLContext = SDL_GL_CreateContext(sdlWindow);

	// Test if the context failed
	if (m_SDLGLContext == nullptr)
	{
		PDebug::Log("SDL failed to create gl context: " + std::string(SDL_GetError()), LT_ERROR);
		PDebug::Log("Graphics engine failed to initialise", LT_ERROR);
		return false;
	}

	// Make the current context active for the SDL window
	// Test if it failed
	if (SDL_GL_MakeCurrent(sdlWindow, m_SDLGLContext) != 0)
	{
		PDebug::Log("SDL failed to make gl context current: " + std::string(SDL_GetError()), LT_ERROR);
		PDebug::Log("Graphics engine failed to initialise", LT_ERROR);
		return false;
	}

	if (vsync)
	{
		// Try enable adaptive vsync and test if it failed
		if (SDL_GL_SetSwapInterval(-1) != 0)
		{
			// Try to enable standard vsync and test if it failed
			if (SDL_GL_SetSwapInterval(1) != 0)
			{
				PDebug::Log("Graphics engine failed to initialise vsync" + std::string(SDL_GetError()), LT_WARN);
				return false;
			}
		}
	}

	// Initialise glew
	GLenum glewResult = glewInit();

	// Test if glew failed
	if (glewResult != GLEW_OK)
	{
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glewResult));
		PDebug::Log("Graphics engine failed to initialise GLEW: " + errorMsg);
		return false;
	}

	// Log the success of the graphics engine
	PDebug::Log("Successfully initialised graphics engine", LT_SUCCESS);

	// Create the debug mesh
	m_Mesh = std::make_unique<PMesh>();
	m_Mesh2 = std::make_unique<PMesh>();


	// Bottom right triangle
	triangle1Vertex.resize(3);
	triangle1Vertex[0].m_Position[0] = -0.5f;
	triangle1Vertex[0].m_Position[1] = 0.5f * 1.78f;
	triangle1Vertex[0].m_Position[2] = 0.0f;

	triangle1Vertex[1].m_Position[0] = -0.5f;
	triangle1Vertex[1].m_Position[1] = -0.5f * 1.78f;
	triangle1Vertex[1].m_Position[2] = 0.0f;

	triangle1Vertex[2].m_Position[0] = 0.5f;
	triangle1Vertex[2].m_Position[1] = -0.5f * 1.78f;
	triangle1Vertex[2].m_Position[2] = 0.0f;

	//Top right triangle
	triangle2Vertex.resize(3);
	triangle2Vertex[0].m_Position[0] = -0.5f;
	triangle2Vertex[0].m_Position[1] = 0.5f * 1.78f;
	triangle2Vertex[0].m_Position[2] = 0.0f;

	triangle2Vertex[1].m_Position[0] = 0.5f;
	triangle2Vertex[1].m_Position[1] = 0.5f * 1.78f;
	triangle2Vertex[1].m_Position[2] = 0.0f;

	triangle2Vertex[2].m_Position[0] = 0.5f;
	triangle2Vertex[2].m_Position[1] = -0.5f * 1.78f;
	triangle2Vertex[2].m_Position[2] = 0.0f;

	indexData.resize(3);
	indexData[0] = 0; // Vertex 1
	indexData[1] = 1; // Vertex 2
	indexData[2] = 2; // Vertex 3

	if (!m_Mesh->CreateMesh(triangle1Vertex, indexData))
	{
		PDebug::Log("Failed to create debug mesh");
	}

	if (!m_Mesh2->CreateMesh(triangle2Vertex, indexData))
	{
		PDebug::Log("Failed to create debug mesh2");
	}

	return true;
}

void PGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// There is a wireframe shader by default, this activates it
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Set a background colour
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	// Clear the back buffer with a solid colour
	glClear(GL_COLOR_BUFFER_BIT);

	// Render custom graphics
	m_Mesh->Render();
	m_Mesh2->Render();

	// Presented the frame to the window
	// Swapping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}
