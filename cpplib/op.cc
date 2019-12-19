#include "op.h"

#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>

#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Common.hxx>

#include <BRepProj_Projection.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepOffsetAPI_NormalProjection.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepOffsetAPI_MakeEvolved.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>

#include <ShapeFix_Shape.hxx>
#include <BRepLib_FuseEdges.hxx>

#include "base.h"

namespace PyCadCpp::op
{
	// to make things easier
	using namespace brep;
	
	Shell* wireToShell(brep::Wire* obj)
	{
		auto makeApi=BRepBuilderAPI_MakeFace(TopoDS::Wire(obj->shape()));
		return new Shell(makeApi.Face());
	}
	
	// aliases
	brep::Solid* union_(brep::Solid* obj1, brep::Solid* obj2) {return fuse(obj1, obj2);}
	brep::Wire* union_(brep::Wire* obj1, brep::Wire* obj2) {return fuse(obj1, obj2);}
	brep::Solid* cut(brep::Solid* obj1, brep::Solid* obj2) {return difference(obj1, obj2);}

	brep::Solid* common(brep::Solid* obj1, brep::Solid* obj2)
	{
		throw new base::Exception("Not implemented!");
	}

	brep::Solid* fuse(brep::Solid* obj1, brep::Solid* obj2)
	{
		auto makeApi=BRepAlgoAPI_Fuse(obj1->shape(), obj2->shape());
		return new Solid(makeApi.Shape());
	}
	brep::Wire* fuse(brep::Wire* obj1, brep::Wire* obj2)
	{
		auto makeApi=BRepBuilderAPI_MakeWire();
		makeApi.Add(TopoDS::Wire(obj1->shape()));
		makeApi.Add(TopoDS::Wire(obj2->shape()));
		return new Wire(makeApi.Wire());
	}
	
	brep::Solid* difference(brep::Solid* obj1, brep::Solid* obj2)
	{
		auto makeApi=BRepAlgoAPI_Cut(obj1->shape(), obj2->shape());
		return new Solid(makeApi.Shape());
	}
	
	brep::Solid* intersect(brep::Solid* obj1, brep::Solid* obj2)
	{
		auto makeApi=BRepAlgoAPI_Common(obj1->shape(), obj2->shape());
		return new Solid(makeApi.Shape());
	}
	
	Shell* extrude_linear(brep::Wire* curve, double h) {return extrude_linear(curve, Vec3(0, 0, h));}
	brep::Solid* extrude_linear(Shell* surface, double h) {return extrude_linear(surface, Vec3(0, 0, h));}
	
	Shell* extrude_linear(brep::Wire* curve, Vec3 direction)
	{
		// extrude to object
		gp_Vec vec(direction.x(), direction.y(), direction.z());
		
		auto makePrism=BRepPrimAPI_MakePrism(curve->shape(), vec, false);
		return new Shell(makePrism.Shape());
	}
	
	brep::Solid* extrude_linear(Shell* surface, Vec3 direction)
	{
		// extrude to object
		gp_Vec vec(direction.x(), direction.y(), direction.z());
		
		auto makePrism=BRepPrimAPI_MakePrism(surface->shape(), vec, false);
		return new Solid(makePrism.Shape());
	}
	
