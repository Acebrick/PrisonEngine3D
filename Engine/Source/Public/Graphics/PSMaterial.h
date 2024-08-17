#pragma once
#include "EngineTypes.h"

class PTexture;

struct PSMaterial
{
	PSMaterial() = default;

	// This is the colour map for the material
	TShared<PTexture> m_BaseColourMap;

	// This is the shininess of an object
	// Usually a black and white map
	// White means completely shiny (reflective > reflection of the light)
	TShared<PTexture> m_SpecularMap;

	// This is the bumpiness/depth of an object to give it a more 3D look
	//TShared<PTexture> m_NormalMap;

	// Material properties
	// Shininess of the material
	float shininess = 32.0f;

	// Intensity of specular
	float specularStrength = 0.5f;
};