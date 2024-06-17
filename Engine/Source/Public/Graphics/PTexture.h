#pragma once
#include "EngineTypes.h"
class PTexture
{
public:
	PTexture();
	~PTexture();

	// Import a file and convert it to a texture
	bool LoadTexture(const PString& fileName, const PString& path);

	// Activates the texture to use for open gl
	void BindTexture(const PUi32& textureNumber);

	// Deactiavtes the texture in open gl
	void Unbind();

	// Gets the import path of the texture
	PString GetImportPath() const { return m_Path; }

	// Gets the custom file name fthe texture
	PString GetName() const { return m_FileName; }

	// Get the ID of the texture for open gl
	PUi32 GetID() const { return m_ID; }

private:
	// Import path of the image
	PString m_Path;

	// Custom name of the texture
	PString m_FileName;

	// The ID for the texture in open gl
	PUi32 m_ID;

	// Texture parameters
	int m_Width, m_Height, m_Channels;

};