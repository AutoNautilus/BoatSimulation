// Headerphile.com OpenGL Tutorial part 2
// A simple example involving VBOs and a VAO to draw a simple square
// Source code is an adaption / simplicication of : https://www.opengl.org/wiki/Tutorial2:_VAOs,_VBOs,_Vertex_and_Fragment_Shaders_(C_/_SDL) 

// Compile :
// 	clang++ main.cpp -lGL -lGLEW -lSDL2 -std=c++11 -o Test
// or
// 	g++ main.cpp -lGL -lGLEW -lSDL2 -std=c++11 -o Test
//
#include <iostream>
#include <string>
#include <vector>
#include "Shader.h"
#include "BoatHandler.h"
#include "Map.h"

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "shapelib/shapefil.h"

#define PROGRAM_NAME "Tutorial2"

std::string programName = "Headerphile SDL2 - OpenGL thing";

// Our SDL_Window ( just like with SDL2 wihout OpenGL)
SDL_Window *mainWindow;

// Our opengl context handle
SDL_GLContext mainContext;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Our wrapper to simplify the shader code
Shader shader;

Map* map;

float scale = 1;
GLint scaleId;
GLfloat translate[2] = { 0.0, 0.0 };
GLint translateId;
GLint transformMatId;

BoatHandler* _boatHandler;


bool SetOpenGLAttributes();
void PrintSDL_GL_Attributes();
void CheckSDLError(int line);

void waitForInput() {
	bool loop = true;

	while (loop)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				return;
			}
		}
	}
}

void Render()
{
	// First, render a square without any colors ( all vertexes will be black )
	// ===================
	// Make our background grey
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform1f(scaleId, scale);
	glUniform2f(translateId, translate[0], translate[1]);

	map->render();

	_boatHandler->render(translateId, translate);

	// Swap our buffers to make our changes visible
	SDL_GL_SwapWindow(mainWindow);


}
bool SetupBufferObjects()
{
	map->init();

	if (!shader.Init())
		return false;

	shader.UseProgram();

	scaleId = glGetUniformLocation(shader.shaderProgram, "scale");
	translateId = glGetUniformLocation(shader.shaderProgram, "translate");
	transformMatId = glGetUniformLocation(shader.shaderProgram, "transform");

	glUniform1f(scaleId, scale);
	glUniform2fv(translateId, 2, translate);
	glm::mat4 identity( 1.0f );
	glUniformMatrix4fv(transformMatId, 1, GL_FALSE, glm::value_ptr(identity));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	_boatHandler->setup(transformMatId);


	return true;
}

bool Init()
{
	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL\n";
		return false;
	}

	// Create our window centered at 512x512 resolution
	mainWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

	// Check that everything worked out okay
	if (!mainWindow)
	{
		std::cout << "Unable to create window\n" << std::endl;;
		CheckSDLError(__LINE__);
		return false;
	}

	SetOpenGLAttributes();

	// Create our opengl context and attach it to our window
	mainContext = SDL_GL_CreateContext(mainWindow);

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	// Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	return true;
}

bool SetOpenGLAttributes()
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

void Cleanup()
{
	//delete[] points2;
	// Cleanup all the things we bound and allocated
	shader.CleanUp();

	glDisableVertexAttribArray(0);
	//glDeleteBuffers(1, vbo);
	//glDeleteVertexArrays(1, vao);

	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	// Destroy our window
	SDL_DestroyWindow(mainWindow);

	// Shutdown SDL 2
	SDL_Quit();
}
void windowLoop() {
	bool loop = true;

	int lastMouseX, lastMouseY;
	float transStartX, transStartY;
	bool mouseDown = false;

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;


	while (loop)
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
		//std::cout << "deltaTime: " << deltaTime << std::endl;

		_boatHandler->update();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				loop = false;
			if (event.type == SDL_MOUSEWHEEL)
			{
				if (event.wheel.y > 0) // scroll up
				{
					scale += 0.5;
				}
				else if (event.wheel.y < 0) // scroll down
				{
					scale -= 0.5;
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				mouseDown = true;
				lastMouseX = event.button.x;
				lastMouseY = event.button.y;
				transStartX = translate[0];
				transStartY = translate[1];
			}
			if (event.type == SDL_MOUSEMOTION && mouseDown) {
				translate[0] = transStartX + ((event.motion.x - lastMouseX) / ((float)(WINDOW_WIDTH) / 2 * scale));
				translate[1] = transStartY - ((event.motion.y - lastMouseY) / ((float)(WINDOW_HEIGHT) / 2 * scale));
			}
			if (event.type == SDL_MOUSEBUTTONUP) {
				mouseDown = false;
			}

			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					loop = false;
					break;
				default:
					break;
				}
			}
		}

		Render();
	}
}



int main(int argc, char *argv[])
{
	std::ofstream out("log.txt");
	std::cout.rdbuf(out.rdbuf());
	std::cout << "made it here" << std::endl;
	map = new Map("C:/Users/Alex Kinley/gitRepos/BoatSimulation/shapelib/110m_physical/ne_110m_land");

	_boatHandler = new BoatHandler();

	if (!Init()) {
		std::cout << "failed to init\n";
		return -1;
	}

	std::cout << "Seting up VBO + VAO..." << std::endl;
	if (!SetupBufferObjects())
		return -1;

	std::cout << "Rendering..." << std::endl;
	Render();

	std::cout << "Rendering done!\n";
	waitForInput();
	windowLoop();

	Cleanup();

	return 0;
}

void CheckSDLError(int line = -1)
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

void PrintSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}
