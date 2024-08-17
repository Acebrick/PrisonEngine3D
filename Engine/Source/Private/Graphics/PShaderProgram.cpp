#include "Graphics/PShaderProgram.h"
#include "Debug/PDebug.h"
#include "Math/PSTransform.h"
#include "Graphics/PTexture.h"
#include "Graphics/PSCamera.h"
#include "Graphics/PSLight.h"
#include "Graphics/PSMaterial.h"

// External Libs
#include <GLEW/glew.h>
#include <GLM/gtc/type_ptr.hpp>

// System Libs
#include <fstream>
#include <sstream>

#define LGET_GLEW_ERROR reinterpret_cast<const char*>(glewGetErrorString(glGetError()));

// Constant value for light amounts
const PUi32 maxDirLights = 2;
const PUi32 maxPointLights = 100;
const PUi32 maxSpotLights = 20;

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

void PShaderProgram::SetMeshTransform(const glm::mat4& matTransform)
{
	// Find the variable in the shader
	// All uniform variables are given an ID by gl
	const int varID = glGetUniformLocation(m_ProgramID, "mesh");

	// Update the value
	glUniformMatrix4fv(varID, 1, GL_FALSE, value_ptr(matTransform));
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

void PShaderProgram::SetWorldTransform(const TShared<PSCamera>& camera)
{
	// Initialise a matrix
	glm::mat4 matrixT = glm::mat4(1.0f);

	// HANDLE THE VIEW MATRIX
	// Translate  and rotate the matrix based on the camera position
	matrixT = glm::lookAt(
		camera->transform.position,
		camera->transform.position + camera->transform.Forward(),
		camera->transform.Up()
	);

	// Find the variable in the shader and update it
	int varID = glGetUniformLocation(m_ProgramID, "view");

	// Update the value
	glUniformMatrix4fv(varID, 1, GL_FALSE, glm::value_ptr(matrixT));

	// HANDLE THE PROJECTION MATRIX
	// Set the projectino matrix to a perspective view
	matrixT = glm::perspective(glm::radians(
		camera->fov), // The zoom of your camera
		camera->aspectRatio, // How wide the view is
		camera->nearClip, // How close you can see 3D models
		camera->farClip); // How far you can see 3D models - all other models woll not render

	// Find the variable in the shader for the projection matrix
	varID = glGetUniformLocation(m_ProgramID, "projection");

	// Update the projection matrix in the shader
	glUniformMatrix4fv(varID, 1, GL_FALSE, value_ptr(matrixT));
}

void PShaderProgram::SetLights(const TArray<TShared<PSLight>>& lights)
{
	PUi32 dirLights = 0;
	PUi32 pointLights = 0;
	PUi32 spotLights = 0;
	int varID = 0;

	// Name of the variable array
	// Will set in the loop depending on the light type
	PString lightIndexStr = "";

	// Loop through all of the lights and add them to the shader
	for (PUi32 i = 0; i < lights.size(); ++i)
	{
		if (const TShared<PSDirLight>& lightRef = std::dynamic_pointer_cast<PSDirLight>(lights[i]))
		{
			// Ignore the light if we have already maxed out
			if (dirLights >= maxDirLights)
			{
				continue;
			}

			// Add a dirLight and use as index
			lightIndexStr = "dirLights[" + std::to_string(dirLights) + "]";

			// COLOUR
			// Get the colour variable from the dir light struct in the shader
			varID = glGetUniformLocation(m_ProgramID,
				(lightIndexStr + ".colour").c_str());
			
			// Change the colour
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->colour));

			// AMBIENT
			// Get the ambient colour variable from the dir light struct in the shader
			varID = glGetUniformLocation(m_ProgramID,
				(lightIndexStr + ".ambient").c_str());

			// Change the ambient colour
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->ambient));

			// DIRECTION
			// Get the direction variable from the fir light struct in the shader
			varID = glGetUniformLocation(m_ProgramID,
				(lightIndexStr + ".direction").c_str());

			// Change the direction
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->direction));

			// INTENSITY
			// Get the intensity variable from the fir light struct in the shader
			varID = glGetUniformLocation(m_ProgramID,
				(lightIndexStr + ".intensity").c_str());

			// Change the intensity
			glUniform1f(varID, lightRef->intensity);

			// Increase the dirLights count
			++dirLights;
			continue;
		}

		if (const TShared<PSPointLight>& lightRef = std::dynamic_pointer_cast<PSPointLight>(lights[i]))
		{
			// Ensure only max lights
			if (pointLights >= maxPointLights)
			{
				continue;
			}

			// Add a dirLight and use as index
			lightIndexStr = "pointLights[" + std::to_string(pointLights) + "]";

			// COLOUR
			// Get the colour variable from the dir light struct in the shader
			varID = glGetUniformLocation(m_ProgramID,
				(lightIndexStr + ".colour").c_str());

			// Change the colour
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->colour));

			// POSITION
			// Get the shader variable ID
			varID = glGetUniformLocation(m_ProgramID,
				(lightIndexStr + ".position").c_str());

			// Update the shader value
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->position));

			// INTENSITY
			// Get the intensity variable from the dir light struct in the shader
			varID = glGetUniformLocation(m_ProgramID,
				(lightIndexStr + ".intensity").c_str());

			// Change the intensity
			glUniform1f(varID, lightRef->intensity);

			// LINEAR ATTENUATION
			// Get the variable ID
			varID = glGetUniformLocation(m_ProgramID, (lightIndexStr + ".linear").c_str());

			// Change the value
			glUniform1f(varID, lightRef->linear);

			// QUADRATIC ATTENUATION
			// Get the variable ID
			varID = glGetUniformLocation(m_ProgramID, (lightIndexStr + ".quadratic").c_str());

			// Change the value
			glUniform1f(varID, lightRef->quadratic);

			// Increment the point light index
			++pointLights;
			
		}

		// SPOT LIGHTS
		if (const TShared<PSSpotLight>& lightRef = std::dynamic_pointer_cast<PSSpotLight>(lights[i]))
		{
			if (spotLights >= maxSpotLights)
				continue;

			// Add a dirLight and use as index
			lightIndexStr = "spotLights[" + std::to_string(spotLights) + "]";

			// COLOUR
			// Get the colour variable from the spot light struct in the shader
			varID = glGetUniformLocation(m_ProgramID,
				(lightIndexStr + ".colour").c_str());

			// Change the colour
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->colour));

			// POSITION
			// Get the shader variable ID
			varID = glGetUniformLocation(m_ProgramID,
				(lightIndexStr + ".position").c_str());

			// Update the shader value
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->position));

			// DIRECTION
			// Get the direction variable from the spot light struct in the shader
			varID = glGetUniformLocation(m_ProgramID,
				(lightIndexStr + ".direction").c_str());

			// Change the direction
			glUniform3fv(varID, 1, glm::value_ptr(lightRef->direction));

			// CUTOFF
			// Get the variable ID
			varID = glGetUniformLocation(m_ProgramID, (lightIndexStr + ".cutOff").c_str());

			// Change the value
			glUniform1f(varID, lightRef->cutOff);

			// OUTER CUTOFF
			// Get the variable ID
			varID = glGetUniformLocation(m_ProgramID, (lightIndexStr + ".outerCutOff").c_str());

			// Change the value
			glUniform1f(varID, lightRef->outerCutOff);

			// INTENSITY
			// Get the variable ID
			varID = glGetUniformLocation(m_ProgramID, (lightIndexStr + ".intensity").c_str());

			// Change the value
			glUniform1f(varID, lightRef->intensity);

			// LINEAR	
			// Get the variable ID
			varID = glGetUniformLocation(m_ProgramID, (lightIndexStr + ".linear").c_str());

			// Change the value
			glUniform1f(varID, lightRef->linear);

			// QUADRATIC
			// Get the variable ID
			varID = glGetUniformLocation(m_ProgramID, (lightIndexStr + ".quadratic").c_str());

			// Change the value
			glUniform1f(varID, lightRef->quadratic);

			++spotLights;
		}
	}
}

