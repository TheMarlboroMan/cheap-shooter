#ifndef BONUS_TIEMPO_H
#define BONUS_TIEMPO_H

#include "bonus.h"

class Bonus_tiempo:public Bonus
{
	protected:
	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	Bonus_tiempo(unsigned int);
	virtual ~Bonus_tiempo();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_BONUS_TIEMPO;}
	void cuentame() {std::cout<<"SOY BONUS TIEMPO"<<std::endl;}
};

#endif
