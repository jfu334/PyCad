#pragma once
#include <string>
#include <vector>

#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>

#include <GeomAbs_Shape.hxx>

#include "vec.h"

namespace PyCadCpp::base
{
	class Exception
	{
	public:
		Exception(std::string message);
		std::string message() {return _message;}
	private:
		std::string _message;
	};
	
	std::string shapeType(TopoDS_Shape shape);
	std::string continuity(GeomAbs_Shape continuity);
	
	TopoDS_Shape copy(TopoDS_Shape shape);
	TopoDS_Shape translate(TopoDS_Shape shape, double x, double y, double z);
	TopoDS_Shape rotate(TopoDS_Shape shape, double x, double y, double z);
	TopoDS_Shape mirror(TopoDS_Shape shape, double x, double y, double z);
	
	TopoDS_Wire shape_wire(TopoDS_Shape shape);
	
	
}
