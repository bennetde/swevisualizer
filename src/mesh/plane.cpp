#include "plane.h"
#include <iostream>
#include <imgui.h>
#include <vector>

/**
 * Initialize plane of size nX*nY by correctly setting variables and calculating vertices and indices
 */
Plane::Plane(size_t nX, size_t nY, float cellWidth, float cellHeight) : _nX(nX), _nY(nY), _renderWireframe(false), _scale(0.3f), displacements(nX * nY), hu(nX * nY), hv(nX * nY), bathymetry(nX * nY)
{
	/**
	 * Initialize vertex and index data
	 */
	std::vector<float> vertices(nX * nY * 3);
	std::vector<unsigned int> indices((nX - 1) * (nY - 1) * 2 * 3);
	size_t index = 0;
	size_t triangleIndex = 0;

	/**
	 * Calculate aspect ratio to adjust cell width
	 */
	float aspect = cellWidth / cellHeight;
	/**
	 * Loop through all vertices
	 */
	for (size_t i = 0; i < nX * nY; i++)
	{
		size_t x = i % nX;
		size_t y = i / nX;

		vertices[index] = x * aspect;
		vertices[index + 1] = y;
		vertices[index + 2] = 0.0f;
		index += 3;

		/**
		 * Create triangles, except when at the edge
		 */
		if (x + 1 != nX && y + 1 < nY)
		{
			/**
			 * Define indices for two triangles i and i+1 forming a square
			 */
			indices[triangleIndex] = i;
			indices[triangleIndex + 1] = i + 1;
			indices[triangleIndex + 2] = i + nX;
			indices[triangleIndex + 3] = i + 1;
			indices[triangleIndex + 4] = i + nX + 1;
			indices[triangleIndex + 5] = i + nX;
			/**
			 * Increment index by 6 for the next triangle set
			 */
			triangleIndex += 6;
		}
	}

	/**
	 * Generate and bind vertex array object
	 */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/**
	 * Generate and bind vertex buffer object, populate it with vertex data, specify pointers and enable attribute array
	 */
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	/**
	 * Generate and bind element buffer object and populate it with index data
	 */
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	/**
	 * Set displacement buffer for saving displacement data
	 */
	glGenBuffers(1, &displacementBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, displacementBuffer);
	glBufferData(GL_ARRAY_BUFFER, displacements.size() * sizeof(float), displacements.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);

	/**
	 * Set hu buffer for saving hu data
	 */
	glGenBuffers(1, &huBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, huBuffer);
	glBufferData(GL_ARRAY_BUFFER, hu.size() * sizeof(float), hu.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
	glEnableVertexAttribArray(2);

	/**
	 * Set hv buffer for saving hv data
	 */
	glGenBuffers(1, &hvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, hvBuffer);
	glBufferData(GL_ARRAY_BUFFER, hv.size() * sizeof(float), hv.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
	glEnableVertexAttribArray(3);

	/**
	 * Set bathymetry buffer for saving bathymetry data
	 */
	glGenBuffers(1, &bathymetryBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, bathymetryBuffer);
	glBufferData(GL_ARRAY_BUFFER, bathymetry.size() * sizeof(float), bathymetry.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

/**
 * Update displacement buffer by binding the GL_ARRAY_BUFFER to the displacementBuffer and updating it with the content
 */
void Plane::updateDisplacementBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, displacementBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, displacements.size() * sizeof(float), displacements.data());
}

/**
 * Update hu buffer by binding the GL_ARRAY_BUFFER to the huBuffer and updating it with the content
 */
void Plane::updateHuBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, huBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, hu.size() * sizeof(float), hu.data());
}

/**
 * Update hv buffer by binding the GL_ARRAY_BUFFER to the hvBuffer and updating it with the content
 */
void Plane::updateHvBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, hvBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, hv.size() * sizeof(float), hv.data());
}

/**
 * Update bathymetry buffer by binding the GL_ARRAY_BUFFER to the bathymetryBuffer and updating it with the content
 */
void Plane::updateBathymetryBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, bathymetryBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bathymetry.size() * sizeof(float), bathymetry.data());
}

/**
 * Render the plane
 */
void Plane::render(Shader &shader)
{
	shader.use();

	shader.setFloat("scale", _scale);
	glPolygonMode(GL_FRONT_AND_BACK, _renderWireframe ? GL_LINE : GL_FILL);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, (_nX - 1) * (_nY - 1) * 2 * 3, GL_UNSIGNED_INT, 0);

	if (ImGui::Begin("Plane Settings"))
	{
		ImGui::Text("Resolution: %dx%d", _nX, _nY);
		ImGui::Text("Vertices: %d", _nX * _nY);
		/**
		 * Give possibility to set wireframe mode
		 */
		ImGui::Checkbox("Wireframe", &_renderWireframe);
		/**
		 * Give possibility to adjust scale and update shader with new scale value
		 */
		if (ImGui::SliderFloat("Scale", &_scale, 0.0f, 0.3f, "%.6f"))
		{
			shader.setFloat("scale", _scale);
		}
	}
	ImGui::End();
}