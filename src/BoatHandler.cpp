#include "BoatHandler.h"
#include <iostream>

const GLfloat triangle[3][3] =
{
	{ 0.0,  0.433 / 90,  0.5 }, // Top
	{ -0.5 / 90,  -0.433 / 90,  0.5 }, // Bottom left
	{ .5 / 90, -0.433 / 90,  0.5 }, // Bottom right 
};

void print_vec(const std::vector<glm::vec2>& vec)
{
	for (auto p : vec) {
		std::cout << " (" << p.x << ", " << p.y << ")";
	}
	std::cout << '\n';
}


BoatHandler::BoatHandler() {
	restart();
	
	identity = glm::mat4(1.0f);
}

void BoatHandler::setup(GLint transformMatId) {
	_transformMatId = transformMatId;

	// Generate and assign a Vertex Array Object to our handle
	glGenVertexArrays(2, vao);

	// Generate and assign two Vertex Buffer Objects to our handle
	glGenBuffers(2, vbo);

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

	makeCircle();

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glBufferData(GL_ARRAY_BUFFER, (3 * circlePoints) * sizeof(GLfloat), _circle, GL_STATIC_DRAW);

	// Enable our attribute within the current VAO
	glEnableVertexAttribArray(positionAttributeIndex);
	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void BoatHandler::update() {
	if (!_paused) {
		_elapsedTime += timeIncr;
		_boat->update(timeIncr);
		//togglePause();
	}
}

void BoatHandler::restart() {
	_elapsedTime = 0;

	float startCoords[2] = { -70.0, 42.0 }; //long, lat in sorta the right area but much easier to do math with
	//float startCoords[2] = { -70.929406, 42.380138, }; //long, lat near boston
	if (_boat != 0) {
		delete _boat;
	}

	_boat = new Boat(startCoords);
	std::cout << "boat is at (" << _boat->getLong() << ", " << _boat->getLat() << ")" << std::endl;


	_boat->addWayPointLast(glm::vec2(-59.0f, 20.0f));
	_boat->addWayPointLast(glm::vec2(-11.0f, 46.0f));
}
void BoatHandler::togglePause() {
	_paused = !_paused;
}
bool BoatHandler::isPaused() {
	return _paused;
}


void BoatHandler::render(GLint translateId, GLfloat translate[2]) {
	glBindVertexArray(vao[0]);
	
	_transform = glm::translate(identity, 
								glm::vec3(_boat->getLong() / 180, _boat->getLat() / 90, 0.0f));
	_transform = glm::rotate(_transform, glm::radians(_boat->getHeading() - 90), glm::vec3(0, 0, 1));
	
	glUniformMatrix4fv(_transformMatId, 1, GL_FALSE, glm::value_ptr(_transform));

	glDrawArrays(GL_LINE_LOOP, 0, 3);

	glBindVertexArray(vao[1]);
	std::vector<glm::vec2> wayPoints = _boat->getWayPoints();

	for (int i = 0; i < wayPoints.size(); i++) {
		glm::vec2 point = wayPoints[i];
		
		_transform = glm::translate(identity, glm::vec3(point.x / 180, point.y / 90, 0.0f));
		glUniformMatrix4fv(_transformMatId, 1, GL_FALSE, glm::value_ptr(_transform));

		glDrawArrays(GL_LINE_LOOP, 0, circlePoints);
	}

	glUniformMatrix4fv(_transformMatId, 1, GL_FALSE, glm::value_ptr(identity));
}

void BoatHandler::makeCircle() {
	_circle = new GLfloat[3 * circlePoints];

	for (int i = 0; i < circlePoints; i++) {
		float angle = (i * 2 * 3.141592) / circlePoints;
		_circle[i * 3] = glm::cos(angle) / 180.0f;
		_circle[i * 3 + 1] = glm::sin(angle) / 90.0f;
		_circle[i * 3 + 2] = 0.5f;
	}
}

float BoatHandler::getElapsedTime() {
	return _elapsedTime;
}