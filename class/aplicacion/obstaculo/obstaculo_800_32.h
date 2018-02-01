#ifndef OBSTACULO_800_32_H
#define OBSTACULO_800_32_H

#include "obstaculo.h"

class Obstaculo_800_32:public Obstaculo
{
	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	float vel_max() {return 50;}
	float vel_min() {return 20;}

	Obstaculo_800_32(unsigned int);
	virtual ~Obstaculo_800_32();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_800_32;}
	bool es_afectado_por_edu() const {return false;}
	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_NO_PREPARADO;}
	bool desaparece_tras_colision() const {return false;}
	void cuentame() {std::cout<<"SOY OBSTACULO_800_32"<<std::endl;}
};

#endif
