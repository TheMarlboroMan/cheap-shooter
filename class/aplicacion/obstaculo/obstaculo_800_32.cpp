#include "obstaculo_800_32.h"

Obstaculo_800_32::Obstaculo_800_32(unsigned int mod_contador):Obstaculo()
{
	this->preparar_actor(mod_contador);
}

Obstaculo_800_32::~Obstaculo_800_32()
{

}

void Obstaculo_800_32::establecer_caracteristicas()
{
	this->w=800;
	this->h=32;
}

void Obstaculo_800_32::establecer_representacion()
{
	SDL_Rect patron=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 96, 96, 32);
	this->establecer_representacion_patron_dinamica(this->posicion, patron, DLibV::Gestor_color::color(0, 255, 255), Recursos::G_TILE_JUEGO);
}
