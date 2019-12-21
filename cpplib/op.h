#pragma once
#include <vector>
#include "objects.h"
#include "vec.h"

namespace PyCadCpp::op
{
	brep::Shell* wireToShell(brep::Wire* obj);
	brep::Solid* shellToSolid(brep::Shell* shell);
	
	// reverse orientation
	brep::Shell* reverse(brep::Shell* shell);
	
	//
	// Boolean operations
	//
	
	brep::Solid* fuse(brep::Solid* obj1, brep::Solid* obj2);
	brep::Shell* fuse(brep::Shell* obj1, brep::Shell* obj2);
	brep::Wire* fuse(brep::Wire* obj1, brep::Wire* obj2);
	
	brep::Solid* cut(brep::Solid* obj1, brep::Solid* obj2);
	brep::Solid* common(brep::Solid* obj1, brep::Solid* obj2);

	
	//
	// extrusions
	//
	
	brep::Shell* extrude_linear(brep::Wire* surface, double h);
	brep::Shell* extrude_linear(brep::Wire* surface, Vec3 direction);
	
	brep::Solid* extrude_linear(brep::Shell* surface, double h);
	brep::Solid* extrude_linear(brep::Shell* surface, Vec3 direction);
	
	brep::Shell* extrude_rotate(brep::Wire* wire, double angle, Vec3 axis);
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
	std::vector<brep::Wire*> project_normal(brep::Wire* wire, brep::Solid* solid);
	
	// Projects a wire onto a surface along given direction. 
	// Returns all found intersections. 
	std::vector<brep::Wire*> project_linear(brep::Wire* wire, brep::Shell* surface, Vec3 direction);
	std::vector<brep::Wire*> project_linear(brep::Wire* wire, brep::Solid* solid, Vec3 direction);
	
	// Projects a wire onto a surface by making a point projection from point. 
	// Returns all found intersections. 
	std::vector<brep::Wire*> project_point(brep::Wire* wire, brep::Shell* surface, Vec3 point);
	std::vector<brep::Wire*> project_point(brep::Wire* wire, brep::Solid* solid, Vec3 point);
	
	
	//
	// other operations
	//
	
	brep::Solid* fillet(brep::Solid* solid, 
		std::vector<brep::Edge> edges, double size);
	brep::Solid* fillet(brep::Solid* solid, 
		std::vector<brep::Edge> edges, std::vector<double> size);
	
	brep::Shell* filling(int degree, std::vector<brep::Wire*> bounds, std::vector<brep::Wire*> supports);
	
	brep::Solid* hollow(brep::Solid* solid, 
		std::vector<brep::Face> openingFaces, double thickness, double tol=1e-3);
	
	//
	// other stuff
	//
	
	brep::Solid* heal(brep::Solid* solid, double tol=1e-3);
	
	
}
