#pragma once
#include "Math/PSTransform.h"

struct PSCamera
{
	PSCamera()
	{
		fov = 70.0f;
		aspectRatio = 1.0f;
		nearClip = 0.01f;
		farClip = 10000.0f;
	}

	PSTransform transform;
	float fov;
	float aspectRatio;
	float nearClip;
	float farClip;
};