#include "stdafx.h"
#include <vector>
#include "star.h"
#include "galaxy.h"

#define PI 3.14159265359


double rad_to_deg(double r)
{
	return (r * 180) / PI;
}



double deg_to_rad(double d)
{
	return (d / 180) * PI;
}



//returns a double between low and high
//if neg = true, returned value has 50% chance to be negative
double randBound(double low, double high, bool neg)
{
	if (neg)
		return ((((double)rand() / RAND_MAX) * (high - low)) + low) * pow(-1, rand());
	else
		return (((double)rand() / RAND_MAX) * (high - low)) + low;
}



std::vector<double> normalize(std::vector<double> v)
{
	double mag = 0;
	for (int i = 0; i < v.size(); i++)
		mag += pow(v.at(i), 2);
	mag = sqrt(mag);

	for (int i = 0; i < v.size(); i++)
		v.at(i) /= mag;

	return v;
}



void matrixMult4x1(double A[4][4], double B[4], double C[4])
{
	double tempA[4][4], tempB[4];
	for (int r = 0; r < 4; r++)
	{
		tempB[r] = B[r];
		for (int c = 0; c < 4; c++)
			tempA[r][c] = A[r][c];
	}

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			C[r] += tempA[r][c] * tempB[c];
		}
	}
}



void matrixMult4x4(double A[4][4], double B[4][4], double C[4][4])
{
	double tempA[4][4], tempB[4][4];
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			tempA[r][c] = A[r][c];
			tempB[r][c] = B[r][c];
		}
	}

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			for (int x = 0; x < 4; x++)
				C[r][c] += tempA[r][x] * tempB[x][c];
		}
	}
}



