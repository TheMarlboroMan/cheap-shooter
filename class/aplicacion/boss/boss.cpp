#include "boss.h"

Boss::Boss():
	activado(false)
{

}

Boss::~Boss()
{
	//Por si acaso, por lo que sea, se han quedado algunos en cola...
	if(disparos_efectuados.size())
	{
		for(Proyectil * pr : disparos_efectuados) delete pr;
		disparos_efectuados.clear();
	}
}

void Boss::recibir_inicio_accion()
{
	Trigger::recibir_inicio_accion();
}

/*Una vez que su acción ha ocurrido ya puede ser destruido.*/

void Boss::recibir_finalizacion_accion()
{
	Trigger::recibir_finalizacion_accion();
	this->marcar_para_borrar();
}
