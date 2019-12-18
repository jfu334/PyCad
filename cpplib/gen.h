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
	// solid construction
	// 
	
	Solid* cube(
		double lx, double ly, double lz, 
		bool center=false, 
		bool center_x=false, bool center_y=false, bool center_z=false);
	
	Solid* cylinder(
		double r=NOT_SET, 
		double d=NOT_SET, 
		double h=NOT_SET);
	
	Solid* cone(
		double r1=NOT_SET, double d1=NOT_SET, 
		double r2=NOT_SET, double d2=NOT_SET, 
		double h=NOT_SET);
	
	Solid* sphere(
		double r=NOT_SET, double d=NOT_SET);
	
	//
	// shell construction
	//
	
	Shell* bezier_surface(int rows, int cols, std::vector<Vec3> points);
	
	//
	// wire construction
	//
	
	// spline continuity values: 
	// C0, G1, C1, G2, C2, C3, CN
	
	Wire* line_2d(std::vector<Vec2> points);
	Wire* bezier_2d(std::vector<Vec2> points);
	
	Wire* spline_2d(std::vector<Vec2> poles,
		std::vector<double> weights,
		std::vector<double> knots,
		int degree);

	
	Wire* line_3d(std::vector<Vec3> points);
	Wire* bezier_3d(std::vector<Vec3> points);
	
	Wire* spline_3d(std::vector<Vec3> poles,
		std::vector<double> weights,
		std::vector<double> knots,
		int degree);
	
	// Builds a helix.
	// d is the diameter, revolutions is number of turns 
	// and ascend is the height ascend per turn. 
	Wire* helix(
		double d,
		double revolutions, 
		double ascend);
	
	//
	// special stuff
	//
	
	enum class FontWeight {Regular, Bold, Italic, BoldItalic};
	
	Solid* text(
		std::string fontFamily, FontWeight fontWeight,
		double size, double elevation,
		std::string text);
	
}
