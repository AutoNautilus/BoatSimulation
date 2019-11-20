BoatSimulatorMake: main.cpp
	g++ -o boatSimulator main.cpp  -lGL -lGLEW -lSDL2 -lSDL2main `pkg-config --cflags --libs shapelib`
