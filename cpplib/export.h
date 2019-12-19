#pragma once
#include <string>
#include "objects.h"

namespace PyCadCpp::export_
{
	void export_stl(std::string filename, brep::Solid* object, double deflection, double adeflection=0.5);
}
