#pragma once
#include <cmath>

namespace PyCadCpp
{
	class Vec2
	{
	public:
		Vec2(){_x[0]=0; _x[1]=0;}
		Vec2(double x, double y) {_x[0]=x; _x[1]=y;}
		
		double x() const {return _x[0];}
		double y() const {return _x[1];}
		
		Vec2 operator+(const Vec2& other) const
		{
			return Vec2(x()+other.x(), y()+other.y());
		}
		Vec2 operator-(const Vec2& other) const
		{
			return Vec2(x()-other.x(), y()-other.y());
		}
		
		Vec2 operator*(double value) const
		{
			return Vec2(x()*value, y()*value);
		}
		
		double length() const
		{
			return sqrt(x()*x()+y()*y());
		}
		
		Vec2 norm() const
		{
			return (*this)*(1.0/length());
		}
		
	protected:
		double _x[2];
	};
	
	class Vec3
	{
	public:
		Vec3() {_x[0]=0; _x[1]=0; _x[2]=0;}
		Vec3(double x, double y, double z) {_x[0]=x; _x[1]=y; _x[2]=z;}
		
		double x() const {return _x[0];}
		double y() const {return _x[1];}
		double z() const {return _x[2];}
		
		Vec3 operator+(const Vec3& other) const
		{
			return Vec3(x()+other.x(), y()+other.y(), z()+other.z());
		}
		Vec3 operator-(const Vec3& other) const
		{
			return Vec3(x()-other.x(), y()-other.y(), z()-other.z());
		}
		Vec3 operator*(double value) const
		{
			return Vec3(x()*value, y()*value, z()*value);
		}
		
		Vec3 cross(const Vec3& other) const
		{
			return Vec3(
				y()*other.z()-z()*other.y(), 
				z()*other.x()-x()*other.z(), 
				x()*other.y()-y()*other.x());
		}
		
		double length() const
		{
			return sqrt(x()*x()+y()*y()+z()*z());
		}
		
		Vec3 norm() const
		{
			return *this*(1.0/length());
		}
		
		
	protected:
		double _x[3];
	};
	
	
}
