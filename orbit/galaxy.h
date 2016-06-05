#ifndef GALAXY_
#define GALAXY_
#include "star.h"

class Galaxy
{
	friend class Universe;
private:
	std::vector<double> p = std::vector<double>(3); //position vector
	std::vector<double> last = std::vector<double>(3); //un-updated position vector (used for force calculations)
	std::vector<double> displayL = std::vector<double>(3);
	std::vector<double> displayR = std::vector<double>(3);
	std::vector<double> speed = std::vector<double>(3); //speed vector
	double *xOld, *yOld, *zOld; //circular buffer for trails
	int buffPlace; //keeps track of place in trail buffer
	double mass; //mass of galaxy
	int planeZ;
	Star star[NUM_STAR]; //stars contained within galaxy
	HBRUSH brush; //color of galaxy (passed on to stars)
public:
	//initializes galaxy randomly
	Galaxy();

	//initializes galaxy with given position, velocity, and mass
	Galaxy
		(
			double xPos,
			double yPos,
			double zPos,
			double xSpeed,
			double ySpeed,
			double zSpeed,
			double mass
			);

	//updates galaxy with given other body position and mass
	void update(double otherX, double otherY, double otherZ, double otherMass);

	//updates a galaxy with no other bodies (just changes position based on velocity)
	void updateSelf();

	//updates both calling galaxy and other galaxy
	void updateBoth(Galaxy &other);

	//pushes all "last" position vectors into current position vectors
	void updateLast()
	{
		last[0] = p[0];
		last[1] = p[1];
		last[2] = p[2];
	}

	//displays galaxy on screen
	void display(HDC hdc, HWND hWnd);

	double starX(int s) { return star[s].p[0]; }
	double starY(int s) { return star[s].p[1]; }
	double starZ(int s) { return star[s].p[2]; }
	double starM(int s) { return star[s].mass; }

	//returns star at given index
	Star getStar(int index) { return star[index]; }

	void movePlane(int d) { planeZ += d; }

	void updateEntireGal();

	//returns true if galaxy is within render boundary
	friend bool shouldRender(Galaxy g);
};
#endif