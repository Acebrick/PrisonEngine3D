#include "Graphics/PGraphicsEngine.h"
#include "Graphics/PModel.h"
#include "Graphics/PShaderProgram.h"
#include "Math/PSTransform.h"
#include "Graphics/PTexture.h"
#include "Graphics/PSCamera.h"
#include "Graphics/PSLight.h"

// External Libs
#include <GLEW/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


const std::vector<PSVertexData> vertexData = {
	//   x      y	   z      r     g     b       tx    ty
	{ {-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 1 (top left)
	{ { 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} }, // vertex data 2 (top right)
	{ {-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 3 (bot left) 
	{ { 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} } // vertex data 4 (bot right)
};

const std::vector<uint32_t> indexData = {
	 0, 1, 2, // Triangle 1
	 1, 2, 3  // Triangle 2
};

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

	// Enable depth to be tested
	glEnable(GL_DEPTH_TEST);

	// Create the shader object
	m_Shader = TMakeShared<PShaderProgram>();

	// Attempt to initialise shdaer and test if failed
	if (!m_Shader->InitShader("Shaders/SimpleShader/SimpleShader.vertex", "Shaders/SimpleShader/SimpleShader.frag"))
	{
		PDebug::Log("Graphics engine failed to intialise due to shader failure");
		return false;
	}

	// Create the camera
	m_Camera = TMakeShared<PSCamera>();
	m_Camera->transform.position.y = 200.0f;
	m_Camera->transform.position.z = -75.0f;

	//TShared<PTexture> defaultTexture = TMakeShared<PTexture>();

	//if (!defaultTexture->LoadTexture("Default grid", "Textures/P_DefaultGrid.png"))
	//{
	//	PDebug::Log("Graphics engine default texture failed to load", LT_ERROR);
	//}

	//m_DefaultMaterial = TMakeShared<PSMaterial>();

	//m_DefaultMaterial->m_BaseColourMap = defaultTexture;

	/*
	// POINT LIGHT
	m_PointLight = CreatePointLight();
	if (const auto& lightRef = m_PointLight.lock())
	{
		m_PointLight.lock()->colour = glm::vec3(1.0f, 0.5f, 0.0f);
		m_PointLight.lock()->linear = 0.007f;
		m_PointLight.lock()->quadratic = 0.0002f;
	}

	// POINT LIGHT 2
	m_PointLight2 = CreatePointLight();
	if (const auto& lightRef = m_PointLight2.lock())
	{
		m_PointLight2.lock()->position.x = 40.0f;
		m_PointLight2.lock()->position.y = 70.0f;
		m_PointLight2.lock()->position.z = 115.0f;
		m_PointLight2.lock()->colour = glm::vec3(1.0f, 0.0f, 1.0f);
		m_PointLight2.lock()->linear = 0.007f;
		m_PointLight2.lock()->quadratic = 0.0002f;
	}

	// SPOT LIGHT
	m_SpotLight = CreateSpotLight();
	if (const auto& lightRef = m_SpotLight.lock())
	{
		m_SpotLight.lock()->SetInnerCutOff(3.5f);
		m_SpotLight.lock()->SetOuterCutOff(7.5f);
		m_SpotLight.lock()->colour = glm::vec3(0.0f, 1.0f, 0.0f);
		m_SpotLight.lock()->linear = 0.0014f;
		m_SpotLight.lock()->quadratic = 0.000007f;
	}
	// SPOT LIGHT 2
	m_SpotLight2 = CreateSpotLight();
	if (const auto& lightRef = m_SpotLight2.lock())
	{
		m_SpotLight2.lock()->SetInnerCutOff(3.5f);
		m_SpotLight2.lock()->SetOuterCutOff(7.5f);
		m_SpotLight2.lock()->colour = glm::vec3(1.0f, 0.0f, 0.0f);
		m_SpotLight2.lock()->linear = 0.0014f;
		m_SpotLight2.lock()->quadratic = 0.000007f;
	}
	
	// FLASHLIGHT
	m_Flashlight = CreateSpotLight();
	if (const auto& lightRef = m_Flashlight.lock())
	{
		m_Flashlight.lock()->linear = 0.0014f;
		m_Flashlight.lock()->quadratic = 0.000007f;
	}

	// NORMAL MAPPING TEST MODEL
	//m_Wall = ImportModel("Models/Wall/scene.gltf");
	//TShared<PTexture> wallTex = TMakeShared<PTexture>();
	//TShared<PTexture> wallNormal = TMakeShared<PTexture>();
	//TShared<PSMaterial> wallMat = TMakeShared<PSMaterial>();
	//wallTex->LoadTexture("Wall diffuse", "Models/Wall/textures/Brick_Wall_02_baseColor.jpeg");
	//wallNormal->LoadTexture("Wall normal", "Models/Wall/textures/Brick_Wall_02_normal.png");
	//m_Wall.lock()->GetTransform().position.x = -400.0f;
	//m_Wall.lock()->GetTransform().position.y = 20.0f;
	//m_Wall.lock()->GetTransform().position.z = -50.0f;
	//wallMat->m_BaseColourMap = wallTex;
	//wallMat->m_NormalMap = wallNormal;
	//m_Wall.lock()->SetMaterialBySlot(0, wallMat);
	*/

	// Create the dir light
	const auto& dirLight = CreateDirLight();

	// Check if exists as a reference and change it
	if (const auto& lightRef = dirLight.lock())
	{
		lightRef->colour = glm::vec3(1.0f, 1.0f, 1.0f);
		lightRef->intensity = 1.0f;
		lightRef->direction = glm::vec3(0.0f, 1.0f, 0.0f);
		lightRef->ambient = glm::vec3(0.5f);

}

	// Log the success of the graphics engine
	PDebug::Log("Successfully initialised graphics engine", LT_SUCCESS);

	return true;
}

void PGraphicsEngine::Render(SDL_Window* sdlWindow)
{
	// Set a background colour
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Clear the back buffer with a solid colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	
	/* SKULL
	

	// POINT LIGHT
	// Lock the first point light to the skull moving around the room
	m_PointLight.lock()->position = m_Skull.lock()->GetTransform().position;

	// POINT LIGHT 2
	// Circle around the throne
	TranslateOffModelRotation(m_Throne.lock()->GetTransform().position.x, m_Throne.lock()->GetTransform().position.z,
							1.0f, m_PointLight2.lock()->position.x, m_PointLight2.lock()->position.z);
		
	// BLUDGEON
	m_Bludgeon.lock()->GetTransform().rotation.y += 0.99f;

	// SPOT LIGHTS
	// Lock the spot lights position/direction to the skull
	if (movingOnX)
	{
		m_SpotLight.lock()->position.x = m_Skull.lock()->GetTransform().position.x;
		m_SpotLight.lock()->position.y = m_Skull.lock()->GetTransform().position.y + 1;
		m_SpotLight.lock()->position.z = m_Skull.lock()->GetTransform().position.z - 3;
		m_SpotLight.lock()->direction.x = m_Skull.lock()->GetTransform().Forward().x;
		m_SpotLight.lock()->direction.y = m_Skull.lock()->GetTransform().Forward().y;
		m_SpotLight.lock()->direction.z = m_Skull.lock()->GetTransform().Forward().z - 0.03f;
		m_SpotLight2.lock()->position.x = m_Skull.lock()->GetTransform().position.x;
		m_SpotLight2.lock()->position.y = m_Skull.lock()->GetTransform().position.y + 1;
		m_SpotLight2.lock()->position.z = m_Skull.lock()->GetTransform().position.z + 3;
		m_SpotLight2.lock()->direction = m_Skull.lock()->GetTransform().Forward();
		m_SpotLight2.lock()->direction.x = m_Skull.lock()->GetTransform().Forward().x;
		m_SpotLight2.lock()->direction.y = m_Skull.lock()->GetTransform().Forward().y;
		m_SpotLight2.lock()->direction.z = m_Skull.lock()->GetTransform().Forward().z + 0.03f;
	}
	else
	{
		m_SpotLight.lock()->position.x = m_Skull.lock()->GetTransform().position.x - 3;
		m_SpotLight.lock()->position.y = m_Skull.lock()->GetTransform().position.y + 1;
		m_SpotLight.lock()->position.z = m_Skull.lock()->GetTransform().position.z;
		m_SpotLight.lock()->direction.x = m_Skull.lock()->GetTransform().Forward().x - 0.03f;
		m_SpotLight.lock()->direction.y = m_Skull.lock()->GetTransform().Forward().y;
		m_SpotLight.lock()->direction.z = m_Skull.lock()->GetTransform().Forward().z;
		m_SpotLight2.lock()->position.x = m_Skull.lock()->GetTransform().position.x + 3;
		m_SpotLight2.lock()->position.y = m_Skull.lock()->GetTransform().position.y + 1;
		m_SpotLight2.lock()->position.z = m_Skull.lock()->GetTransform().position.z;
		m_SpotLight2.lock()->direction = m_Skull.lock()->GetTransform().Forward();
		m_SpotLight2.lock()->direction.x = m_Skull.lock()->GetTransform().Forward().x + 0.03f;
		m_SpotLight2.lock()->direction.y = m_Skull.lock()->GetTransform().Forward().y;
		m_SpotLight2.lock()->direction.z = m_Skull.lock()->GetTransform().Forward().z;
	}

	// How fast the spotlights will change in size
	static float radiusChangeRate = 0.05f;

	// Stop increasing if size is reached
	if (m_SpotLight.lock()->innerDegrees >= 3.5f)
	{
		// Decrease in size
		radiusChangeRate = -0.05f;
	}
	// Stop decreasing if size is reached
	else if (m_SpotLight.lock()->innerDegrees <= 0.5f)
	{
		// Increase in size
		radiusChangeRate = 0.05f;
	}

	// Apply the new size to the spotlights
	m_SpotLight.lock()->SetInnerCutOff(m_SpotLight.lock()->innerDegrees += radiusChangeRate);
	m_SpotLight.lock()->SetOuterCutOff(m_SpotLight.lock()->outerDegrees += radiusChangeRate);
	m_SpotLight2.lock()->SetInnerCutOff(m_SpotLight2.lock()->innerDegrees += radiusChangeRate);
	m_SpotLight2.lock()->SetOuterCutOff(m_SpotLight2.lock()->outerDegrees += radiusChangeRate);
	
	// FLASHLIGHT
	// Follow the camera's direction and position
	m_Flashlight.lock()->position = m_Camera->transform.position;
	m_Flashlight.lock()->direction = m_Camera->transform.Forward();
	*/

	// Activate the shader
	m_Shader->Activate();

	// Set the world transformations based on the camera
	m_Shader->SetWorldTransform(m_Camera);

	// Render custom graphics
	// Models will update their own positions in the mesh based on the transform
	for (int i = m_Models.size() - 1; i >= 0; --i)
	{
		// Checking if the reference still exists
		if (const auto& modelRef = m_Models[i].lock())
			// Render if there is a reference
			modelRef->Render(m_Shader, m_Lights);
		else
			// Erasing from the array if there is no reference
			m_Models.erase(m_Models.begin() + i);
	} 

	// Presented the frame to the window
	// Swapping the back buffer with the front buffer
	SDL_GL_SwapWindow(sdlWindow);
}

TWeak<PSPointLight> PGraphicsEngine::CreatePointLight()
{
	const auto& newLight = TMakeShared<PSPointLight>();
	m_Lights.push_back(newLight);

	return newLight;
}

TWeak<PSDirLight> PGraphicsEngine::CreateDirLight()
{
	const auto& newLight = TMakeShared<PSDirLight>();
	m_Lights.push_back(newLight);

	return newLight;
}

TWeak<PSSpotLight> PGraphicsEngine::CreateSpotLight()
{
	const auto& newLight = TMakeShared<PSSpotLight>();
	m_Lights.push_back(newLight);

	return newLight;
}

TShared<PModel> PGraphicsEngine::ImportModel(const PString& path)
{
	const auto& newModel = TMakeShared<PModel>();
	newModel->ImportModel(path, m_DefaultMaterial);
	m_Models.push_back(newModel);
	return newModel;
}

TShared<PSMaterial> PGraphicsEngine::CreateMaterial()
{
	return TMakeShared<PSMaterial>();
}


/*
void PGraphicsEngine::ToggleFlashlight()
{
	if (m_Flashlight.lock()->intensity > 0)
		m_Flashlight.lock()->intensity = 0.0f;
	else
		m_Flashlight.lock()->intensity = 1.0f;
}

void PGraphicsEngine::IncreaseFlashlightRed()
{
	if (m_Flashlight.lock()->colour.r >= 1.0f)
	{
		return;
	}

	m_Flashlight.lock()->colour = glm::vec3(
		m_Flashlight.lock()->colour.r + 0.1f,
		m_Flashlight.lock()->colour.g,
		m_Flashlight.lock()->colour.b);
}

void PGraphicsEngine::IncreaseFlashlightGreen()
{
	if (m_Flashlight.lock()->colour.r >= 1.0f)
	{
		return;
	}

	m_Flashlight.lock()->colour = glm::vec3(
		m_Flashlight.lock()->colour.g,
		m_Flashlight.lock()->colour.g + 0.1f,
		m_Flashlight.lock()->colour.b);
}

void PGraphicsEngine::IncreaseFlashlightBlue()
{
	if (m_Flashlight.lock()->colour.b >= 1.0f)
	{
		return;
	}

	m_Flashlight.lock()->colour = glm::vec3(
		m_Flashlight.lock()->colour.r,
		m_Flashlight.lock()->colour.g,
		m_Flashlight.lock()->colour.b + 0.1f);
}

void PGraphicsEngine::DecreaseFlashlightRed()
{
	if (m_Flashlight.lock()->colour.r <= 0.0f)
	{
		return;
	}

	m_Flashlight.lock()->colour = glm::vec3(
		m_Flashlight.lock()->colour.r - 0.1f,
		m_Flashlight.lock()->colour.g,
		m_Flashlight.lock()->colour.b);
}

void PGraphicsEngine::DecreaseFlashlightGreen()
{
	if (m_Flashlight.lock()->colour.g <= 0.0f)
	{
		return;
	}

	m_Flashlight.lock()->colour = glm::vec3(
		m_Flashlight.lock()->colour.r,
		m_Flashlight.lock()->colour.g - 0.1f,
		m_Flashlight.lock()->colour.b);
}

void PGraphicsEngine::DecreaseFlashlightBlue()
{
	if (m_Flashlight.lock()->colour.b <= 0.0f)
	{
		return;
	}

	m_Flashlight.lock()->colour = glm::vec3(
		m_Flashlight.lock()->colour.r,
		m_Flashlight.lock()->colour.g,
		m_Flashlight.lock()->colour.b - 0.1f);
}

void PGraphicsEngine::IncreaseFlashlightInnerRadius()
{
	m_Flashlight.lock()->SetInnerCutOff(m_Flashlight.lock()->innerDegrees + 1);
}

void PGraphicsEngine::IncreaseFlashlightOuterRadius()
{
	m_Flashlight.lock()->SetOuterCutOff(m_Flashlight.lock()->outerDegrees + 1);
}

void PGraphicsEngine::DecreaseFlashlightInnerRadius()
{
	m_Flashlight.lock()->SetInnerCutOff(m_Flashlight.lock()->innerDegrees - 1);
}

void PGraphicsEngine::DecreaseFlashlightOuterRadius()
{
	m_Flashlight.lock()->SetOuterCutOff(m_Flashlight.lock()->outerDegrees - 1);
}

void PGraphicsEngine::IncreaseFlashlightIntensity()
{
	m_Flashlight.lock()->intensity += 0.1f;
}

void PGraphicsEngine::DecreaseFlashlightIntensity()
{
	m_Flashlight.lock()->intensity -= 0.1f;
}
*/