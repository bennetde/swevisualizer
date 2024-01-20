#include "plane.h"
#include <iostream>
#include <imgui.h>
#include <vector>

Plane::Plane(size_t nX, size_t nY) : _nX(nX), _nY(nY), _renderWireframe(false), _scale(1.0f), displacements(nX * nY) {
	std::vector<float> vertices(nX * nY * 3);
	std::vector<unsigned int> indices((nX-1)*(nY-1)*2*3);
	// std::cout << (nX-1)*(nY-1)*2*3 << std::endl;
	size_t index = 0;
	size_t triangleIndex = 0;
	for(size_t i = 0; i < nX * nY; i++) {
		size_t x = i % nX;
		size_t y = i / nX;
		// std::cout << "Adding point i=" << i << " (x=" << x << ", y=" << y << ")\n";
		vertices[index] = x;
		vertices[index+1] = y;
		vertices[index+2] = 0.0f;
		index += 3;

		if(x+1 != nX && y+1 < nY) {
			indices[triangleIndex] = i;
			indices[triangleIndex+1] = i+1;
			indices[triangleIndex+2] = i+nX;
			indices[triangleIndex+3] = i+1;
			indices[triangleIndex+4] = i+nX+1;
			indices[triangleIndex+5] = i+nX;
			// std::cout << "Added Triangle " << i << ", " << i+1 << ", " << i+nX << std::endl;
			// std::cout << "Added Triangle " << i+1 << ", " << i+nX+1 << ", " << i+nX << std::endl;
			triangleIndex += 6;
		}
	}
	// std::cout << "Done\n";


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &displacementBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, displacementBuffer);
	glBufferData(GL_ARRAY_BUFFER, displacements.size() * sizeof(float), displacements.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*) 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

}

void Plane::updateDisplacementBuffer() {
	glBindBuffer(GL_ARRAY_BUFFER, displacementBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, displacements.size() * sizeof(float), displacements.data());
}

void Plane::render(Shader& shader) {
	shader.use();
	glPolygonMode( GL_FRONT_AND_BACK, _renderWireframe ? GL_LINE : GL_FILL );
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, (_nX-1)*(_nY-1)*2*3, GL_UNSIGNED_INT, 0);

	if(ImGui::Begin("Plane Settings")) {
		ImGui::Text("Resolution: %dx%d", _nX, _nY);
		ImGui::Text("Vertices: %d", _nX * _nY);
		ImGui::Checkbox("Wireframe", &_renderWireframe);
		if(ImGui::SliderFloat("Scale", &_scale, 0.0f, 2.0f)) {
			shader.setFloat("scale", _scale);
		}
	}
	ImGui::End();
}