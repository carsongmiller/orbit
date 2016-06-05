#include "stdafx.h"
#include "universe.h"
#include "galaxy.h"
#include "star.h" 
#include "camera.h"
#include "util.h"


Universe::Universe()
{
	if (TEST_PROJECTION)
	{
		testGal = new Galaxy[8];
		testGal[0] = Galaxy(100, 100, 100, 0, 0, 0, GALAXY_MASS);
		testGal[1] = Galaxy(-100, 100, 100, 0, 0, 0, GALAXY_MASS);
		testGal[2] = Galaxy(-100, -100, 100, 0, 0, 0, GALAXY_MASS);
		testGal[3] = Galaxy(100, -100, 100, 0, 0, 0, GALAXY_MASS);
		testGal[0] = Galaxy(100, 100, -100, 0, 0, 0, GALAXY_MASS);
		testGal[1] = Galaxy(-100, 100, -100, 0, 0, 0, GALAXY_MASS);
		testGal[2] = Galaxy(-100, -100, -100, 0, 0, 0, GALAXY_MASS);
		testGal[3] = Galaxy(100, -100, -100, 0, 0, 0, GALAXY_MASS);
	}
	else
	{
		galaxy = new Galaxy[NUM_GALAXY];
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
}



void Universe::update()
{
	if(!TEST_PROJECTION)
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

					for (int c = a + 1; c < NUM_GALAXY; c++)
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
		if (GAL_GAL_INTER)
		{
			for (int a = 0; a < NUM_GALAXY; a++)
			{
				for (int c = a + 1; c < NUM_GALAXY; c++)
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

		//push all "last" position vectors into current position vectors
		for (int i = 0; i < NUM_GALAXY; i++)
		{
			for (int j = 0; j < NUM_STAR; j++)
			{
				galaxy[i].star[j].updateLast();
			}
			galaxy[i].updateLast();
		}
	}
}



void Universe::display(HDC hdc, HWND hWnd, Camera cam)
{	
	if (TEST_PROJECTION)
		testProjection(hdc, hWnd, cam);
	else if (ENABLE_CAMERA)
		cameraView(hdc, hWnd, cam);
	else
	{
		for (int i = 0; i < NUM_GALAXY; i++)
			galaxy[i].display(hdc, hWnd);
	}
}



void Universe::cameraView(HDC hdc, HWND hWnd, Camera cam)
{
	double transform[4][4] =
	{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};

	buildTransformationMatrix(hWnd, cam, transform);
	double plot[4];

	for (int g = 0; g < NUM_GALAXY; g++)
	{
		SelectObject(hdc, galaxy[g].brush);
		projectToCamera(galaxy[g], cam, transform, plot);

		if (shouldRender(galaxy[g]))
		{
			Ellipse
				(
					hdc,
					plot[0] - GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]),
					plot[1] - GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]),
					plot[0] + GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]),
					plot[1] + GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2])
				);
		}

		for (int s = 0; s < NUM_STAR; s++)
		{
			projectToCamera(galaxy[g].star[s], cam, transform, plot);

			if (shouldRender(galaxy[g].star[s]))
			{
				Ellipse
					(
						hdc,
						plot[0] - STAR_SIZE*pow(GROWTH_FACTOR, plot[2]),
						plot[1] - STAR_SIZE*pow(GROWTH_FACTOR, plot[2]),
						plot[0] + STAR_SIZE*pow(GROWTH_FACTOR, plot[2]),
						plot[1] + STAR_SIZE*pow(GROWTH_FACTOR, plot[2])
					);
			}
		}
	}
}



void Universe::testProjection(HDC hdc, HWND hWnd, Camera cam)
{
	double plot[4];
	double t[4][4] =
	{
		{ 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 }
	};

	buildTransformationMatrix(hWnd, cam, t);
	
	for (int g = 0; g < 8; g++)
	{
		SelectObject(hdc, testGal[g].brush);
		projectToCamera(testGal[g], cam, t, plot);

		if (shouldRender(testGal[g]))
		{
			Ellipse
				(
					hdc,
					plot[0] - GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]),
					plot[1] - GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]),
					plot[0] + GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]),
					plot[1] + GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2])
				);
		}
	}

	Ellipse
		(
			hdc,
			plot[0] - GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]),
			plot[1] - GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]),
			plot[0] + GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2]),
			plot[1] + GALAXY_SIZE*pow(GROWTH_FACTOR, plot[2])
		);
}



void Universe::projectToCamera(Galaxy g, Camera cam, double t[4][4], double plot[4])
{
	plot[0] = g.p.at(0);
	plot[1] = g.p.at(1);
	plot[2] = g.p.at(2);
	plot[3] = 1;

	matrixMult4x1(t, plot, plot);

	plot[0] /= plot[3];
	plot[1] /= plot[3];
	plot[2] /= plot[3];
	plot[3] /= plot[3];
}



void Universe::projectToCamera(Star s, Camera cam, double t[4][4], double plot[4])
{
	plot[0] = s.p.at(0);
	plot[1] = s.p.at(1);
	plot[2] = s.p.at(2);
	plot[3] = 1;

	matrixMult4x1(t, plot, plot);

	plot[0] /= plot[3];
	plot[1] /= plot[3];
	plot[2] /= plot[3];
	plot[3] /= plot[3];
}



void Universe::buildTransformationMatrix(HWND hWnd, Camera cam, double t[4][4])
{
	cam.updateMats();
	double pTemp[4];

	matrixMult4x4(t, cam.rollMat, t);
	matrixMult4x4(t, cam.tiltMat, t);
	matrixMult4x4(t, cam.panMat, t);
	matrixMult4x4(t, cam.translateMat, t);

	invertMatrix4x4(t, t);

	RECT *r = new RECT;
	GetClientRect(hWnd, r);

	if (PROJECTION_TYPE == 1) //orthographic projection
	{
		double projection[4][4] =
		{
			{ (1 / r->right),		0,					0,					0 },
			{ 0,					(1 / r->bottom),	0,					0 },
			{ 0,					0,					-2 / (Z_FAR - Z_NEAR),	-1 * (Z_FAR + Z_NEAR) / (Z_FAR - Z_NEAR) },
			{ 0,					0,					0,					1 }
		};

		/*
		{1/width,			0,					0,					0},
		{0,					1/height,			0,					0},
		{0,					0,					-2/(zFar - zNear),	-1*(zFar + zNear)/(zFar + zNear)},
		{0,					0,					0,					1}
		*/

		matrixMult4x4(projection, t, t);
	}
	else if (PROJECTION_TYPE == 2) //perspective projection
	{
		double projection[4][4] =
		{
			{ atan(deg_to_rad(FOV_X) / 2),	0,								0,											0 },
			{ 0,							atan(deg_to_rad(FOV_Y) / 2),	0,											0 },
			{ 0,							0,								-1 * (Z_FAR + Z_NEAR) / (Z_FAR - Z_NEAR),	(-2 * Z_NEAR * Z_FAR) / (Z_FAR - Z_NEAR) },
			{ 0,							0,								-1,											0 }
		};

		/*
		{1/width,			0,					0,					0},
		{0,					1/height,			0,					0},
		{0,					0,					-2/(zFar - zNear),	-1*(zFar + zNear)/(zFar + zNear)},
		{0,					0,					0,					1}
		*/

		matrixMult4x4(projection, t, t);
	}
}