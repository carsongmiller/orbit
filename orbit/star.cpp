#include "stdafx.h"
#include "star.h"
#include <cmath>
#include "util.h"


Star::Star()
{
	p[0] = randBound(100, 150, true);
	p[1] = randBound(100, 150, true);
	p[2] = randBound(100, 150, true);
	speed[0] = ((double)rand() / RAND_MAX) + 1;
	speed[1] = ((double)rand() / RAND_MAX) + 1;
	speed[2] = ((double)rand() / RAND_MAX) + 1;
	mass = ((double)rand() / RAND_MAX) + 1;
	planeZ = PLANE_Z;
	if (S_TRAILS)
	{
		xOld = new double[S_TRAIL_LENGTH];
		yOld = new double[S_TRAIL_LENGTH];
		zOld = new double[S_TRAIL_LENGTH];

		for (int i = 0; i < S_TRAIL_LENGTH; i++)
		{
			xOld[i] = p[0];
			yOld[i] = p[1];
			zOld[i] = p[2];
		}

		for (int i = 0; i < S_TRAIL_LENGTH; i++)
		{
			xOld[i] = p[0] + STAR_SIZE / 2;
			yOld[i] = p[1] + STAR_SIZE / 2;
			xOld[i] = p[0] + STAR_SIZE / 2;
		}
		buffPlace = 0;
	}
	updateLast();
}



Star::Star(
	double xPos,
	double yPos,
	double zPos,
	double xSpeed,
	double ySpeed,
	double zSpeed,
	double mass,
	HBRUSH brush
	)
{
	this->p[0] = xPos;
	this->p[1] = yPos;
	this->p[2] = xPos;
	this->speed[0] = xSpeed;
	this->speed[1] = ySpeed;
	this->speed[2] = zSpeed;
	this->mass = mass;
	this->brush = brush;
	this->planeZ = PLANE_Z;

	if (S_TRAILS)
	{
		xOld = new double[S_TRAIL_LENGTH];
		yOld = new double[S_TRAIL_LENGTH];
		zOld = new double[S_TRAIL_LENGTH];

		for (int i = 0; i < S_TRAIL_LENGTH; i++)
		{
			xOld[i] = p[0];
			yOld[i] = p[1];
			zOld[i] = p[2];

		}

		for (int i = 0; i < S_TRAIL_LENGTH; i++)
		{
			xOld[i] = p[0] + STAR_SIZE / 2;
			yOld[i] = p[1] + STAR_SIZE / 2;
			xOld[i] = p[0] + STAR_SIZE / 2;
		}
	}
	buffPlace = 0;
	updateLast();
}



Star::Star(
	double galX,
	double galY,
	double galZ,
	HBRUSH brush
	)
{
	/*
		p[0] = randBound(0, STAR_ORBIT_R, true);
		p[1] = sqrt(pow(STAR_ORBIT_R, 2) - pow(p[0], 2)) * pow(-1, rand());
		p[2] = sqrt(pow(STAR_ORBIT_R, 2) - pow(p[0], 2) - pow(p[1], 2)) * pow(-1, rand()) + randBound(0, STAR_ORBIT_TOL, true);
		p[0] += galX;
		p[1] += galY;
		p[2] += galZ;
	*/

	/*
		p[0] = galX + (rand() % STAR_ORBIT_TOL + STAR_ORBIT_R) * pow(-1, rand());
		p[1] = galY + (rand() % STAR_ORBIT_TOL + STAR_ORBIT_R) * pow(-1, rand());
		p[2] = galZ + (rand() % STAR_ORBIT_TOL + STAR_ORBIT_R) * pow(-1, rand());
	*/

		double theta = deg_to_rad(rand() % 180);
		double phi = deg_to_rad(rand() % 360);
		double r = STAR_ORBIT_R + randBound(0, STAR_ORBIT_TOL, true);

		p[0] = galX + r*sin(theta)*cos(phi);
		p[1] = galY + r*sin(theta)*sin(phi);
		p[2] = galZ + r*cos(theta);

		
	
	if (CIRC_ORBIT)
	{
		/*std::vector<double> p(3);
		p.at(0) = p[0] - galX;
		p.at(1) = p[1] - galY;
		p.at(2) = p[2] - galZ;


		speed[0] = 1;
		speed[1] = 1;
		speed[2] = ((2*p[0]*p[0]) + (2*p[1]*p[1]) + (2*p[2]*p[2]) - 2*p[0] - 2*p[1]) / (2*p[2]);
		double mag = sqrt(2 + pow(speed[2], 2));
		speed[0] /= mag;
		speed[1] /= mag;
		speed[2] /= mag;
		mag = sqrt(G_C * GALAXY_MASS);
		speed[0] *= mag;
		speed[1] *= mag;
		speed[2] *= mag;*/

		speed[1] = randBound(.75, 1, false);
		speed[2] = randBound(.75, 1, false);
		speed[0] = (((p[1] - galY) * speed[1]) + ((p[2] - galZ)*speed[2])) / (galX - p[0]);
	}
	
	else
	{
		speed[0] = (((double)rand() / RAND_MAX) + 1) * pow(-1, rand());
		speed[1] = (((double)rand() / RAND_MAX) + 1) * pow(-1, rand());
		speed[2] = (((double)rand() / RAND_MAX) + 1) * pow(-1, rand());
	}
	

	mass = STAR_MASS;
	planeZ = PLANE_Z;
	this->brush = brush;
	if (S_TRAILS)
	{
		xOld = new double[S_TRAIL_LENGTH];
		yOld = new double[S_TRAIL_LENGTH];
		zOld = new double[S_TRAIL_LENGTH];
		for (int i = 0; i < S_TRAIL_LENGTH; i++)
		{
			xOld[i] = p[0];
			yOld[i] = p[1];
			xOld[i] = p[0];
		}
	}

	buffPlace = 0;
	updateLast();
}


