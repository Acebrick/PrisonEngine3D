#include "Game/PGameEngine.h"

// Smart pointers delete themselves when there is no reference
// Shared pointer = Shares ownership across all references
// Unique pointer = Does not share ownership with anything
// Weak pointer = this has no ownership over any references

int main(int argc, char* argv[])
{
	int result = 0;
	// Initialise the engine
	// Test if int fails
	if (!PGameEngine::GetGameEngine()->Run())
	{
		result = -1;
	}

	// Cleanup the engine
	PGameEngine::DestroyEngine();

	return result;
}