void PShaderProgram::SetMaterial(const TShared<PSMaterial>& material)
{
	if (material == nullptr)
	{
		return;
	}

	// The ID for the variable in the shader
	int varID = 0;

	// BASE COLOUR (DIFFUSE)
	if (material->m_BaseColourMap)
	{
		// Bind the texture to the 0 index
		material->m_BaseColourMap->BindTexture(0);

		// Get the base colour map id
		varID = glGetUniformLocation(m_ProgramID, "material.baseColourMap");

		// Update the shader
		glUniform1i(varID, 0);
	}

	// SPECULAR MAP
	if (material->m_SpecularMap)
	{
		// Bind the texture to the 1 index
		material->m_SpecularMap->BindTexture(1);

		// Get the specular map id
		varID = glGetUniformLocation(m_ProgramID, "material.specularMap");

		glUniform1f(varID, 1);
	}

	// NORMAL MAP
	//if (material->m_NormalMap)
	//{
	//	// Bind the texture to the 1 index
	//	material->m_NormalMap->BindTexture(2);

	//	// Get the specular map id
	//	varID = glGetUniformLocation(m_ProgramID, "material.normalMap");

	//	glUniform1f(varID, 2);
	//}

	// SHININESS
	// Get the base colour map id
	varID = glGetUniformLocation(m_ProgramID, "material.shininess");

	// Update the shader
	glUniform1f(varID, material->shininess);

	// SPECULAR STRENGTH
	// Get the base colour map id
	varID = glGetUniformLocation(m_ProgramID, "material.specularStrength");

	// Update the shader
	glUniform1f(varID, material->specularStrength);
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
