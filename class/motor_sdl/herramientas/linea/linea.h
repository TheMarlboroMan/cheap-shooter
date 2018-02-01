#ifndef LINEA_H
#define LINEA_H

#include <cmath>
#include <cstdlib>


namespace DLibH
{

class Linea
{
	private:

	Linea();

	protected:

	bool vertical;
	bool horizontal;
	float m;	//Pendiente.
	float b;	//Constante.

	public:

	struct Punto
	{
		float x;
		float y;
		Punto(float p_x, float p_y):x(p_x), y(p_y)
		{}
	};

	struct Set_puntos
	{
		Punto p1;
		Punto p2;
		bool erroneo;
		Set_puntos(Punto p_1, Punto p_2):p1(p_1), p2(p_2), erroneo(false)
		{}
	};

	~Linea();

	static Linea crear_desde_puntos(float, float, float, float);
	static Linea crear_desde_parametros(float, float);
	static float distancia_entre_puntos(float, float, float, float);
	static Set_puntos obtener_para_linea_y_caja_limite(float, float, float, float, float, float, float, float);
	static Set_puntos obtener_para_linea_y_caja_limite(float, float, float, float, Linea);

	bool es_vertical() const {return this->vertical;}
	bool es_horizontal() const {return this->horizontal;}
	float obtener_y_para_x(float) const;
	float obtener_x_para_y(float) const;
	float acc_m() const {return this->m;}	
	float acc_b() const {return this->b;}
};

}
#endif
