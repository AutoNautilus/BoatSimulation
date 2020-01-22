#include "Renderer.h"

Renderer::Renderer(){

}
Renderer::~Renderer(){

}

void Renderer::init(){
	if (!_shader.Init())
		std::cout << "failed to init shader" << std::endl;
}


void Renderer::render(){
	_shader.UseProgram();
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	for(int i = 0; i < _meshes.size(); i++){
		_meshes[i]->render();
	}
}

void Renderer::addMesh(Mesh* mesh){
  mesh->init();
  _meshes.push_back(mesh);
}
