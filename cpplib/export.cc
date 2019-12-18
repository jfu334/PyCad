#include "export.h"
#include <BRepMesh_IncrementalMesh.hxx>
#include <StlAPI_Writer.hxx>

namespace PyCadCpp::export_
{
	void export_stl(std::string filename, Solid* object, double deflection)
	{
		auto makeMesh=BRepMesh_IncrementalMesh(object->shape(), deflection);
		makeMesh.Perform();
		
		if(!makeMesh.IsDone())
			throw "Operation failed!";
		
		auto stlWriter=StlAPI_Writer();
		stlWriter.Write(object->shape(), filename.c_str());
		
	}
}
