#pragma once
#include <glad/gl.h>

#include "../renderer/shader.h"

class Plane {
	unsigned int vbo, vao, ebo;
	size_t _nX, _nY;

	bool _renderWireframe;
public:
	Plane(size_t nX, size_t nY);
	void render(Shader& shader);
};