void Star::update(double otherX, double otherY, double otherZ, double otherMass)
{
	double g, gx, gy, gz, r;
	r = sqrt(pow(p[0] - otherX, 2) + pow(p[1] - otherY, 2) + pow(p[2] - otherZ, 2));
	g = (mass * otherMass) / pow(r, 2);
	gx = g * (otherX - p[0]) / r;
	gy = g * (otherY - p[1]) / r;
	gz = g * (otherZ - p[2]) / r;

	speed[0] += (gx / mass)*TIME_INT;
	speed[1] += (gy / mass)*TIME_INT;
	speed[2] += (gz / mass)*TIME_INT;

	p[0] += speed[0];
	p[1] += speed[1];
	p[2] += speed[2];

	if (USE_3D)
	{
		displayL[0] = p[0] - LEYE_X;
		displayL[1] = p[1] - LEYE_Y;
		displayL[2] = p[2] - LEYE_Z;
		displayR[0] = p[0] - REYE_X;
		displayR[1] = p[1] - REYE_Y;
		displayR[2] = p[2] - REYE_Z;

		double lt = PLANE_Z / displayL[2]; //works for display plane parallel to xy-plane
		double rt = PLANE_Z / displayR[2]; //works for display plane parallel to xy-plane

		displayL[0] *= lt;
		displayL[1] *= lt;
		displayL[2] *= lt;
		displayR[0] *= rt;
		displayR[1] *= rt;
		displayR[2] *= rt;
	}

	if (S_TRAILS && !USE_3D)
	{
		xOld[buffPlace] = p[0];
		yOld[buffPlace] = p[1];
		zOld[buffPlace] = p[2];
		if (buffPlace < S_TRAIL_LENGTH - 1) buffPlace++;
		else buffPlace = 0;
	}
}



void Star::updateBoth(Star &other)
{
	double g, gx, gy, gz, r;
	r = sqrt(pow(last[0] - other.last[0], 2) + pow(last[1] - other.last[1], 2) + pow(last[2] - other.last[2], 2));
	g = (mass * other.mass) / pow(r, 2);
	gx = g * (other.last[0] - last[0]) / r;
	gy = g * (other.last[1] - last[1]) / r;
	gz = g * (other.last[2] - last[2]) / r;

	speed[0] += (gx / mass)*TIME_INT;
	speed[1] += (gy / mass)*TIME_INT;
	speed[2] += (gz / mass)*TIME_INT;

	other.speed[0] += (gx / other.mass)*TIME_INT;
	other.speed[1] += (gy / other.mass)*TIME_INT;
	other.speed[2] += (gz / other.mass)*TIME_INT;

	p[0] += speed[0];
	p[1] += speed[1];
	p[2] += speed[2];

	other.p[0] += other.speed[0];
	other.p[1] += other.speed[1];
	other.p[2] += other.speed[2];

	if (USE_3D)
	{
		displayL[0] = p[0] - LEYE_X;
		displayL[1] = p[1] - LEYE_Y;
		displayL[2] = p[2] - LEYE_Z;
		displayR[0] = p[0] - REYE_X;
		displayR[1] = p[1] - REYE_Y;
		displayR[2] = p[2] - REYE_Z;

		other.displayL[0] = other.p[0] - LEYE_X;
		other.displayL[1] = other.p[1] - LEYE_Y;
		other.displayL[2] = other.p[2] - LEYE_Z;
		other.displayR[0] = other.p[0] - REYE_X;
		other.displayR[1] = other.p[1] - REYE_Y;
		other.displayR[2] = other.p[2] - REYE_Z;

		double lt = PLANE_Z / displayL[2]; //works for display plane parallel to xy-plane
		double rt = PLANE_Z / displayL[2]; //works for display plane parallel to xy-plane

		displayL[0] *= lt;
		displayL[1] *= lt;
		displayL[2] *= lt;
		displayR[0] *= rt;
		displayR[1] *= rt;
		displayR[2] *= rt;

		lt = PLANE_Z / other.displayL[2]; //works for display plane parallel to xy-plane
		rt = PLANE_Z / other.displayL[2]; //works for display plane parallel to xy-plane

		other.displayL[0] *= lt;
		other.displayL[1] *= lt;
		other.displayL[2] *= lt;
		other.displayR[0] *= rt;
		other.displayR[1] *= rt;
		other.displayR[2] *= rt;
	}

	if (S_TRAILS && !USE_3D)
	{
		xOld[buffPlace] = p[0];
		yOld[buffPlace] = p[1];
		zOld[buffPlace] = p[2];
		if (buffPlace < S_TRAIL_LENGTH - 1) buffPlace++;
		else buffPlace = 0;

		other.xOld[buffPlace] = other.p[0];
		other.yOld[buffPlace] = other.p[1];
		other.zOld[buffPlace] = other.p[2];
		if (other.buffPlace < S_TRAIL_LENGTH - 1) other.buffPlace++;
		else other.buffPlace = 0;
	}
}



