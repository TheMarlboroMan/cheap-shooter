#include "escudo_parte.h"

Escudo_parte::Escudo_parte()
{
	this->w=W;
	this->h=H;
	this->establecer_representacion();
	this->posicion=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, this->w, this->h);
	this->establecer_posicion();
}

Escudo_parte::~Escudo_parte()
{

}

void Escudo_parte::establecer_posicion()
{
	int x=this->acc_int_x();
	int y=this->acc_int_y();

	Actor::establecer_posicion(x, y);
	Representable::establecer_posicion(x,y);	
}

void Escudo_parte::establecer_representacion()
{
	this->establecer_representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUEGO));
	Representable::establecer_posicion(0,0, W_REPRESENTACION, H_REPRESENTACION);
	Representable::establecer_alpha(ALPHA_INACTIVO);
	this->establecer_recorte(X_REPRESENTACION, Y_REPRESENTACION, W_REPRESENTACION, H_REPRESENTACION);
}

void Escudo_parte::establecer_transparencia(unsigned short int alpha)
{
	Representable::establecer_alpha(alpha);
}

void Escudo_parte::procesar_turno(float p_delta)
{
	//NOOP.
}
