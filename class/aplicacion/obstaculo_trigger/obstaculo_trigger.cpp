#include "obstaculo_trigger.h"


Obstaculo_trigger::Obstaculo_trigger():
	Obstaculo(), Trigger()
{
}

Obstaculo_trigger::~Obstaculo_trigger()
{

}

void Obstaculo_trigger::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	Trigger::mut_tag_accion(p.param1);
	Trigger::mut_repetible(p.param2 ? true : false);
}
