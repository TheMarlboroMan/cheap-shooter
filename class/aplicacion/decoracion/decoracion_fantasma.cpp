#include "decoracion_fantasma.h"

const float Decoracion_fantasma::MAX_DURACION=0.2;

Decoracion_fantasma::Decoracion_fantasma(float p_x, float p_y)
	:Decoracion(), duracion(MAX_DURACION)
{	
	this->vector_mov=DLibH::Vector_2d(0, 0);
	this->x=p_x;
	this->y=p_y;

	this->establecer_caracteristicas(); 
	this->establecer_representacion();
}

Decoracion_fantasma::~Decoracion_fantasma()
{

}

void Decoracion_fantasma::procesar_turno(float p_delta)
{
	Decoracion::procesar_turno(p_delta);

	this->duracion-=p_delta / No_jugador::divisor_movimiento;

	if(this->duracion < 0) this->duracion=0;
	this->actualizar_representacion();
}

void Decoracion_fantasma::establecer_caracteristicas()
{
	this->w=16;
	this->h=16;
	this->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect(this->x, this->y, this->w, this->h);
}

void Decoracion_fantasma::establecer_representacion()
{	
	this->establecer_representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUGADOR));
	Representable::establecer_posicion(this->x,this->y, this->w, this->h);
	Representable::establecer_recorte(160, 0, 32, 32);
}

void Decoracion_fantasma::actualizar_representacion()
{
	float valor_alpha=(255 * this->duracion) / MAX_DURACION;
	if(valor_alpha <= 0) valor_alpha=0;

	Representable::establecer_alpha(ceil(valor_alpha));
}

bool Decoracion_fantasma::es_finalizado()
{
	return this->duracion <= 0;
}

