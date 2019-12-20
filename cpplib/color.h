#pragma once

namespace PyCadCpp
{
	class Color
	{
	public:
		Color();
		Color(float r, float g, float b);
		Color(float r, float g, float b, float alpha);
		
		float r() const;
		float g() const;
		float b() const;
		float alpha() const;
		
	private:
		float _r, _g, _b;
		float _alpha;
	};
	
	namespace colors
	{
		const Color red=Color(1, 0, 0);
		const Color green=Color(0, 1, 0);
		const Color blue=Color(0, 0, 1);
		
		const Color yellow=Color(1, 1, 0);
		const Color cyan=Color(1, 0, 1);
		const Color pink=Color(0, 1, 1);
		
	}
}
