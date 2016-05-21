#pragma once
#define PI 3.14159265359
#include <vector>


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