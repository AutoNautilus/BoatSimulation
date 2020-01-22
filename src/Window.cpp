#include "Window.h"

Window::Window(std::string name){
  _windowName = name;
  _width = DEFAULT_WIDTH;
  _height = DEFAULT_HEIGHT;
}
Window::Window(std::string name, int width, int height){
  _windowName = name;
  _width = width;
  _height = height;
}
Window::~Window(){
  SDL_GL_DeleteContext(_context);
  SDL_DestroyWindow(_window);
}

bool Window::setup(){
  printf("window size is (%d, %d)\n", _width, _height);
	_window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL);
  
	// Check that everything worked out okay
	if (_window == NULL)
	{
		std::cout << "Unable to create window" << SDL_GetError() << std::endl;;
		CheckSDLError(25);
		return false;
	}

	SetOpenGLAttributes();

	// Create our opengl context and attach it to our window
	_context = SDL_GL_CreateContext(_window);

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	// Init GLEW
	SDL_GL_MakeCurrent(_window, _context);
	glewExperimental = GL_TRUE;
	glewInit();

  return true;
}

void Window::ImGuiNewFrame(){
  ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(_window);
	ImGui::NewFrame();
}
bool Window::SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}
void Window::CheckSDLError(int line = -1)
{
	std::string error = SDL_GetError();

	if (error != "")
    {
      std::cout << "SLD Error : " << error << std::endl;

      if (line != -1)
        std::cout << "\nLine : " << line << std::endl;

      SDL_ClearError();
    }
}
void Window::render(){
  SDL_GL_MakeCurrent(_window, _context);

  for (int i = 0; i < _renderers.size(); i++) {
	  _renderers[i]->render();
  }
}

void Window::swapWindow(){
  SDL_GL_SwapWindow(_window);
}
void Window::handleInput(){

}

SDL_Window* Window::getWindow(){
  return _window;
}
SDL_GLContext Window::getContext(){
  return _context;
}


void Window::addRenderer(Renderer* renderer) {
	_renderers.push_back(renderer);
}
