#ifndef BOAT_H
#define BOAT_H
#include <vector>

class Boat {
public:
	Boat(float startCoords[2]);
	void update();
	void addWayPoint(float wayPoint[2]);

	float getLong();
	float getLat();
	float getHeading();

private:
	float _long;
	float _lat; 
	float _heading;
	std::vector<float[2]> _wayPoints;
};

#endif
