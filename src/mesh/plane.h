#pragma once
#include <glad/gl.h>
#include <vector>

#include "../renderer/shader.h"

/**
 * Represents a 3D plane
 */
class Plane
{
	unsigned int vbo, vao, ebo;
	unsigned int displacementBuffer;
	unsigned int huBuffer;
	unsigned int hvBuffer;
	unsigned int bathymetryBuffer;
	size_t _nX, _nY;

	bool _renderWireframe;
	float _scale;

public:
	/**
	 * Plane properties
	 */
	std::vector<float> displacements;
	std::vector<float> hu;
	std::vector<float> hv;
	std::vector<float> bathymetry;

	/**
	 * Constructor
	 */
	Plane(size_t nX, size_t nY, float cellWidth, float cellHeight);

	/**
	 * Render plane using the provided shader
	 */
	void render(Shader &shader);

	/**
	 * Update buffers in the GPU
	 */
	void updateDisplacementBuffer();
	void updateHuBuffer();
	void updateHvBuffer();
	void updateBathymetryBuffer();
};