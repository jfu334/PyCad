%Module(name=PyCadCpp, keyword_arguments="Optional")
%HideNamespace(name=PyCadCpp)

%Import QtCore/QtCoremod.sip
%Import QtOpenGL/QtOpenGLmod.sip

%Include basetypes.sip

//
// open cascade objects
//

class TopoDS_Shape
{
	%TypeHeaderCode
	#include <TopoDS_Shape.hxx>
	%End
};
class TopoDS_Wire
{
	%TypeHeaderCode
	#include <TopoDS_Wire.hxx>
	%End
};

//
// our objects
//

namespace PyCadCpp
{
	%Exception Exception(SIP_Exception) /PyName=StdException/
	{
		%TypeHeaderCode
		#include "base.h"
		%End
		
		%RaiseCode
			const char *detail = sipExceptionRef.what();
			SIP_BLOCK_THREADS
			PyErr_SetString(sipException_TipShotgun_Shotgun_Exception, detail);
			SIP_UNBLOCK_THREADS
		%End
	};

	class Vec2
	{
		%TypeHeaderCode
		#include "vec.h"
		%End
		
		public:
			Vec2(double x, double y);
			double x();
			double y();
			
			Vec2 operator+(const Vec2& other) const;
			Vec2 operator-(const Vec2& other) const;
			Vec2 operator*(double value) const;
			
			double length() const;
			Vec2 norm() const;
	};

	class Vec3
	{
		%TypeHeaderCode
		#include "vec.h"
		%End
		public:
			Vec3(double x, double y, double z);
			double x();
			double y();
			double z();
			
			Vec3 operator+(const Vec3& other) const;
			Vec3 operator-(const Vec3& other) const;
			Vec3 operator*(double value) const;
			Vec3 cross(const Vec3& other) const;
			
			double length() const;
			Vec3 norm() const;
	};

	class Color
	{
		%TypeHeaderCode
		#include "color.h"
		%End
		public:
			Color();
			Color(float r, float g, float b);
			Color(float r, float g, float b, float alpha);
			
			float r();
			float g();
			float b();
			float alpha();
	};
	
	struct GeometryBase
	{
		%TypeHeaderCode
		#include "objects.h"
		%End
		
		virtual TopoDS_Shape shape()=0;
	};

	class Vertex : public PyCadCpp::GeometryBase
	{
		%TypeHeaderCode
		#include "objects.h"
		%End
		
		public:
			Vec3 vec() const;
		private:
			Vertex();
	};
	
	class Edge : public PyCadCpp::GeometryBase
	{
		%TypeHeaderCode
		#include "objects.h"
		%End
		
		public:
			std::vector<PyCadCpp::Vertex> vertices();
		private:
			Edge();
	};
	
	class Face : public PyCadCpp::GeometryBase
	{
		%TypeHeaderCode
		#include "objects.h"
		%End
		
		public:
			std::vector<PyCadCpp::Edge> edges();
			std::vector<PyCadCpp::Vertex> vertices();
		private:
			Face();
	};
	
	class ModellingObjectBase : public PyCadCpp::GeometryBase
	{
		%TypeHeaderCode
		#include "objects.h"
		%End
		
		public:
			virtual TopoDS_Shape shape();
			
			Color color();
			void setColor(Color color);
			
			std::vector<PyCadCpp::Face> faces();
			std::vector<PyCadCpp::Edge> edges();
			std::vector<PyCadCpp::Vertex> vertices();
	};
	
	class Solid : public PyCadCpp::ModellingObjectBase
	{
		%TypeHeaderCode
		#include "objects.h"
		%End
		
		public:
			Solid* copy();
			Solid* translate(double x, double y, double z);
			Solid* rotate(double x, double y, double z);
			Solid* mirror(double x, double y, double z);
		private:
			Solid (TopoDS_Shape shape);
	};
	
	class Shell : public PyCadCpp::ModellingObjectBase
	{
		%TypeHeaderCode
		#include "objects.h"
		%End
		
		public:
			Shell* copy();
			Shell* translate(double x, double y, double z);
			Shell* rotate(double x, double y, double z);
			Shell* mirror(double x, double y, double z);
		private:
			Shell (TopoDS_Shape shape);
	};
	
	class Wire : public PyCadCpp::ModellingObjectBase
	{
		%TypeHeaderCode
		#include "objects.h"
		%End
		
		public:
			Wire(Edge edge);
			
			Wire* copy();
			Wire* translate(double x, double y, double z);
			Wire* rotate(double x, double y, double z);
			Wire* mirror(double x, double y, double z);
		private:
			Wire (TopoDS_Shape shape);
	};
	
	class Point : public PyCadCpp::ModellingObjectBase
	{
		%TypeHeaderCode
		#include "objects.h"
		%End
		
		public:
			Point(Vec2 vec);
			Point(Vec3 vec);
			
