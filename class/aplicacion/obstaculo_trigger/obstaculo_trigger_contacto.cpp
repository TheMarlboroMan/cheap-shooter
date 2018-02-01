#include "obstaculo_trigger_contacto.h"

Obstaculo_trigger_contacto::Obstaculo_trigger_contacto()
	:Obstaculo_trigger(), activable_por(A_JUGADOR | A_FANTASMA)
{
	this->preparar_actor(0);
}

Obstaculo_trigger_contacto::~Obstaculo_trigger_contacto()
{

}

void Obstaculo_trigger_contacto::establecer_caracteristicas()
{
	this->w=64;
	this->h=16;
}

/* Realmente no tiene representación... Tan fácil como eso.*/

void Obstaculo_trigger_contacto::establecer_representacion()
{

}

void Obstaculo_trigger_contacto::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	Obstaculo_trigger::parametrizar_actor_en_nivel_preparado(p);
	activable_por=p.param3;
}
