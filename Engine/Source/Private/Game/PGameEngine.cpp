#include "Game/PGameEngine.h"
#include "Game/GameObjects/PWorldObject.h"
#include "Game/GameObjects/MyObjects/Player.h"

// CUSTOM
#include "Game/GameObjects/MyObjects/Throne.h"
#include "Game/GameObjects/MyObjects/Dungeon.h"
#include "Game/GameObjects/MyObjects/Skull.h"
#include "Game/GameObjects/MyObjects/Bludgeon.h"
#include "Game/GameObjects/MyObjects/Torch.h"

PGameEngine* PGameEngine::GetGameEngine()
{
	static PGameEngine* instance = new PGameEngine();

	return instance;
}

void PGameEngine::DestroyEngine()
{
	delete GetGameEngine();
}

bool PGameEngine::Run()
{
	if (!Initialise())
	{
		PDebug::Log("Game Engine failed to initialise", LT_ERROR);
		return false;
	}

	Start();

	GameLoop();

	return true; 
}

void PGameEngine::DestroyObject(const TShared<PObject>& object)
{
	m_ObjectsPendingDestroy.push_back(object);
}

TUnique<PGraphicsEngine>& PGameEngine::GetGraphics()
{
	return m_Window->GetGraphics();
}

PGameEngine::PGameEngine()
{
	m_LastTickTime = 0.0;
	m_DeltaTime = 0.0;
	PDebug::Log("Game Engine created");
}

PGameEngine::~PGameEngine()
{
	Cleanup();

	PDebug::Log("Game Engine destroyed");
}

bool PGameEngine::Initialise()
{
	// Initialise the components of SDL that we need
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0)
	{
		PDebug::Log("Failed to init SDL: " + PString(SDL_GetError()), LT_ERROR);
		return false;
	}

	// Tell SDL that we'll be rendering in open gl version 460 or 4.60
	// 4 is the major version
	// .60 is the minor version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	// Make sure if open gl is using a version not in SDL we run SDL in compatibility mode
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	// Set the bit depth for each colour
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);

	m_Window = TMakeShared<PWindow>();


	// Creating an SDL window
	if (!m_Window->CreateWindow({ "Game Window",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		720, 720 }))
		return false;

	//  Create the input class and assign the window
	m_Input = TMakeShared<PInput>();
	m_Input->InitInput(m_Window);

	return true;
}

void PGameEngine::Start()
{
	// Register the window inputs
	m_Window->RegisterInput(m_Input);

	// Create world objects
	if (const auto& throne = CreateObject<Throne>().lock())
	{
		throne->GetTransform().position.z = 200.0f;
		throne->GetTransform().rotation.y = 180.0f;
	}
	CreateObject<Player>();
	CreateObject<Dungeon>();

	if (const auto& skull = CreateObject<Skull>().lock())
	{
		skull->GetTransform().position.x = 800.0f;
		skull->GetTransform().position.y = 300.0f;
		skull->GetTransform().position.z = 800.0f;
		skull->GetTransform().scale = glm::vec3(50.0f);
	}

	m_Bludgeon = CreateObject<Bludgeon>();	
	
	if (const auto& torchRef = CreateObject<Torch>().lock())
	{
		torchRef->GetTransform().position.x = -410.0f;
		torchRef->GetTransform().position.y = 200.0f;
		torchRef->GetTransform().position.z = -330.0f;
	}

	if (const auto& torchRef = CreateObject<Torch>().lock())
	{
		torchRef->GetTransform().position.x = 410.0f;
		torchRef->GetTransform().position.y = 200.0f;
		torchRef->GetTransform().position.z = -330.0f;
	}
}

void PGameEngine::GameLoop()
{
	// Keep the game open as long as the window is open
	while (!m_Window->IsPendingClose())
	{
		// Create delta time
		// Set the current tick time
		// SDL_GetTicks64() gives us time since the engine started in milliseconds
		double curTickTime = static_cast<double>(SDL_GetTicks64());

		// Convert the tick time into delta time in milliseconds
		// How much time has passed since the last frame
		double deltaMilli = curTickTime - m_LastTickTime;

		// Convert deltaMilli to seconds
		m_DeltaTime = deltaMilli / 1000.0;

		// Update the last tick time to the current time for the next loop
		m_LastTickTime = curTickTime;

		// Order of these functions is important
		// We want to detect input > react to input with logic > render based on logic
		PreLoop();

		// Process all engine input functions
		ProcessInput();

		// Process all engine tick functions
		Tick();

		// Process all engine render functions
		Render();

		PostLoop();
	}
}

void PGameEngine::Cleanup()
{
	m_Input = nullptr;
	m_Window = nullptr;

	SDL_Quit();
}

void PGameEngine::Tick()
{
	// Run through all PObjects in the game and run their ticks
	for (const auto& pObjectRef : m_ObjectStack)
	{
		pObjectRef->Tick(DeltaTimeF());

		// Check the object is a world object, otherwise skip logic
		if (const auto& woRef = std::dynamic_pointer_cast<PWorldObject>(pObjectRef))
		{
			// Check the object has collisions
			if (woRef->HasCollisions())
			{
				// Loop through all objects to test against
				for (const auto& otherObj : m_ObjectStack)
				{
					// Test if the other object is also a world object
					if (const auto& otherWoRef = std::dynamic_pointer_cast<PWorldObject>(otherObj))
					{
						if (!otherWoRef->HasCollisions())
						{
							continue;
						}

						// If all is good, test if the collisions are overlapping
						woRef->TestCollision(otherWoRef);
					}
				}
			}
		}

		pObjectRef->PostTick(DeltaTimeF());
	}
}

void PGameEngine::ProcessInput()
{
	if (!m_Input)
		return;

	// Handle inputs
	m_Input->UpdateInputs();
}

void PGameEngine::Render()
{
	if (!m_Window)
		return;

	// Render the window
	m_Window->Render();
}

void PGameEngine::PreLoop()
{
	// Running through al objects to be spawned and running their start logic
	// and adding them into the game object stack
	for (auto& pObjectRef : m_ObjectsToBeInstantiated)
	{
		pObjectRef->Start();
		m_ObjectStack.push_back(std::move(pObjectRef));
	}

	m_ObjectsToBeInstantiated.clear();
}

void PGameEngine::PostLoop()
{
	// Loop throug all objects pending destroy and remove their references from object stack
	for (const auto& pObjectRef : m_ObjectsPendingDestroy)
	{
		auto it = std::find(m_ObjectStack.begin(), m_ObjectStack.end(), pObjectRef);

		if (it == m_ObjectStack.end())
			continue;

		m_ObjectStack.erase(it);
	}

	// Make sure to clear the pending destroy array so no references of the object exist
	m_ObjectsPendingDestroy.clear();
}
