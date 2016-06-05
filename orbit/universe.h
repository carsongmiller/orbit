#ifndef UNIVERSE_
#define UNIVERSE_

#include "galaxy.h"
#include "star.h"

class Camera;

class Universe
{
private:
	Galaxy *galaxy; //main galaxy array
	Galaxy *testGal; //galaxies used for testing the camera projection
	double speed = 10; //delay (ms) between display updates
public:
	//Universe constructor
	Universe();

	//umbrella function, updates entire universe	
	void update();

	//umbrella function for displaying bodies on screen
	void display(HDC hdc, HWND hWnd, Camera cam);

	//increase delay between display updates
	void speedUp() { speed += SPEED_INC; }

	//decrease delay between display updates
	void speedDown() { speed -= SPEED_INC; }

	//returns the specified galaxy
	Galaxy getGal(int index) { return galaxy[index]; }

	//returns the current delay between display updates
	double getSpeed() { return speed; }

	//displays bodies from camera viewpoint
	void cameraView(HDC hdc, HWND hWnd, Camera cam);

	//displays a cube for testing projection
	void testProjection(HDC hdc, HWND hWnd, Camera cam);

	//transforms plot[] to a display vector
	void projectToCamera(Galaxy g, Camera cam, double t[4][4], double plot[4]);

	//transforms plot[] to a display vector
	void projectToCamera(Star s, Camera cam, double t[4][4], double plot[4]);

	//builds the transformation matrix for camera projection
	void buildTransformationMatrix(HWND hWnd, Camera cam, double t[4][4]);
};
#endif