bool invertMatrix4x4(double m[4][4], double inverse[4][4])
{
	double inv[4][4], det;
	int i;

	inv[0][0] = m[1][1] * m[2][2] * m[3][3] -
		m[1][1] * m[2][3] * m[3][2] -
		m[2][1] * m[1][2] * m[3][3] +
		m[2][1] * m[1][3] * m[3][2] +
		m[3][1] * m[1][2] * m[2][3] -
		m[3][1] * m[1][3] * m[2][2];

	inv[1][0] = -m[1][0] * m[2][2] * m[3][3] +
		m[1][0] * m[2][3] * m[3][2] +
		m[2][0] * m[1][2] * m[3][3] -
		m[2][0] * m[1][3] * m[3][2] -
		m[3][0] * m[1][2] * m[2][3] +
		m[3][0] * m[1][3] * m[2][2];

	inv[2][0] = m[1][0] * m[2][1] * m[3][3] -
		m[1][0] * m[2][3] * m[3][1] -
		m[2][0] * m[1][1] * m[3][3] +
		m[2][0] * m[1][3] * m[3][1] +
		m[3][0] * m[1][1] * m[2][3] -
		m[3][0] * m[1][3] * m[2][1];

	inv[3][0] = -m[1][0] * m[2][1] * m[3][2] +
		m[1][0] * m[2][2] * m[3][1] +
		m[2][0] * m[1][1] * m[3][2] -
		m[2][0] * m[1][2] * m[3][1] -
		m[3][0] * m[1][1] * m[2][2] +
		m[3][0] * m[1][2] * m[2][1];

	inv[0][1] = -m[0][1] * m[2][2] * m[3][3] +
		m[0][1] * m[2][3] * m[3][2] +
		m[2][1] * m[0][2] * m[3][3] -
		m[2][1] * m[0][3] * m[3][2] -
		m[3][1] * m[0][2] * m[2][3] +
		m[3][1] * m[0][3] * m[2][2];

	inv[1][1] = m[0][0] * m[2][2] * m[3][3] -
		m[0][0] * m[2][3] * m[3][2] -
		m[2][0] * m[0][2] * m[3][3] +
		m[2][0] * m[0][3] * m[3][2] +
		m[3][0] * m[0][2] * m[2][3] -
		m[3][0] * m[0][3] * m[2][2];

	inv[2][1] = -m[0][0] * m[2][1] * m[3][3] +
		m[0][0] * m[2][3] * m[3][1] +
		m[2][0] * m[0][1] * m[3][3] -
		m[2][0] * m[0][3] * m[3][1] -
		m[3][0] * m[0][1] * m[2][3] +
		m[3][0] * m[0][3] * m[2][1];

	inv[3][1] = m[0][0] * m[2][1] * m[3][2] -
		m[0][0] * m[2][2] * m[3][1] -
		m[2][0] * m[0][1] * m[3][2] +
		m[2][0] * m[0][2] * m[3][1] +
		m[3][0] * m[0][1] * m[2][2] -
		m[3][0] * m[0][2] * m[2][1];

	inv[0][2] = m[0][1] * m[1][2] * m[3][3] -
		m[0][1] * m[1][3] * m[3][2] -
		m[1][1] * m[0][2] * m[3][3] +
		m[1][1] * m[0][3] * m[3][2] +
		m[3][1] * m[0][2] * m[1][3] -
		m[3][1] * m[0][3] * m[1][2];

	inv[1][2] = -m[0][0] * m[1][2] * m[3][3] +
		m[0][0] * m[1][3] * m[3][2] +
		m[1][0] * m[0][2] * m[3][3] -
		m[1][0] * m[0][3] * m[3][2] -
		m[3][0] * m[0][2] * m[1][3] +
		m[3][0] * m[0][3] * m[1][2];

	inv[2][2] = m[0][0] * m[1][1] * m[3][3] -
		m[0][0] * m[1][3] * m[3][1] -
		m[1][0] * m[0][1] * m[3][3] +
		m[1][0] * m[0][3] * m[3][1] +
		m[3][0] * m[0][1] * m[1][3] -
		m[3][0] * m[0][3] * m[1][1];

	inv[3][2] = -m[0][0] * m[1][1] * m[3][2] +
		m[0][0] * m[1][2] * m[3][1] +
		m[1][0] * m[0][1] * m[3][2] -
		m[1][0] * m[0][2] * m[3][1] -
		m[3][0] * m[0][1] * m[1][2] +
		m[3][0] * m[0][2] * m[1][1];

	inv[0][3] = -m[0][1] * m[1][2] * m[2][3] +
		m[0][1] * m[1][3] * m[2][2] +
		m[1][1] * m[0][2] * m[2][3] -
		m[1][1] * m[0][3] * m[2][2] -
		m[2][1] * m[0][2] * m[1][3] +
		m[2][1] * m[0][3] * m[1][2];

	inv[1][3] = m[0][0] * m[1][2] * m[2][3] -
		m[0][0] * m[1][3] * m[2][2] -
		m[1][0] * m[0][2] * m[2][3] +
		m[1][0] * m[0][3] * m[2][2] +
		m[2][0] * m[0][2] * m[1][3] -
		m[2][0] * m[0][3] * m[1][2];

	inv[2][3] = -m[0][0] * m[1][1] * m[2][3] +
		m[0][0] * m[1][3] * m[2][1] +
		m[1][0] * m[0][1] * m[2][3] -
		m[1][0] * m[0][3] * m[2][1] -
		m[2][0] * m[0][1] * m[1][3] +
		m[2][0] * m[0][3] * m[1][1];

	inv[3][3] = m[0][0] * m[1][1] * m[2][2] -
		m[0][0] * m[1][2] * m[2][1] -
		m[1][0] * m[0][1] * m[2][2] +
		m[1][0] * m[0][2] * m[2][1] +
		m[2][0] * m[0][1] * m[1][2] -
		m[2][0] * m[0][2] * m[1][1];

	det = m[0][0] * inv[0][0] + m[0][1] * inv[1][0] + m[0][2] * inv[2][0] + m[0][3] * inv[3][0];

	if (det == 0)
		return false;

	det = 1.0 / det;

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
			inverse[r][c] = inv[r][c] * det;
	}

	return true;
}



bool shouldRender(Star s)
{
	if (
		s.p.at(0) >= -RENDER_BOUNDARY &&
		s.p.at(0) <= RENDER_BOUNDARY &&
		s.p.at(1) >= -RENDER_BOUNDARY &&
		s.p.at(1) <= RENDER_BOUNDARY &&
		s.p.at(2) >= -RENDER_BOUNDARY &&
		s.p.at(2) <= RENDER_BOUNDARY
		)
		return true;
	else
		return false;
}



bool shouldRender(Galaxy g)
{
	if (
		g.p.at(0) >= -RENDER_BOUNDARY &&
		g.p.at(0) <= RENDER_BOUNDARY &&
		g.p.at(1) >= -RENDER_BOUNDARY &&
		g.p.at(1) <= RENDER_BOUNDARY &&
		g.p.at(2) >= -RENDER_BOUNDARY &&
		g.p.at(2) <= RENDER_BOUNDARY
		)
		return true;
	else
		return false;
}