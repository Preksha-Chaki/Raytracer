#ifndef COLOR_H
#define COLOR_H
#include"vec3.h"
#include<iostream>

using color=vec3;

//write a single pixels colour out to the std outpiut stream
void write_color(std::ostream& out, const color& pix_color)
{
	//auto allows the compiler to automatically deduce the data type of a variable
	auto r=pix_color.x();//e[0]
	auto g=pix_color.y();//e[1]
	auto b=pix_color.z();//e[2]

	//translate [0,1] component values to [0,255]byte range

	int rbyte = int(255.999 * r);
 	int gbyte = int(255.999 * g);
 	int bbyte = int(255.999 * b);

	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
