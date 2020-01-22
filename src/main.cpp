#include <iostream>
#include <string>
#include <vector>
#include "Shader.h"
#include "BoatHandler.h"
#include "Map.h"
#include "Window.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "shapelib/shapefil.h"

#define PROGRAM_NAME "Tutorial2"

Window* mainWindow;
Window* debugWindow;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int DEBUG_WINDOW_WIDTH = 600;
const int DEBUG_WINDOW_HEIGHT = 500;

// Our wrapper to simplify the shader code
Shader shader;

Map* map;

float scale = 1;
GLint scaleId;
GLfloat translate[2] = { 0.0, 0.0 };
GLint translateId;
GLint transformMatId;

BoatHandler* _boatHandler;

ImGuiIO* io;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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

void ImGuiDraw() {
	// Start the Dear ImGui frame
  debugWindow->render();
  debugWindow->ImGuiNewFrame();
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		ImGui::Begin("Controls");

		if (ImGui::Button("Restart")) {
			_boatHandler->restart();
		}

		if (_boatHandler->isPaused()) {
			if (ImGui::Button("Unpause")) {
				_boatHandler->togglePause();
			}
		}
		else {
			if (ImGui::Button("Pause")) {
				_boatHandler->togglePause();
			}
		}

		Boat* boat = _boatHandler->getBoat();
		float lat = boat->getLat();
		float longitude = boat->getLong();
		float heading = boat->getHeading();
		float velocity = boat->getVelocity();
		std::vector<glm::vec2> wayPoints = boat->getWayPoints();

		ImGui::Text("Time: %f hours", _boatHandler->getElapsedTime()); //displaying the elapsed time
		ImGui::Text("Boat cooridnates: ( %f, %f)", longitude, lat); //displaying the boat coordinates
		ImGui::Text("Boat heading: %f", heading); //displaying the boat heading
		ImGui::Text("Boat velocity: %f m/s", velocity); //displaying the boat velocity
		//should probably use an iterator but this will do
		ImGui::Text("Waypoints");
		for (int i = 0; i < wayPoints.size(); i++) {
			glm::vec2 point = wayPoints[i];
			ImGui::Text("\t( %f, %f)", point.x, point.y);
		}

		std::vector <std::string> log = boat->getLog();
		ImGui::Text("\nLog");
		for (int i = 0; i < log.size(); i++) {
			ImGui::Text(log[i].c_str());
		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	ImGui::Render();
	glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  debugWindow->swapWindow();
}


void Render()
{
	mainWindow->render();

	//shader.UseProgram();
	//glClearColor(0.5, 0.5, 0.5, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);

	glUniform1f(scaleId, scale);
	glUniform2f(translateId, translate[0], translate[1]);

	//map->render();

	_boatHandler->render(translateId, translate);
	 mainWindow->swapWindow();

	ImGuiDraw();
}


bool SetupBufferObjects()
{

	map->init();
	mainWindow->addRenderer(map->makeRenderer());
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
  mainWindow = new Window("main window", WINDOW_WIDTH, WINDOW_HEIGHT);
  debugWindow = new Window("debug window", DEBUG_WINDOW_WIDTH, DEBUG_WINDOW_HEIGHT);
  bool mW = mainWindow->setup();
  bool dW =debugWindow->setup();
  if(!mW || !dW){
    std::cout<<"failed to make windows"<<std::endl;
    return false;
  }
 
  //Make the main window the current context
	mainWindow->render();
	//IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(debugWindow->getWindow(), debugWindow->getContext());
	ImGui_ImplOpenGL3_Init("#version 130");
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

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

  delete mainWindow;
  delete debugWindow;

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
			ImGui_ImplSDL2_ProcessEvent(&event);
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
	//std::ofstream out("log.txt");
	//std::cout.rdbuf(out.rdbuf());
	std::cout << "made it here" << std::endl;
	map = new Map("../deps/shapelib/110m_physical/ne_110m_land");

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
	//waitForInput();
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
