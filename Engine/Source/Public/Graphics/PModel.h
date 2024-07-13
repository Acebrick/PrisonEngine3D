#pragma once
#include "EngineTypes.h"
#include "Graphics/PMesh.h"
#include "Math/PSTransform.h"

class PTexture;
class PShaderProgram;

class PModel
{
public:
	PModel() = default;
	~PModel() = default;
	
	// Create a poly model and add a texture to it
	void MakePoly(const TShared<PTexture>& texture);

	// Create a cube model and add a texture to it
	void MakeCube(const TShared<PTexture>& texture);

	// Render all of the meshes within the model
	// Transform of mesges will be based on the models transform
	void Render(const TShared<PShaderProgram>& shader);

	void MakeLeftTreads(const TShared<PTexture>& texture);

	void MakeRightTreads(const TShared<PTexture>& texture);

	// Get the transform of the model
	PSTransform& GetTransform() { return m_Transform; }
	
private:
	// Array of meshes
	TArray<TUnique<PMesh>> m_MeshStack;

	// Transform for the model in 3D space
	PSTransform m_Transform;
};