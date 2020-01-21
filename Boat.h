#ifndef BOAT_H
#define BOAT_H
#include <vector>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Boat;

#include "BoatHandler.h"

class Boat {
public:
	Boat(float startCoords[2]);
	void update(float deltaTime); //time in hours
	void addWayPointLast(glm::vec2 wayPoint);
	void addWayPointFirst(glm::vec2 wayPoint);

	float getLong();
	float getLat();
	float getHeading();
	float getVelocity();
	std::vector<std::string> getLog();
	std::vector<glm::vec2> getWayPoints();

private:
	std::vector<std::string> _log;
	float calculateHeading(glm::vec2 wayPoint);
	bool reachedWayPoint(glm::vec2 wayPoint, float tolerance);

	void moveForward(float deltaTime); 
	void log(std::string);
	float _long;
	float _lat; 
	float _heading; //degrees CCW from east
	float _velocity; //velocity in m/s in the heading direction

	float _batteryCapacity = 5000; //5kWh battery
	float _batterySOC = 1.0; //0 is empty 1.0 is full

	const float EARTH_RADIUS = 6371000; //average earth radius in meters
	std::vector<glm::vec2> _wayPoints;
};

#endif
