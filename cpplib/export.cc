#include "export.h"
#include <Bnd_Box.hxx>
#include <BRepTools.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepMesh_FastDiscret.hxx>
#include <StlAPI_Writer.hxx>

namespace PyCadCpp::export_
{
	void export_stl(std::string filename, brep::Solid* object, double deflection, double adeflection)
	{
		
		BRepMesh_IncrementalMesh makeMesh;
		makeMesh.ChangeParameters().Deflection=deflection;
		makeMesh.ChangeParameters().Angle=adeflection;
		makeMesh.ChangeParameters().InParallel=Standard_True;
		
		makeMesh.SetShape(object->shape());
		
		makeMesh.Perform();
		if(!makeMesh.IsDone())
			throw "Operation failed!";
		
		
		/*
		Bnd_Box bndBox;
		bndBox.SetWhole();
		
		BRepMesh_FastDiscret::Parameters parameters;
		parameters.Angle=adeflection;
		parameters.Deflection=deflection;
		parameters.InParallel=Standard_True;
		parameters.ControlSurfaceDeflection=Standard_True;
		
		auto makeMesh=BRepMesh_FastDiscret(bndBox, parameters);
		makeMesh.Perform(object->shape());
		*/
		
		std::cout << "meshing valid: " <<
			BRepTools::Triangulation(object->shape(), deflection) << std::endl;
		
		auto stlWriter=StlAPI_Writer();
		stlWriter.Write(object->shape(), filename.c_str());
		
	}
}
