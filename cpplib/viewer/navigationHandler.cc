#include "navigationHandler.h"
#include <cmath>
#include <iostream>

namespace PyCadCpp::viewer
{
	
	NavigationHandler::NavigationHandler()
	{
		_target=Vec3(0, 0, 0);
		_azimuth=0;
		_elevation=0;
		_distance=10;
	}
	
	void NavigationHandler::moveView(double x, double y, double z)
	{
		Vec3 fwd=(_target-camera()).norm();
		Vec3 up=this->up();
		Vec3 left=fwd.cross(up).norm();
		
		Vec3 movement=left*(-x)*_distance+up*y*_distance+fwd*z*_distance;
		_target=_target+movement;
	}
	void NavigationHandler::rotateView(double x, double y, double z)
	{
		_azimuth-=(x*2*3.141592653);
		_elevation+=(y*2*3.141592653);
		_distance*=pow(4, z);
	}
	
	Vec3 NavigationHandler::camera()
	{
		Vec3 cameraRelativPos(
			cos(_elevation)*cos(_azimuth)*_distance, 
			cos(_elevation)*sin(_azimuth)*_distance,
			sin(_elevation)*_distance);
		
		return _target+cameraRelativPos;
		
	}
	Vec3 NavigationHandler::up()
	{
		Vec3 vUp(
			cos(_elevation+M_PI/2)*cos(_azimuth), 
			cos(_elevation+M_PI/2)*sin(_azimuth),
			sin(_elevation+M_PI/2));
		
		return vUp;
		
	}
	
}
