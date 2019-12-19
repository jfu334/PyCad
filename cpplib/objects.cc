#include "objects.h"
#include "base.h"

#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include <TopExp_Explorer.hxx>

namespace PyCadCpp
{
	namespace brep
	{
		
		namespace
		{
			std::vector<TopoDS_Shape> collectOfType(TopoDS_Shape& shape, TopAbs_ShapeEnum type)
			{
				std::vector<TopoDS_Shape> result;
				for(TopExp_Explorer it(shape, type);it.More();it.Next())
					result.push_back(it.Current());
				return result;
			}
			
			std::vector<Face> collectFaces(TopoDS_Shape& shape)
			{
				auto faces=collectOfType(shape, TopAbs_FACE);
				
				std::vector<Face> result;
				for(auto i: faces) result.push_back(Face(TopoDS::Face(i)));
				return result;
			}
			std::vector<Edge> collectEdges(TopoDS_Shape& shape)
			{
				auto edges=collectOfType(shape, TopAbs_EDGE);
				
				std::vector<Edge> result;
				for(auto i: edges) result.push_back(Edge(TopoDS::Edge(i)));
				return result;
			}
			std::vector<Vertex> collectVertices(TopoDS_Shape& shape)
			{
				auto vertices=collectOfType(shape, TopAbs_VERTEX);
				
				std::vector<Vertex> result;
				for(auto i: vertices) result.push_back(Vertex(TopoDS::Vertex(i)));
				return result;
			}
		}
	
		std::vector<Edge> Face::edges() 
		{
			return collectEdges(_data);
		}
		std::vector<Vertex> Face::vertices()
		{
			return collectVertices(_data);
		}
		
		std::vector<Vertex> Edge::vertices() {return collectVertices(_data);}
		
		Vec3 Vertex::vec() const
		{
			auto pnt=BRep_Tool::Pnt(_data);
			return Vec3(pnt.X(), pnt.Y(), pnt.Z());
		}
		
		std::vector<Face> Object::faces() {return collectFaces(_shape);}
		std::vector<Edge> Object::edges() {return collectEdges(_shape);}
		std::vector<Vertex> Object::vertices() {return collectVertices(_shape);}
	
		Object::Object(Type type) : _type(type) {}
		
		Object::Object(Object::Type type, TopoDS_Shape shape)
		{
			// DEBUG: Uncomment to allow any shape type in objects. 
// 			if(true) {} else 
			if(type==Object::Type::Solid)
			{
				if(shape.ShapeType()!=TopAbs_SOLID 
					&& shape.ShapeType()!=TopAbs_COMPSOLID 
					&& shape.ShapeType()!=TopAbs_COMPOUND)
				throw base::Exception("Unsupported shape type: "+base::shapeType(shape));
			}
			else if(type==Object::Type::Shell)
			{
				if(shape.ShapeType()!=TopAbs_SHELL 
					&& shape.ShapeType()!=TopAbs_FACE
					&& shape.ShapeType()!=TopAbs_COMPOUND)
					throw base::Exception("Unsupported shape type: "+base::shapeType(shape));
			}
			else if(type==Object::Type::Wire)
			{
				if(shape.ShapeType()!=TopAbs_WIRE && shape.ShapeType()!=TopAbs_EDGE)
					throw base::Exception("Unsupported shape type: "+base::shapeType(shape));
			}
			else if(type==Object::Type::Point)
			{
				if(shape.ShapeType()!=TopAbs_VERTEX)
					throw base::Exception("Unsupported shape type: "+base::shapeType(shape));
			}
			else throw new base::Exception("Invalid type!");
			
			_shape=shape;
		}
	
		Object::Object(const Object* other)
		{
			_shape=base::copy(other->_shape);
			_color=other->_color;
			_material=other->_material;
		}
	
		void Object::intTranslate(double x, double y, double z)
		{
			_shape=base::translate(_shape, x, y, z); 
		}
		void Object::intRotate(double x, double y, double z) 
		{
			_shape=base::rotate(_shape, x, y, z); 
		}
		void Object::intMirror(double x, double y, double z)
		{
			_shape=base::mirror(_shape, x, y, z);
		}
		
		Wire::Wire(Edge edge) : Object(Object::Type::Wire)
		{
			_shape=edge.shape();
		}
	
	}
}
