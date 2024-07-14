#include "Graphics/PModel.h"

const std::vector<PSVertexData> gunVData =
{
	// Front vertices
	{ {-0.5f,  0.5f,  5.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 0 (top left)
	{ { 0.5f,  0.5f,  5.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} }, // vertex data 1 (top right)
	{ {-0.5f, -0.5f,  5.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 2 (bot left) 
	{ { 0.5f, -0.5f,  5.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} }, // vertex data 3 (bot right)

	// Back vertices
	{ { 0.5f,  0.5f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 4 (top left)
	{ {-0.5f,  0.5f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} }, // vertex data 5 (top right)
	{ { 0.5f, -0.5f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 6 (bot left) 
	{ {-0.5f, -0.5f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} }, // vertex data 7 (bot right)

	// Left vertices
	{ {-0.5f,  0.5f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 8 (top left)
	{ {-0.5f,  0.5f,  5.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 9 (top right)
	{ {-0.5f, -0.5f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 10 (bot left) 
	{ {-0.5f, -0.5f,  5.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} },  // vertex data 11 (bot right)

	// Right vertices
	{ { 0.5f,  0.5f,  5.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 12 (top left)
	{ { 0.5f,  0.5f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 13 (top right)
	{ { 0.5f, -0.5f,  5.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 14 (bot left) 
	{ { 0.5f, -0.5f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} },   // vertex data 15 (bot right)

	// Top vertices
	{ {-0.5f,  0.5f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 16 (top left)
	{ { 0.5f,  0.5f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 17 (top right)
	{ {-0.5f,  0.5f,  5.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 18 (bot left) 
	{ { 0.5f,  0.5f,  5.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} },   // vertex data 19 (bot right)

	// Bottom vertices
	{ { 0.5f, -0.5f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 20 (top left)
	{ {-0.5f, -0.5f,  1.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 21 (top right)
	{ { 0.5f, -0.5f,  5.0f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 22 (bot left) 
	{ {-0.5f, -0.5f,  5.0f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} }   // vertex data 23 (bot right)
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

const std::vector<PSVertexData> leftTreadsVData =
{
	// Front vertices
	{ {-1.4f, -1.0f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 0 (top left)
	{ {-0.6f, -1.0f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} }, // vertex data 1 (top right)
	{ {-1.4f, -1.4f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 2 (bot left) 
	{ {-0.6f, -1.4f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} }, // vertex data 3 (bot right)

	// Back vertices
	{ {-0.6f, -1.0f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 4 (top left)
	{ {-1.4f, -1.0f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} }, // vertex data 5 (top right)
	{ {-0.6f, -1.4f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 6 (bot left) 
	{ {-1.4f, -1.4f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} }, // vertex data 7 (bot right)

	// Left vertices
	{ {-1.4f, -1.0f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 8 (top left)
	{ {-1.4f, -1.0f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 9 (top right)
	{ {-1.4f, -1.4f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 10 (bot left) 
	{ {-1.4f, -1.4f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} },  // vertex data 11 (bot right)

	// Right vertices
	{ {-0.6f, -1.0f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 12 (top left)
	{ {-0.6f, -1.0f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 13 (top right)
	{ {-0.6f, -1.4f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 14 (bot left) 
	{ {-0.6f, -1.4f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} },   // vertex data 15 (bot right)

	// Top vertices
	{ {-1.4f, -1.0f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 16 (top left)
	{ {-0.6f, -1.0f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 17 (top right)
	{ {-1.4f, -1.0f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 18 (bot left) 
	{ {-0.6f, -1.0f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} },   // vertex data 19 (bot right)

	// Bottom vertices
	{ {-0.6f, -1.4f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 20 (top left)
	{ {-1.4f, -1.4f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 21 (top right)
	{ {-0.6f, -1.4f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 22 (bot left) 
	{ {-1.4f, -1.4f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} }   // vertex data 23 (bot right)
};


const std::vector<PSVertexData> rightTreadsVData =
{
	// Front vertices
	{ { 1.4f, -1.0f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 0 (top left)
	{ { 0.6f, -1.0f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} }, // vertex data 1 (top right)
	{ { 1.4f, -1.4f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 2 (bot left) 
	{ { 0.6f, -1.4f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} }, // vertex data 3 (bot right)

	// Back vertices
	{ { 0.6f, -1.0f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 4 (top left)
	{ { 1.4f, -1.0f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} }, // vertex data 5 (top right)
	{ { 0.6f, -1.4f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 6 (bot left) 
	{ { 1.4f, -1.4f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} }, // vertex data 7 (bot right)

	// Left vertices
	{ { 1.4f, -1.0f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 8 (top left)
	{ { 1.4f, -1.0f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 9 (top right)
	{ { 1.4f, -1.4f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 10 (bot left) 
	{ { 1.4f, -1.4f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} },  // vertex data 11 (bot right)

	// Right vertices
	{ { 0.6f, -1.0f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 12 (top left)
	{ { 0.6f, -1.0f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 13 (top right)
	{ { 0.6f, -1.4f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 14 (bot left) 
	{ { 0.6f, -1.4f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} },   // vertex data 15 (bot right)

	// Top vertices
	{ { 1.4f, -1.0f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 16 (top left)
	{ { 0.6f, -1.0f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 17 (top right)
	{ { 1.4f, -1.0f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 18 (bot left) 
	{ { 0.6f, -1.0f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} },   // vertex data 19 (bot right)

	// Bottom vertices
	{ { 0.6f, -1.4f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f} }, // vertex data 20 (top left)
	{ { 1.4f, -1.4f, -1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f} },  // vertex data 21 (top right)
	{ { 0.6f, -1.4f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 0.0f, 0.0f} }, // vertex data 22 (bot left) 
	{ { 1.4f, -1.4f,  1.6f}, {1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f} }   // vertex data 23 (bot right)
};

void PModel::MakePoly(const TShared<PTexture>& texture)
{
	// Create the debug mesh
	TUnique<PMesh> mesh = TMakeUnique<PMesh>();

	if (!mesh->CreateMesh(gunVData, cubeIData))
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

void PModel::Render(const TShared<PShaderProgram>& shader)
{
	for (const auto& mesh : m_MeshStack)
	{
		mesh->Render(shader, m_Transform);
	}
}

void PModel::MakeLeftTreads(const TShared<PTexture>& texture)
{
	// Create the debug mesh
	TUnique<PMesh> mesh = TMakeUnique<PMesh>();

	if (!mesh->CreateMesh(leftTreadsVData, cubeIData))
	{
		PDebug::Log("Failed to create debug mesh");
	}

	// Add the texture and add it to the mesh stack
	mesh->SetTexture(texture);
	// std::move will move the reference from the previous reference to a new one without destroying or copying the reference
	m_MeshStack.push_back(std::move(mesh));
}

void PModel::MakeRightTreads(const TShared<PTexture>& texture)
{
	// Create the debug mesh
	TUnique<PMesh> mesh = TMakeUnique<PMesh>();

	if (!mesh->CreateMesh(rightTreadsVData, cubeIData))
	{
		PDebug::Log("Failed to create debug mesh");
	}

	// Add the texture and add it to the mesh stack
	mesh->SetTexture(texture);
	// std::move will move the reference from the previous reference to a new one without destroying or copying the reference
	m_MeshStack.push_back(std::move(mesh));
}

// Translate the model based on the translation passed in
void PModel::Translate(glm::vec3 translation, glm::vec3 scale)
{
	// Move the input direction forward if required
	glm::vec3 moveDir = GetTransform().Forward() * translation.z;
	moveDir += GetTransform().Right() * translation.x;
	moveDir.y += translation.y;

	if (glm::length(moveDir) != 0.0f)
		moveDir = glm::normalize(moveDir);

	GetTransform().position += moveDir * scale * 0.1f;
}
