#ifndef BONUS_CARGA_ESCUDO_H
#define BONUS_CARGA_ESCUDO_H

#include "bonus.h"

class Bonus_carga_escudo:public Bonus
{
	protected:
	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	Bonus_carga_escudo(unsigned int);
	virtual ~Bonus_carga_escudo();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_BONUS_CARGA_ESCUDO;}
	void cuentame() {std::cout<<"SOY BONUS CARGA_ESCUDO"<<std::endl;}
};

#endif
