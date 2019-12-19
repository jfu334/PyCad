#include "gen.h"
#include <cmath>


#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakePrism.hxx>

#include <Geom_BezierCurve.hxx>
#include <Geom_BezierSurface.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom2d_Line.hxx>
#include <Geom2d_TrimmedCurve.hxx>

#include <GeomAPI_Interpolate.hxx>
#include <GeomAPI_PointsToBSpline.hxx>

#include <Font_BRepFont.hxx>
#include <Font_BRepTextBuilder.hxx>
#include <Font_FontMgr.hxx>
#include <BRepLib.hxx>

#include "base.h"

namespace PyCadCpp::gen
{
	namespace 
	{
		bool isSet(double value)
		{
			return !std::isnan(value);
		}
		
		double optionalOneOf(const char* name1, double value1, const char* name2, double value2)
		{
			if(isSet(value1))
				return value1;
			else if(isSet(value2))
				return value2;
			else
				return NOT_SET;
		}
		double oneOf(const char* name1, double value1, const char* name2, double value2)
		{
			auto result=optionalOneOf(name1, value1, name2, value2);
			if(!isSet(result))
				throw "None of the required values is set!";
			return result;
		}
		
		void checkOneOf(const char* name1, double value1, const char* name2, double value2)
		{
			if(!isSet(value1) && !isSet(value2))
				throw "None of required values is set!";
			if(isSet(value1) && isSet(value2))
				throw "Only one value must be set!";
		}
		void checkIsSet(const char* name, double value)
		{
			if(!isSet(value))
				throw "Required argument not set!";
		}
		
		// return radius for the provided arguments
		double radiusOrDiameter(const char* suffix, double radius, double diameter)
		{
			if(!isSet(radius) && !isSet(diameter))
				throw "Neither radius nor diameter specified";
			if(isSet(radius) && isSet(diameter))
				throw "Both radius and diameter specified";
			
			if(isSet(radius))
				return radius;
			else
				return diameter/2;
		}
		
	}
	
	brep::Point* point_2d(Vec2 point)
	{
		return point_3d(Vec3(point.x(), point.y(), 0));
	}
	brep::Point* point_3d(Vec3 point)
	{
		auto shape=BRepBuilderAPI_MakeVertex(gp_Pnt(point.x(), point.y(), point.z()));
		return new brep::Point(shape);
	}
	
	brep::Solid* cube(double lx, double ly, double lz, 
		bool center, bool center_x, bool center_y, bool center_z)
	{
		auto makeApi=BRepPrimAPI_MakeBox(lx, ly, lz);
		auto result=new brep::Solid(makeApi.Solid());
		
		double tx=(center || center_x)? -lx/2: 0;
		double ty=(center || center_y)? -ly/2: 0;
		double tz=(center || center_z)? -lz/2: 0;
		result->translate(tx, ty, tz);
		
		return result;
	}
	
	brep::Solid* cylinder(double r, double d, double h)
	{
		double value_r=radiusOrDiameter("", r, d);
		checkIsSet("h", h);
		
		auto makeApi=BRepPrimAPI_MakeCylinder(value_r, h);
		auto result=new brep::Solid(makeApi.Solid());
		return result;
	}
	
	brep::Solid* sphere(double r, double d)
	{
		double value_r=radiusOrDiameter("", r, d);
		auto makeApi=BRepPrimAPI_MakeSphere(value_r);
		auto result=new brep::Solid(makeApi.Solid());
		return result;
	}
	
