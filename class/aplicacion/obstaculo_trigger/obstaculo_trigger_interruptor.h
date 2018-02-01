#ifndef OBSTACULO_TRIGGER_INTERRUPTOR_H
#define OBSTACULO_TRIGGER_INTERRUPTOR_H

/*Un trigger.*/

#include "../obstaculo_trigger/obstaculo_trigger.h"

class Obstaculo_trigger_interruptor:public Obstaculo_trigger
{
	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	Obstaculo_trigger_interruptor();
	virtual ~Obstaculo_trigger_interruptor();

	//Esta es propia.
	bool procesar_colision_proyectil(Proyectil *);

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_TRIGGER_INTERRUPTOR;}
	void cuentame() {std::cout<<"SOY OBSTACULO TRIGGER INTERRUPTOR"<<std::endl;}
	bool es_representable() const {return true;}
	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL_LANZA_ACCION;}
};

#endif
