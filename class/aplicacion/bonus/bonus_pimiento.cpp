#include "bonus_pimiento.h"

Bonus_pimiento::Bonus_pimiento(unsigned int mod_contador):Bonus()
{
	this->preparar_actor(mod_contador);
}

Bonus_pimiento::~Bonus_pimiento()
{

}

void Bonus_pimiento::establecer_representacion()
{
	Bonus::establecer_representacion();
	this->establecer_recorte(0,72,24,24);
}

void Bonus_pimiento::establecer_caracteristicas()
{
	this->w=24;
	this->h=24;
}
