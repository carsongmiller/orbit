#ifndef GALAXY_
#define GALAXY_
#include "star.h"

class Galaxy
{
	friend class Universe;
private:
	//double xPos, yPos, zPos;
	std::vector<double> p = std::vector<double>(3);
	//double xLast, yLast, zLast;
	std::vector<double> last = std::vector<double>(3);
	//double xDisplay_L, yDisplay_L, zDisplay_L; //coordinates for 3d display (left eye)
	std::vector<double> displayL = std::vector<double>(3);
	//double xDisplay_R, yDisplay_R, zDisplay_R; //coordinates for 3d display (right eye)
	std::vector<double> displayR = std::vector<double>(3);
	//double xSpeed, ySpeed, zSpeed;
	std::vector<double> speed = std::vector<double>(3);
	//double xOld[G_TRAIL_LENGTH], yOld[G_TRAIL_LENGTH], zOld[G_TRAIL_LENGTH];
	double *xOld, *yOld, *zOld;
	int buffPlace;
	double mass;
	int planeZ;
	Star star[NUM_STAR];
	HBRUSH brush;
public:
	Galaxy();
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
	void update(double otherX, double otherY, double otherZ, double otherMass);
	void updateSelf();
	void updateBoth(Galaxy &other);
	void updateLast()
	{
		last[0] = p[0];
		last[1] = p[1];
		last[2] = p[2];
	}
	void display(HDC hdc, HWND hWnd);

	double starX(int s) { return star[s].p[0]; }
	double starY(int s) { return star[s].p[1]; }
	double starZ(int s) { return star[s].p[2]; }
	double starM(int s) { return star[s].mass; }
	Star getStar(int index) { return star[index]; }
	void movePlane(int d) { planeZ += d; }

	void updateEntireGal();
};
#endif