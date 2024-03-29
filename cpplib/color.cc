#include "color.h"

namespace PyCadCpp
{
	Color::Color() : _r(0.5), _g(0.5), _b(0.5), _alpha(1.0) {}
	Color::Color(float r, float g, float b)
		: _r(r), _g(g), _b(b), _alpha(1.0) {}
	
	Color::Color(float r, float g, float b, float alpha)
		: _r(r), _g(g), _b(b), _alpha(alpha) {}
	
	float Color::r() const {return _r;}
	float Color::g() const {return _g;}
	float Color::b() const {return _b;}
	float Color::alpha() const {return _alpha;}
	
}
