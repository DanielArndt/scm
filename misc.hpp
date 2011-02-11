#ifndef MISC_HPP
#define MISC_HPP

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <map>

#include <sys/resource.h>

using namespace std;

#define NEARZERO 10e-12
#define EPSILON 1e-5

/* Returns a normally distributed deviate with zero mean and unit variance
   based on Section 7.2 of 'Numerical Recipes in C'.*/
float gasdev();

/* Gaussian probability function with mean 'mu' and standard deviation 'sd'. */
inline double GaussPF(double x, double mu, double sd)
{ return exp( - ( x - mu ) * ( x - mu ) / ( 2 * sd * sd ) ) / ( sd * sqrt( 2 * M_PI ) ); }

/* We can use this as a cheaper bid activation function. */
inline double GaussPF(double x)
{ return exp(- x * x); }

void die(const char*, const char *, const int, const char *);

double EuclideanDistSqrd(double *, double *, int);
double EuclideanDistSqrd(vector < double > &, vector < double > &);

int hammingDist(vector < int > &, vector < int > &);

inline bool isEqual(double x, double y) 
{ return fabs(x - y) < EPSILON; }

inline bool isEqual(double x, double y, double e) 
{ return fabs(x - y) < e; }

bool isEqual(vector < int > &, vector < int > &);
bool isEqual(vector < double > &, vector < double > &, double);

int stringToInt(string);
long stringToLong(string);
double stringToDouble(string);

int readMap(string, map < string, string > &);

bool getusage(double &, double &);

template < class vtype > string vecToStr(vector < vtype > &v)
{ ostringstream oss; for(int i = 0; i < v.size(); i++) { oss << " " << v[i]; } return oss.str(); }

#endif
