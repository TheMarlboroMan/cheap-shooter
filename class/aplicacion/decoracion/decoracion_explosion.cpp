#include "decoracion_explosion.h"

Decoracion_explosion::Decoracion_explosion(float p_x, float p_y, DLibH::Vector_2d p_v):Decoracion(), duracion(0.5f)
{
	this->vector_mov=p_v;

	this->x=p_x;
	this->y=p_y;

	this->establecer_caracteristicas(); 
	this->establecer_representacion();
}

Decoracion_explosion::~Decoracion_explosion()
{

}

void Decoracion_explosion::procesar_turno(float p_delta)
{
	Decoracion::procesar_turno(p_delta);

	this->duracion-=p_delta;

	if(this->duracion < 0) this->duracion=0;
	this->actualizar_representacion();
}

void Decoracion_explosion::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
	this->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect(this->x, this->y, this->w, this->h);
}

void Decoracion_explosion::establecer_representacion()
{
	this->establecer_representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUEGO));
	Representable::establecer_posicion(this->x,this->y, this->w, this->h);
	this->establecer_recorte(0, 0, 0, 0);

	this->actualizar_representacion();
}

void Decoracion_explosion::actualizar_representacion()
{
	if(this->duracion>0.4f)
	{
		Representable::establecer_recorte(0, 160, this->w, this->h);
		Representable::establecer_alpha(33);
	}
	else if(this->duracion > 0.3f)
	{
		Representable::establecer_recorte(32, 160, this->w, this->h);
		Representable::establecer_alpha(66);
	}
	else if(this->duracion > 0.2f)
	{
		Representable::establecer_recorte(64, 160, this->w, this->h);
		Representable::establecer_alpha(132);
	}
	else if(this->duracion > 0.1f)
	{
		Representable::establecer_recorte(96, 160, this->w, this->h);
		Representable::establecer_alpha(200);
	}
}

bool Decoracion_explosion::es_finalizado()
{
	return this->duracion==0;
}

