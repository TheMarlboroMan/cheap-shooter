#include "bonus_tiempo.h"

Bonus_tiempo::Bonus_tiempo(unsigned int mod_contador):Bonus()
{
	this->preparar_actor(mod_contador);
}

Bonus_tiempo::~Bonus_tiempo()
{

}

void Bonus_tiempo::establecer_representacion()
{
	Bonus::establecer_representacion();
	this->establecer_recorte(0,0,24,24);
}

void Bonus_tiempo::establecer_caracteristicas()
{
	this->w=24;
	this->h=24;
}
