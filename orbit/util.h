#ifndef UTIL_H
#define UTIL_H

class Star;
class Galaxy;

//converts a radian value to degrees
double rad_to_deg(double r); 

//converts a degree value to radians
double deg_to_rad(double d); 

//returns a double between low and high
//if neg = true, returned value has 50% chance to be negative
double randBound(double low, double high, bool neg);

//normalizes a vector of arbitrary length
std::vector<double> normalize(std::vector<double> v); 

//performs matrix multiplication A x B, stores in C
void matrixMult4x1(double A[4][4], double B[4], double C[4]);

//performs matrix multiplication A x B, stores in C
void matrixMult4x4(double A[4][4], double B[4][4], double C[4][4]);

//inverts a 4x4 matrix m, stores in inverse
//returns true if inversion is possible, false otherwise
bool invertMatrix4x4(double m[4][4], double inverse[4][4]); 

//returns true if s is within the render boundary, false otherwise
bool shouldRender(Star s);

//returns true if g is within the render boundary, false otherwise
bool shouldRender(Galaxy g); 
#endif