#pragma once
#include <vector>
#include "objects.h"
#include "vec.h"

namespace PyCadCpp::op
{
	brep::Shell* wireToShell(brep::Wire* obj);
	
	//
	// Boolean operations
	//
	
	brep::Solid* fuse(brep::Solid* obj1, brep::Solid* obj2);
	brep::Wire* fuse(brep::Wire* obj1, brep::Wire* obj2);
	
	brep::Solid* cut(brep::Solid* obj1, brep::Solid* obj2);
	brep::Solid* common(brep::Solid* obj1, brep::Solid* obj2);

	// name aliases like openscad
	brep::Solid* union_(brep::Solid* obj1, brep::Solid* obj2);
	brep::Wire* union_(brep::Wire* obj1, brep::Wire* obj2);
	
	brep::Solid* difference(brep::Solid* obj1, brep::Solid* obj2);
	brep::Solid* intersect(brep::Solid* obj1, brep::Solid* obj2);
	
	
	//
	// extrusions
	//
	
	brep::Shell* extrude_linear(brep::Wire* surface, double h);
	brep::Shell* extrude_linear(brep::Wire* surface, Vec3 direction);
	
	brep::Solid* extrude_linear(brep::Shell* surface, double h);
	brep::Solid* extrude_linear(brep::Shell* surface, Vec3 direction);
	
	brep::Solid* extrude_rotate(brep::Shell* surface, double angle, Vec3 axis);
	
	//
	// piping
	//
	
	brep::Shell* pipe_shell(brep::Wire* profile, brep::Wire* spine, brep::Wire* spineNormal=nullptr);
	brep::Solid* pipe_solid(brep::Wire* profile, brep::Wire* spine, brep::Wire* spineNormal=nullptr);
	
	//
	// projection
	//
	
	// Projects a wire onto a surface using the surface normals. 
	// Returns all resulting wires. 
	std::vector<brep::Wire*> project_normal(brep::Wire* wire, brep::Shell* surface);
	
	// Projects a wire onto a surface along given direction. 
	// Returns all found intersections. 
	std::vector<brep::Wire*> project_linear(brep::Wire* wire, brep::Shell* surface, Vec3 direction);
	
	// Projects a wire onto a surface by making a point projection from point. 
	// Returns all found intersections. 
	std::vector<brep::Wire*> project_point(brep::Wire* wire, brep::Shell* surface, Vec3 point);
	
	
	//
	// other operations
	//
	
	brep::Solid* fillet(brep::Solid* solid, 
		std::vector<brep::Edge> edges, double size);
	brep::Solid* fillet(brep::Solid* solid, 
		std::vector<brep::Edge> edges, std::vector<double> size);
	
	brep::Solid* hollow(brep::Solid* solid, 
		std::vector<brep::Face> openingFaces, double thickness, double tol=1e-3);
	
	//
	// other stuff
	//
	
	brep::Solid* heal(brep::Solid* solid, double tol=1e-3);
	
	
}
