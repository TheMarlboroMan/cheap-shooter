#ifndef PUNTO_2D_LIBDANSDL_H
#define PUNTO_2D_LIBDANSDL_H

#include <cmath>

namespace DLibH
{

template<typename T>
struct Punto_2d
{
	public:

	T x,y;

	Punto_2d(): x(0), y(0) {}
	Punto_2d(T px, T py):x(px), y(py) {}
	Punto_2d(const Punto_2d& p):x(p.x), y(p.y) {}
	Punto_2d& operator=(const Punto_2d& p)
	{
		x=p.x;
		y=p.y;
		return *this;
	}

	T distancia_hasta(const Punto_2d& p)
	{
		T x=(this.x-p.x)*(this.x-p.x);
		T y=(this.y-p.y)*(this.y-p.y);
		return sqrt(x+y);
	}

	static T distancia_entre(const Punto_2d& p1, const Punto_2d& p2)
	{
		T x=(p1.x-p2.x)*(p1.x-p2.x);
		T y=(p1.y-p2.y)*(p1.y-p2.y);
		return sqrt(x+y);
	}
};

}//Fin namespace...

#endif
