#ifndef WINDOW_H_
#define WINDOW_H_

#include <iostream>
#include <string>
#include <vector>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "Renderer.h"


class Window
{
 public:
  Window(std::string name);
  Window(std::string name, int width, int height);
  ~Window();

  bool setup();
  void render();
  void swapWindow();
  void handleInput();
  void ImGuiNewFrame();
  SDL_Window* getWindow();
  SDL_GLContext getContext();
  void addRenderer(Renderer*);
 private:

  bool SetOpenGLAttributes();
  void CheckSDLError(int);
  const int DEFAULT_WIDTH = 200;
  const int DEFAULT_HEIGHT = 200;
  
  std::vector<Renderer*> _renderers;
  int _width, _height;
  std::string _windowName;
  SDL_Window* _window;
  SDL_GLContext _context;
};

#endif /* WINDOW_H_ */
