#include "BoatHandler.h"
#include <iostream>

const GLfloat triangle[3][3] =
{
	{ 0.0,  0.433 / 90,  0.5 }, // Top
	{ -0.5 / 90,  -0.433 / 90,  0.5 }, // Bottom left
	{ .5 / 90, -0.433 / 90,  0.5 }, // Bottom right 
};


BoatHandler::BoatHandler() {
	float startCoords[2] = { -70.929406, 42.380138, }; //long, lat near boston
	_boat = new Boat(startCoords);
	std::cout << "boat is at (" << _boat->getLong() << ", " << _boat->getLat() << ")" << std::endl;
}

void BoatHandler::setup() {
	// Generate and assign a Vertex Array Object to our handle
	glGenVertexArrays(1, vao);

	// Generate and assign two Vertex Buffer Objects to our handle
	glGenBuffers(1, vbo);

	// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vao[0]);
	// Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	// Copy the vertex data from diamond to our buffer
	glBufferData(GL_ARRAY_BUFFER, (3 * 3) * sizeof(GLfloat), triangle, GL_STATIC_DRAW);

	// Enable our attribute within the current VAO
	glEnableVertexAttribArray(positionAttributeIndex);
	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void BoatHandler::render(GLint translateId, GLfloat translate[2]) {
	glBindVertexArray(vao[0]);
	glUniform2f(translateId, translate[0] + _boat->getLong() / 180, translate[1] + _boat->getLat() / 90);
	glDrawArrays(GL_LINE_LOOP, 0, 3);
}