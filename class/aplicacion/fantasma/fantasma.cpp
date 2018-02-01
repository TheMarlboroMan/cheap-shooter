#include "fantasma.h"

Fantasma::Fantasma():Actor_jugable(), visible(false)
{
	this->establecer_representacion();
	this->x=400;
	this->y=500;

	this->w=32;
	this->h=32;

	this->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect(400, 500, 32, 32);
}

Fantasma::~Fantasma()
{

}

void Fantasma::establecer_representacion()
{
	this->mut_indice_recurso(Recursos::G_TILE_JUGADOR);
	this->establecer_representacion_bitmap_dinamica();
	Representable::establecer_posicion(400,500);
	Representable::establecer_recorte(160, 0, 32, 32);
	Representable::establecer_alpha(128);
}

void Fantasma::hacer_visible(float p_x, float p_y)
{
	this->vector_mov.x=0;
	this->vector_mov.y=0;
	this->visible=true;
	this->ir_a(p_x, p_y);
}

void Fantasma::hacer_invisible()
{
	this->visible=false;
}

void Fantasma::procesar_turno(float p_delta)
{
	this->aplicar_vector(p_delta);
	this->controlar_posiciones();
	this->procesar_retraccion_vectores(p_delta);		
}
