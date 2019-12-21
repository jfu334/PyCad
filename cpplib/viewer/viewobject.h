#pragma once
#include "objects.h"
#include "color.h"

namespace PyCadCpp::viewer
{
	class ViewObject
	{
	public:
		ViewObject(GeometryBase* object) : _object(object), _text(nullptr) {}
		ViewObject(PyCadCpp::annotation::Text* text) : _object(nullptr), _text(text) {}
		
		GeometryBase* object() {return _object;}
		annotation::Text* text() {return _text;}
		
		Color color() {return _color;}
		void setColor(Color color) {_color=color;}
		
	private:
		GeometryBase* _object;
		annotation::Text* _text;
		
		Color _color;
	};
}
