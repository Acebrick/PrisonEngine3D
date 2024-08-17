#version 460 core

// out = going out of the shader into something else
out vec4 finalColour;

void main() {
	finalColour = vec4(vec3(1.0f), 1.0f);
} 