#pragma once

namespace PyCadCpp
{
	class Color
	{
	public:
		Color();
		Color(float r, float g, float b);
		Color(float r, float g, float b, float alpha);
		
		float r();
		float g();
		float b();
		float alpha();
		
	private:
		float _r, _g, _b;
		float _alpha;
	};
}
