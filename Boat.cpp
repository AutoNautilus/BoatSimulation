#include "Boat.h"

double degToRad(double deg) {
	return deg * 3.141592653 / 180.0;
}
double radToDeg(double rad) {
	return rad * 180.0 / 3.141592653;
}

Boat::Boat(float startCoords[2]) {
	_long = startCoords[0];
	_lat = startCoords[1];
	_heading = 60.0f;
	_velocity = 2.0f; // in m/s
}

void Boat::update(float deltaTime) { //deltaTime in hours
	if (_wayPoints.size() == 0) {
		return;
	}

	if (reachedWayPoint(_wayPoints.front(), 0.5f)) {
		auto it = _wayPoints.begin();
		_wayPoints.erase(it);
		if (_wayPoints.size() == 0) {
			return;
		}
	}

	glm::vec2 nextPoint = _wayPoints.front();

	
	_heading = calculateHeading(nextPoint);

	moveForward(deltaTime);
}

bool Boat::reachedWayPoint(glm::vec2 wayPoint, float tolerance) {
	float dx = wayPoint.x - _long;
	float dy = wayPoint.y - _lat;
	float distance = sqrt(dx * dx + dy * dy);
	if (distance < tolerance) {
		return true;
	}

	return false;
}

float Boat::calculateHeading(glm::vec2 wayPoint) {
	float phi1 = _lat * 3.14159 / 180;
	float phi2 = wayPoint.y * 3.14159 / 180;
	
	float lambda1 = _long * 3.14159 / 180;
	float lambda2 = wayPoint.x * 3.14159 / 180;

	float lambda12 = lambda2 - lambda1;

	float alpha = atan2(cos(phi2) * sin(lambda12), cos(phi1) * sin(phi2) - sin(phi1) * cos(phi2) * cos(lambda12));

	float heading = 90 - (alpha * 180 / 3.14159);

	return heading;
}

void Boat::moveForward(float deltaTime) {
	char msg[100];
	sprintf(msg, "heading is: %f", _heading);
	this->log(msg);
	double headingRads = degToRad(_heading);
	sprintf(msg, "heading in rads is: %f", headingRads);
	this->log(msg);
	
	float timeInSec = deltaTime * 60 * 60;
	
	float distanceMoved = timeInSec * _velocity; //in meters
	
	sprintf(msg, "distanced moved: %f", distanceMoved);
	this->log(msg);

	long double centralAngle = distanceMoved / EARTH_RADIUS; //in radians
	sprintf(msg, "centralAngle: %f", centralAngle);
	this->log(msg);
	double latOne = degToRad(_lat);
	double alpha = degToRad(90 - _heading);
	sprintf(msg, "alpha: %f", alpha);
	this->log(msg);
	double latTwo = asin(sin(latOne) * cos(centralAngle) + cos(latOne)*sin(centralAngle)*cos(alpha));
	double deltaLong = asin((sin(alpha) * sin(centralAngle)) / cos(latTwo));
	_lat = radToDeg(latTwo);
	_long += radToDeg(deltaLong);
}

void Boat::log(std::string msg) {
	_log.push_back(msg);
}

void Boat::addWayPointLast(glm::vec2 wayPoint) {
	_wayPoints.push_back(wayPoint);
}
void Boat::addWayPointFirst(glm::vec2 wayPoint) {
	auto it = _wayPoints.begin();
	it = _wayPoints.insert(it, wayPoint);
}

float Boat::getLong() { return _long; }
float Boat::getLat() { return _lat; }
float Boat::getHeading() { return _heading; }
float Boat::getVelocity() { return _velocity; }
std::vector<std::string> Boat::getLog() { return _log; }
std::vector<glm::vec2> Boat::getWayPoints() { return _wayPoints; }