			Point* copy();
			Point* translate(double x, double y, double z);
			Point* rotate(double x, double y, double z);
			Point* mirror(double x, double y, double z);
		private:
			Point(TopoDS_Shape shape);
	};

	namespace gen
	{
		%TypeHeaderCode
		#include "gen.h"
		%End
		
		Solid* cube(
			double lx, double ly, double lz, 
			bool center=false, 
			bool center_x=false, bool center_y=false, bool center_z=false);
		
		Solid* cylinder(
			double r=PyCadCpp::gen::NOT_SET, 
			double d=PyCadCpp::gen::NOT_SET, 
			double h=PyCadCpp::gen::NOT_SET);

		Solid* sphere(
			double r=PyCadCpp::gen::NOT_SET, double d=PyCadCpp::gen::NOT_SET);
		
		Solid* cone(
			double r1=PyCadCpp::gen::NOT_SET, double d1=PyCadCpp::gen::NOT_SET, 
			double r2=PyCadCpp::gen::NOT_SET, double d2=PyCadCpp::gen::NOT_SET, 
			double h=PyCadCpp::gen::NOT_SET);
	
		Shell* bezier_surface(int rows, int cols, std::vector<PyCadCpp::Vec3> points);
		
		Wire* line_2d(std::vector<PyCadCpp::Vec2> points);
		Wire* line_3d(std::vector<PyCadCpp::Vec3> points);
		
		Wire* bezier_2d(std::vector<PyCadCpp::Vec2> points);
		Wire* bezier_3d(std::vector<PyCadCpp::Vec3> points);
		
		Wire* spline_2d(std::vector<PyCadCpp::Vec2> poles,
			std::vector<double> weights,
			std::vector<double> knots,
			int degree);
		
		Wire* spline_3d(std::vector<PyCadCpp::Vec3> poles,
			std::vector<double> weights,
			std::vector<double> knots,
			int degree);
		
		Wire* helix(
			double d,
			double revolutions, 
			double ascend);
		
		
		enum class FontWeight {Regular, Bold, Italic, BoldItalic};
		
		Solid* text(
			std::string fontFamily, FontWeight fontWeight, double size, double elevation,
			std::string text);
	};
	
	namespace op
	{
		%TypeHeaderCode
		#include "op.h"
		%End
		
		Shell* wireToShell(Wire* curve);
		
		Solid* fuse(Solid* obj1, Solid* obj2);
		Solid* union_(Solid* obj1, Solid* obj2);
		Solid* difference(Solid* obj1, Solid* obj2);
		Solid* cut(Solid* obj1, Solid* obj2);
		Solid* intersect(Solid* obj1, Solid* obj2);
		
		Wire* fuse(Wire* obj1, Wire* obj2);
		Wire* union_(Wire* obj1, Wire* obj2);
		
		Shell* extrude_linear(Wire* surface, double h);
		Shell* extrude_linear(Wire* surface, Vec3 direction);
		
		Solid* extrude_linear(Shell* surface, double h);
		Solid* extrude_linear(Shell* surface, Vec3 direction);
		
		Solid* extrude_rotate(Shell* surface, double angle, Vec3 axis);
		
		Shell* pipe_shell(Wire* profile, Wire* spine, Wire* spineNormal=nullptr);
		Solid* pipe_solid(Wire* profile, Wire* spine, Wire* spineNormal=nullptr);
		
		Solid* fillet(Solid* solid, std::vector<PyCadCpp::Edge> edges, double size);
		Solid* fillet(Solid* solid, std::vector<PyCadCpp::Edge> edges, std::vector<double> size);
		
		Solid* hollow(Solid* solid, std::vector<PyCadCpp::Face> openingFaces, 
			double thickness, double tol=1e-3);
			
		Solid* heal(Solid* solid, double tol=1e-3);
	};
	
	namespace calc
	{
		%TypeHeaderCode
		#include "calc.h"
		%End
		
		double distance(GeometryBase& obj1, GeometryBase& obj2, double max=1e9);
		double distance(Vec3 obj1, GeometryBase& obj2, double max=1e9);
		double distance(GeometryBase& obj1, Vec3 obj2, double max=1e9);
	};
	
	namespace export_
	{
		%TypeHeaderCode
		#include "export.h"
		%End
		
		void export_stl(std::string filename, Solid* object, double deflection);
	};
	
	namespace viewer
	{
		%TypeHeaderCode
		#include "viewer/qtviewer.h"
		%End
		
		class ViewObject
		{
		public:
			ViewObject(PyCadCpp::ModellingObjectBase* object);
			PyCadCpp::ModellingObjectBase* object();
			
			PyCadCpp::Color color();
			void setColor(PyCadCpp::Color color);
		};
		
		class QtViewer : public QGLWidget
		{
		public:
			QtViewer();
			
			void init();
			void update();
			
			void addObject(ViewObject* object);
			void removeObject(ViewObject* object);
			void clearObjects();
		};
	};
	
};
