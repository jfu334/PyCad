#include "objects.h"
#include "base.h"

#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>

namespace PyCadCpp
{
	namespace
	{
		std::vector<TopoDS_Shape> collectOfType(TopoDS_Shape shape, TopAbs_ShapeEnum type)
		{
			std::vector<TopoDS_Shape> result;
			for(TopExp_Explorer it(shape, type);it.More();it.Next())
				result.push_back(it.Current());
			return result;
		}
		
		std::vector<Face> collectFaces(TopoDS_Shape shape)
		{
			auto faces=collectOfType(shape, TopAbs_FACE);
			
			std::vector<Face> result;
			for(auto i: faces) result.push_back(Face(TopoDS::Face(i)));
			return result;
		}
		std::vector<Edge> collectEdges(TopoDS_Shape shape)
		{
			auto edges=collectOfType(shape, TopAbs_EDGE);
			
			std::vector<Edge> result;
			for(auto i: edges) result.push_back(Edge(TopoDS::Edge(i)));
			return result;
		}
		std::vector<Vertex> collectVertices(TopoDS_Shape shape)
		{
			auto vertices=collectOfType(shape, TopAbs_VERTEX);
			
			std::vector<Vertex> result;
			for(auto i: vertices) result.push_back(Vertex(TopoDS::Vertex(i)));
			return result;
		}
	}
	
	std::vector<Edge> Face::edges() {return collectEdges(_data);}
	std::vector<Vertex> Face::vertices() {return collectVertices(_data);}
	std::vector<Vertex> Edge::vertices() {return collectVertices(_data);}
	
	Vec3 Vertex::vec() const
	{
		auto pnt=BRep_Tool::Pnt(_data);
		return Vec3(pnt.X(), pnt.Y(), pnt.Z());
	}
	
	std::vector<Face> ModellingObjectBase::faces() {return collectFaces(_shape);}
	std::vector<Edge> ModellingObjectBase::edges() {return collectEdges(_shape);}
	std::vector<Vertex> ModellingObjectBase::vertices() {return collectVertices(_shape);}

	
	Solid::Solid(TopoDS_Shape shape)
	{
		if(shape.ShapeType()!=TopAbs_SOLID 
				&& shape.ShapeType()!=TopAbs_COMPSOLID 
				&& shape.ShapeType()!=TopAbs_COMPOUND)
			throw base::Exception("Unsupported shape type: "+base::shapeType(shape));
		_shape=shape;
	}
	Solid* Solid::copy()
	{
		auto result=new Solid(base::copy(_shape));
		result->setColor(_color);
		return result;
	}
	Solid* Solid::translate(double x, double y, double z)
	{
		_shape=base::translate(_shape, x, y, z); 
		return this;
	}
	Solid* Solid::rotate(double x, double y, double z)
	{
		_shape=base::rotate(_shape, x, y, z);
		return this;
	}
	Solid* Solid::mirror(double x, double y, double z)
	{
		_shape=base::mirror(_shape, x, y, z);
		return this;
	}
	
	
	Shell::Shell(TopoDS_Shape shape)
	{
		if(shape.ShapeType()!=TopAbs_SHELL && shape.ShapeType()!=TopAbs_FACE)
			throw base::Exception("Unsupported shape type: "+base::shapeType(shape));
		_shape=shape;
	}
	Shell* Shell::copy()
	{
		auto result=new Shell(base::copy(_shape));
		result->setColor(_color);
		return result;
	}
	Shell* Shell::translate(double x, double y, double z)
	{
		_shape=base::translate(_shape, x, y, z); 
		return this;
	}
	Shell* Shell::rotate(double x, double y, double z) 
	{
		_shape=base::rotate(_shape, x, y, z); 
		return this;
	}
	Shell* Shell::mirror(double x, double y, double z)
	{
		_shape=base::mirror(_shape, x, y, z);
		return this;
	}
	
	
	Wire::Wire(TopoDS_Shape shape)
	{
		if(shape.ShapeType()!=TopAbs_WIRE && shape.ShapeType()!=TopAbs_EDGE)
			throw base::Exception("Unsupported shape type: "+base::shapeType(shape));
		_shape=shape;
	}
	Wire::Wire(Edge edge)
	{
		_shape=base::copy(edge._data);
	}
	
	TopoDS_Wire Wire::shape_wire()
	{
		if(_shape.ShapeType()==TopAbs_EDGE)
		{
			auto makeWire=BRepLib_MakeWire();
			makeWire.Add(TopoDS::Edge(_shape));
			return makeWire.Wire();
		}
		else return TopoDS::Wire(_shape);
	}
	
	Wire* Wire::copy()
	{
		auto result=new Wire(base::copy(_shape));
		result->setColor(_color);
		return result;
	}
	
	Wire* Wire::translate(double x, double y, double z)
	{
		_shape=base::translate(_shape, x, y, z);
		return this;
	}
	Wire* Wire::rotate(double x, double y, double z) 
	{
		_shape=base::rotate(_shape, x, y, z); 
		return this;
	}
	Wire* Wire::mirror(double x, double y, double z)
	{
		_shape=base::mirror(_shape, x, y, z);
		return this;
	}
	
	Point::Point(Vec2 vec)
	{
		_shape=BRepBuilderAPI_MakeVertex(gp_Pnt(vec.x(), vec.y(), 0));
	}
	Point::Point(Vec3 vec)
	{
		_shape=BRepBuilderAPI_MakeVertex(gp_Pnt(vec.x(), vec.y(), vec.z()));
	}
	Point::Point(TopoDS_Shape shape)
	{
		if(shape.ShapeType()!=TopAbs_VERTEX)
			throw base::Exception("Unsupported shape type: "+base::shapeType(shape));
		_shape=shape;
	}
	Point* Point::copy()
	{
		auto result=new Point(base::copy(_shape));
		result->setColor(_color);
		return result;
	}
	Point* Point::translate(double x, double y, double z)
	{
		_shape=base::translate(_shape, x, y, z);
		return this;
	}
	Point* Point::rotate(double x, double y, double z) 
	{
		_shape=base::rotate(_shape, x, y, z); 
		return this;
	}
	Point* Point::mirror(double x, double y, double z)
	{
		_shape=base::mirror(_shape, x, y, z);
		return this;
	}
}
