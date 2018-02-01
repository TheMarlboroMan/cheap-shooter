#include "obstaculo_destruible.h"

Obstaculo_destruible::Obstaculo_destruible(unsigned int mod_contador):Actor_mapa()
{
	this->preparar_actor(mod_contador);
}

Obstaculo_destruible::~Obstaculo_destruible()
{

}

void Obstaculo_destruible::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
	this->resistencia=15;
}

void Obstaculo_destruible::establecer_representacion()
{
	this->mut_indice_recurso(Recursos::G_TILE_JUEGO);
	this->establecer_representacion_bitmap_dinamica();
	Representable::establecer_posicion(this->posicion.x, this->posicion.y);
	this->establecer_recorte(0,256,this->w,this->h);
}

bool Obstaculo_destruible::procesar_colision_proyectil(Proyectil * proyectil)
{
	herir();
	resistencia-=proyectil->acc_potencia();
	return resistencia <= 0;
}
