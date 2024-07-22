#pragma once
#include "EngineTypes.h"
#include "PWindow.h"
#include "Listeners/PInput.h"

// External Libs
#include <SDL/SDL.h>

class PObject;

class PGameEngine 
{
public:
	// Get or create a game engine if one doesn't exist
	static PGameEngine* GetGameEngine();

	// Destroy the game engine
	static void DestroyEngine();

	// Run the game engine
	bool Run();

	// Return the delta time between frames
	double DeltaTime() const { return m_DeltaTime; }

	// Return the delta time between frames as a float
	float DeltaTimeF() const { return static_cast<float>(m_DeltaTime); }

	// Create a PObject type
	template<typename T, std::enable_if_t<std::is_base_of_v<PObject, T>, T>* = nullptr>
	TWeak<T> CreateObject()
	{
		// Create an object with the template class
		TShared<T> newObject = TMakeShared<T>();

		// Add the object into the stack
		m_ObjectsToBeInstantiated.push_back(newObject);

		return newObject;
	}

	// Mark an object for destroy
	// All game objects destroy functions will automatically run this
	void DestroyObject(const TShared<PObject>& object);

private:
	// Constructor and destructor are private to ensure we can only have 1 game engine
	PGameEngine();
	~PGameEngine();

	// Initialise all required libraries for the game
	bool Initialise();

	// Runs after initialise of the engine
	void Start();

	// Run the loop of the game
	void GameLoop();

	// Cleanup the game engine
	void Cleanup();

	// LOOP FUNCTIONS
	// Run game logic for the frame
	void Tick();

	// Process the input for each frame
	void ProcessInput();

	// Render the graphics for each frame
	void Render();

	// Runs at the start of each loop
	void PreLoop();

	// Runs at the end of each loop
	void PostLoop();

	// Store the window for the game engine
	TShared<PWindow> m_Window;
	
	// Store the input of the game engine
	TShared<PInput> m_Input; 

	// Last time ticked
	double m_LastTickTime;

	// The delta time between frames
	double m_DeltaTime;

	// Store all PObjects in the game
	TArray<TShared<PObject>> m_ObjectStack;

	// Store all PObjects to be started next frame
	TArray<TShared<PObject>> m_ObjectsToBeInstantiated;

	// Store all objects that have been marked for destroy
	TArray<TShared<PObject>> m_ObjectsPendingDestroy;
};
