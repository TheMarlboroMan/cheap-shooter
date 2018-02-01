#include "proyectil.h"

Proyectil::Proyectil()
	:No_jugador(),
	origen(NULL), potencia(0), color(C_ROJO), 
	faccion(Definiciones::F_NADA), velocidad(0.0f)
{

}

Proyectil::~Proyectil()
{

}

void Proyectil::establecer_velocidad(float p_param)
{
	this->velocidad=p_param;
	this->vector_mov*=this->velocidad;
}

void Proyectil::establecer_color(unsigned short int p_param)
{
	if(p_param >= C_MAX_COLOR) this->color=C_ROJO;
	else this->color=p_param;

	this->actualizar_representacion();
}

void Proyectil::establecer_dimensiones(unsigned short int p_param)
{
	if(p_param >= D_MAX_DIMENSIONES) 
	{
		this->w=D_NORMAL;
		this->h=D_NORMAL;
	}
	else
	{
		this->w=p_param;
		this->h=p_param;
	}
}

void Proyectil::establecer_representacion()
{
	this->mut_indice_recurso(Recursos::G_TILE_JUEGO);
	this->establecer_representacion_bitmap_dinamica();
	this->establecer_recorte(0,0, this->w, this->h);
	Representable::establecer_posicion(this->posicion.x, this->posicion.y);
}

void Proyectil::actualizar_representacion()
{
	int x=0, y=0;

	switch(this->color)
	{
		case C_AZUL: x=96; y=96; break;
		case C_ROJO: x=112; y=96; break;
		case C_VERDE: x=96; y=112; break;
		case C_VIOLETA: x=112; y=112; break;

	}

	this->establecer_recorte(x,y,this->w,this->h);
}

void Proyectil::establecer_caracteristicas()
{
	//Esto no hace nada...
}

Proyectil * Proyectil::nuevo_proyectil(unsigned short int p_dimensiones)
{
	Proyectil * pr=new Proyectil();
	pr->establecer_dimensiones(p_dimensiones);

	return pr;
}

//Especializa la generación con la posición exacta desde la cual se dispara.

Proyectil * Proyectil::generar(int px, int py, const Actor& p_origen, DLibH::Vector_2d p_vector, unsigned short int p_dimensiones)
{
	Proyectil * resultado=nuevo_proyectil(p_dimensiones);
	resultado->origen=&p_origen;
	resultado->vector_mov=p_vector;

	//Establecer posición...
	resultado->x=px-(p_dimensiones/2);
	resultado->y=py-(p_dimensiones/2);
	resultado->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect(px, py, p_dimensiones, p_dimensiones);

	//Establecemos una representación por defecto. Al cambiar el color usaremos otra.
	resultado->establecer_representacion();
	resultado->actualizar_representacion();
	
	return resultado;
}

Proyectil * Proyectil::generar(const Actor& p_origen, DLibH::Vector_2d p_vector, unsigned short int p_dimensiones, int p_desp_x, int p_desp_y)
{
	Proyectil * resultado=nuevo_proyectil(p_dimensiones);
	resultado->origen=&p_origen;
	resultado->vector_mov=p_vector;

	//Establecer posición...
	resultado->x=p_origen.acc_x()+(p_origen.acc_w()/2)-(p_dimensiones/2)+p_desp_x;
	resultado->y=p_origen.acc_y()+(p_origen.acc_h()/2)-(p_dimensiones/2)+p_desp_y;
	resultado->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect(resultado->x, resultado->y, p_dimensiones, p_dimensiones);

	//Establecemos una representación por defecto. Al cambiar el color usaremos otra.
	resultado->establecer_representacion();
	resultado->actualizar_representacion();
	
	return resultado;
}

Proyectil * Proyectil::generar(const Actor& p_origen, const Actor& p_destino, unsigned short int p_dimensiones, int p_desp_x, int p_desp_y, int p_ruido)
{
	DLibH::Vector_2d v;

	if(p_ruido)
	{
		float dx=p_destino.acc_x() + ( (rand() % p_ruido) - (p_ruido / 2));
		float dy=p_destino.acc_y() + ( (rand() % p_ruido) - (p_ruido / 2));
	
		v=DLibH::Vector_2d::obtener_para_puntos(dx, dy, p_origen.acc_x(), p_origen.acc_y());	
	}
	else
	{
		v=Actor::obtener_vector_unidad_desde_hasta(p_origen, p_destino);
	}

	return generar(p_origen, v, p_dimensiones, p_desp_x, p_desp_y);
}
