#ifndef OBSTACULO_DESTRUIBLE_H
#define OBSTACULO_DESTRUIBLE_H

#include "../actor_mapa/actor_mapa.h"

class Obstaculo_destruible:public Actor_mapa
{
	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	void procesar_tiempo_herido_herido() {this->establecer_recorte(32,256,32,32);}
	void procesar_tiempo_herido_ok() {this->establecer_recorte(0,256,32,32);}

	public:

	Obstaculo_destruible(unsigned int);
	virtual ~Obstaculo_destruible();

	float vel_max() {return 150;}
	float vel_min() {return 50;}
	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_DESTRUIBLE;}
	short int puntuacion_por_destruir() {return 20;}
	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR_DA_ITEM;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_NO_PREPARADO;}

	bool procesar_colision_proyectil(Proyectil *);
	void cuentame() {std::cout<<"SOY OBSTACULO ROMPIBLE"<<std::endl;}
	virtual bool es_cuenta_destruible() const {return true;}
};

#endif
