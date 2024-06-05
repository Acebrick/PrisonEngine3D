#include "Graphics/PMesh.h"
#include "Debug/PDebug.h"

// External Libs
#include <GLEW/glew.h>

PMesh::PMesh()
{
	m_VAO = m_VBO = m_EAO = 0;
	PDebug::Log("Mesh created");
}

PMesh::~PMesh()
{
	PDebug::Log("Mesh destroyed");
}

bool PMesh::CreateMesh(const std::vector<PSVertexData>& vertices, const std::vector<uint32_t>& indices)
{
	// Store the vertex data
	m_Vertices = vertices;
	m_Indices = indices;

	// Create a vertex array object (VAO)
	// Assign the id for the object to the m_VAO variable
	// Stores a reference to any VBO's attached to the VAO
	glGenVertexArrays(1, &m_VAO);

	if (m_VAO == 0)
	{
		// Convert the error into a readable string
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		PDebug::Log("Mesh failed to create VAO: " + errorMsg, LT_WARN);
		return false;
	}

	// This says use the VAO as the active working VAO for any VAO functions
	glBindVertexArray(m_VAO);

	// Create a buffer object
	// The vertex buffer object holds the data for the vertices in the gpu
	glGenBuffers(1, &m_VBO);

	// Test if the VBO failed
	if (m_VBO == 0)
	{
		// Convert the error into a readable string
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		PDebug::Log("Mesh failed to create VBO: " + errorMsg, LT_WARN);
		return false;
	}

	// Bind the buffer object to say this is the active working VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// Create an element array buffer
	glGenBuffers(1, &m_EAO);

	// Test if the EAO failed
	if (m_EAO == 0)
	{
		// Convert the error into a readable string
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		PDebug::Log("Mesh failed to create EAO: " + errorMsg, LT_WARN);
		return false;
	}

	// Bind the EAO as the active elemnt array buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EAO);

	// Set the buffer data
	// Start with the VBO which stores the vertex data
	glBufferData(
		GL_ARRAY_BUFFER, //  Type of data that we're storing
		static_cast<GLsizeiptr>(m_Vertices.size() * sizeof(PSVertexData)), // Size of each data in bytes
		m_Vertices.data(), // Memory location of the data
		GL_STATIC_DRAW // This data will not be modified frequently
	);

	// Set the data for the EAO
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(m_Indices.size()) * sizeof(uint32_t),
		m_Indices.data(),
		GL_STATIC_DRAW
	);

	// Pass out the verterx data in separate formats
	glEnableVertexAttribArray(0);

	// Set the position of that data to the 0 index of the attribute array
	glVertexAttribPointer(
		0, // Location to store the data in the attribute array
		3, // How many numbers to pass into the attribute array index
		GL_FLOAT, // The type of data to store (only one per index)
		GL_FALSE, // Should we normalise the values, generally no
		sizeof(PSVertexData), // How big is each data array in a VertexData
		nullptr // How many numbers to skip
	);

	// Common practice to clear the VAO from the GPU
	glBindVertexArray(0); // Set to 0 because there is no such thing as a 0 id

	return true;
}

void PMesh::Render()
{
	// Binding this mesh as the active VAO
	glBindVertexArray(m_VAO);

	// Render the VAO
	glDrawElements(
		GL_TRIANGLES, // Draw the mesh as triangles
		static_cast<GLsizei>(m_Indices.size()), // How many vertices are there
		GL_UNSIGNED_INT, // What type of data is the index array
		nullptr // How many are you gonna skip
	);
	
	// Clear the VAO
	glBindVertexArray(0);
}
