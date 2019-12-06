#include "Boat.h"


Boat::Boat(float startCoords[2]) {
	_long = startCoords[0];
	_lat = startCoords[1];
	_heading = 60.0f;
	_velocity = 1.0f;
}

void Boat::update() {
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

	moveForward(0.1f);
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
	_long += deltaTime * (_velocity * glm::cos(glm::radians(_heading)));
	_lat += deltaTime * (_velocity * glm::sin(glm::radians(_heading)));
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

std::vector<glm::vec2> Boat::getWayPoints() { return _wayPoints; }