#ifndef MESH_H_
#define MESH_H_
#include <GL/glew.h>
#include <vector>
#include <iostream>

class Mesh
{
 public:
  Mesh(const GLfloat*, int, int);
  ~Mesh();

  void init();
  void render();

  const GLfloat* _data;
  int _pointCount;
  int _numPerPoint;
  GLuint vao;
  GLuint vbo;
  const uint32_t positionAttributeIndex = 0;
};
#endif /* MESH_H_ */
