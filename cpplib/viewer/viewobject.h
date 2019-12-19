#pragma once
#include "objects.h"
#include "color.h"

namespace PyCadCpp::viewer
{
	class ViewObject
	{
	public:
		ViewObject(GeometryBase* object) : _object(object) {}
		GeometryBase* object() {return _object;}
		
		Color color() {return _color;}
		void setColor(Color color) {_color=color;}
		
	private:
		GeometryBase* _object;
		Color _color;
	};
}
