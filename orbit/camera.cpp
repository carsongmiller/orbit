#include "stdafx.h"
#include "camera.h"
#include "util.h"


Camera::Camera()
{
	p[0] = CAMERA_X;
	p[1] = CAMERA_Y;
	p[2] = CAMERA_Z;
	pan = 0;
	tilt = 0;
	roll = 0;

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
			if (r == c)
			{
				rollMat[r][c] = 1;
				panMat[r][c] = 1;
				tiltMat[r][c] = 1;
				translateMat[r][c] = 1;
			}
			else
			{
				rollMat[r][c] = 0;
				panMat[r][c] = 0;
				tiltMat[r][c] = 0;
				translateMat[r][c] = 0;
			}
	}

	updateMats();
}



void Camera::updateMats()
{
	rollMat[0][0] = cos(deg_to_rad(roll));
	rollMat[0][1] = -sin(deg_to_rad(roll));
	rollMat[1][0] = sin(deg_to_rad(roll));
	rollMat[1][1] = cos(deg_to_rad(roll));

	panMat[0][0] = cos(deg_to_rad(pan));
	panMat[0][2] = sin(deg_to_rad(pan));
	panMat[2][0] = -sin(deg_to_rad(pan));
	panMat[2][2] = cos(deg_to_rad(pan));

	tiltMat[1][1] = cos(deg_to_rad(tilt));
	tiltMat[1][2] = -sin(deg_to_rad(tilt));
	tiltMat[2][1] = sin(deg_to_rad(tilt));
	tiltMat[2][2] = cos(deg_to_rad(tilt));

	translateMat[0][3] = p[0];
	translateMat[1][3] = p[1];
	translateMat[2][3] = p[2];
}