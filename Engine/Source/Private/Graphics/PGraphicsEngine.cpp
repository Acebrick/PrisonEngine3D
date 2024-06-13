#include "Graphics/PGraphicsEngine.h"
#include "Debug/PDebug.h"
#include "Graphics/PMesh.h"
#include "Graphics/PShaderProgram.h"
#include "Math/PSTransform.h"

// External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

std::vector<PSVertexData> vertexData; // Triangle
std::vector<uint32_t> indexData;

// Test mesh for debug
std::unique_ptr<PMesh> m_MeshTriangle;
std::unique_ptr<PMesh> m_MeshSquare;
std::unique_ptr<PMesh> m_MeshSclera;
std::unique_ptr<PMesh> m_MeshIris;
std::unique_ptr<PMesh> m_MeshLightning;

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

	m_Shader = std::make_shared<PShaderProgram>();

	// Attempt to initialise shdaer and test if failed
	if (!m_Shader->InitShader("Shaders/SimpleShader/SimpleShader.vertex", "Shaders/SimpleShader/SimpleShader.frag"))
	{
		PDebug::Log("Graphics engine failed to intialise due to shader failure");
		return false;
	}

	// Log the success of the graphics engine
	PDebug::Log("Successfully initialised graphics engine", LT_SUCCESS);

	// Create the debug mesh
	m_MeshTriangle = std::make_unique<PMesh>();
	m_MeshSquare = std::make_unique<PMesh>();
	m_MeshSclera = std::make_unique<PMesh>();
	m_MeshIris = std::make_unique<PMesh>();
	m_MeshLightning = std::make_unique<PMesh>();

	vertexData.resize(3);

	// TRIANGLE
	SetTriangleData();
	if (!m_MeshTriangle->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create triangle debug mesh");

	// SCLERA
	SetScleraData();
	if (!m_MeshSclera->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create sclera debug mesh");

	// IRIS
	SetIrisData();
	if (!m_MeshIris->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create iris debug mesh");

	// SQUARE
	SetSquareData();
	if (!m_MeshSquare->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create square debug mesh");

	// LIGHTNING
	SetLightningData();
	if (!m_MeshLightning->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create lightning debug mesh");

	return true;
}

void PGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// Set a background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Clear the back buffer with a solid colour
	glClear(GL_COLOR_BUFFER_BIT);

	// TRIANGLE
	// Transform of the triangle
	PSTransform transform;
	transform.position.y = -0.5f;
	transform.scale = glm::vec3(0.5f);
	// Render the triangle
	m_MeshTriangle->Render(m_Shader, transform);

	// SQUARE
	// Transform of the square
	transform.position.x = -0.9f;
	transform.position.y = -0.85f;
	transform.rotation.z = 45.0f;
	transform.scale = glm::vec3(0.1f);
	// Render the square
	m_MeshSquare->Render(m_Shader, transform);
	// Render another square on the other side
	transform.position.x = 0.9f;
	m_MeshSquare->Render(m_Shader, transform);

	// SCLERA
	transform.position.x = -0.5f;
	transform.position.y = 0.0f;
	transform.scale = glm::vec3(0.5f);
	// Loop inner loop twice for two eyes
	for (int i = 0; i < 2; i++)
	{
		// Loop rendering a triangle while slightly rotating each iteration
		for (float i = 0.0f; i < 360.0f; i += 0.1f)
		{
			// Render the triangle
			m_MeshSclera->Render(m_Shader, transform);
			// Increase/set rotation for next loop
			transform.rotation.z = i;
		}
		// Set the position of the second eye
		transform.position.x = 0.5f;
	}

	// IRIS
	transform.position.x = -0.5f;
	transform.scale = glm::vec3(0.2f);
	// Loop twice for two eyes
	for (int i = 0; i < 2; i++)
	{
		// Loop rendering a triangle while slightly rotating each iteration
		for (float i = 0.0f; i < 360.0f; i += 0.1f)
		{
			// Render the triangle
			m_MeshIris->Render(m_Shader, transform);
			// Increase/set rotation for next loop
			transform.rotation.z = i;
		}
		// Set the position of the second eye
		transform.position.x = 0.5f;
	}

	// LIGHTNING BOLT LEFT
	transform.position.x = -0.5f;
	transform.position.y = 0.6f;
	transform.scale = glm::vec3(0.5f);
	transform.rotation.z = 65.0f;
	m_MeshLightning->Render(m_Shader, transform);
	// LIGHTNING BOLT RIGHT
	transform.position.x = 0.5f;
	transform.rotation.y = 180.0f; // Rotate on the y axis to "flip" the mesh
	transform.rotation.z = 65.0f;
	m_MeshLightning->Render(m_Shader, transform);

	// Presented the frame to the window
	// Swapping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}

void PGraphicsEngine::SetTriangleData()
{
	indexData.resize(3);
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;

	// Top middle
	vertexData[0].m_Position[0] = 0.0f;
	vertexData[0].m_Position[1] = 0.5f;
	vertexData[0].m_Position[2] = 0.0f;
	// Colour for V1
	vertexData[0].m_Colour[0] = 1.0f;
	vertexData[0].m_Colour[1] = 0.0f;
	vertexData[0].m_Colour[2] = 0.0f;

	// Bottom left
	vertexData[1].m_Position[0] = -0.5f;
	vertexData[1].m_Position[1] = -0.5f;
	vertexData[1].m_Position[2] = 0.0f;
	// Colour for V2
	vertexData[1].m_Colour[0] = 0.0f;
	vertexData[1].m_Colour[1] = 1.0f;
	vertexData[1].m_Colour[2] = 0.0f;

	// Bottom right
	vertexData[2].m_Position[0] = 0.5f;
	vertexData[2].m_Position[1] = -0.5f;
	vertexData[2].m_Position[2] = 0.0f;
	// Colour for V3
	vertexData[2].m_Colour[0] = 0.0f;
	vertexData[2].m_Colour[1] = 0.0f;
	vertexData[2].m_Colour[2] = 1.0f;
}

void PGraphicsEngine::SetSquareData()
{
	indexData.resize(6);
	// Triangle 1
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	// Triangle 2
	indexData[3] = 1;
	indexData[4] = 2;
	indexData[5] = 3;

	vertexData.resize(4);
	// Top right
	vertexData[0].m_Position[0] = 0.8f;
	vertexData[0].m_Position[1] = 0.8f;
	vertexData[0].m_Position[2] = 0.0f;
	// Colour for V1
	vertexData[0].m_Colour[0] = 1.0f;
	vertexData[0].m_Colour[1] = 1.0f;
	vertexData[0].m_Colour[2] = 0.0f;

	// Top left
	vertexData[1].m_Position[0] = -0.4f;
	vertexData[1].m_Position[1] = 0.4f;
	vertexData[1].m_Position[2] = 0.0f;
	// Colour for V2
	vertexData[1].m_Colour[0] = 1.0f;
	vertexData[1].m_Colour[1] = 1.0f;
	vertexData[1].m_Colour[2] = 0.0f;

	// Bottom right
	vertexData[2].m_Position[0] = 0.4f;
	vertexData[2].m_Position[1] = -0.4f;
	vertexData[2].m_Position[2] = 0.0f;
	// Colour for V3
	vertexData[2].m_Colour[0] = 1.0f;
	vertexData[2].m_Colour[1] = 1.0f;
	vertexData[2].m_Colour[2] = 0.0f;

	// Bottom left
	vertexData[3].m_Position[0] = -0.8f;
	vertexData[3].m_Position[1] = -0.8f;
	vertexData[3].m_Position[2] = 0.0f;
	// Colour for V4
	vertexData[3].m_Colour[0] = 1.0f;
	vertexData[3].m_Colour[1] = 1.0f;
	vertexData[3].m_Colour[2] = 0.0f;
}

void PGraphicsEngine::SetScleraData()
{
	// Colour for V1
	vertexData[0].m_Colour[0] = 1.0f;
	vertexData[0].m_Colour[1] = 1.0f;
	vertexData[0].m_Colour[2] = 1.0f;

	// Colour for V2
	vertexData[1].m_Colour[0] = 1.0f;
	vertexData[1].m_Colour[1] = 1.0f;
	vertexData[1].m_Colour[2] = 1.0f;

	// Colour for V3
	vertexData[2].m_Colour[0] = 1.0f;
	vertexData[2].m_Colour[1] = 1.0f;
	vertexData[2].m_Colour[2] = 1.0f;
}

void PGraphicsEngine::SetIrisData()
{
	// Colour for V1
	vertexData[0].m_Colour[0] = 0.0f;
	vertexData[0].m_Colour[1] = 0.0f;
	vertexData[0].m_Colour[2] = 1.0f;

	// Colour for V2
	vertexData[1].m_Colour[0] = 0.0f;
	vertexData[1].m_Colour[1] = 0.0f;
	vertexData[1].m_Colour[2] = 1.0f;

	// Colour for V3
	vertexData[2].m_Colour[0] = 0.0f;
	vertexData[2].m_Colour[1] = 0.0f;
	vertexData[2].m_Colour[2] = 1.0f;
}

void PGraphicsEngine::SetLightningData()
{
	indexData.resize(6);
	// Top of lightning
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	// Bottom of lightning
	indexData[3] = 5;
	indexData[4] = 3;
	indexData[5] = 4;

	vertexData.resize(6);
	// TOP OF LIGHTNING TRIANGLE
	// Top
	vertexData[0].m_Position[0] = 0.15f;
	vertexData[0].m_Position[1] = 0.8f;
	vertexData[0].m_Position[2] = 0.0f;
	// Bottom left
	vertexData[1].m_Position[0] = -0.15f;
	vertexData[1].m_Position[1] = -0.1f;
	vertexData[1].m_Position[2] = 0.0f;
	// Bottom right
	vertexData[2].m_Position[0] = 0.025f;
	vertexData[2].m_Position[1] = 0.05f;
	vertexData[2].m_Position[2] = 0.0f;

	// BOTTOM OF LIGHTNING TRIANGLE
	// Top left
	vertexData[3].m_Position[0] = 0.0f;
	vertexData[3].m_Position[1] = 0.12f;
	vertexData[3].m_Position[2] = 0.0f;
	// Top right
	vertexData[4].m_Position[0] = 0.2f;
	vertexData[4].m_Position[1] = 0.3f;
	vertexData[4].m_Position[2] = 0.0f;
	// Bottom
	vertexData[5].m_Position[0] = -0.15f;
	vertexData[5].m_Position[1] = -0.8f;
	vertexData[5].m_Position[2] = 0.0f;
	
	// Change all vertices to yellow
	vertexData[0].m_Colour[0] = 1.0f;
	vertexData[0].m_Colour[1] = 1.0f;
	vertexData[0].m_Colour[2] = 0.0f;

	vertexData[1].m_Colour[0] = 1.0f;
	vertexData[1].m_Colour[1] = 1.0f;
	vertexData[1].m_Colour[2] = 0.0f;

	vertexData[2].m_Colour[0] = 1.0f;
	vertexData[2].m_Colour[1] = 1.0f;
	vertexData[2].m_Colour[2] = 0.0f;

	vertexData[3].m_Colour[0] = 1.0f;
	vertexData[3].m_Colour[1] = 1.0f;
	vertexData[3].m_Colour[2] = 0.0f;

	vertexData[4].m_Colour[0] = 1.0f;
	vertexData[4].m_Colour[1] = 1.0f;
	vertexData[4].m_Colour[2] = 0.0f;

	vertexData[5].m_Colour[0] = 1.0f;
	vertexData[5].m_Colour[1] = 1.0f;
	vertexData[5].m_Colour[2] = 0.0f;
}