void Star::display(HDC hdc, HWND hWnd)
{
	if(p[2] < Z_RENDER_BOUNDARY)
	{
		if (USE_3D)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);
			double XC = rc.right / 2;
			double YC = rc.bottom / 2;
			HBRUSH red;
			HBRUSH blue;
			SetBkMode(hdc, TRANSPARENT);
			if (HATCH_3D)
			{
				red = CreateHatchBrush(HS_BDIAGONAL, RGB(255, 0, 0));
				blue = CreateHatchBrush(HS_FDIAGONAL, RGB(0, 0, 255));
			}
			else
			{
				red = CreateSolidBrush(RGB(255, 0, 0));
				blue = CreateSolidBrush(RGB(0, 0, 255));
			}

			SelectObject(hdc, blue);
			Ellipse(hdc,
				displayL[0] - STAR_SIZE*pow(GROWTH_FACTOR, p[2] - planeZ + PLANE_Z),
				displayL[1] - STAR_SIZE*pow(GROWTH_FACTOR, p[2] - planeZ + PLANE_Z),
				displayL[0] + STAR_SIZE*pow(GROWTH_FACTOR, p[2] - planeZ + PLANE_Z),
				displayL[1] + STAR_SIZE*pow(GROWTH_FACTOR, p[2] - planeZ + PLANE_Z)
				);
			SelectObject(hdc, red);
			Ellipse(hdc,
				displayR[0] - STAR_SIZE*pow(GROWTH_FACTOR, p[2] - planeZ + PLANE_Z),
				displayR[1] - STAR_SIZE*pow(GROWTH_FACTOR, p[2] - planeZ + PLANE_Z),
				displayR[0] + STAR_SIZE*pow(GROWTH_FACTOR, p[2] - planeZ + PLANE_Z),
				displayR[1] + STAR_SIZE*pow(GROWTH_FACTOR, p[2] - planeZ + PLANE_Z)
				);

			DeleteObject(red);
			DeleteObject(blue);
		}
		else if (USE_Z_SCALING)
		{
			SelectObject(hdc, brush);
			Ellipse(hdc,
				p[0] - STAR_SIZE*pow(GROWTH_FACTOR, p[2]),
				p[1] - STAR_SIZE*pow(GROWTH_FACTOR, p[2]),
				p[0] + STAR_SIZE*pow(GROWTH_FACTOR, p[2]),
				p[1] + STAR_SIZE*pow(GROWTH_FACTOR, p[2])
				);
		}
		else
		{
			SelectObject(hdc, brush);
			Ellipse(hdc, p[0] - STAR_SIZE, p[1] - STAR_SIZE, p[0] + STAR_SIZE, p[1] + STAR_SIZE);
		}
		if (S_TRAILS && !USE_3D)
		{
			HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, white);
			RECT rect;
			for (int i = 0; i < S_TRAIL_LENGTH; i++)
			{
				rect.left = xOld[i];
				rect.right = xOld[i] + 1;
				rect.top = yOld[i];
				rect.bottom = yOld[i] + 1;
				FillRect(hdc, &rect, white);
			}
			DeleteObject(white);
		}
	}
}