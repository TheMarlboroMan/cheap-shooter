#include "bonus.h"

Bonus::Bonus():Actor_mapa()
{
	this->resistencia=1;
}

Bonus::~Bonus()
{

}

//Asumimos que todos los bonus son iguales...

void Bonus::establecer_representacion()
{
	this->mut_indice_recurso(Recursos::G_TILE_ITEMS);
	this->establecer_representacion_bitmap_dinamica();
	Representable::establecer_posicion(this->posicion.x, this->posicion.y);
}
