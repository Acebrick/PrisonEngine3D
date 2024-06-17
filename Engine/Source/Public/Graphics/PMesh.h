#pragma once
#include "EngineTypes.h"

class PShaderProgram;
struct PSTransform;
class PTexture;

struct PSVertexData
{
	float m_Position[3] = { 0.0f, 0.0f, 0.0f };
	float m_Colour[3] = { 1.0f, 1.0f, 1.0f };
	float m_TexCoords[2] = { 0.0f, 0.0f };
	float m_Normal[3] = { 0.0f, 0.0f, 0.0f };
};

class PMesh
{
public:
	PMesh();
	~PMesh();

	// Creating a mesh using vertex ad index data
	bool CreateMesh(const std::vector<PSVertexData>& vertices, const std::vector<uint32_t>& indices);

	void Render(const std::shared_ptr<PShaderProgram>& shader, const PSTransform& transform);

	// Set the texture in the mesh
	void SetTexture(const TShared<PTexture>& texture) { m_Texture = texture; }

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

	// Texture for the mesh
	TShared<PTexture> m_Texture;
};