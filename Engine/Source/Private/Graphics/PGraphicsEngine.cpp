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
std::unique_ptr<PMesh> m_MeshTriangle3D;
std::unique_ptr<PMesh> m_MeshSquareYellow;
std::unique_ptr<PMesh> m_MeshSquareBlue;
std::unique_ptr<PMesh> m_MeshSclera;
std::unique_ptr<PMesh> m_MeshIris;
std::unique_ptr<PMesh> m_MeshLightning;
std::unique_ptr<PMesh> m_Background;
std::unique_ptr<PMesh> m_MeshIrisPoint;
std::unique_ptr<PMesh> m_MeshStar;
std::unique_ptr<PMesh> m_MeshRectangle3D;

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
	m_MeshTriangle3D = std::make_unique<PMesh>();
	m_MeshSquareYellow = std::make_unique<PMesh>();
	m_MeshSquareBlue = std::make_unique<PMesh>();
	m_MeshSclera = std::make_unique<PMesh>();
	m_MeshIris = std::make_unique<PMesh>();
	m_MeshLightning = std::make_unique<PMesh>();
	m_Background = std::make_unique<PMesh>();
	m_MeshIrisPoint = std::make_unique<PMesh>();
	m_MeshStar = std::make_unique<PMesh>();
	m_MeshRectangle3D = std::make_unique<PMesh>();

	// TRIANGLE
	Draw3DTriangle();
	if (!m_MeshTriangle3D->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create triangle debug mesh");

	// SCLERA
	MakeMeshSolidColour(1.0f, 1.0f, 1.0f);
	if (!m_MeshSclera->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create sclera debug mesh");

	// IRIS
	DrawTriangle(0.0f, 1.25f, -0.5f, -0.5f, 0.5f, -0.5f);
	MakeMeshSolidColour(1.0f, 0.0f, 1.0f);
	if (!m_MeshIris->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create iris debug mesh");

	// IRIS POINT
	DrawTriangle(0.0f, 0.75f, -0.25f, -0.5f, 0.25f, -0.5f);
	MakeMeshSolidColour(1.0f, 0.0f, 1.0f);
	if (!m_MeshIrisPoint->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create iris point debug mesh");

	// SQUARE YELLOW
	DrawSquare(1.0f, 0.75f, 0.75f, 1.0f);
	MakeMeshSolidColour(1.0f, 1.0f, 0.0f);
	if (!m_MeshSquareYellow->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create square debug mesh");

	// SQUARE BLUE
	MakeMeshSolidColour(0.0f, 0.0f, 1.0f);
	if (!m_MeshSquareBlue->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create square debug mesh");

	// BACKGROUND
	DrawSquare(2.0f, 2.0f, 2.0f, 2.0f);
	if (!m_Background->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create background debug mesh");

	// LIGHTNING
	DrawLightning();
	MakeMeshSolidColour(0.2f, 0.0f, 0.5f);
	if (!m_MeshLightning->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create lightning debug mesh");

	// STAR
	DrawStar();
	if (!m_MeshStar->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create star debug mesh");

	// 3D RECTANGLE
	Draw3DRectangle();
	if (!m_MeshRectangle3D->CreateMesh(vertexData, indexData))
		PDebug::Log("Failed to create square debug mesh");

	return true;
}

void PGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// Set a background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Clear the back buffer with a solid colour
	glClear(GL_COLOR_BUFFER_BIT);

	PSTransform transform;
	static PSTransform transformBackground;
	static PSTransform transformStar;
	static PSTransform transformNose;
	static PSTransform transformDiamond;
	
	// BACKGROUND
	static float moveSpeed = 0.0f;
	static bool bottomReached = false;

	// Move the background up and down
	transformBackground.position.y += moveSpeed;

	m_Background->Render(m_Shader, transformBackground);

	// Check if moveSpeed needs to change direction
	if (transformBackground.position.y < -2.5f)
		bottomReached = true;
	else if (transformBackground.position.y > 0.95f)
		bottomReached = false;

	// Set the moveSpeed to be up or down based on the bottomReached boolean
	if (bottomReached)
		moveSpeed = 0.01f;
	else
		moveSpeed = -0.01f;

	//STAR
	static float scale = 0.0f;
	static bool increaseScale = true;
	transformStar.position.y = 0.75f;

	// Change the scale and rotation of the star
	transformStar.scale = glm::vec3(scale);
	transformStar.rotation.z += 10.0f;

	m_MeshStar->Render(m_Shader, transformStar);

	// Check if the scale bounds have been reached
	if (scale > 0.3f)
		increaseScale = false;
	else if (scale < 0.0f)
		increaseScale = true;

	// Change the scale value based on the increaseScale boolean
	if (!increaseScale)
		scale -= 0.001f;
	else
		scale += 0.001f;

	// 3D TRIANGLE
	transformNose.position.y = -0.5f;
	transformNose.scale = glm::vec3(0.5f);
	transformNose.rotation.x = 15.0f;
	transformNose.rotation.y += 0.5f;
	m_MeshTriangle3D->Render(m_Shader, transformNose);

	// 3D RECTANGLE
	transformDiamond.position.y = -0.9f;
	transformDiamond.rotation.y = 225.0f;
	//transformDiamond.rotation.x = 45.0f;
	transformDiamond.rotation.z += 0.5f;
	transformDiamond.scale = glm::vec3(0.5f);
	m_MeshRectangle3D->Render(m_Shader, transformDiamond);

	// 2D DIAMOND/SQUARE
	// YELLOW SQUARE
	transform.position.x = -0.875f;
	transform.position.y = -0.85f;
	transform.scale = glm::vec3(0.075f);
	transform.rotation.z = -45.0f;
	m_MeshSquareYellow->Render(m_Shader, transform);
	//BLUE SQUARE
	transform.position.x = 0.875;
	m_MeshSquareBlue->Render(m_Shader, transform);

	// SCLERA
	transform.position.x = -0.5f;
	transform.position.y = 0.0f;
	transform.scale = glm::vec3(0.5f);

	// Outer controls the number of eyes
	for (int i = 0; i < 2; i++)
	{
		// Inner loop rendering a triangle while slightly rotating each iteration for the sclera and iris
		for (float j = 0.0f; j < 360.0f; j += 1.0f)
		{
			// Render the triangle for the sclera
			m_MeshSclera->Render(m_Shader, transform);

			// Increase and set rotation for next loop
			transform.rotation.z = j;
		}
		// Set the position of the second eye
		transform.position.x = 0.5f;
	}

	// IRIS
	transform.position.x = -0.5f;
	transform.scale = glm::vec3(0.2f);
	transform.rotation.y = 77.8f; // Rotate on y axis to turn the circle into an oval
	// Loop twice for two eyes
	for (int i = 0; i < 2; i++)
	{
		// Loop rendering a triangle while slightly rotating each iteration
		for (float i = 0.0f; i < 360.0f; i += 1.0f)
		{
			// Render the triangle
			m_MeshIris->Render(m_Shader, transform);
			// Increase/set rotation for next loop
			transform.rotation.z = i;
		}
		// Set the position of the second eye
		transform.position.x = 0.5f;
	}

	// Top iris point
	transform.rotation.y = 0.0f;
	transform.rotation.z = 0.0f;
	transform.position.x = -0.5f;
	transform.scale = glm::vec3(0.195f);
	transform.position.y = 0.2f;
	m_MeshIrisPoint->Render(m_Shader, transform); // Left
	transform.position.x = 0.5f;
	m_MeshIrisPoint->Render(m_Shader, transform); // Right
	// Bottom iris point
	transform.rotation.x = 180.0f;
	transform.position.y = -0.2f;
	m_MeshIrisPoint->Render(m_Shader, transform); // Right
	transform.position.x = -0.5f;
	m_MeshIrisPoint->Render(m_Shader, transform); // Left

	// LIGHTNING BOLT LEFT
	transform.position.x = -0.5f;
	transform.position.y = 0.6f;
	transform.scale = glm::vec3(0.5f);
	transform.rotation.x = 0.0f;
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

void PGraphicsEngine::DrawTriangle(float topX, float topY, float botLeftX, float botLeftY, float botRightX, float botRightY)
{
	indexData.resize(3);
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;

	vertexData.resize(3);

	vertexData[0].m_Position[0] = topX;
	vertexData[0].m_Position[1] = topY;
	vertexData[0].m_Position[2] = 0.0f;

	vertexData[1].m_Position[0] = botLeftX;
	vertexData[1].m_Position[1] = botLeftY;
	vertexData[1].m_Position[2] = 0.0f;

	vertexData[2].m_Position[0] = botRightX;
	vertexData[2].m_Position[1] = botRightY;
	vertexData[2].m_Position[2] = 0.0f;
	
	// Colour for nose
	vertexData[0].m_Colour[0] = 1.0f;
	vertexData[0].m_Colour[1] = 0.0f;
	vertexData[0].m_Colour[2] = 0.0f;

	vertexData[1].m_Colour[0] = 0.0f;
	vertexData[1].m_Colour[1] = 1.0f;
	vertexData[1].m_Colour[2] = 0.0f;

	vertexData[2].m_Colour[0] = 0.0f;
	vertexData[2].m_Colour[1] = 0.0f;
	vertexData[2].m_Colour[2] = 1.0f;
}

void PGraphicsEngine::Draw3DTriangle()
{
	// Bottom right triangle
	vertexData.resize(5);
	// TRIANGLE 1
	// Top middle
	vertexData[0].m_Position[0] = 0.0f;
	vertexData[0].m_Position[1] = 0.5f;
	vertexData[0].m_Position[2] = 0.0f;

	vertexData[0].m_Colour[0] = 1.0f;
	vertexData[0].m_Colour[1] = 0.0f;
	vertexData[0].m_Colour[2] = 0.0f;

	// Bottom left
	vertexData[1].m_Position[0] = -0.5f;
	vertexData[1].m_Position[1] = -0.5f;
	vertexData[1].m_Position[2] = 0.5f;

	vertexData[1].m_Colour[0] = 0.0f;
	vertexData[1].m_Colour[1] = 1.0f;
	vertexData[1].m_Colour[2] = 0.0f;

	// Bottom right
	vertexData[2].m_Position[0] = 0.5f;
	vertexData[2].m_Position[1] = -0.5f;
	vertexData[2].m_Position[2] = 0.5f;

	vertexData[2].m_Colour[0] = 0.0f;
	vertexData[2].m_Colour[1] = 0.0f;
	vertexData[2].m_Colour[2] = 1.0f;

	// TRIANGLE 2 (opposite triangle 1)
	// Bottom left
	vertexData[3].m_Position[0] = -0.5f;
	vertexData[3].m_Position[1] = -0.5f;
	vertexData[3].m_Position[2] = -0.5f;

	vertexData[3].m_Colour[0] = 0.0f;
	vertexData[3].m_Colour[1] = 1.0f;
	vertexData[3].m_Colour[2] = 0.0f;

	// Bottom right
	vertexData[4].m_Position[0] = 0.5f;
	vertexData[4].m_Position[1] = -0.5f;
	vertexData[4].m_Position[2] = -0.5f;

	vertexData[4].m_Colour[0] = 0.0f;
	vertexData[4].m_Colour[1] = 0.0f;
	vertexData[4].m_Colour[2] = 1.0f;

	indexData.resize(18);
	// SIDE 1 & 2
	// Triangle 1
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	// Triangle opposite 1
	indexData[3] = 0;
	indexData[4] = 3;
	indexData[5] = 4;
	// SIDE 3 & 4
	// Triangle 3
	indexData[6] = 0;
	indexData[7] = 1;
	indexData[8] = 3;
	// Triangle opposite 3
	indexData[9] = 0;
	indexData[10] = 2;
	indexData[11] = 4;
	// Base triangle 1
	indexData[9] = 1;
	indexData[10] = 3;
	indexData[11] = 2;
	// Base triangle 2
	indexData[12] = 2;
	indexData[13] = 4;
	indexData[14] = 3;
}

void PGraphicsEngine::DrawSquare(float topLeft, float topRight, float botLeft, float botRight)
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
	vertexData[0].m_Position[0] = topRight;
	vertexData[0].m_Position[1] = topRight;
	vertexData[0].m_Position[2] = 0.0f;

	// Top left
	vertexData[1].m_Position[0] = -topLeft;
	vertexData[1].m_Position[1] = topLeft;
	vertexData[1].m_Position[2] = 0.0f;

	// Bottom right
	vertexData[2].m_Position[0] = botRight;
	vertexData[2].m_Position[1] = -botRight;
	vertexData[2].m_Position[2] = 0.0f;

	// Bottom left
	vertexData[3].m_Position[0] = -botLeft;
	vertexData[3].m_Position[1] = -botLeft;
	vertexData[3].m_Position[2] = 0.0f;

	// Colour used for background
	vertexData[0].m_Colour[0] = 0.0f;
	vertexData[0].m_Colour[1] = 0.0f;
	vertexData[0].m_Colour[2] = 0.0f;

	vertexData[1].m_Colour[0] = 0.0f;
	vertexData[1].m_Colour[1] = 0.0f;
	vertexData[1].m_Colour[2] = 0.0f;

	vertexData[2].m_Colour[0] = 1.0f;
	vertexData[2].m_Colour[1] = 0.0f;
	vertexData[2].m_Colour[2] = 0.0f;

	vertexData[3].m_Colour[0] = 1.0f;
	vertexData[3].m_Colour[1] = 0.0f;
	vertexData[3].m_Colour[2] = 0.0f;
}

void PGraphicsEngine::Draw3DRectangle()
{
	float topRight = 0.1f;
	float topLeft = 0.1f;
	float botRight = 0.1f;
	float botLeft = 0.1f;
	float length = 2.0f;

	vertexData.resize(8);
	MakeMeshSolidColour(1.0f, 1.0f, 0.0f);

	// FRONT SQUARE
	// Top left
	vertexData[0].m_Position[0] = -topLeft;
	vertexData[0].m_Position[1] = topLeft;
	vertexData[0].m_Position[2] = length;
	// Top right
	vertexData[1].m_Position[0] = topRight;
	vertexData[1].m_Position[1] = topRight;
	vertexData[1].m_Position[2] = length;
	// Bottom left
	vertexData[2].m_Position[0] = -botLeft;
	vertexData[2].m_Position[1] = -botLeft;
	vertexData[2].m_Position[2] = length;
	// Bottom right
	vertexData[3].m_Position[0] = botRight;
	vertexData[3].m_Position[1] = -botRight;
	vertexData[3].m_Position[2] = length;

	// BACK SQUARE
	// Top left
	vertexData[4].m_Position[0] = -topLeft;
	vertexData[4].m_Position[1] = topLeft;
	vertexData[4].m_Position[2] = -length;
	vertexData[4].m_Colour[0] = 0.0f;
	vertexData[4].m_Colour[1] = 0.0f;
	vertexData[4].m_Colour[2] = 1.0f;
	// Top right
	vertexData[5].m_Position[0] = topRight;
	vertexData[5].m_Position[1] = topRight;
	vertexData[5].m_Position[2] = -length;
	vertexData[5].m_Colour[0] = 0.0f;
	vertexData[5].m_Colour[1] = 0.0f;
	vertexData[5].m_Colour[2] = 1.0f;
	// Bottom left
	vertexData[6].m_Position[0] = -botLeft;
	vertexData[6].m_Position[1] = -botLeft;
	vertexData[6].m_Position[2] = -length;
	vertexData[6].m_Colour[0] = 0.0f;
	vertexData[6].m_Colour[1] = 0.0f;
	vertexData[6].m_Colour[2] = 1.0f;
	// Bottom right
	vertexData[7].m_Position[0] = botRight;
	vertexData[7].m_Position[1] = -botRight;
	vertexData[7].m_Position[2] = -length;
	vertexData[7].m_Colour[0] = 0.0f;
	vertexData[7].m_Colour[1] = 0.0f;
	vertexData[7].m_Colour[2] = 1.0f;

	indexData.resize(36);
	
	// FRONT SQUARE
	// Triangle 1
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	// Triangle 2
	indexData[3] = 1;
	indexData[4] = 2;
	indexData[5] = 3;

	// BACK SQUARE
	// Triangle 1
	indexData[6] = 4;
	indexData[7] = 5;
	indexData[8] = 6;
	// Triangle 2
	indexData[9] = 5;
	indexData[10] = 6;
	indexData[11] = 7;

	// LEFT SIDE
	// Triangle 1
	indexData[12] = 0;
	indexData[13] = 2;
	indexData[14] = 6;
	// Triangle 2
	indexData[15] = 0;
	indexData[16] = 6;
	indexData[17] = 4;

	// RIGHT SIDE
	// Triangle 1
	indexData[18] = 1;
	indexData[19] = 3;
	indexData[20] = 5;
	// Triangle 2
	indexData[21] = 3;
	indexData[22] = 5;
	indexData[23] = 7;

	// TOP SIDE
	// Triangle 1
	indexData[24] = 0;
	indexData[25] = 1;
	indexData[26] = 4;
	// Triangle 2
	indexData[27] = 1;
	indexData[28] = 4;
	indexData[29] = 5;

	// BOTTOM SIDE
	// Triangle 1
	indexData[30] = 2;
	indexData[31] = 6;
	indexData[32] = 3;
	// Triangle 2
	indexData[33] = 3;
	indexData[34] = 6;
	indexData[35] = 7;
}

void PGraphicsEngine::DrawLightning()
{
	indexData.resize(6);
	// Top of lightning
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	// Bottom of lightning
	indexData[3] = 3;
	indexData[4] = 4;
	indexData[5] = 5;

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
}

void PGraphicsEngine::DrawStar()
{
	indexData.resize(6);
	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;
	indexData[3] = 3;
	indexData[4] = 4;
	indexData[5] = 5;

	vertexData.resize(12);
	// NORMAL TRIANGLE
	// Top
	vertexData[0].m_Position[0] = 0.0f;
	vertexData[0].m_Position[1] = 0.5f;
	vertexData[0].m_Position[2] = 0.0f;
	vertexData[0].m_Colour[0] = 1.0f;
	vertexData[0].m_Colour[1] = 0.0f;
	vertexData[0].m_Colour[2] = 1.0f;
	// Bottom left
	vertexData[1].m_Position[0] = -0.5f;
	vertexData[1].m_Position[1] = -0.5f;
	vertexData[1].m_Position[2] = 0.0f;
	vertexData[1].m_Colour[0] = 1.0f;
	vertexData[1].m_Colour[1] = 1.0f;
	vertexData[1].m_Colour[2] = 0.0f;
	// Bottom right
	vertexData[2].m_Position[0] = 0.5f;
	vertexData[2].m_Position[1] = -0.5f;
	vertexData[2].m_Position[2] = 0.0f;
	vertexData[2].m_Colour[0] = 0.0f;
	vertexData[2].m_Colour[1] = 1.0f;
	vertexData[2].m_Colour[2] = 1.0f;

	// UPSIDE DOWN TRIANGLE
	// Top left
	vertexData[3].m_Position[0] = -0.5f;
	vertexData[3].m_Position[1] = 0.25f;
	vertexData[3].m_Position[2] = 0.0f;
	vertexData[3].m_Colour[0] = 1.0f;
	vertexData[3].m_Colour[1] = 1.0f;
	vertexData[3].m_Colour[2] = 0.0f;
	// Top right
	vertexData[4].m_Position[0] = 0.5f;
	vertexData[4].m_Position[1] = 0.25f;
	vertexData[4].m_Position[2] = 0.0f;
	vertexData[4].m_Colour[0] = 1.0f;
	vertexData[4].m_Colour[1] = 0.0f;
	vertexData[4].m_Colour[2] = 1.0f;
	// Bottom
	vertexData[5].m_Position[0] = 0.0f;
	vertexData[5].m_Position[1] = -0.75f;
	vertexData[5].m_Position[2] = 0.0f;
	vertexData[5].m_Colour[0] = 0.0f;
	vertexData[5].m_Colour[1] = 1.0f;
	vertexData[5].m_Colour[2] = 1.0f;
}

void PGraphicsEngine::MakeMeshSolidColour(float r, float g, float b)
{
	// Loop through all the vertices changing them to one colour
	for (int i = 0; i < vertexData.size(); i++)
	{
		vertexData[i].m_Colour[0] = r;
		vertexData[i].m_Colour[1] = g;
		vertexData[i].m_Colour[2] = b;
	}
}