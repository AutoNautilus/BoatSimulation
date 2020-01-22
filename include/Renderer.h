#ifndef RENDERER_H_
#define RENDERER_H_

#include <vector>

#include "Shader.h"
#include "Mesh.h"
class Renderer {
 public:
  Renderer();
  ~Renderer();

  void init();
  void render();

  void addMesh(Mesh*);
 private:
  Shader _shader;

  std::vector<Mesh*> _meshes;

};

#endif /* RENDERER_H_ */