	brep::Solid* extrude_rotate(Shell* shell, double angle, Vec3 axis)
	{
		auto occ_axis=gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(axis.x(), axis.y(), axis.z()));
		auto makeRevol=BRepPrimAPI_MakeRevol(shell->shape(), occ_axis, angle);
		return new Solid(makeRevol.Shape());
	}
	
	Shell* pipe_shell(brep::Wire* profile, brep::Wire* spine, brep::Wire* spineNormal)
	{
		auto makePipe=BRepOffsetAPI_MakePipeShell(base::shape_wire(spine->shape()));
		
		if(spineNormal!=nullptr)
			makePipe.SetMode(base::shape_wire(spineNormal->shape()), true);
		
		makePipe.Add(base::shape_wire(profile->shape()));
		makePipe.Build();
		
		return new Shell(makePipe.Shape());
	}
	brep::Solid* pipe_solid(brep::Wire* profile, brep::Wire* spine, brep::Wire* spineNormal)
	{
		auto makePipe=BRepOffsetAPI_MakePipeShell(base::shape_wire(spine->shape()));
		
		if(spineNormal!=nullptr)
			makePipe.SetMode(base::shape_wire(spineNormal->shape()), true);
		
		makePipe.Add(base::shape_wire(profile->shape()));
		makePipe.Build();
		makePipe.MakeSolid();
		
		return new Solid(makePipe.Shape());
	}
	
	std::vector<brep::Wire*> project_normal(brep::Wire* wire, brep::Shell* surface)
	{
		auto makeApi=BRepOffsetAPI_NormalProjection(surface->shape());
		makeApi.Add(wire->shape());
		makeApi.Build();
		
		TopTools_ListOfShape list;
		if(!makeApi.BuildWire(list))
			throw new base::Exception("Unable to perform! Result is not a single wire.");
		
		std::vector<brep::Wire*> result;
		for(auto i: list) result.push_back(new brep::Wire(i));
		
		return result;
	}
	
	std::vector<brep::Wire*> project_linear(brep::Wire* wire, brep::Shell* surface, Vec3 direction)
	{
		auto makeApi=BRepProj_Projection(wire->shape(), surface->shape(), 
			gp_Dir(direction.x(), direction.y(), direction.z()));
		
		std::vector<brep::Wire*> result;
		for(;makeApi.More();makeApi.Next())
			result.push_back(new Wire(makeApi.Current()));
		
		return result;
	}
	
	std::vector<brep::Wire*> project_point(brep::Wire* wire, brep::Shell* surface, Vec3 point)
	{
		auto makeApi=BRepProj_Projection(wire->shape(), surface->shape(), 
			gp_Pnt(point.x(), point.y(), point.z()));
		
		std::vector<brep::Wire*> result;
		for(;makeApi.More();makeApi.Next())
			result.push_back(new Wire(makeApi.Current()));
		
		return result;
	}
	
	brep::Solid* fillet(brep::Solid* solid, std::vector<Edge> edges, double size)
	{
		std::vector<double> vsize;
		for(auto i: edges) vsize.push_back(size);
		return fillet(solid, edges, vsize);
	}
	
	brep::Solid* fillet(brep::Solid* solid, std::vector<Edge> edges, std::vector<double> size)
	{
		auto makeFillet=BRepFilletAPI_MakeFillet(solid->shape());
		for(size_t i=0;i<size.size();i++)
			makeFillet.Add(size[i], edges[i]._data);
		
		makeFillet.Build();
		
		// if failed, give info
		if(!makeFillet.IsDone())
		{
			std::cout << makeFillet.NbFaultyContours() << " contours failed!" << std::endl;
			std::cout << makeFillet.NbFaultyVertices() << " vertices failed!" << std::endl;
			throw new base::Exception("Failed to build fillet!");
		}
		
		return new Solid(makeFillet.Shape());
	}
	
	brep::Solid* hollow(brep::Solid* solid, std::vector<Face> openingFaces, double thickness, double tol)
	{
		TopTools_ListOfShape facesToRemove;
		for(auto i: openingFaces) facesToRemove.Append(i._data);
		
		auto makeHollow=BRepOffsetAPI_MakeThickSolid();
		makeHollow.MakeThickSolidByJoin(solid->shape(), facesToRemove, thickness, tol);
		
		return new Solid(makeHollow.Shape());
	}
	
	brep::Solid* heal(brep::Solid* solid, double tol)
	{
		auto sfs=ShapeFix_Shape(); 
		sfs.Init(solid->shape()); 
		sfs.SetMaxTolerance(tol);
		sfs.SetMinTolerance(tol);
		sfs.Perform();
		
		BRepLib_FuseEdges makeFuse(sfs.Shape());
		
		
		return new Solid(makeFuse.Shape());
	}
}
