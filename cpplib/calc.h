#pragma once
#include "objects.h"

namespace PyCadCpp::calc
{
	double distance(GeometryBase& obj1, GeometryBase& obj2, double max=1e9);
	double distance(Vec3 obj1, GeometryBase& obj2, double max=1e9);
	double distance(GeometryBase& obj1, Vec3 obj2, double max=1e9);
	
	
}
