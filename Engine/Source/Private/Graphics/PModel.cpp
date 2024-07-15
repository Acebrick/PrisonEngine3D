#include "Graphics/PModel.h"

// External Libs
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>
#include <ASSIMP/mesh.h>

const std::vector<PSVertexData> polyVData = {
	//   x      y	   z      r     g     b       tx    ty
	{ {-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 1 (top left)
	{ { 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} }, // vertex data 2 (top right)
	{ {-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 3 (bot left) 
	{ { 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} } // vertex data 4 (bot right)
};

const std::vector<uint32_t> polyIData = {
	 0, 1, 2, // Triangle 1
	 1, 2, 3  // Triangle 2
};

const std::vector<PSVertexData> cubeVData = {
	//   x      y	   z       r     g     b       tx    ty
	// Front vertices
	{ {-1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 0 (top left)
	{ { 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} }, // vertex data 1 (top right)
	{ {-1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 2 (bot left) 
	{ { 1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} }, // vertex data 3 (bot right)
	// Back vertices
	{ { 1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 4 (top left)
	{ {-1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} }, // vertex data 5 (top right)
	{ { 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 6 (bot left) 
	{ {-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} }, // vertex data 7 (bot right)

	// Left vertices
	{ {-1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 8 (top left)
	{ {-1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 9 (top right)
	{ {-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 10 (bot left) 
	{ {-1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} },  // vertex data 11 (bot right)

	// Right vertices
	{ { 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 12 (top left)
	{ { 1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 13 (top right)
	{ { 1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 14 (bot left) 
	{ { 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} },   // vertex data 15 (bot right)

	// Top vertices
	{ {-1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 16 (top left)
	{ { 1.0f,  1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 17 (top right)
	{ {-1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 18 (bot left) 
	{ { 1.0f,  1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} },   // vertex data 19 (bot right)

	// Bottom vertices
	{ { 1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 20 (top left)
	{ {-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 21 (top right)
	{ { 1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 22 (bot left) 
	{ {-1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} }   // vertex data 23 (bot right)
};

const std::vector<uint32_t> cubeIData = {
	// FRONT
	0, 1, 2, 
	1, 2, 3, 
	// BACK
	4, 5, 6,
	5, 6, 7,
	// LEFT
	8, 9, 10,
	9, 10, 11,
	// RIGHT
	12, 13, 14,
	13, 14, 15,
	// TOP
	16, 17, 18,
	17, 18, 19,
	// BOTTOM
	20, 21, 22,
	21, 22, 23
};

void PModel::MakePoly(const TShared<PTexture>& texture)
{
	// Create the debug mesh
	TUnique<PMesh> mesh = TMakeUnique<PMesh>();

	if (!mesh->CreateMesh(polyVData, polyIData))
	{
		PDebug::Log("Failed to create debug mesh");
	}

	// Add the texture and add it to the mesh stack
	mesh->SetTexture(texture);
	// std::move will move the reference from the previous reference to a new one without destroying or copying the reference
	m_MeshStack.push_back(std::move(mesh));
}

void PModel::MakeCube(const TShared<PTexture>& texture)
{
	// Create the debug mesh
	TUnique<PMesh> mesh = TMakeUnique<PMesh>();

	if (!mesh->CreateMesh(cubeVData, cubeIData))
	{
		PDebug::Log("Failed to create debug mesh");
	}

	// Add the texture and add it to the mesh stack
	mesh->SetTexture(texture);
	// std::move will move the reference from the previous reference to a new one without destroying or copying the reference
	m_MeshStack.push_back(std::move(mesh));
}

void PModel::ImportModel(const PString& filePath)
{
	// Create an assimp importer
	Assimp::Importer importer;

	// Read the file and convert the model to an assimp scene
	// Add post processing flag triangulate to make sure the model is triangles
	const auto scene = importer.ReadFile(filePath, aiProcess_Triangulate);

	// Check if the import failed in any way
	// !scene is checking if the object is null
	// FLAGS_INCOMPLETE is checking if the import failed
	// mRootNode is checking if the model has a mesh at all
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		PDebug::Log("Error importing model from " + filePath + " : " + importer.GetErrorString(), LT_ERROR);
		return;
	}

	aiMatrix4x4 sceneTransform;

	// Update the scene matrix to start the location at x0, y0, z0
	aiMatrix4x4::Translation({ 0.0f, 0.0f, 0.0f }, sceneTransform);

	// Set the rotation to x0, y0, z0
	sceneTransform.FromEulerAnglesXYZ({ 0.0f, 0.0f, 0.0f });

	// Set the scale to x1, y1, z1
	aiMatrix4x4::Scaling({ 1.0f, 1.0f, 1.0f }, sceneTransform);

	// Find all meshes in the scene and fail if any of them fail
	if (!FindAndImportMeshes(*scene->mRootNode, *scene, sceneTransform))
	{
		PDebug::Log("Model failed to convert ASSIMP scene: " + filePath, LT_ERROR);
		return;
	}

	// Log the successful import of the model
	PDebug::Log("Model successfully imported: " + filePath, LT_SUCCESS);
}

void PModel::Render(const TShared<PShaderProgram>& shader)
{
	for (const auto& mesh : m_MeshStack)
	{
		mesh->Render(shader, m_Transform);
	}
}

bool PModel::FindAndImportMeshes(const aiNode& node, const aiScene& scene, const aiMatrix4x4& parentTransform)
{
	// Looping through all the meshes in the node
	for (PUi32 i = 0; i < node.mNumMeshes; ++i)
	{
		// TO DO: Convert aiMesh to PMesh
		
		// Get the mesh index for the mesh in the scene
		const auto aMesh = scene.mMeshes[node.mMeshes[i]];

		// Store mesh vertices and indices
		TArray<PSVertexData> meshVertices;
		TArray<PUi32> meshIndices;

		// Loop through every vertex and get the data for conversion
		for (PUi64 j = 0; j < aMesh->mNumVertices; ++j)
		{
			// Create an empty vertex
			PSVertexData vertex;

			// Get the positions of the vertex
			vertex.m_Position[0] = aMesh->mVertices[j].x;
			vertex.m_Position[1] = aMesh->mVertices[j].y;
			vertex.m_Position[2] = aMesh->mVertices[j].z;

			// If there are vertex colours then update
			if (aMesh->HasVertexColors(j))
			{
				vertex.m_Colour[0] = aMesh->mColors[j]->r;
				vertex.m_Colour[1] = aMesh->mColors[j]->g;
				vertex.m_Colour[2] = aMesh->mColors[j]->b;
			}

			// Set the texture coordinates
			// Texture coordinates can have multiple sets
			// The first array index is the set number [0]
			// The second array index is the vertex data
			if (aMesh->HasTextureCoords(j))
			{
				vertex.m_TexCoords[0] = aMesh->mTextureCoords[0][j].x;
				vertex.m_TexCoords[1] = aMesh->mTextureCoords[0][j].y;
			}
			
			// Get the normals for the model
			vertex.m_Normal[0] = aMesh->mNormals[j].x;
			vertex.m_Normal[1] = aMesh->mNormals[j].y;
			vertex.m_Normal[2] = aMesh->mNormals[j].z;

			// Add the data into our vertex array
			meshVertices.push_back(vertex);
		}

		// The gpu requires a minimum of 3 vertices to render (triangle)
		// Fail if there are less than 3
		if (meshVertices.size() < 3)
		{
			PDebug::Log("Mesh has less than 3 vertices", LT_ERROR);
			return false;
		}

		// Loop through all of the faces on the mesh to get the indices
		for (PUi64 j = 0; j < aMesh->mNumFaces; ++j)
		{
			// Store the face as a variable
			auto face = aMesh->mFaces[j];

			// Looping through all the indices in the face, should only be 3
			for (PUi32 k = 0; k < face.mNumIndices; ++k)
			{
				meshIndices.push_back(face.mIndices[k]);
			}
		}

		// Create the mesh object
		auto pMesh = TMakeUnique<PMesh>();

		// Test if the mesh fails to create
		if (!pMesh->CreateMesh(meshVertices, meshIndices))
		{
			PDebug::Log("Mesh failed to convert from A Mesh to P Mesh", LT_ERROR);
			return false;
		}

		// Set the relative transformation for the mesh
		aiMatrix4x4 relTransform = parentTransform * node.mTransformation;

		// Set a default matrix transform for glm
		glm::mat4 matTransform(1.0f);

		// Conert the relative ASSIMP transform into a glm transform
		matTransform[0][0] = relTransform.a1; matTransform[1][0] = relTransform.a2;
		matTransform[2][0] = relTransform.a3; matTransform[3][0] = relTransform.a4;

		matTransform[0][1] = relTransform.b1; matTransform[1][1] = relTransform.b2;
		matTransform[2][1] = relTransform.b3; matTransform[3][1] = relTransform.b4;

		matTransform[0][2] = relTransform.c1; matTransform[1][2] = relTransform.c2;
		matTransform[2][2] = relTransform.c3; matTransform[3][2] = relTransform.c4;

		matTransform[0][3] = relTransform.d1; matTransform[1][3] = relTransform.d2;
		matTransform[2][3] = relTransform.d3; matTransform[3][3] = relTransform.d4;

		// Update the relative transform on the mesh
		pMesh->SetRelativeTransform(matTransform);

		// Add the new mesh to the mesh stack
		m_MeshStack.push_back(std::move(pMesh));
	}

	// Adding the relative transform to the parent transform
	const aiMatrix4x4 nodeRelTransform = parentTransform * node.mTransformation;

	// Loop through all of the child nodes inside this node
	for (PUi32 i = 0; i < node.mNumChildren; ++i)
	{
		if (!FindAndImportMeshes(*node.mChildren[i], scene, nodeRelTransform))
		{
			return false;
		}
	}

	return true;
}