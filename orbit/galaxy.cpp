#include "stdafx.h"
#include "galaxy.h"
#include "star.h"
#include <cmath>

Galaxy::Galaxy()
{
	p[0] = rand() % 400;
	p[1] = rand() % 400;
	p[2] = rand() % 400;
	speed[0] = ((double)rand() / RAND_MAX) + .25;
	speed[1] = ((double)rand() / RAND_MAX) + .25;
	speed[2] = ((double)rand() / RAND_MAX) + .25;
	mass = (rand() % 10000) + 5000;
	brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
	buffPlace = 0;
	planeZ = PLANE_Z;

	for (int i = 0; i < NUM_STAR; i++)
		star[i] = Star(p[0], p[1], p[2], brush);
}



Galaxy::Galaxy
(
	double xPos,
	double yPos,
	double zPos,
	double xSpeed,
	double ySpeed,
	double zSpeed,
	double mass
	)
{
	this->p[0] = xPos;
	this->p[1] = yPos;
	this->p[2] = zPos;
	this->speed[0] = xSpeed;
	this->speed[1] = ySpeed;
	this->speed[2] = zSpeed;
	this->mass = mass;
	brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
	this->buffPlace = 0;

	if (USE_3D)
	{
		this->planeZ = PLANE_Z;

		displayL[0] = this->p[0] - LEYE_X;
		displayL[1] = this->p[1] - LEYE_Y;
		displayL[2] = this->p[2] - LEYE_Z;
		displayR[0] = this->p[0] - REYE_X;
		displayR[1] = this->p[1] - REYE_Y;
		displayR[2] = this->p[2] - REYE_Z;

		double lt = PLANE_Z / displayL[2]; //works for display plane parallel to xy-plane
		double rt = PLANE_Z / displayR[2]; //works for display plane parallel to xy-plane

		displayL[0] *= lt;
		displayL[1] *= lt;
		displayL[2] *= lt;
		displayR[0] *= rt;
		displayR[1] *= rt;
		displayR[2] *= rt;
	}

	if (G_TRAILS)
	{
		xOld = new double[G_TRAIL_LENGTH];
		yOld = new double[G_TRAIL_LENGTH];
		zOld = new double[G_TRAIL_LENGTH];
	}

	for (int i = 0; i < NUM_STAR; i++)
		star[i] = Star(p[0], p[1], p[2], brush);
}



void Galaxy::update(double otherX, double otherY, double otherZ, double otherMass)
{
	double g, gx, gy, gz, r;
	r = sqrt(pow(p[0] - otherX, 2) + pow(p[1] - otherY, 2) + pow(p[2] - otherZ, 2));
	g = G_C * (mass * otherMass) / pow(r, 2);
	
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

	if (G_TRAILS && !USE_3D)
	{
		xOld[buffPlace] = p[0];
		yOld[buffPlace] = p[1];
		zOld[buffPlace] = p[2];
		if (buffPlace < G_TRAIL_LENGTH - 1) buffPlace++;
		else buffPlace = 0;
	}
}



//updates both the galaxy calling the function and the galaxy passed as an argument
//eliminates the need to calculate forces between two galaxies twice
void Galaxy::updateBoth(Galaxy other)
{
	double g, gx, gy, gz, r;
	r = sqrt(pow(last[0] - other.last[0], 2) + pow(last[1] - other.last[1], 2) + pow(last[2] - other.last[2], 2));
	g = G_C * (mass * other.mass) / pow(r, 2);

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
		double rt = PLANE_Z / displayR[2]; //works for display plane parallel to xy-plane

		displayL[0] *= lt;
		displayL[1] *= lt;
		displayL[2] *= lt;
		displayR[0] *= rt;
		displayR[1] *= rt;
		displayR[2] *= rt;

		lt = PLANE_Z / other.displayL[2]; //works for display plane parallel to xy-plane
		rt = PLANE_Z / other.displayR[2]; //works for display plane parallel to xy-plane

		other.displayL[0] *= lt;
		other.displayL[1] *= lt;
		other.displayL[2] *= lt;
		other.displayR[0] *= rt;
		other.displayR[1] *= rt;
		other.displayR[2] *= rt;
	}

	if (G_TRAILS && !USE_3D)
	{
		xOld[buffPlace] = p[0];
		yOld[buffPlace] = p[0];
		zOld[buffPlace] = p[0];
		if (buffPlace < G_TRAIL_LENGTH - 1) buffPlace++;
		else buffPlace = 0;

		other.xOld[buffPlace] = other.p[0];
		other.yOld[buffPlace] = other.p[0];
		other.zOld[buffPlace] = other.p[0];
		if (other.buffPlace < G_TRAIL_LENGTH - 1) other.buffPlace++;
		else other.buffPlace = 0;
	}
}



