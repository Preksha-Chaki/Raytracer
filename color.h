#ifndef COLOR_H
#define COLOR_H
#include"vec3.h"
#include"rtweekend.h"
#include"interval.h"

using color=vec3;

//gamma correction
inline double linear_to_gamma(double linear_component)
{
	if(linear_component>0)
		return std::sqrt(linear_component);

	return 0;
}

//write a single pixels colour out to the std outpiut stream
void write_color(std::ostream& out, const color& pix_color)
{
	//auto allows the compiler to automatically deduce the data type of a variable
	auto r=pix_color.x();//e[0]
	auto g=pix_color.y();//e[1]
	auto b=pix_color.z();//e[2]

	//applying gamma transform for gamma2
	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	//translate [0,1] component values to [0,255]byte range

	static const interval intensity(0.000,0.999);
	int rbyte = int(255.999 * intensity.clamp(r));
 	int gbyte = int(255.999 * intensity.clamp(g));
 	int bbyte = int(255.999 * intensity.clamp(b));

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
