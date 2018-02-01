#ifndef BONUS_VIDA_H
#define BONUS_VIDA_H

#include "bonus.h"

class Bonus_vida:public Bonus
{
	protected:
	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	Bonus_vida(unsigned int);
	virtual ~Bonus_vida();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_BONUS_VIDA;}
	void cuentame() {std::cout<<"SOY BONUS VIDA"<<std::endl;}
};

#endif
