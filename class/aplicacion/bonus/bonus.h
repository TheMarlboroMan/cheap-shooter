#ifndef BONUS_H
#define BONUS_H

#include "../actor_mapa/actor_mapa.h"
#include "../recursos/recursos.h"

class Bonus:public Actor_mapa
{
	protected:

	virtual void establecer_representacion();
	virtual void establecer_caracteristicas()=0;

	public:

	Bonus();
	virtual ~Bonus();

	float vel_max() {return 70;}
	float vel_min() {return 30;}
	virtual void cuentame()=0;
	virtual bool es_cuenta_recogible() const {return true;}

	unsigned int tipo_a_colision_proyectil() const {return T_NADA;}
	unsigned int tipo_a_colision_jugador() const {return T_OTORGA_BONUS_JUGADOR;}
	unsigned int tipo_a_cuenta() const {return T_CUENTA_BONUS;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_NO_PREPARADO;}

};

#endif
