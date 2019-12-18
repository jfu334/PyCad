#include "op.h"
#include <TopoDS.hxx>


#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Common.hxx>

#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>

#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepOffsetAPI_MakeEvolved.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>

#include <ShapeFix_Shape.hxx>
#include <BRepLib_FuseEdges.hxx>

#include "base.h"

namespace PyCadCpp::op
{
	Shell* wireToShell(Wire* obj)
	{
		auto makeApi=BRepBuilderAPI_MakeFace(TopoDS::Wire(obj->shape()));
		return new Shell(makeApi.Face());
	}
	
	// aliases
	Solid* union_(Solid* obj1, Solid* obj2) {return fuse(obj1, obj2);}
	Wire* union_(Wire* obj1, Wire* obj2) {return fuse(obj1, obj2);}
	Solid* cut(Solid* obj1, Solid* obj2) {return difference(obj1, obj2);}
	
	Solid* fuse(Solid* obj1, Solid* obj2)
	{
		auto makeApi=BRepAlgoAPI_Fuse(obj1->shape(), obj2->shape());
		return new Solid(makeApi.Shape());
	}
	Wire* fuse(Wire* obj1, Wire* obj2)
	{
		auto makeApi=BRepBuilderAPI_MakeWire();
		makeApi.Add(TopoDS::Wire(obj1->shape()));
		makeApi.Add(TopoDS::Wire(obj2->shape()));
		return new Wire(makeApi.Wire());
	}
	
	Solid* difference(Solid* obj1, Solid* obj2)
	{
		auto makeApi=BRepAlgoAPI_Cut(obj1->shape(), obj2->shape());
		return new Solid(makeApi.Shape());
	}
	
	Solid* intersect(Solid* obj1, Solid* obj2)
	{
		auto makeApi=BRepAlgoAPI_Common(obj1->shape(), obj2->shape());
		return new Solid(makeApi.Shape());
	}
	
	Shell* extrude_linear(Wire* curve, double h) {return extrude_linear(curve, Vec3(0, 0, h));}
	Solid* extrude_linear(Shell* surface, double h) {return extrude_linear(surface, Vec3(0, 0, h));}
	
	Shell* extrude_linear(Wire* curve, Vec3 direction)
	{
		// extrude to object
		gp_Vec vec(direction.x(), direction.y(), direction.z());
		
		auto makePrism=BRepPrimAPI_MakePrism(curve->shape(), vec, false);
		return new Shell(makePrism.Shape());
	}
	
	Solid* extrude_linear(Shell* surface, Vec3 direction)
	{
		// extrude to object
		gp_Vec vec(direction.x(), direction.y(), direction.z());
		
		auto makePrism=BRepPrimAPI_MakePrism(surface->shape(), vec, false);
		return new Solid(makePrism.Shape());
	}
	
	Solid* extrude_rotate(Shell* shell, double angle, Vec3 axis)
	{
		auto occ_axis=gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(axis.x(), axis.y(), axis.z()));
		auto makeRevol=BRepPrimAPI_MakeRevol(shell->shape(), occ_axis, angle);
		return new Solid(makeRevol.Shape());
	}
	
	Shell* pipe_shell(Wire* profile, Wire* spine, Wire* spineNormal)
	{
		auto makePipe=BRepOffsetAPI_MakePipeShell(spine->shape_wire());
		
		if(spineNormal!=nullptr)
			makePipe.SetMode(spineNormal->shape_wire(), true);
		
		makePipe.Add(profile->shape_wire());
		makePipe.Build();
		
		return new Shell(makePipe.Shape());
	}
	Solid* pipe_solid(Wire* profile, Wire* spine, Wire* spineNormal)
	{
		auto makePipe=BRepOffsetAPI_MakePipeShell(spine->shape_wire());
		
		if(spineNormal!=nullptr)
			makePipe.SetMode(spineNormal->shape_wire(), true);
		
		makePipe.Add(profile->shape_wire());
		makePipe.Build();
		makePipe.MakeSolid();
		
		return new Solid(makePipe.Shape());
	}
	
	
	Solid* fillet(Solid* solid, std::vector<Edge> edges, double size)
	{
		std::vector<double> vsize;
		for(auto i: edges) vsize.push_back(size);
		return fillet(solid, edges, vsize);
	}
	
	Solid* fillet(Solid* solid, std::vector<Edge> edges, std::vector<double> size)
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
	
	Solid* hollow(Solid* solid, std::vector<Face> openingFaces, double thickness, double tol)
	{
		TopTools_ListOfShape facesToRemove;
		for(auto i: openingFaces) facesToRemove.Append(i._data);
		
		auto makeHollow=BRepOffsetAPI_MakeThickSolid();
		makeHollow.MakeThickSolidByJoin(solid->shape(), facesToRemove, thickness, tol);
		
		return new Solid(makeHollow.Shape());
	}
	
	Solid* heal(Solid* solid, double tol)
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
