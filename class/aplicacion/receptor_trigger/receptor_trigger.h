#ifndef RECEPTOR_TRIGGER_H
#define RECEPTOR_TRIGGER_H

#include "../../motor_sdl/herramientas/vector_2d/vector_2d.h"
#include <vector>
#include <iostream>

class Accion_trigger_datos;

class Receptor_trigger
{
	protected:

	struct Estructura_ruta
	{
		DLibH::Vector_2d vector_mov;
		float x;	//El destino...
		float y;
		float velocidad;
		bool activa;
		bool en_marcha;
		unsigned int indice_punto;

		Estructura_ruta(float p_x, float p_y, float p_v):
			vector_mov(0,0) ,x(p_x), y(p_y), velocidad(p_v), activa(false),
			en_marcha(false), indice_punto(0)
		{}

/*		void inicializar(unsigned int t_punto)
		{
			this->indice_punto=t_punto;
		}
*/

		void reasignar(float p_x, float p_y, float p_posx, float p_posy, float p_v, unsigned int p_punto=0)
		{
			this->x=p_x;
			this->y=p_y;
			this->velocidad=p_v;
			this->activa=true;
			this->en_marcha=true;
			this->vector_mov=DLibH::Vector_2d::obtener_para_puntos(this->x, this->y, p_posx, p_posy);
			this->vector_mov*=this->velocidad;
			this->indice_punto=p_punto;
		}

		void desactivar(){this->activa=false;}
		void detener_marcha(){this->en_marcha=false;}

		//Comprueba si ha llegado y además ajusta los parámetros que llegan por referencia,
		//que son la posición del actor.
		bool en_destino_y_ajustar(float &x, float &y)
		{
			if(this->vector_mov.x > 0)
			{				
				if(x >= this->x) 
				{
					x=this->x;
				}
			}
			else if(this->vector_mov.x < 0)
			{
				if(x <= this->x) 
				{
					x=this->x;
				}
			}
			else
			{
				x=this->x;
			}

			if(this->vector_mov.y > 0)
			{
				if(y >= this->y) y=this->y;
			}
			else if(this->vector_mov.y < 0)
			{
				if(y <= this->y) y=this->y;
			}
			else
			{
				y=this->y;				
			}

			return x==this->x && y==this->y;
		}
	};

	Estructura_ruta estructura_ruta;

	public:

	virtual bool recibir_senal_trigger(const Accion_trigger_datos&)=0;
	Receptor_trigger(): estructura_ruta(0,0,0)
	{}
	~Receptor_trigger(){}
};

#endif
