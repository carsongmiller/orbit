#pragma once
#include <vector>

class Camera
{
private:
	/*
	Rotation matrices
		Rz = [  cos(pan),  -sin(pan), 0;
				sin(pan),   cos(pan), 0;
				0,          0,        1];

		Ry = [  cos(roll),   0,   sin(roll);
				0,           1,   0       ;
				-sin(roll),  0,   cos(roll)];

		Rx = [1,    0,          0         ;
			  0,    cos(tilt), -sin(tilt);
			  0,    sin(tilt),  cos(tilt)];
	*/
public:
	double translateMat[4][4];
	double rollMat[4][4];
	double tiltMat[4][4];
	double panMat[4][4];
	std::vector<double> p = std::vector<double>(3);
	double pan, tilt, roll;
	
	void updateMats();
	Camera();
};