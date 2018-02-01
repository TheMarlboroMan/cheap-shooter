#include "vector_2d.h"
#include "../herramientas/herramientas.h"

using namespace DLibH;

//Operaciones con vectores.

Vector_2d Vector_2d::operator+(const Vector_2d &otro)
{
	float x=this->x+otro.x;
	float y=this->y+otro.y;
	return Vector_2d(x,y);
}

Vector_2d Vector_2d::operator-(const Vector_2d &otro)
{
	float x=this->x-otro.x;
	float y=this->y-otro.y;
	return Vector_2d(x,y);
}

Vector_2d Vector_2d::operator*(const Vector_2d &otro)
{
	float x=this->x*otro.x;
	float y=this->y*otro.y;
	return Vector_2d(x,y);
}

Vector_2d Vector_2d::operator/(const Vector_2d &otro)
{
	float x=this->x/otro.x;
	float y=this->y/otro.y;
	return Vector_2d(x,y);
}

Vector_2d& Vector_2d::operator=(const Vector_2d &otro)
{
	this->x=otro.x;
	this->y=otro.y;

	return *this;
}

Vector_2d& Vector_2d::operator+=(const Vector_2d &otro)
{
	this->x+=otro.x;
	this->y+=otro.y;
	return *this;
}

Vector_2d& Vector_2d::operator-=(const Vector_2d &otro)
{
	this->x-=otro.x;
	this->y-=otro.y;
	return *this;
}


Vector_2d& Vector_2d::operator*=(const Vector_2d &otro)
{
	this->x*=otro.x;
	this->y*=otro.y;
	return *this;
}

Vector_2d& Vector_2d::operator/=(const Vector_2d &otro)
{
	this->x/=otro.x;
	this->y/=otro.y;
	return *this;
}

//Operaciones con números.

Vector_2d& Vector_2d::operator*(const float p_multiplicador)
{
	this->x=this->x*p_multiplicador;
	this->y=this->y*p_multiplicador;
	return *this;
}

Vector_2d& Vector_2d::operator/(const float p_divisor)
{
	this->x=this->x/p_divisor;
	this->y=this->y/p_divisor;
	return *this;
}

Vector_2d& Vector_2d::operator*=(const float p_multiplicador)
{
	this->x*=p_multiplicador;
	this->y*=p_multiplicador;
	return *this;
}

Vector_2d& Vector_2d::operator/=(const float p_divisor)
{
	this->x/=p_divisor;
	this->y/=p_divisor;
	return *this;
}


void Vector_2d::normalizar()
{
	float m=this->longitud();
	this->x=this->x / m;
	this->y=this->y / m;
}

float Vector_2d::longitud()
{
	return sqrtf(this->x*this->x + this->y*this->y);
}

/*El orden de los factores PUEDE alterar el producto.*/

Vector_2d Vector_2d::obtener_para_puntos(float p_xa, float p_ya, float p_xb, float p_yb)
{
	Vector_2d r(0,0);

	if(p_xa==p_xb && p_ya==p_yb)
	{

	}
	else
	{
		r.x=p_xa-p_xb;
		r.y=p_ya-p_yb;
		r.normalizar();
	}

	return r;
}

Vector_2d Vector_2d::vector_unidad_para_angulo(float p_angulo)
{
	float rad=DLibH::Herramientas::grados_a_radianes(p_angulo);
	float v_x=sin(rad);
	float v_y=cos(rad);

	Vector_2d r(v_x, v_y);
	return r;
}

float Vector_2d::obtener_angulo_para_vector_unidad_radianes(const Vector_2d& p_vector)
{
	float rad=std::atan2(p_vector.y, p_vector.x);
	return rad;
}

float Vector_2d::obtener_angulo_para_vector_unidad_grados(const Vector_2d& p_vector)
{
	float rad=obtener_angulo_para_vector_unidad_radianes(p_vector);
	float grados=(rad / M_PI) * 180.0;
	return grados;
}
