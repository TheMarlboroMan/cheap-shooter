#include "bonus_vida.h"

Bonus_vida::Bonus_vida(unsigned int mod_contador):Bonus()
{
	this->preparar_actor(mod_contador);
}

Bonus_vida::~Bonus_vida()
{

}

void Bonus_vida::establecer_caracteristicas()
{
	this->w=24;
	this->h=24;
}

void Bonus_vida::establecer_representacion()
{
	Bonus::establecer_representacion();
	this->establecer_recorte(0,48,24,24);
}
