#pragma once
#include "vec.h"

namespace PyCadCpp::viewer
{
	
	class NavigationHandler
	{
	public:
		
		NavigationHandler();
		
		// where the camera points to
		Vec3 target() {return _target;}
		void setTarget(Vec3 target) {_target=target;}
		
		// inquire current position of camera
		Vec3 camera();
		
		// inquire current up vector of camera 
		Vec3 up();
		
		// positioning properties of camera
		double elevation() {return _elevation;}
		void setElevation(double elevation) {_elevation=elevation;}
		
		double azimuth() {return _azimuth;}
		void setAzimuth(double azimuth) {_azimuth=azimuth;}
		
		double distance() {return _distance;}
		void setDistance(double distance) {_distance=distance;}
		
		// move/rotate view among all 3 dimensions
		void moveView(double x, double y, double z);
		void rotateView(double x, double y, double z);
		
		
	private:
		
		Vec3 _target;
		double _azimuth, _elevation;
		double _distance;
	};
	
}
