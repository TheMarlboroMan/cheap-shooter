#include "bonus_carga_escudo.h"

Bonus_carga_escudo::Bonus_carga_escudo(unsigned int mod_contador):Bonus()
{
	this->preparar_actor(mod_contador);
}

Bonus_carga_escudo::~Bonus_carga_escudo()
{

}

void Bonus_carga_escudo::establecer_representacion()
{
	Bonus::establecer_representacion();
	this->establecer_recorte(0,96,24,24);
}

void Bonus_carga_escudo::establecer_caracteristicas()
{
	this->w=24;
	this->h=24;
}
