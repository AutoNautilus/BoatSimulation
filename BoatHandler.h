#ifndef BOAT_HANDLER_H
#define BOAT_HANDLER_H

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include "Boat.h"
#include <glm\glm.hpp>	

class BoatHandler {
public:
	BoatHandler();

	void setup();
	void render(GLint translateId, GLfloat translate[2]);

private:
	Boat* _boat;
	GLuint vbo[1], vao[1];
	const uint32_t positionAttributeIndex = 0;


};


#endif
