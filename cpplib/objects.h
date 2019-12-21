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
	// Base class for all geometry that can be visualized. 
	struct GeometryBase
	{
		GeometryBase() {}
		virtual TopoDS_Shape shape()=0;
	};
	
	// Objects represented by boundaries. 
	namespace brep
	{
	
		/*
		* Internal structure of objects
		*/
		
		struct Vertex : public GeometryBase
		{
			Vertex(TopoDS_Vertex data) : _data(data) {}
			Vec3 vec() const;
			
			virtual TopoDS_Shape shape() {return _data;}
			virtual TopoDS_Vertex shape_vertex() {return _data;}
			
		private:
			TopoDS_Vertex _data;
		};
		
		struct Edge : public GeometryBase
		{
			Edge(TopoDS_Edge data) : _data(data) {}
			std::vector<Vertex> vertices();
			
			virtual TopoDS_Shape shape() {return _data;}
			virtual TopoDS_Edge shape_edge() {return _data;}
			
		private:
			TopoDS_Edge _data;
		};
		
		struct Face : public GeometryBase
		{
			Face(TopoDS_Face data) : _data(data) {}
			
			std::vector<Edge> edges();
			std::vector<Vertex> vertices();
			
			virtual TopoDS_Shape shape() {return _data;}
			TopoDS_Face shape_face() {return _data;}
			
		private:
			TopoDS_Face _data;
		};
		
		
		/*
		* Modelling objects
		*/
		
		// common root for all modelling objects
		class Object : public GeometryBase
		{
		public:
			enum class Type {Point, Wire, Shell, Solid};
			
			Object(Type type);
			Object(Type type, TopoDS_Shape shape);
			Object(const Object* other);
			
			// get internal structure
			std::vector<Face> faces();
			std::vector<Edge> edges();
			std::vector<Vertex> vertices();
			
			// Color used for visual representation. 
			Color color() const {return _color;}
			Object* setColor(Color color) {_color=color; return this;}
			
			// Material used. Not used by visualization but only by export plugins. 
			std::string material() const {return _material;}
			Object* setMaterial(std::string material) {_material=material; return this;}
			
			// access to the underlying shape
			virtual TopoDS_Shape shape() {return _shape;}
			
		protected:
			
			// internal methods for transformation
			void intTranslate(double x, double y, double z);
			void intRotate(double x, double y, double z);
			void intMirror(double x, double y, double z);
			
			Type _type;
			TopoDS_Shape _shape;
			
			Color _color;
			std::string _material;
		};
		
		class Solid : public Object
		{
		public:
			Solid(TopoDS_Shape shape) : Object(Object::Type::Solid, shape) {}
			Solid(const Solid* other) : Object(other) {}
			
			Solid* copy() {return new Solid(this);}
			Solid* translate(double x, double y, double z) {intTranslate(x, y, z); return this;}
			Solid* rotate(double x, double y, double z) {intRotate(x, y, z); return this;}
			Solid* mirror(double x, double y, double z) {intMirror(x, y, z); return this;}
		};
		
		class Shell : public Object
		{
		public:
			Shell(TopoDS_Shape shape) : Object(Object::Type::Shell, shape) {}
			Shell(const Shell* other) : Object(other) {}
			Shell(Face face) : Object(Object::Type::Shell, face.shape()) {}
			
			Shell* copy() {return new Shell(this);}
			Shell* translate(double x, double y, double z) {intTranslate(x, y, z); return this;}
			Shell* rotate(double x, double y, double z) {intRotate(x, y, z); return this;}
			Shell* mirror(double x, double y, double z) {intMirror(x, y, z); return this;}
		};
		
		class Wire : public Object
		{
		public:
			Wire(Edge edge);
			
			Wire(TopoDS_Shape shape) : Object(Object::Type::Wire, shape) {}
			Wire(const Wire* other) : Object(other) {}
			
			Wire* copy() {return new Wire(this);}
			Wire* translate(double x, double y, double z) {intTranslate(x, y, z); return this;}
			Wire* rotate(double x, double y, double z) {intRotate(x, y, z); return this;}
			Wire* mirror(double x, double y, double z) {intMirror(x, y, z); return this;}
		};
		
		class Point : public Object
		{
		public:
			Point(TopoDS_Shape shape) : Object(Object::Type::Point, shape) {}
			Point(const Point* other) : Object(other) {}
			
			Point* copy() {return new Point(this);}
			Point* translate(double x, double y, double z) {intTranslate(x, y, z); return this;}
			Point* rotate(double x, double y, double z) {intRotate(x, y, z); return this;}
			Point* mirror(double x, double y, double z) {intMirror(x, y, z); return this;}
		};
	}
	
	// Objects represented by numerical vertices, edges and faces. 
	namespace mesh
	{
		// TODO
	}
	
	// Objects used for annotation purposes
	namespace annotation
	{
		class Text
		{
		public:
			Text() {}
			Text(Vec3 position, std::string text)
				: _position(position), _text(text) {}
			
			Vec3 position() const {return _position;}
			void setPosition(Vec3 position) {_position=position;}
			
			std::string text() const {return _text;}
			void setText(std::string text) {_text=text;}
			
			Color color() const {return _color;}
			void setColor(Color color) {_color=color;}
			
		private:
			Vec3 _position;
			std::string _text;
			Color _color;
		};
	}

}
