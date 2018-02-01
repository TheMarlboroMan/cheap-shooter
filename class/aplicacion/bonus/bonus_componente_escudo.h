#ifndef BONUS_COMPONENTE_ESCUDO_H
#define BONUS_COMPONENTE_ESCUDO_H

#include "bonus.h"

class Bonus_componente_escudo:public Bonus
{
	protected:
	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	Bonus_componente_escudo(unsigned int);
	virtual ~Bonus_componente_escudo();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_BONUS_COMPONENTE_ESCUDO;}
	void cuentame() {std::cout<<"SOY BONUS COMPONENTE_ESCUDO"<<std::endl;}
};

#endif
