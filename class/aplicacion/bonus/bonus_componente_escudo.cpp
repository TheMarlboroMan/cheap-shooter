#include "bonus_componente_escudo.h"

Bonus_componente_escudo::Bonus_componente_escudo(unsigned int mod_contador):Bonus()
{
	this->preparar_actor(mod_contador);
}

Bonus_componente_escudo::~Bonus_componente_escudo()
{

}

void Bonus_componente_escudo::establecer_representacion()
{
	Bonus::establecer_representacion();
	this->establecer_recorte(0,120,24,24);
}

void Bonus_componente_escudo::establecer_caracteristicas()
{
	this->w=24;
	this->h=24;
}
