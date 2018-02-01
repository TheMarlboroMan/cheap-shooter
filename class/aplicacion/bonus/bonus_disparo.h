#ifndef BONUS_DISPARO_H
#define BONUS_DISPARO_H

#include "bonus.h"

class Bonus_disparo:public Bonus
{
	protected:
	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	Bonus_disparo(unsigned int);
	virtual ~Bonus_disparo();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_BONUS_DISPARO;}
	void cuentame() {std::cout<<"SOY BONUS DISPARO"<<std::endl;}
};

#endif
