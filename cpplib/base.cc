#include "base.h"
#include <cmath>
#include <iostream>

#include <gp_Quaternion.hxx>
#include <gp_Trsf.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_GTransform.hxx>
#include <BRepBuilderAPI_Copy.hxx>

namespace PyCadCpp::base
{
	namespace
	{
		double d2r(double d) {return d/180.0*M_PI;}
	
		gp_Quaternion rotationQuaternionYPR(double r, double p, double y)
		{
			double cr=cos(d2r(r/2)), cp=cos(d2r(p/2)), cy=cos(d2r(y/2));
			double sr=sin(d2r(r/2)), sp=sin(d2r(p/2)), sy=sin(d2r(y/2));

			return gp_Quaternion(
				cy*cp*sr-sy*sp*cr, 
				sy*cp*sr+cy*sp*cr,
				sy*cp*cr-cy*sp*sr,
				cy*cp*cr+sy*sp*sr);
		}
	}
	
	Exception::Exception(std::string message) : _message(message)
	{
		std::cerr << "ERROR: " << message << std::endl << std::flush;
	}
	
	
	TopoDS_Shape copy(TopoDS_Shape shape)
	{
		auto makeCopy=BRepBuilderAPI_Copy(shape);
		return makeCopy.Shape();
	}

	TopoDS_Shape translate(TopoDS_Shape shape, double x, double y, double z)
	{
		auto m=gp_Trsf();
		m.SetTranslation(gp_Vec(x, y, z));

		auto opApi=BRepBuilderAPI_Transform(m);
		opApi.Perform(shape, true);
		return opApi.Shape();
	}
	TopoDS_Shape rotate(TopoDS_Shape shape, double x, double y, double z)
	{
		auto m=gp_Trsf();
		m.SetRotation(rotationQuaternionYPR(x, y, z));

		auto opApi=BRepBuilderAPI_Transform(m);
		opApi.Perform(shape, true);
		return opApi.Shape();
	}
	
	TopoDS_Shape mirror(TopoDS_Shape shape, double x, double y, double z)
	{
		auto m=gp_Trsf();
		m.SetMirror(gp_Ax2(gp_Pnt(0, 0, 0), gp_Dir(x, y, z)));
		
		auto opApi=BRepBuilderAPI_Transform(m);
		opApi.Perform(shape, true);
		return opApi.Shape();
	}
	
	std::string shapeType(TopoDS_Shape shape)
	{
		switch(shape.ShapeType())
		{
			case TopAbs_COMPOUND:
				return "COMPOUND";
			case TopAbs_COMPSOLID: 
				return "COMPSOLID";
			case TopAbs_SOLID: 
				return "SOLID";
			case TopAbs_SHELL:
				return "SHELL";
			case TopAbs_FACE:
				return "FACE";
			case TopAbs_WIRE:
				return "WIRE";
			case TopAbs_EDGE:
				return "EDGE";
			case TopAbs_VERTEX:
				return "VERTEX";
			case TopAbs_SHAPE:
				return "SHAPE";
			default:
				return "?";
		}
	}
	
	std::string continuity(GeomAbs_Shape continuity)
	{
		switch(continuity)
		{
			case GeomAbs_C0: 
				return "C0";
			case GeomAbs_C1: 
				return "C1";
			case GeomAbs_C2: 
				return "C2";
			case GeomAbs_C3: 
				return "C3";
			case GeomAbs_CN: 
				return "CN";
			case GeomAbs_G1: 
				return "G1";
			case GeomAbs_G2: 
				return "G2";
			default:
				return "?";
		}
	}
	
}
