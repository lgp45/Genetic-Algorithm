#pragma once
#ifndef UTILITY_H
#define UTILITY_H
#include <stdlib.h>
#include <math.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

inline int	  RandInt(int x, int y) { return rand() % (y - x + 1) + x; }

inline double RandFloat() { return (rand()) / (RAND_MAX + 1.0); }

inline double RandFloat(float range) { return float(rand()) / float((RAND_MAX)) * range; }

inline bool   RandBool()
{
	if (RandInt(0, 1)) return true;

	else return false;
}

//returns a random float in the range -1 < n < 1
inline double RandomClamped() { return RandFloat() - RandFloat(); }

//-----------------------------------------------------------------------
// useful string functions
//-----------------------------------------------------------------------

//int to string function
string itos(int arg);

//Augmented from original set -> Author: Mat Buckland in Pathfinder. Ease of use functions

#endif