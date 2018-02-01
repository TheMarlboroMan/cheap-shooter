#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <cmath>

namespace DLibH
{


struct Vector_2d
{
	float x;
	float y;

	Vector_2d(): x(0.f), y(0.f) {}
	Vector_2d(const Vector_2d& o): x(o.x), y(o.y) {}
	Vector_2d(float p_x, float p_y):x(p_x), y(p_y) {}

	Vector_2d operator+(const Vector_2d &otro);
	Vector_2d operator-(const Vector_2d &otro);
	Vector_2d operator*(const Vector_2d &otro);
	Vector_2d operator/(const Vector_2d &otro);

	Vector_2d& operator=(const Vector_2d &otro);

	Vector_2d& operator+=(const Vector_2d &otro);
	Vector_2d& operator-=(const Vector_2d &otro);
	Vector_2d& operator*=(const Vector_2d &otro);
	Vector_2d& operator/=(const Vector_2d &otro);

	Vector_2d& operator*(const float p_multiplicador);
	Vector_2d& operator/(const float p_divisor);
	Vector_2d& operator*=(const float p_multiplicador);
	Vector_2d& operator/=(const float p_divisor);

	void normalizar();
	float longitud();
	static float obtener_angulo_para_vector_unidad_radianes(const Vector_2d&);
	static float obtener_angulo_para_vector_unidad_grados(const Vector_2d&);
	static Vector_2d obtener_para_puntos(float p_xa, float p_ya, float p_xb, float p_yb);
	static Vector_2d vector_unidad_para_angulo(float);
};

} //Fin namespace DLibH
#endif
