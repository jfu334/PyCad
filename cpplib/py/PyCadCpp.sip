%Module(name=PyCadCpp, keyword_arguments="All")
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
	
	namespace colors
	{
		%TypeHeaderCode
		#include "color.h"
		%End
		
		const Color red;
		const Color green;
		const Color blue;
		
		const Color yellow;
		const Color cyan;
		const Color pink;
	};
	
	struct GeometryBase
	{
		%TypeHeaderCode
		#include "objects.h"
		%End
		
		virtual TopoDS_Shape shape()=0;
	};

	namespace brep
	{
	
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
				std::vector<PyCadCpp::brep::Vertex> vertices();
			private:
				Edge();
		};
		
		class Face : public PyCadCpp::GeometryBase
		{
			%TypeHeaderCode
			#include "objects.h"
			%End
			
			
			public:
				std::vector<PyCadCpp::brep::Edge> edges();
				std::vector<PyCadCpp::brep::Vertex> vertices();
			private:
				Face();
		};
		
		class Object : public PyCadCpp::GeometryBase
		{
			%TypeHeaderCode
			#include "objects.h"
			%End
			
			public:
				enum class Type {Point, Wire, Shell, Solid};
				
				Object(Type type);
				Object(Type type, TopoDS_Shape shape);
				
				virtual TopoDS_Shape shape();
				
				Color color();
				void setColor(Color color);
				
				std::vector<PyCadCpp::brep::Face> faces();
				std::vector<PyCadCpp::brep::Edge> edges();
				std::vector<PyCadCpp::brep::Vertex> vertices();
		};
		
		class Solid : public PyCadCpp::brep::Object
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
		
		class Shell : public PyCadCpp::brep::Object
		{
			%TypeHeaderCode
			#include "objects.h"
			%End
			
			public:
				Shell(Face face);
				
				Shell* copy();
				Shell* translate(double x, double y, double z);
				Shell* rotate(double x, double y, double z);
				Shell* mirror(double x, double y, double z);
			private:
				Shell (TopoDS_Shape shape);
		};
		
		class Wire : public PyCadCpp::brep::Object
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
		
		class Point : public PyCadCpp::brep::Object
		{
			%TypeHeaderCode
			#include "objects.h"
			%End
			
			public:
				Point* copy();
				Point* translate(double x, double y, double z);
				Point* rotate(double x, double y, double z);
				Point* mirror(double x, double y, double z);
			private:
				Point(TopoDS_Shape shape);
		};
	};
	
	
	namespace gen
	{
		%TypeHeaderCode
		#include "gen.h"
		%End
		

		brep::Point* point_2d(Vec2 point);
		brep::Point* point_3d(Vec3 point);
		
		brep::Solid* cube(
			double lx, double ly, double lz, 
			bool center=false, 
			bool center_x=false, bool center_y=false, bool center_z=false);
		
		brep::Solid* cylinder(
			double r=PyCadCpp::gen::NOT_SET, 
			double d=PyCadCpp::gen::NOT_SET, 
			double h=PyCadCpp::gen::NOT_SET);

		brep::Solid* sphere(
			double r=PyCadCpp::gen::NOT_SET, double d=PyCadCpp::gen::NOT_SET);
		
		brep::Solid* cone(
			double r1=PyCadCpp::gen::NOT_SET, double d1=PyCadCpp::gen::NOT_SET, 
			double r2=PyCadCpp::gen::NOT_SET, double d2=PyCadCpp::gen::NOT_SET, 
			double h=PyCadCpp::gen::NOT_SET);
	
		brep::Shell* bezier_surface(int rows, int cols, std::vector<PyCadCpp::Vec3> points);
		
		brep::Wire* line_2d(std::vector<PyCadCpp::Vec2> points);
		brep::Wire* line_3d(std::vector<PyCadCpp::Vec3> points);
		
		brep::Wire* bezier_2d(std::vector<PyCadCpp::Vec2> points);
		brep::Wire* bezier_3d(std::vector<PyCadCpp::Vec3> points);
		
		brep::Wire* spline_2d(std::vector<PyCadCpp::Vec2> poles,
			std::vector<double> weights,
			std::vector<double> knots,
			int degree);
		
		brep::Wire* spline_3d(std::vector<PyCadCpp::Vec3> poles,
			std::vector<double> weights,
			std::vector<double> knots,
			int degree);
		
		brep::Wire* helix(
			double d,
			double revolutions, 
			double ascend);
		
		
		enum class FontWeight {Regular, Bold, Italic, BoldItalic};
		
		brep::Solid* text(
			std::string fontFamily, FontWeight fontWeight, double size, double elevation,
			std::string text);
	};
	
	namespace op
	{
		%TypeHeaderCode
		#include "op.h"
		%End
		
		brep::Shell* wireToShell(brep::Wire* obj);
		brep::Solid* shellToSolid(brep::Shell* shell);
		
		brep::Shell* reverse(brep::Shell* shell);
	
		brep::Solid* fuse(brep::Solid* obj1, brep::Solid* obj2);
		brep::Shell* fuse(brep::Shell* obj1, brep::Shell* obj2);
		brep::Wire* fuse(brep::Wire* obj1, brep::Wire* obj2);
		
		brep::Solid* cut(brep::Solid* obj1, brep::Solid* obj2);
		brep::Solid* common(brep::Solid* obj1, brep::Solid* obj2);

		
		
		brep::Shell* extrude_linear(brep::Wire* surface, double h);
		brep::Shell* extrude_linear(brep::Wire* surface, Vec3 direction);
		
		brep::Solid* extrude_linear(brep::Shell* surface, double h);
		brep::Solid* extrude_linear(brep::Shell* surface, Vec3 direction);

		brep::Shell* extrude_rotate(brep::Wire* wire, double angle, Vec3 axis);
		brep::Solid* extrude_rotate(brep::Shell* surface, double angle, Vec3 axis);
		
		brep::Shell* pipe_shell(brep::Wire* profile, brep::Wire* spine, brep::Wire* spineNormal=nullptr);
		brep::Solid* pipe_solid(brep::Wire* profile, brep::Wire* spine, brep::Wire* spineNormal=nullptr);
		
		std::vector<PyCadCpp::brep::Wire*> project_normal(brep::Wire* wire, brep::Shell* surface);
		
		std::vector<PyCadCpp::brep::Wire*> project_linear(brep::Wire* wire, brep::Shell* surface, Vec3 direction);
		std::vector<PyCadCpp::brep::Wire*> project_point(brep::Wire* wire, brep::Shell* surface, Vec3 point);
		
		brep::Solid* fillet(brep::Solid* solid, 
			std::vector<PyCadCpp::brep::Edge> edges, double size);
		brep::Solid* fillet(brep::Solid* solid, 
			std::vector<PyCadCpp::brep::Edge> edges, std::vector<double> size);
		
		brep::Shell* filling(int degree, std::vector<PyCadCpp::brep::Wire*> bounds, std::vector<PyCadCpp::brep::Wire*> supports);
		
		brep::Solid* hollow(brep::Solid* solid, 
			std::vector<PyCadCpp::brep::Face> openingFaces, double thickness, double tol=1e-3);
		
		brep::Solid* heal(brep::Solid* solid, double tol=1e-3);
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
		
		void export_stl(std::string filename, brep::Solid* object, double deflection, double adeflection=0.5);
	};
	
	namespace viewer
	{
		%TypeHeaderCode
		#include "viewer/qtviewer.h"
		%End
		
		class ViewObject
		{
		public:
			ViewObject(PyCadCpp::GeometryBase* object);
			PyCadCpp::GeometryBase* object();
			
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
			
			void saveImage(std::string path);
			
			void setTarget(double x, double y, double z);
			void setCamera(double azimuth, double elevation, double distance);
		};
	};
	
};
