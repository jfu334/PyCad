#pragma once
#include "objects.h"
#include "color.h"

namespace PyCadCpp::viewer
{
	class ViewObject
	{
	public:
		ViewObject(ModellingObjectBase* object) : _object(object) {}
		ModellingObjectBase* object() {return _object;}
		
		Color color() {return _color;}
		void setColor(Color color) {_color=color;}
		
	private:
		ModellingObjectBase* _object;
		Color _color;
	};
}
