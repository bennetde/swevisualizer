#include "plane.h"
#include <iostream>

Plane::Plane() {
	vertices[0] = vertices[1] = vertices[4] = -0.5f;
	vertices[2] = vertices[5] = vertices[8] = vertices[6] = 0.0f;
	vertices[3] = vertices[7] = 0.5f;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

}

void Plane::render(Shader& shader) {
	shader.use();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}