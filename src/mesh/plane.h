#pragma once
#include <glad/gl.h>

#include "../renderer/shader.h"

class Plane {
	unsigned int vbo, vao, ebo;
	unsigned int displacementBuffer;
	unsigned int huBuffer;
	unsigned int hvBuffer;
	unsigned int bathymetryBuffer;
	size_t _nX, _nY;

	bool _renderWireframe;
	float _scale;
public:
	std::vector<float> displacements;
	std::vector<float> hu;
	std::vector<float> hv;
	std::vector<float> bathymetry;
	Plane(size_t nX, size_t nY, float cellWidth, float cellHeight);
	void render(Shader& shader);
	void updateDisplacementBuffer();
	void updateHuBuffer();
	void updateHvBuffer();
	void updateBathymetryBuffer();
};