	brep::Solid* cone(double r1, double d1, double r2, double d2, double h)
	{
		double value_r1=radiusOrDiameter("1", r1, d1);
		double value_r2=radiusOrDiameter("2", r2, d2);
		checkIsSet("h", h);
		
		auto makeApi=BRepPrimAPI_MakeCone(value_r1, value_r2, h);
		auto result=new brep::Solid(makeApi.Solid());
		return result;
	}
	
	
	brep::Shell* bezier_surface(int rows, int cols, std::vector<Vec3> points)
	{
		// check array size
		if(points.size()!=rows*cols)
			throw new base::Exception("points must have rows*cols elements!");
		
		auto parr=TColgp_Array2OfPnt(0, rows-1, 0, cols-1);
		for(size_t i=0;i<rows*cols;i++)
			parr.SetValue(i/cols, i%cols, 
				gp_Pnt(points[i].x(), points[i].y(), points[i].z()));
		
		Handle(Geom_BezierSurface) surf=new Geom_BezierSurface(parr);
		auto face=BRepBuilderAPI_MakeFace(surf, 1e-3).Face();
		return new brep::Shell(face);
	}
	
	brep::Wire* line_2d(std::vector<Vec2> points)
	{
		std::vector<Vec3> p3d;
		for(auto i: points) p3d.push_back(Vec3(i.x(), i.y(), 0));
		return line_3d(p3d);
	}
	brep::Wire* bezier_2d(std::vector<Vec2> points)
	{
		std::vector<Vec3> p3d;
		for(auto i: points) p3d.push_back(Vec3(i.x(), i.y(), 0));
		return bezier_3d(p3d);
	}
	brep::Wire* spline_2d(std::vector<Vec2> poles,
		std::vector<double> weights,
		std::vector<double> knots,
		int degree)
	{
		std::vector<Vec3> p3d;
		for(auto i: poles) p3d.push_back(Vec3(i.x(), i.y(), 0));
		return spline_3d(p3d, weights, knots, degree);
	}
	
	brep::Wire* line_3d(std::vector<Vec3> points)
	{
		auto makeApi=BRepBuilderAPI_MakeWire();
		for(size_t i=0;i<points.size()-1;i++)
		{
			auto edge=BRepBuilderAPI_MakeEdge(
				gp_Pnt(points[i].x(), points[i].y(), points[i].z()), 
				gp_Pnt(points[i+1].x(), points[i+1].y(), points[i+1].z())).Edge();
			makeApi.Add(edge);
		}
		return new brep::Wire(makeApi.Wire());
	}

	brep::Wire* bezier_3d(std::vector<Vec3> points)
	{
		auto parr=TColgp_Array1OfPnt(1, points.size());
		for(size_t i=0;i<points.size();i++)
			parr.SetValue(1+i, gp_Pnt(points[i].x(), points[i].y(), points[i].z()));
		
		Handle(Geom_BezierCurve) curve=new Geom_BezierCurve(parr);
		
		auto edge=BRepBuilderAPI_MakeEdge(curve).Edge();
		auto makeWire=BRepBuilderAPI_MakeWire();
		makeWire.Add(edge);
		
		return new brep::Wire(makeWire.Wire());
	}
	
	brep::Wire* spline_3d(std::vector<Vec3> poles,
		std::vector<double> weights,
		std::vector<double> knots,
		int degree)
	{
		if(knots.size()!=poles.size()+degree+1)
			throw new base::Exception("Invalid number of knots: Must be number of poles + degree + 1!");
		
		// create knots and multiplicities from input values
		std::vector<double> knots_unique;
		std::vector<int> multiplicities;
		
		for(auto i: knots)
		{
			if(knots_unique.size()==0 || i!=knots_unique[knots_unique.size()-1])
			{
				knots_unique.push_back(i);
				multiplicities.push_back(1);
			}
			else
			{
				multiplicities[multiplicities.size()-1]++;
			}
		}
		
		
		auto arr_poles=TColgp_Array1OfPnt(0, poles.size()-1);
		auto arr_weights=TColStd_Array1OfReal(0, weights.size()-1);
		auto arr_knots=TColStd_Array1OfReal(0, knots_unique.size()-1);
		auto arr_mult=TColStd_Array1OfInteger(0, multiplicities.size()-1);
		
		for(size_t i=0;i<poles.size();i++)
			arr_poles.SetValue(i, gp_Pnt(poles[i].x(), poles[i].y(), poles[i].z()));
		for(size_t i=0;i<weights.size();i++)
			arr_weights.SetValue(i, weights[i]);
		for(size_t i=0;i<knots_unique.size();i++)
			arr_knots.SetValue(i, knots_unique[i]);
		for(size_t i=0;i<multiplicities.size();i++)
			arr_mult.SetValue(i, multiplicities[i]);
		
		Handle(Geom_BSplineCurve) bspline=new Geom_BSplineCurve(arr_poles, arr_weights, arr_knots, arr_mult, degree);
		
		auto edge=BRepBuilderAPI_MakeEdge(bspline).Edge();
		auto makeWire=BRepBuilderAPI_MakeWire();
		makeWire.Add(edge);
		
		return new brep::Wire(makeWire.Wire());
	}
	
