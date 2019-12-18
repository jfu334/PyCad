#pragma once
#include <vector>
#include "vec.h"
#include "color.h"

#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>

namespace PyCadCpp
{
	struct GeometryBase
	{
		GeometryBase() {}
		virtual TopoDS_Shape shape()=0;
	};
	
	/*
	 * Internal structure of objects
	 */
	
	struct Vertex : public GeometryBase
	{
		Vertex(TopoDS_Vertex data) : _data(data) {}
		Vec3 vec() const;
		
		TopoDS_Vertex _data;
		virtual TopoDS_Shape shape() {return _data;}
	};
	
	struct Edge : public GeometryBase
	{
		Edge(TopoDS_Edge data) : _data(data) {}
		std::vector<Vertex> vertices();
		
		TopoDS_Edge _data;
		virtual TopoDS_Shape shape() {return _data;}
	};
	
	struct Face : public GeometryBase
	{
		Face(TopoDS_Face data) : _data(data) {}
		
		std::vector<Edge> edges();
		std::vector<Vertex> vertices();
		
		TopoDS_Face _data;
		virtual TopoDS_Shape shape() {return _data;}
	};
	
	
	/*
	 * Modelling objects
	 */
	
	// next step:
	enum class ObjectType {Point, Wire, Shell, Solid};
	
	
	// common root for all modelling objects
	class ModellingObjectBase : public GeometryBase
	{
	public:
		virtual TopoDS_Shape shape() {return _shape;}
		
		Color color() {return _color;}
		void setColor(Color color) {_color=color;}
		
		std::vector<Face> faces();
		std::vector<Edge> edges();
		std::vector<Vertex> vertices();
	protected:
		TopoDS_Shape _shape;
		Color _color;
	};
	
	class Solid : public ModellingObjectBase
	{
	public:
		Solid(TopoDS_Shape shape);
		
		Solid* copy();
		Solid* translate(double x, double y, double z);
		Solid* rotate(double x, double y, double z);
		Solid* mirror(double x, double y, double z);
	};
	
	class Shell : public ModellingObjectBase
	{
	public:
		Shell(TopoDS_Shape shape);
		
		Shell* copy();
		Shell* translate(double x, double y, double z);
		Shell* rotate(double x, double y, double z);
		Shell* mirror(double x, double y, double z);
	};
	
	class Wire : public ModellingObjectBase
	{
	public:
		Wire(TopoDS_Shape shape);
		Wire(Edge edge);
		
		TopoDS_Wire shape_wire();
		
		Wire* copy();
		Wire* translate(double x, double y, double z);
		Wire* rotate(double x, double y, double z);
		Wire* mirror(double x, double y, double z);
	};
	
	class Point : public ModellingObjectBase
	{
	public:
		Point(TopoDS_Shape shape);
		Point(Vec2 vec);
		Point(Vec3 vec);
		
		Point* copy();
		Point* translate(double x, double y, double z);
		Point* rotate(double x, double y, double z);
		Point* mirror(double x, double y, double z);
	};
	
	

}
