#pragma once
#include "EngineTypes.h"

class PTexture;
struct PSCamera;

// Enum to determine the typw of shader
enum PEShaderType : PUi8
{
	ST_VERTEX = 0U,
	ST_FRAGMENT
};

struct PSTransform;

class PShaderProgram 
{
public:
	PShaderProgram();
	~PShaderProgram();

	// Create the shader using a vertex and fragment file
	bool InitShader(const PString& vShaderPath, const PString& fShaderPath);

	// Activate the shader to update
	// You can't change values in a shader without activating it
	void Activate();

	// Set the transform of the model in the shader
	void SetModelTransform(const PSTransform& transform);

	// Set the 3D coordinates for the model
	void SetWorldTransform(const TShared<PSCamera>& camera);

	// Set a texture in the shader based on the slot
	void RunTexture(const TShared<PTexture>& texture, const PUi32& slot);

private:
	// Store the file paths
	PString m_FilePath[2] = { "", "" };

	// Store the shader IDs
	PUi32 m_ShaderIDs[2] = { 0, 0 };

	// Store the ID for the program
	PUi32 m_ProgramID;

	// Import a shader based on the shader type
	bool ImportShaderByType(const PString& filePath, PEShaderType shaderType);

	// Convert a file into a string
	PString ConvertFileToString(const PString& filePath);

	// Link the shader to the GPU through open gl
	bool LinkToGPU();
};
