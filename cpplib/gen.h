#pragma once
#include <limits>
#include <vector>
#include <string>

#include "objects.h"
#include "vec.h"

namespace PyCadCpp::gen
{
	const double NOT_SET=std::numeric_limits<double>::quiet_NaN();
	
	//
	// point construction
	//
	
	brep::Point* point_2d(Vec2 point);
	brep::Point* point_3d(Vec3 point);
	
	//
	// wire construction
	//
	
	brep::Wire* line_2d(std::vector<Vec2> points);
	brep::Wire* bezier_2d(std::vector<Vec2> points);
	
	brep::Wire* spline_2d(std::vector<Vec2> poles,
		std::vector<double> weights,
		std::vector<double> knots,
		int degree);

	
	brep::Wire* line_3d(std::vector<Vec3> points);
	brep::Wire* bezier_3d(std::vector<Vec3> points);
	
	brep::Wire* spline_3d(std::vector<Vec3> poles,
		std::vector<double> weights,
		std::vector<double> knots,
		int degree);
	
	// Builds a helix.
	// d is the diameter, revolutions is number of turns 
	// and ascend is the height ascend per turn. 
	brep::Wire* helix(
		double d,
		double revolutions, 
		double ascend);

	//
	// shell construction
	//
	
	brep::Shell* bezier_surface(int rows, int cols, std::vector<Vec3> points);
	
	//
	// solid construction
	// 
	
	brep::Solid* cube(
		double lx, double ly, double lz, 
		bool center=false, 
		bool center_x=false, bool center_y=false, bool center_z=false);
	
	brep::Solid* cylinder(
		double r=NOT_SET, 
		double d=NOT_SET, 
		double h=NOT_SET);
	
	brep::Solid* cone(
		double r1=NOT_SET, double d1=NOT_SET, 
		double r2=NOT_SET, double d2=NOT_SET, 
		double h=NOT_SET);
	
	brep::Solid* sphere(
		double r=NOT_SET, double d=NOT_SET);
	

	
	//
	// special stuff
	//
	
	enum class FontWeight {Regular, Bold, Italic, BoldItalic};
	
	brep::Solid* text(
		std::string fontFamily, FontWeight fontWeight,
		double size, double elevation,
		std::string text);
	
}
