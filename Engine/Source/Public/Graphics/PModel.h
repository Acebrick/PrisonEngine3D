#pragma once
#include "EngineTypes.h"
#include "Graphics/PMesh.h"
#include "Math/PSTransform.h"

// External Libs
#include <ASSIMP/matrix4x4.h>

class PTexture;
class PShaderProgram;
struct aiScene;
struct aiNode;
struct PSLight;

class PModel
{
public:
	PModel() = default;
	~PModel() = default;
	
	// Create a poly model and add a texture to it
	void MakePoly(const TShared<PTexture>& texture);

	// Create a cube model and add a texture to it
	void MakeCube(const TShared<PTexture>& texture);

	// Import a 3D model from file
	// Uses the ASSIMP import library, check docs to know file types accepted
	void ImportModel(const PString& filePath);

	// Render all of the meshes within the model
	// Transform of mesges will be based on the models transform
	void Render(const TShared<PShaderProgram>& shader, const TArray<TShared<PSLight>>& lights);

	// Get the transform of the model
	PSTransform& GetTransform() { return m_Transform; }
	
private:
	// Array of meshes
	TArray<TUnique<PMesh>> m_MeshStack;

	// Transform for the model in 3D space
	PSTransform m_Transform;

	// Find all of the meshes in a scene and convert them to a LMesh
	bool FindAndImportMeshes(const aiNode& node, const aiScene& scene, 
		const aiMatrix4x4& parentTransform, PUi32* meshesCreated);
};