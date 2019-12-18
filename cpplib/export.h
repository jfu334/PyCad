#pragma once
#include <string>
#include "objects.h"

namespace PyCadCpp::export_
{
	void export_stl(std::string filename, Solid* object, double deflection);
}
