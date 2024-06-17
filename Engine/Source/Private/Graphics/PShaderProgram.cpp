#include "Graphics/PShaderProgram.h"
#include "Debug/PDebug.h"
#include "Math/PSTransform.h"
#include "Graphics/PTexture.h"

// External Libs
#include <GLEW/glew.h>
#include <GLM/gtc/type_ptr.hpp>

// System Libs
#include <fstream>
#include <sstream>

#define LGET_GLEW_ERROR reinterpret_cast<const char*>(glewGetErrorString(glGetError()));

PShaderProgram::PShaderProgram()
{
	m_ProgramID = 0;
}

PShaderProgram::~PShaderProgram()
{
	PDebug::Log("Shader program " + std::to_string(m_ProgramID) + " destroyed");
}

bool PShaderProgram::InitShader(const PString& vShaderPath, const PString& fShaderPath)
{
	// Create the shader program in open gl
	m_ProgramID = glCreateProgram();

	// Test if the create program failed
	if (m_ProgramID == 0)
	{
		const std::string errorMessage = LGET_GLEW_ERROR;
		PDebug::Log("Shader failed to initialise, couldn't create program: " + errorMessage);
		return false;
	}

	// If either of the shaders fail to import then fail the whole program
	if (!ImportShaderByType(vShaderPath, ST_VERTEX) || !ImportShaderByType(fShaderPath, ST_FRAGMENT))
	{
		PDebug::Log("Shader program failed to initialise, couldn't import shaders");
		return false;
	}

	return LinkToGPU();
}

void PShaderProgram::Activate()
{
	glUseProgram(m_ProgramID);
}

void PShaderProgram::SetModelTransform(const PSTransform& transform)
{
	// Translate (move) > rotate > scale (this allows us to rotate around the new location)
	// Initialise a default matrix transform
	glm::mat4 matrixT = glm::mat4(1.0f);

	// Translate the matrix
	matrixT = glm::translate(matrixT, transform.position);

	// Rotate per axis
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrixT = glm::rotate(matrixT, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Scale the matrix
	matrixT = glm::scale(matrixT, transform.scale);

	// Find the variable in the shader
	// All uniform variables are given an ID by gl
	const int varID = glGetUniformLocation(m_ProgramID, "model");

	// Update the value
	glUniformMatrix4fv(varID, 1, GL_FALSE, value_ptr(matrixT));
}

void PShaderProgram::RunTexture(const TShared<PTexture>& texture, const PUi32& slot)
{
	// Bind the texture
	texture->BindTexture(slot);

	// The ID for the variable in the shader
	int varID = 0;

	// Get the ID depending on the slot
	switch (slot)
	{
	case 0:
		varID = glGetUniformLocation(m_ProgramID, "colourMap");
		break;
	default:
		break;
	}

	// Update the shader
	glUniform1i(varID, slot);
}

bool PShaderProgram::ImportShaderByType(const PString& filePath, PEShaderType shaderType)
{
	// Convert the shader to a string
	const PString shaderStr = ConvertFileToString(filePath);

	// Make sure there is a string path
	if (shaderStr.empty())
	{
		// Error that the string failed to import
		PDebug::Log("Shader failed to import", LT_ERROR);
		return false;
	}

	// Set and create an ID for the shader based on the shader type
	switch (shaderType)
	{
	case ST_VERTEX:
		m_ShaderIDs[shaderType] = glCreateShader(GL_VERTEX_SHADER);
		break;
	case ST_FRAGMENT:
		m_ShaderIDs[shaderType] = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	default:
		break;
	}

	// Make sure there is a string path
	if (m_ShaderIDs[shaderType] == 0)
	{
		const PString errorMessage = LGET_GLEW_ERROR;
		// Error that the string failed to import
		PDebug::Log("Shader program could not assign shader ID: " + errorMessage, LT_ERROR);
		return false;
	}

	// Compile the shader onto the GPU
	const char* shaderCStr = shaderStr.c_str();
	glShaderSource(m_ShaderIDs[shaderType], 1, &shaderCStr, nullptr);
	glCompileShader(m_ShaderIDs[shaderType]);

	// Test if the compile worked
	GLint success;
	glGetShaderiv(m_ShaderIDs[shaderType], GL_COMPILE_STATUS, &success);

	if (!success)
	{
		// Create an empty log
		char infoLog[512];
			
		// Fill the log with info from gl about what happened
		glGetShaderInfoLog(m_ShaderIDs[shaderType], 512, nullptr, infoLog);

		// Log it
		PDebug::Log("Shader compilation error: " + PString(infoLog), LT_ERROR);
		return false;
	}

	// Attach the shader to the program ID
	glAttachShader(m_ProgramID, m_ShaderIDs[shaderType]);

	return true;
}

PString PShaderProgram::ConvertFileToString(const PString& filePath)
{
	// Convert the file path into an ifstream
	std::ifstream shaderSource(filePath);

	// Test if we can open the file
	if (!shaderSource.is_open())
	{
		PDebug::Log("Failed to open file: " + filePath, LT_ERROR);
		return "";
	}

	// Initialise a string stream
	std::stringstream shaderStream;

	// Convert the fstream into an sstream
	// Basically turns the file into a string readable by our code
	shaderStream << shaderSource.rdbuf();

	// Close the file since we don't need it anymore
	shaderSource.close();

	return shaderStream.str();;
}

bool PShaderProgram::LinkToGPU()
{
	// Link the program to the GPU
	glLinkProgram(m_ProgramID);

	// Test if the compile worked
	GLint success;
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);

	if (!success)
	{
		// Create an empty log
		char infoLog[512];

		// Fill the log with info from gl about what happened
		glGetShaderInfoLog(m_ProgramID, 512, nullptr, infoLog);

		// Log it
		PDebug::Log("Shader link error: " + PString(infoLog), LT_ERROR);
		return false;
	}

	PDebug::Log("Shader successfully initialised and linked at index: " + std::to_string(m_ProgramID));

	return true;
}
