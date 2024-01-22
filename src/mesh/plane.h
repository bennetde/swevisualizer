#pragma once
#include <glad/gl.h>

#include "../renderer/shader.h"

class Plane {
	unsigned int vbo, vao, ebo;
	unsigned int displacementBuffer; // todo: add ids for hu, hv, bathymetry
	size_t _nX, _nY;

	bool _renderWireframe;
	float _scale;
public:
	std::vector<float> displacements; //TODO: add hu, hv, bathymetry vectors
	Plane(size_t nX, size_t nY);
	void render(Shader& shader);
	void updateDisplacementBuffer();
};