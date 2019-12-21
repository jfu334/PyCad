#include "calc.h"
#include <Bnd_Box.hxx>
#include <BRepExtrema_DistanceSS.hxx>
#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>

namespace PyCadCpp::calc
{
	double distance(GeometryBase& obj1, GeometryBase& obj2, double max)
	{
		return 0;
	}
	double distance(Vec3 obj1, GeometryBase& obj2, double max)
	{
		return 0;
	}
	double distance(GeometryBase& obj, Vec3 vec, double max)
	{
		auto p=BRepBuilderAPI_MakeVertex(gp_Pnt(vec.x(), vec.y(), vec.z())).Shape();
		
		auto bbox=Bnd_Box();
		bbox.SetWhole();
		
		auto calcApi=BRepExtrema_DistShapeShape(obj.shape(), p);
		return calcApi.Value();
	}
}
