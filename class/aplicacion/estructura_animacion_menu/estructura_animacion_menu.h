#ifndef ESTRUCTURA_ANIMACION_MENU_H
#define ESTRUCTURA_ANIMACION_MENU_H

/*Esto es para controlar animaciones de representaciones en movimiento,
básicamente cosas del menú principal que vamos a mover de un sitio a otro,
a hacer transparente o cosas así...*/

#include <vector>
#include "../../motor_sdl/video/pantalla/pantalla.h"
#include "../../motor_sdl/video/representacion/representacion.h"
#include "../../motor_sdl/herramientas/herramientas/herramientas.h"
#include "../../motor_sdl/herramientas/vector_2d/vector_2d.h"

//Clase base para todas ellas.

class Estructura_animacion
{
	private:

	virtual void generar_porcentajes()=0;	//Su finalidad es calcular los datos necesarios para los porcentajes.
	virtual void calcular_porcentajes()=0;  //Esto es para calcular los porcentajes después de cada turno.

	public:


	DLibV::Representacion * rep;
	float porcentaje;

	virtual void turno(float)=0;
	virtual void forzar_finalizacion()=0;
	virtual ~Estructura_animacion() {}

	bool es_completa() {return this->porcentaje>=100;}

	Estructura_animacion(DLibV::Representacion * p_rep):
		porcentaje(0)
	{
		this->rep=p_rep;
	}
};

//Esta controla movimientos.

class Estructura_animacion_movimiento:public Estructura_animacion
{
	public:

	float pos_x;
	float pos_y;
	float dest_x;
	float dest_y;
	DLibH::Vector_2d v;

	float mult_porcentaje_x;
	float mult_porcentaje_y;
	float dist_x;
	float dist_y;
	float dist_tot;

	private:

	void generar_porcentajes()
	{
		this->dist_x=abs(this->pos_x-this->dest_x);
		this->dist_y=abs(this->pos_y-this->dest_y);

		this->dist_tot=dist_x+dist_y;

		this->mult_porcentaje_x=DLibH::Herramientas::porcentaje(dist_x, dist_tot) / 100;
		this->mult_porcentaje_y=DLibH::Herramientas::porcentaje(dist_y, dist_tot) / 100;
	}

	void calcular_porcentajes()
	{
		float dist_rest_x=abs(this->pos_x-this->dest_x);
		float dist_rest_y=abs(this->pos_y-this->dest_y);

		if(!dist_rest_x && !dist_rest_y)
		{
			porcentaje=100;
		}
		else
		{
			float porc_x=DLibH::Herramientas::porcentaje(this->dist_x-dist_rest_x, this->dist_x);
			float porc_y=DLibH::Herramientas::porcentaje(this->dist_y-dist_rest_y, this->dist_y);

			this->porcentaje=(porc_x*this->mult_porcentaje_x)+(porc_y*this->mult_porcentaje_y);
		}
	}

	//Devolverá la distancia restante,

	void comprobacion_y_movimiento(float delta, float vector, float &pos, float dest)
	{
		//Desplazamiento hacia la derecha / arriba.
		if(vector > 0 && pos < dest)
		{
			pos+=vector*delta;
			if(pos >= dest) pos=dest;
		}
		//esplazamiento hacia la izquierda / abajo.
		else if(vector < 0  && pos > dest)
		{
			pos+=vector*delta;
			if(pos <= dest) pos=dest;
		}
		//No hay desplazamiento
		else
		{

		}
	}

	public:

	Estructura_animacion_movimiento(DLibV::Representacion * p_rep, const DLibH::Vector_2d & pv, float dx, float dy):
		Estructura_animacion(p_rep),
		dest_x(dx), dest_y(dy), v(pv),
		dist_x(0), dist_y(0), dist_tot(0)
	{
		this->pos_x=p_rep->acc_posicion().x;
		this->pos_y=p_rep->acc_posicion().y;

		generar_porcentajes();
	}


	void turno(float p_delta)
	{
		//Horizontal...
		comprobacion_y_movimiento(p_delta, this->v.x, this->pos_x, this->dest_x);
		comprobacion_y_movimiento(p_delta, this->v.y, this->pos_y, this->dest_y);

		this->rep->establecer_posicion(this->pos_x, this->pos_y);

		//Calcular porcentaje.
		this->calcular_porcentajes();
	}

	void forzar_finalizacion()
	{
		this->pos_x=this->dest_x;
		this->pos_y=this->dest_y;

		this->rep->establecer_posicion(this->pos_x, this->pos_y);

		this->porcentaje=100;
	}
};

//Esto aglutina varias animaciones para que no haya que poner tánto código.

class Grupo_animacion
{
	private:

	std::vector<Estructura_animacion *> animaciones;
	std::vector<Estructura_animacion *>::iterator ini;
	std::vector<Estructura_animacion *>::iterator fin;

	float porcentaje;
	float porcentaje_parcial;
	float porcentaje_minimo_cancelar;
	unsigned int total_animaciones;

	public:

	float acc_porcentaje() const {return porcentaje;}

	Grupo_animacion(float porc=0):
		porcentaje(0), porcentaje_parcial(0), porcentaje_minimo_cancelar(porc),
		total_animaciones(0)
	{
	}

	~Grupo_animacion()
	{
		ini=animaciones.begin();
		fin=animaciones.end();

		while(ini < fin)
		{
			delete *(ini);
			ini++;
		}

		animaciones.clear();
	}

	void asignar_animacion(Estructura_animacion * temp)
	{
		animaciones.push_back(temp);
		fin=animaciones.end();
		++total_animaciones;
	}

	void turno(float p_delta)
	{
		porcentaje_parcial=0;
		ini=animaciones.begin();

		while(ini < fin)
		{
			(*ini)->turno(p_delta);

			if((*ini)->es_completa()) porcentaje_parcial+=100;
			else porcentaje_parcial+=(*ini)->porcentaje;
			++ini;
		}

		porcentaje=porcentaje_parcial / total_animaciones;
	}

	bool es_finalizado()
	{
		return porcentaje == 100;
	}

	bool finalizar()
	{
		if(this->porcentaje >= porcentaje_minimo_cancelar)
		{
			this->forzar_finalizacion();
			return true;
		}
		else
		{
			return false;
		}
	}

	void forzar_finalizacion()
	{
		ini=animaciones.begin();
		while(ini < fin)
		{
			(*ini)->forzar_finalizacion();
//(*ini++).forzar_finalizacion();
			++ini;
		}
	}

	void mostrar(DLibV::Pantalla & pantalla)
	{
		for(Estructura_animacion * a : animaciones) a->rep->volcar(pantalla);
	}
};


#endif