void Galaxy::updateSelf()
{
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

	if (G_TRAILS && !USE_3D)
	{
		xOld[buffPlace] = p[0];
		yOld[buffPlace] = p[1];
		zOld[buffPlace] = p[2];
		if (buffPlace < G_TRAIL_LENGTH - 1) buffPlace++;
		else buffPlace = 0;
	}
}



void Galaxy::display(HDC hdc, HWND hWnd)
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
				displayL[0] - GALAXY_SIZE*pow(S_GROWTH_FACTOR, p[2]),
				displayL[1] - GALAXY_SIZE*pow(S_GROWTH_FACTOR, p[2]),
				displayL[0] + GALAXY_SIZE*pow(S_GROWTH_FACTOR, p[2]),
				displayL[1] + GALAXY_SIZE*pow(S_GROWTH_FACTOR, p[2])
				);
			SelectObject(hdc, red);
			Ellipse(hdc,
				displayR[0] - GALAXY_SIZE*pow(S_GROWTH_FACTOR, p[2]),
				displayR[1] - GALAXY_SIZE*pow(S_GROWTH_FACTOR, p[2]),
				displayR[0] + GALAXY_SIZE*pow(S_GROWTH_FACTOR, p[2]),
				displayR[1] + GALAXY_SIZE*pow(S_GROWTH_FACTOR, p[2])
				);

			DeleteObject(red);
			DeleteObject(blue);
		}

		else if (USE_Z_SCALING)
		{
			SelectObject(hdc, brush);
			Ellipse(hdc,
				p[0] - GALAXY_SIZE*pow(S_GROWTH_FACTOR, p[2]),
				p[1] - GALAXY_SIZE*pow(S_GROWTH_FACTOR, p[2]),
				p[0] + GALAXY_SIZE*pow(S_GROWTH_FACTOR, p[2]),
				p[1] + GALAXY_SIZE*pow(S_GROWTH_FACTOR, p[2])
				);
		}
		else
		{
			SelectObject(hdc, brush);
			Ellipse(hdc, p[0] - STAR_SIZE, p[1] - STAR_SIZE, p[0] + STAR_SIZE, p[1] + STAR_SIZE);
		}

		if (G_TRAILS && !USE_3D)
		{
			HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdc, white);
			RECT rect;
			for (int i = 0; i < G_TRAIL_LENGTH; i++)
			{
				rect.left = xOld[i];
				rect.right = xOld[i] + 1;
				rect.top = yOld[i];
				rect.bottom = yOld[i] + 1;
				FillRect(hdc, &rect, white);
			}
			DeleteObject(white);
		}
		for (int i = 0; i < NUM_STAR; i++)
			star[i].display(hdc, hWnd);
	}
}



void Galaxy::updateEntireGal()
{
	for (int i = 0; i < NUM_STAR; i++)
	{
		star[i].update(p[0], p[1], p[2], mass); //update stars for galaxy center
		update(star[i].p[0], star[i].p[1], star[i].p[2], star[i].mass); //update galaxy center for all stars
		/*
		for (int j = 0; j < NUM_STAR; j++)
		{
			if (j != i)
				star[i].update(star[j].p[0], star[j].p[1], star[j].p[2], star[j].mass); //update each star for all other stars
		}
		*/
	}

	for (int i = 0; i < NUM_STAR; i++)
		star[i].updateLast();

	updateLast();
}