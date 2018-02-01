#include "bonus_disparo.h"

Bonus_disparo::Bonus_disparo(unsigned int mod_contador):Bonus()
{
	this->preparar_actor(mod_contador);
}

Bonus_disparo::~Bonus_disparo()
{

}

void Bonus_disparo::establecer_representacion()
{
	Bonus::establecer_representacion();
	this->establecer_recorte(0,24,24,24);
}

void Bonus_disparo::establecer_caracteristicas()
{
	this->w=24;
	this->h=24;
}
