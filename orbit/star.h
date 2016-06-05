#ifndef STAR_
#define STAR_
#include <vector>

class Galaxy;
class Universe;

class Star
{
	friend class Galaxy;
	friend class Universe;
private:
	std::vector<double> p = std::vector<double>(3); //position vector
	std::vector<double> last = std::vector<double>(3); //un-updated position vector (used for force calculations during update)
	std::vector<double> speed = std::vector<double>(3); //speed vector
	std::vector<double> displayL = std::vector<double>(3);
	std::vector<double> displayR = std::vector<double>(3);
	double mass; //star mass
	double *xOld, *yOld, *zOld; //circular buffer for trails
	int buffPlace; //keeps track of place in trail buffer
	int planeZ;
	HBRUSH brush; //color of star (inhereted from parent galaxy)

public:
	//default constructor
	Star();

	//constructor with given position and velocity
	Star(
		double xPos,
		double yPos,
		double zPos,
		double xSpeed,
		double ySpeed,
		double zSpeed,
		double mass,
		HBRUSH brush
		);

	//constructor with given galaxy location (most used constructor)
	Star(
		double galX,
		double galY,
		double galZ,
		HBRUSH brush
		);

	//udpates position and velocity of star with given position of other body
	void update(double otherX, double otherY, double otherZ, double otherMass);
	
	//udpates position and velocity of star with given other star
	void updateBoth(Star &other);

	//push "last" position vector into current position vector
	void updateLast()
	{
		last[0] = p[0];
		last[1] = p[1];
		last[2] = p[2];
	}

	//display star on screen
	void display(HDC hdc, HWND hWnd);

	double x() { return p[0]; };
	double y() { return p[1]; };
	double z() { return p[2]; };
	double m() { return mass; };
	void movePlane(int d) { planeZ += d; }

	//returns true if star is within render boundary, false otherwise
	friend bool shouldRender(Star s);
};
#endif