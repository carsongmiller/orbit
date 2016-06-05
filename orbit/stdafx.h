// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

/*
------------------------------
Display Variables
------------------------------
*/

#define TIME_INT				.0001			//time interval over which to update object velocities
#define G_TRAILS				false			//use galaxy trails
#define S_TRAILS				false			//use star trails
#define G_TRAIL_LENGTH			1000			//length of galaxy trails
#define S_TRAIL_LENGTH			1000			//length of star trails
#define GALAXY_SIZE				20				//radius of galaxy center
#define STAR_SIZE				5				//radius of stars
#define GROWTH_FACTOR			1.006			//scaling of z coordinate to star radius
#define USE_3D					false			//use 3D projection
#define HATCH_3D				true			//use cross hatching with 3D display
#define USE_Z_SCALING			true			//whether to change radius of star based on z-position
#define	SPEED					0				//how long to wait between updates of display
#define SPEED_INC				1				//how much to increment speed by when button presses (left/right arrow keys)
#define Z_RENDER_BOUNDARY		600				//point along z-axis along which objects are no longer rendered

/*
------------------------------
Camera Variables
------------------------------
*/

#define TEST_PROJECTION			false			//plots a cube to test projection and camera movement
#define ENABLE_CAMERA			false			//enable the moveable camera view
#define PAN_INC					1				//value to change position by when camera is moved
#define TILT_INC				.25
#define ROLL_INC				5
#define TRANSLATE_INC			50
#define PROJECTION_TYPE			1				//1 = orthographic, 2 = perspective
#define FOV_X					60
#define FOV_Y					60
#define Z_NEAR					0
#define Z_FAR					-1
#define RENDER_BOUNDARY			1500

#define CAMERA_X				0
#define CAMERA_Y				0
#define CAMERA_Z				500

/*
------------------------------
Initialization Variables
------------------------------
*/

#define NUM_GALAXY				2				//number of galaxies per star
#define NUM_STAR				10				//number of stars per galaxy
#define GALAXY_MASS				10000000		//mass of galaxy center
#define STAR_MASS				50				//mass of star (larger -> more distant orbit b/c more inertia)
#define G_C						2				//gravitational constant (not working currently)
#define STAR_ORBIT_R			250				//star initial position radius from galaxy center
#define STAR_ORBIT_TOL			20				//tolerance of star initial position
#define CIRC_ORBIT				true			//generate stars with more circular orbits

/*
------------------------------
Gravitaional Interactions
------------------------------
*/

#define STAR_STAR_INTER			false			//include star->star interactions
#define GAL_GAL_INTER			false			//include galaxy->galaxy interactions
#define GAL_STAR_INTER			true			//include galaxy->star interactions
#define STAR_GAL_INTER			false			//include star->galaxy interactions

/*
------------------------------
3D Display Geomectric Info
------------------------------
*/

#define LEYE_X					695
#define LEYE_Y					400
#define LEYE_Z					500

#define REYE_X					705
#define REYE_Y					400
#define REYE_Z					500

#define PLANE_Z					400
#define PLANE_INC				10

/*
------------------------------
Galaxy Initial Positions
------------------------------
*/

//Galaxy 1:
#define G1_XPOS					500
#define G1_YPOS					300
#define G1_ZPOS					0
#define G1_XSPEED				0
#define G1_YSPEED				0
#define G1_ZSPEED				0

//Galaxy 2:
#define G2_XPOS					900
#define G2_YPOS					500
#define G2_ZPOS					0
#define G2_XSPEED				0
#define G2_YSPEED				0
#define G2_ZSPEED				0

//Galaxy 3:
#define G3_XPOS					700
#define G3_YPOS					500
#define G3_ZPOS					0
#define G3_XSPEED				0
#define G3_YSPEED				0
#define G3_ZSPEED				0



#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here