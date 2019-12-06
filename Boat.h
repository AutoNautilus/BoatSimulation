#ifndef BOAT_H
#define BOAT_H
#include <vector>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Boat {
public:
	Boat(float startCoords[2]);
	void update();
	void addWayPointLast(glm::vec2 wayPoint);
	void addWayPointFirst(glm::vec2 wayPoint);

	float getLong();
	float getLat();
	float getHeading();

	std::vector<glm::vec2> getWayPoints();

private:

	float calculateHeading(glm::vec2 wayPoint);
	bool reachedWayPoint(glm::vec2 wayPoint, float tolerance);

	void moveForward(float deltaTime); //time in some unit for the moment

	float _long;
	float _lat; 
	float _heading; //degrees CCW from east
	float _velocity; //velocity in m/s in the heading direction

	std::vector<glm::vec2> _wayPoints;
};

#endif
