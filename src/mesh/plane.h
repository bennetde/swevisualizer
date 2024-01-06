#pragma once
#include <glad/gl.h>

#include "../renderer/shader.h"

class Plane {
	unsigned int vbo, vao, ebo;
	float vertices[9];
public:
	Plane();
	void render(Shader& shader);
};