#ifndef BONUS_PIMIENTO_H
#define BONUS_PIMIENTO_H

#include "bonus.h"

class Bonus_pimiento:public Bonus
{
	protected:
	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	Bonus_pimiento(unsigned int);
	virtual ~Bonus_pimiento();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_BONUS_PIMIENTO;}
	void cuentame() {std::cout<<"SOY BONUS PIMIENTO"<<std::endl;}
};

#endif
