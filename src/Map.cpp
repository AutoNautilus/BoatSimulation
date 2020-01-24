#include "Map.h"
#include <iostream>

Map::Map(const char* fileName) {
	SHPHandle _handle = SHPOpen(fileName, "rb");

	SHPGetInfo(_handle, &numEntities, &shapeType, minBound, maxBound);
	std::cout << numEntities << std::endl;
	for (int i = 0; i < numEntities; i++) {
		SHPObject* obj = SHPReadObject(_handle, i);
		pointCount += obj->nVertices;
		shapeObjects.push_back(obj);
	}
}

void Map::init() {
	points2 = new GLfloat[pointCount * 3];

	int counter = 0;
	for (int i = 0; i < numEntities; i++) {
		SHPObject* obj = shapeObjects[i];
		int vertices = obj->nVertices;
		for (int j = 0; j < vertices; j++) {
			points2[counter * 3 + 0] = obj->padfX[j] / 180.0f;
			points2[counter * 3 + 1] = obj->padfY[j] / 90.0f;
			points2[counter * 3 + 2] = 0.5;
			counter++;
		}
	}

	// Generate and assign a Vertex Array Object to our handle
	glGenVertexArrays(3, vao);

	// Generate and assign two Vertex Buffer Objects to our handle
	glGenBuffers(3, vbo);

	// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	// Copy the vertex data from diamond to our buffer
	glBufferData(GL_ARRAY_BUFFER, (pointCount * 3) * sizeof(GLfloat), points2, GL_STATIC_DRAW);
	// Enable our attribute within the current VAO
	glEnableVertexAttribArray(positionAttributeIndex);
	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//east line
	// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	// Copy the vertex data from diamond to our buffer
	glBufferData(GL_ARRAY_BUFFER, (5 * 3) * sizeof(GLfloat), eastLine, GL_STATIC_DRAW);
	// Enable our attribute within the current VAO
	glEnableVertexAttribArray(positionAttributeIndex);
	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//west line
	// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);

	// Copy the vertex data from diamond to our buffer
	glBufferData(GL_ARRAY_BUFFER, (6 * 3) * sizeof(GLfloat), westLine, GL_STATIC_DRAW);
	// Enable our attribute within the current VAO
	glEnableVertexAttribArray(positionAttributeIndex);
	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Map::render() {
	glBindVertexArray(vao[0]);

	int globalIndex = 0;
	int startIndex = 0;
	int endIndex;
	for (int i = 0; i < shapeObjects.size(); i++) {
		SHPObject* obj = shapeObjects[i];
		int parts;
		if (obj->nParts > 1) {
			parts = obj->nParts;
		}
		else {
			parts = 1;
		}
		startIndex = 0;
		for (int j = 0; j < parts; j++) {
			if (parts == 1) {
				endIndex = obj->nVertices;
			}
			else {
				if (j == parts - 1) {
					endIndex = obj->nVertices;
				}
				else {
					endIndex = obj->panPartStart[j + 1];
				}
			}
			glDrawArrays(GL_LINE_STRIP, globalIndex, endIndex - startIndex);
			globalIndex += endIndex - startIndex;
			startIndex = endIndex;
		}
	}

	//east line
	glBindVertexArray(vao[1]);
	glDrawArrays(GL_LINE_STRIP, 0, 5);

	//west line
	glBindVertexArray(vao[2]);
	glDrawArrays(GL_LINE_STRIP, 0, 6);
}

Renderer* Map::makeRenderer() {
	Renderer* renderer = new Renderer();

	int globalIndex = 0;
	int startIndex = 0;
	int endIndex;
	for (int i = 0; i < shapeObjects.size(); i++) {
		SHPObject* obj = shapeObjects[i];
		int parts;
		if (obj->nParts > 1) {
			parts = obj->nParts;
		}
		else {
			parts = 1;
		}
		startIndex = 0;
		for (int j = 0; j < parts; j++) {
			if (parts == 1) {
				endIndex = obj->nVertices;
			}
			else {
				if (j == parts - 1) {
					endIndex = obj->nVertices;
				}
				else {
					endIndex = obj->panPartStart[j + 1];
				}
			}
			renderer->addMesh(new Mesh(&points2[globalIndex * 3], endIndex - startIndex, 3));
			globalIndex += endIndex - startIndex;
			startIndex = endIndex;
		}
	}
	//renderer->addMesh(new Mesh(points2, pointCount, 3));
	renderer->addMesh(new Mesh(eastLine, 5, 3));
	renderer->addMesh(new Mesh(westLine, 6, 3));

	renderer->init();
	return renderer;
}
