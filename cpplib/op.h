#pragma once
#include <vector>
#include "objects.h"
#include "vec.h"

namespace PyCadCpp::op
{
	Shell* wireToShell(Wire* obj);
	
	//
	// Boolean operations
	//
	
	Solid* fuse(Solid* obj1, Solid* obj2);
	Solid* union_(Solid* obj1, Solid* obj2);
	
	Solid* difference(Solid* obj1, Solid* obj2);
	Solid* cut(Solid* obj1, Solid* obj2);
	
	Solid* intersect(Solid* obj1, Solid* obj2);
	
	Wire* fuse(Wire* obj1, Wire* obj2);
	Wire* union_(Wire* obj1, Wire* obj2);
	
	//
	// extrusions
	//
	
	Shell* extrude_linear(Wire* surface, double h);
	Shell* extrude_linear(Wire* surface, Vec3 direction);
	
	Solid* extrude_linear(Shell* surface, double h);
	Solid* extrude_linear(Shell* surface, Vec3 direction);
	
	Solid* extrude_rotate(Shell* surface, double angle, Vec3 axis);
	
	//
	// 
	//
	
	Shell* pipe_shell(Wire* profile, Wire* spine, Wire* spineNormal=nullptr);
	Solid* pipe_solid(Wire* profile, Wire* spine, Wire* spineNormal=nullptr);
	
	
	//
	// other operations
	//
	
	Solid* fillet(Solid* solid, std::vector<Edge> edges, double size);
	Solid* fillet(Solid* solid, std::vector<Edge> edges, std::vector<double> size);
	
	Solid* hollow(Solid* solid, std::vector<Face> openingFaces, double thickness, double tol=1e-3);
	
	//
	// other stuff
	//
	
	Solid* heal(Solid* solid, double tol=1e-3);
	
	
}
