#include "stdafx.h"
#include "universe.h"
#include "galaxy.h"
#include "star.h" 
#include "camera.h"
#include "util.h"


Universe::Universe()
{
	for (int i = 0; i < NUM_GALAXY; i++)
	{
		if (i == 0)
			galaxy[i] = Galaxy(G1_XPOS, G1_YPOS, G1_ZPOS, G1_XSPEED, G1_YSPEED, G1_ZSPEED, GALAXY_MASS);
		else if (i == 1)
			galaxy[i] = Galaxy(G2_XPOS, G2_YPOS, G2_ZPOS, G2_XSPEED, G2_YSPEED, G2_ZSPEED, GALAXY_MASS);
		else if (i == 2)
			galaxy[i] = Galaxy(G3_XPOS, G3_YPOS, G3_ZPOS, G3_XSPEED, G3_YSPEED, G3_ZSPEED, GALAXY_MASS);
		else
			galaxy[i] = Galaxy();
	}
}



void Universe::update()
{
	/*
	The following interactions are accounted for:
	star->star
	star->galaxy
	galaxy->star
	galaxy->galaxy

	a = updating galaxy
	b = updating star
	c = interacting galaxy
	d = interacting star
	*/

	//update all star->star interactions
	if (STAR_STAR_INTER)
	{
		for (int a = 0; a < NUM_GALAXY; a++)
		{
			for (int b = 0; b < NUM_STAR; b++)
			{
				for (int x = b + 1; x < NUM_STAR; x++) //updating the remaining stars in galaxy "a"
					galaxy[a].star[b].updateBoth(galaxy[a].star[x]);

				for (int c = a+1; c < NUM_GALAXY; c++)
				{
					for (int d = 0; d < NUM_STAR; d++)
					{
						if (a != c || b != d)
							galaxy[a].star[b].updateBoth(galaxy[c].star[d]);
					}
				}
			}
		}
	}

	//update all star->galaxy interactions
	if (STAR_GAL_INTER)
	{
		for (int a = 0; a < NUM_GALAXY; a++)
		{
			for (int c = 0; c < NUM_GALAXY; c++)
			{
				for (int d = 0; d < NUM_STAR; d++)
				{
					galaxy[a].update
						(
							galaxy[c].star[d].last[0],
							galaxy[c].star[d].last[1],
							galaxy[c].star[d].last[2],
							galaxy[c].star[d].mass
						);
				}
			}
		}
	}

	//update all galaxy->star interactions
	if (GAL_STAR_INTER)
	{
		for (int a = 0; a < NUM_GALAXY; a++)
		{
			for (int b = 0; b < NUM_STAR; b++)
			{
				for (int c = 0; c < NUM_GALAXY; c++)
				{
					galaxy[a].star[b].update
						(
							galaxy[c].last[0],
							galaxy[c].last[1],
							galaxy[c].last[2],
							galaxy[c].mass
						);
				}
			}
		}
	}



	//updating all galaxy->galaxy interactions
	if(GAL_GAL_INTER)
	{
		for (int a = 0; a < NUM_GALAXY; a++)
		{
			for (int c = a+1; c < NUM_GALAXY; c++)
			{
				if (a != c)
				{
					galaxy[a].updateBoth(galaxy[c]);
				}
			}
		}
	}

	if (!GAL_GAL_INTER && !STAR_GAL_INTER)
	{
		for (int i = 0; i < NUM_GALAXY; i++)
			galaxy[i].updateSelf();
	}

	//set all "last" values to the new position values
	for (int i = 0; i < NUM_GALAXY; i++)
	{
		for (int j = 0; j < NUM_STAR; j++)
		{
			galaxy[i].star[j].updateLast();
		}
		galaxy[i].updateLast();
	}
}



void Universe::display(HDC hdc, HWND hWnd, Camera cam)
{	
	if (ENABLE_CAMERA)
	{
		cam.updateMats();
		double pTemp[4];
		double transform[4][4] = 
		{ 
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}
		};

		matrixMult4x4(transform, cam.rollMat, transform);
		matrixMult4x4(transform, cam.tiltMat, transform);
		matrixMult4x4(transform, cam.panMat, transform);
		matrixMult4x4(transform, cam.translateMat, transform);

		invertMatrix4x4(transform, transform);

		RECT *r = new RECT;
		GetClientRect(hWnd, r);
		double perspective[4][4] = 
		{
			{(1/r->right),		0,					0,					0},
			{0,					(1 / r->bottom),	0,					0},
			{0,					0,					-2/(-1000 - 1000),	-1*(-1000+1000)/(-1000-1000)},
			{0,					0,					0,					1}
		};

		/*
			{1/width,			0,					0,					0},
			{0,					1/height,			0,					0},
			{0,					0,					-2/(zFar - zNear),	-1*(zFar + zNear)/(zFar + zNear)},
			{0,					0,					0,					1}
		*/

		matrixMult4x4(perspective, transform, transform);
		
		double plot[4];
		for (int g = 0; g < NUM_GALAXY; g++)
		{
			SelectObject(hdc, galaxy[g].brush);
			plot[0] = galaxy[g].p.at(0);
			plot[1] = galaxy[g].p.at(1);
			plot[2] = galaxy[g].p.at(2);
			plot[3] = 1;

			matrixMult4x1(transform, plot, plot);

			plot[0] /= plot[3];
			plot[1] /= plot[3];
			plot[2] /= plot[3];
			plot[3] /= plot[3];

			double r =
				sqrt(
					pow(galaxy[g].p.at(0) - cam.p.at(0), 2) +
					pow(galaxy[g].p.at(1) - cam.p.at(1), 2) +
					pow(galaxy[g].p.at(2) - cam.p.at(2), 2)
					);
			if (plot[0] + GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]) - plot[0] - GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]) < 1000)
			{
				Ellipse(hdc,
					plot[0] - GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]),
					plot[1] - GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]),
					plot[0] + GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]),
					plot[1] + GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2])
					);
			}

			for (int s = 0; s < NUM_STAR; s++)
			{
				plot[0] = galaxy[g].star[s].p.at(0);
				plot[1] = galaxy[g].star[s].p.at(1);
				plot[2] = galaxy[g].star[s].p.at(2);
				plot[3] = 1;

				matrixMult4x1(transform, plot, plot);

				plot[0] /= plot[3];
				plot[1] /= plot[3];
				plot[2] /= plot[3];
				plot[3] /= plot[3];

				double r =
					sqrt(
						pow(galaxy[g].star[s].p.at(0) - cam.p.at(0), 2) +
						pow(galaxy[g].star[s].p.at(1) - cam.p.at(1), 2) +
						pow(galaxy[g].star[s].p.at(2) - cam.p.at(2), 2)
						);

				if (plot[0] + STAR_SIZE*pow(GROWTH_FACTOR, plot[2]) - plot[0] - STAR_SIZE*pow(GROWTH_FACTOR, plot[2]) < 1000)
				{
					Ellipse(hdc,
						plot[0] - STAR_SIZE*pow(GROWTH_FACTOR, plot[2]),
						plot[1] - STAR_SIZE*pow(GROWTH_FACTOR, plot[2]),
						plot[0] + STAR_SIZE*pow(GROWTH_FACTOR, plot[2]),
						plot[1] + STAR_SIZE*pow(GROWTH_FACTOR, plot[2])
						);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < NUM_GALAXY; i++)
			galaxy[i].display(hdc, hWnd);
	}
}