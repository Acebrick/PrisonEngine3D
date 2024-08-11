#pragma once
#include "EngineTypes.h"

// External Libs
#include <GLM/mat4x4.hpp>

class PShaderProgram;
struct PSTransform;
struct PSLight;
struct PSMaterial;

struct PSVertexData
{
	// 0 = x
	// 1 = y
	// 2 = z
	float m_Position[3] = { 0.0f, 0.0f, 0.0f };

	// 0 = r
	// 1 = g
	// 2 = b
	float m_Colour[3] = { 1.0f, 1.0f, 1.0f };

	// 0 = x, u, s
	// 1 = y, v, t
	float m_TexCoords[2] = { 0.0f, 0.0f };

	// 0 = x
	// 1 = y
	// 2 = z
	float m_Normal[3] = { 0.0f, 0.0f, 0.0f };

	// 0 = x
	// 1 = y
	// 2 = z
	float m_Tangent[3] = { 0.0f, 0.0f, 0.0f };
};

class PMesh
{
public:
	PMesh();
	~PMesh();

	// Creating a mesh using vertex ad index data
	bool CreateMesh(const std::vector<PSVertexData>& vertices, const std::vector<uint32_t>& indices);

	void Render(const std::shared_ptr<PShaderProgram>& shader, const PSTransform& transform, 
		const TArray<TShared<PSLight>>& lights, const TShared<PSMaterial>& material);

	// Set the transform of the mesh relative to the model
	void SetRelativeTransform(const glm::mat4& transform) { m_MatTransform = transform; }

	// The index for the material relative to the model
	unsigned int materialIndex;

private:
	// Store the vertices
	std::vector<PSVertexData> m_Vertices;

	// Store the indices for the data
	std::vector<uint32_t> m_Indices;

	// Store the ID for the vertex array object
	uint32_t m_VAO;

	// Store the ID for the vertex buffer object
	uint32_t m_VBO;

	// Store the ID for the element array object
	uint32_t m_EAO;

	// Relative transform of the mesh
	glm::mat4 m_MatTransform;
};