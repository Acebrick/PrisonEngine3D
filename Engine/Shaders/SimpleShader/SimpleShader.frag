#version 460 core

in vec3 fColour;

// out = going out of the shader into something else
out vec4 finalColour;

void main() {
	finalColour = vec4(fColour, 1.0);
}