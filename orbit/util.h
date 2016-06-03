#ifndef UTIL_H
#define UTIL_H

double rad_to_deg(double r);

double deg_to_rad(double d);

//returns a double between low and high
//if neg = true, returned value has 50% chance to be negative
double randBound(double low, double high, bool neg);

std::vector<double> normalize(std::vector<double> v);

void matrixMult4x1(double A[4][4], double B[4], double C[4]);

void matrixMult4x4(double A[4][4], double B[4][4], double C[4][4]);

bool invertMatrix4x4(double m[4][4], double inverse[4][4]);
#endif