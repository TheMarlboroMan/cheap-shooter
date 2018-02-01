#include "decoracion_humo.h"

const float Decoracion_humo::MAX_DURACION=0.2;

Decoracion_humo::Decoracion_humo(float p_x, float p_y, unsigned short int p_parte)
	:Decoracion(), duracion(MAX_DURACION), duracion_original(MAX_DURACION),
	auto_actualizar_parte(false)
{	
	this->vector_mov=DLibH::Vector_2d(0, 0);
	this->x=p_x;
	this->y=p_y;

	this->establecer_caracteristicas(); 
	this->establecer_representacion();
	this->establecer_recorte(64+(p_parte * 16), 256, 16, 16);
}

Decoracion_humo::~Decoracion_humo()
{

}

void Decoracion_humo::procesar_turno(float p_delta)
{
	Decoracion::procesar_turno(p_delta);
	this->duracion-=p_delta / No_jugador::divisor_movimiento;
	this->actualizar_representacion();
}

void Decoracion_humo::establecer_caracteristicas()
{
	this->w=16;
	this->h=16;
	this->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect(this->x, this->y, this->w, this->h);
}

void Decoracion_humo::establecer_representacion()
{	
	this->establecer_representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUEGO));
	Representable::establecer_posicion(this->x,this->y, this->w, this->h);
}

void Decoracion_humo::actualizar_representacion()
{
	float valor_alpha=(255 * this->duracion) / duracion_original;
	if(valor_alpha <= 0) valor_alpha=0;

	Representable::establecer_alpha(ceil(valor_alpha));

	if(auto_actualizar_parte)
	{
		int parte=(4 * this->duracion) / duracion_original;
		this->establecer_recorte(64+(parte * 16), 256, 16, 16);
	}
}

bool Decoracion_humo::es_finalizado()
{
	return this->duracion <= 0;
}

