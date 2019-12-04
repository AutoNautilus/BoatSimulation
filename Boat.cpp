#include "Boat.h"


Boat::Boat(float startCoords[2]) {
	_long = startCoords[0];
	_lat = startCoords[1];
}

void Boat::update() {

}
void Boat::addWayPoint(float wayPoint[2]) {

}


float Boat::getLong() { return _long; }
float Boat::getLat() { return _lat; }
float Boat::getHeading() { return _heading; }