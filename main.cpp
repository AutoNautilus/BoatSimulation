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

// Our object has 4 points
const uint32_t points = 4;

// Each poin has three values ( x, y, z)
const uint32_t floatsPerPoint = 3;

// Each color has 4 values ( red, green, blue, alpha )
const uint32_t floatsPerColor = 4;


// Create variables for storing the ID of our VAO and VBO
GLuint vbo[2], vao[1];

// The positons of the position and color data within the VAO
const uint32_t positionAttributeIndex = 0, colorAttributeIndex = 1;

// Our wrapper to simplify the shader code
Shader shader;

std::vector<SHPObject*> shapeObjects;
int numEntities = 0;
int shapeType = 0;
double minBound[4], maxBound[4];
int pointCount = 0;
float scale = 1;
GLint scaleId;
GLfloat translate[2] = { 0.0, 0.0 };
GLint translateId;
GLfloat* points2;


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

	// Invoke glDrawArrays telling that our data is a line loop and we want to draw 2-4 vertexes
	int globalIndex = 0;
	int startIndex = 0;
	int endIndex;
	for (int i = 0; i < shapeObjects.size(); i++) {
		SHPObject* obj = shapeObjects[i];
		//std::cout<<"nParts: " << obj->nParts - 1 << std::endl;
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
			//SDL_GL_SwapWindow(mainWindow);
			//waitForInput();
			//std::cout << "rendering from " << globalIndex << " to " << globalIndex + endIndex - startIndex << std::endl;
			globalIndex += endIndex - startIndex;
			startIndex = endIndex;
		}
	}
	//glDrawArrays(GL_LINE_LOOP, startIndex, endIndex - startIndex);

	// Swap our buffers to make our changes visible
	SDL_GL_SwapWindow(mainWindow);


}
bool SetupBufferObjects()
{
	points2 = new GLfloat[pointCount * 3];

	int counter = 0;
	for (int i = 0; i < numEntities; i++) {
		SHPObject* obj = shapeObjects[i];
		int vertices = obj->nVertices;
		for (int j = 0; j < vertices; j++) {
			points2[counter * 3 + 0] = obj->padfX[j] / 180;
			points2[counter * 3 + 1] = obj->padfY[j] / 90;
			points2[counter * 3 + 2] = 0.5;
			counter++;
		}
	}
	std::cout << counter << std::endl;
	for (int i = 0; i < pointCount; i++) {
		//std::cout<<"("<<points2[i * 3 + 0]<<", "<<points2[i * 3 + 1]<<", "<<points2[i * 3 + 2]<<")"<<std::endl;
	}

	// Generate and assign two Vertex Buffer Objects to our handle
	glGenBuffers(2, vbo);

	// Generate and assign a Vertex Array Object to our handle
	glGenVertexArrays(1, vao);

	// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vao[0]);

	// Positions
	// ===================
	// Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	// Copy the vertex data from diamond to our buffer
	glBufferData(GL_ARRAY_BUFFER, (pointCount * floatsPerPoint) * sizeof(GLfloat), points2, GL_STATIC_DRAW);
	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable our attribute within the current VAO
	glEnableVertexAttribArray(positionAttributeIndex);

	// Colors
	// =======================
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	// Copy the vertex data from diamond to our buffer
	//glBufferData(GL_ARRAY_BUFFER, ( points * floatsPerColor) * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	//glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// Note : We didn't enable the colors here!

	// Set up shader ( will be covered in the next part )
	// ===================
	if (!shader.Init())
		return false;

	shader.UseProgram();

	scaleId = glGetUniformLocation(shader.shaderProgram, "scale");
	translateId = glGetUniformLocation(shader.shaderProgram, "translate");
	std::cout << "translate: " << translateId << std::endl;
	glUniform1f(scaleId, scale);
	glUniform2fv(translateId, 2, translate);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
		800, 600, SDL_WINDOW_OPENGL);

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
	delete[] points2;
	// Cleanup all the things we bound and allocated
	shader.CleanUp();

	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);

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
	bool mouseDown = false;

	while (loop)
	{
		Render();

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
			}
			if (event.type == SDL_MOUSEMOTION && mouseDown) {
				translate[0] += (event.motion.xrel) / 180.0f;
				translate[1] -= (event.motion.yrel) / 180.0f;
				std::cout << "translate: [" << translate[0] << ", " << translate[1] << "]" << std::endl;
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
	}
}
int main(int argc, char *argv[])
{
	std::ofstream out("log.txt");
	std::cout.rdbuf(out.rdbuf());
	std::cout << "made it here" << std::endl;
	SHPHandle _handle = SHPOpen("C:/Users/Alex Kinley/gitRepos/BoatSimulation/shapelib/110m_physical/ne_110m_land", "rb");

	SHPGetInfo(_handle, &numEntities, &shapeType, minBound, maxBound);
	std::cout << numEntities << std::endl;
	for (int i = 0; i < numEntities; i++) {
		SHPObject* obj = SHPReadObject(_handle, i);
		pointCount += obj->nVertices;
		shapeObjects.push_back(obj);
	}
	std::cout << "point count:" << pointCount << std::endl;

	if (!Init()) {
		std::cout << "failed to init\n";
		return -1;
	}

	// Clear our buffer with a grey background
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(mainWindow);

	std::cout << "Seting up VBO + VAO..." << std::endl;
	if (!SetupBufferObjects())
		return -1;

	std::cout << "Rendering..." << std::endl;
	Render();
	std::cout << "scaleid " << scaleId << std::endl;

	std::cout << "Rendering done!\n";

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
