#pragma once
#include <glad/gl.h>

#include "../renderer/shader.h"

class Plane {
	unsigned int vbo, vao, ebo;
public:
	Plane();
	void render(Shader& shader);
};