	brep::Wire* helix(
		double d,
		double revolutions, 
		double ascend)
	{
		double x=ascend/(2*M_PI);
		double lengthPerRevolution=(2*M_PI)*sqrt(1+x*x);
		
		Handle(Geom_CylindricalSurface) cyl=
			new Geom_CylindricalSurface(gp_Ax3(gp_Pnt(0, 0, 0), gp_Dir(0, 0, 1)), d/2);
		Handle(Geom2d_Line) line=
			new Geom2d_Line(gp_Ax2d(gp_Pnt2d(0, 0), gp_Dir2d(1, ascend/(2*M_PI))));
		
		// make one edge for a full revolution
		auto makeEdgeFull=BRepBuilderAPI_MakeEdge(line, cyl, 0, lengthPerRevolution);
		TopoDS_Edge edgeFull=makeEdgeFull.Edge();
		BRepLib::BuildCurve3d(edgeFull);
		
		// make a wire and add all full revs to it
		auto makeWire=BRepLib_MakeWire();
		
		for(int i=0;i<floor(revolutions)-0.5;i++)
		{
			auto edge=TopoDS::Edge(base::translate(edgeFull, 0, 0, ascend*i));
			makeWire.Add(edge);
		}
		
		// make one edge for partial final revolution
		double p=revolutions-floor(revolutions);
		if(p>0)
		{
			
			auto makeEdgePartial(BRepBuilderAPI_MakeEdge(line, cyl, 0, p*lengthPerRevolution));
			TopoDS_Edge edgePartial=makeEdgePartial.Edge();
			BRepLib::BuildCurve3d(edgePartial);
			
			auto edge=TopoDS::Edge(base::translate(edgePartial, 0, 0, ascend*floor(revolutions)));
			makeWire.Add(edge);
		}
		
		return new brep::Wire(makeWire.Wire());
	}
	
	
	brep::Solid* text(
		std::string fontFamily, FontWeight fontWeight, double size, double elevation,
		std::string text)
	{
		Font_FontAspect aspect;
		if(fontWeight==FontWeight::Regular)
			aspect=Font_FA_Regular;
		else if(fontWeight==FontWeight::Bold)
			aspect=Font_FA_Bold;
		else if(fontWeight==FontWeight::Italic)
			aspect=Font_FA_Italic;
		else if(fontWeight==FontWeight::BoldItalic)
			aspect=Font_FA_BoldItalic;
		else throw new base::Exception("Invalid font weight!");
		
		// build the font
		Font_BRepFont font;
		if(!font.Init(fontFamily.c_str(), aspect, size))
			throw new base::Exception("Failed to initialize font!");
		
		// create shape
		Font_BRepTextBuilder textBuilder;
		auto shape=textBuilder.Perform(font, text.c_str());
		
		// extrude to object
		auto makePrism=BRepPrimAPI_MakePrism(shape, gp_Vec(0, 0, elevation), false);
		
		return new brep::Solid(makePrism.Shape());
		
	}
	
	
}
