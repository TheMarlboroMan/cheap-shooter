#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "../actor_mapa/actor_mapa.h"

class Obstaculo:public Actor_mapa
{
	protected:

	virtual void establecer_representacion()=0;
	virtual void establecer_caracteristicas()=0;

	public:

	Obstaculo();
	virtual ~Obstaculo();

	virtual float vel_max() {return 150;}
	virtual float vel_min() {return 50;}

	virtual void cuentame()=0;
};

#endif
