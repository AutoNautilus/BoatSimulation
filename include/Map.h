#ifndef MAP_H
#define MAP_H

#include <vector>

#include "shapelib/shapefil.h"
#include <GL/glew.h>
#include "Renderer.h"

class Map {
public:
	Map(const char* fileName);

	void init();

	void render();

	Renderer* makeRenderer();

private:
	const uint32_t positionAttributeIndex = 0;

	const GLfloat eastLine[3 * 5] =
	{ -16.0f / 180.0f, 55.0f / 90.0f, 0.5f,
		-16.0f / 180.0f, 51.0f / 90.0f, 0.5f,
		-11.0f / 180.0f, 46.0f / 90.0f, 0.5f,
		-13.5f / 180.0f, 44.5f / 90.0f, 0.5f,
		-13.5f / 180.0f, 37.0f / 90.0f, 0.5f };

	const GLfloat westLine[3 * 6] =
	{ -47.0f / 180.0f, 48.0f / 90.0f, 0.5f,
	  -47.0f / 180.0f, 45.5f / 90.0f, 0.5f,
	  -65.0f / 180.0f, 40.0f / 90.0f, 0.5f,
	  -77.0f / 180.0f, 30.0f / 90.0f, 0.5f,
	  -59.0f / 180.0f, 20.0f / 90.0f, 0.5f,
	  -56.0f / 180.0f, 10.0f / 90.0f, 0.5f };


	std::vector<SHPObject*> shapeObjects;
	int numEntities = 0;
	int shapeType = 0;
	double minBound[4], maxBound[4];
	int pointCount = 0;

	GLfloat* points2;

	GLuint vbo[3], vao[3];
};

#endif /* MAP_H */
