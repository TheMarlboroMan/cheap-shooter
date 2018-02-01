#ifndef OBSTACULO_DESTRUIBLE_SECRETO_H
#define OBSTACULO_DESTRUIBLE_SECRETO_H

#include "../obstaculo_destruible/obstaculo_destruible.h"

//A todas luces es igual que el obstáculo destruible. De hecho no tiene especificados
//otros métodos que no sean el destructor y el constructor.

class Obstaculo_destruible_secreto:public Obstaculo_destruible
{
	public:

	Obstaculo_destruible_secreto(unsigned int);
	virtual ~Obstaculo_destruible_secreto();

	float vel_max() {return 150;}
	float vel_min() {return 50;}
	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_DESTRUIBLE_SECRETO;}
	short int puntuacion_por_destruir() {return 20;}
	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR_ES_SECRETO;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_NO_PREPARADO;}
	virtual bool es_cuenta_destruible() const {return true;}
	void cuentame() {std::cout<<"SOY OBSTACULO ROMPIBLE SECRETO"<<std::endl;}
};

#endif
