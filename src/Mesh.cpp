#include "Mesh.h"

Mesh::Mesh(const GLfloat* data, int pointCount, int numPerPoint){
  _data = data;
  _pointCount = pointCount;
  _numPerPoint = numPerPoint;
}
Mesh::~Mesh() {

}

void Mesh::init(){
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, (_pointCount * _numPerPoint) * sizeof(GLfloat), _data, GL_STATIC_DRAW);
  glEnableVertexAttribArray(positionAttributeIndex);
	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(positionAttributeIndex, _numPerPoint, GL_FLOAT, GL_FALSE, 0, 0);
}

void Mesh::render(){
  glBindVertexArray(vao);
  glDrawArrays(GL_LINE_STRIP, 0, _pointCount);
}

