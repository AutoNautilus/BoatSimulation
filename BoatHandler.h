#ifndef BOAT_HANDLER_H
#define BOAT_HANDLER_H

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include "Boat.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class BoatHandler {
public:
	BoatHandler();

	void setup(GLint transformMatId);
	void render(GLint translateId, GLfloat translate[2]);
	void update();
	void restart();
	void togglePause();
	bool isPaused();
	Boat* getBoat() { return _boat; }
private:

	bool _paused = false;

	void makeCircle();
	int circlePoints = 24;
	GLfloat* _circle;

	Boat* _boat;
	GLuint vbo[2], vao[2];
	const uint32_t positionAttributeIndex = 0;

	GLuint _transformMatId;
	

	glm::mat4 _transform;
	glm::mat4 identity;

};


#endif
