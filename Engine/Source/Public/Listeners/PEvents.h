#pragma once
#include "EngineTypes.h"

// System libs
#include <functional>

template<typename... Args>

class PEvents
{
public:
	// Adding a function into our callbacks array
	PUi8 Bind(const std::function<void(Args...)>& callback)
	{
		// Make a unique structure of PCallback
		auto newNode = TMakeUnique<PCallbackNode>();
		// Move the function into the unique struct
		newNode->callback = std::move(callback);
		// This will give us the index for the callback as push_back adds the element to the end
		PUi8 id = 0;

		if (m_CallbackNodes.size() > 0)
		{
			
			// Find an id to assign to the callback node
			// Starting with 1 ID
			PUi8 potentialID = 1;
			// Looping through each number to test if an id exists
			while (id == 0)
			{
				// Default the found as true
				bool foundID = false;
				// If a node has that id set found to false and break out of the for loop
				for (const auto& node : m_CallbackNodes)
				{
					if (node->id == potentialID)
					{
						foundID = true;
						break;
					}
				}
				// If no ID matches the potentialIDN then break the while loop and set ID
				if (!foundID)
				{
					id = potentialID;
					break;
				}
				// If an ID was found that matched increase to the next potential
				++potentialID;
			}
		}
		else
		{
			id = 0;
		}

		newNode->id = id;

		// Add the unique callback into the array
		m_CallbackNodes.push_back(std::move(newNode));

		return id;
	}

	// Run all functions bound to this event listener
	void Run(const Args... args)
	{
		// Loop through all of the stored functions
		for (const auto& node : m_CallbackNodes)
		{
			// Run each function with the arguments
			node->callback(args...);
		}
	}

	// Unbind a function based on the index
	// Get the index from the initial bind
	void Unbind(const PUi8& index)
	{
		std::erase_if(m_CallbackNodes.begin(), m_CallbackNodes.end(),
			[index](const PCallbackNode& node) {
				return node->id == index;
			}
		);
	}
private:
	struct PCallbackNode
	{
		std::function<void(Args...)> callback;
		PUi8 id;
	};
	
	// Storing function to run when the event runs
	TArray<TUnique<PCallbackNode>> m_CallbackNodes;
};

typedef PEvents<> PEventsVoid;