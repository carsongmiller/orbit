#ifndef STAR_
#define STAR_

class Galaxy;
class Universe;

class Star
{
	friend class Galaxy;
	friend class Universe;
private:
	//double xPos, yPos, zPos;
	std::vector<double> p = std::vector<double>(3);
	//double xLast, yLast, zLast;
	std::vector<double> last = std::vector<double>(3);
	//double xSpeed, ySpeed, zSpeed;
	std::vector<double> speed = std::vector<double>(3);
	//double xDisplay_L, yDisplay_L, zDisplay_L; //coordinates for 3d display (left eye)
	std::vector<double> displayL = std::vector<double>(3);
	//double xDisplay_R, yDisplay_R, zDisplay_R; //coordinates for 3d display (right eye)
	std::vector<double> displayR = std::vector<double>(3);
	double mass;
	//double xOld[S_TRAIL_LENGTH], yOld[S_TRAIL_LENGTH], zOld[S_TRAIL_LENGTH];
	double *xOld, *yOld, *zOld;
	int buffPlace;
	int planeZ;
	HBRUSH brush;

public:
	Star();
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
	Star(
		double galX,
		double galY,
		double galZ,
		HBRUSH brush
		);

	void update(double otherX, double otherY, double otherZ, double otherMass);
	void updateBoth(Star other);
	void updateLast()
	{
		last[0] = p[0];
		last[1] = p[1];
		last[2] = p[2];
	}
	void display(HDC hdc, HWND hWnd);

	double x() { return p[0]; };
	double y() { return p[1]; };
	double z() { return p[2]; };
	double m() { return mass; };
	void movePlane(int d) { planeZ += d; }
};
#endif