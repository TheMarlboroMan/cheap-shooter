#ifndef OBSTACULO_PUERTA_DISPARAR_H
#define OBSTACULO_PUERTA_DISPARAR_H

#include "obstaculo_puerta.h"
#include "../visitante/visitante.h"

class Obstaculo_puerta_disparar:public Obstaculo_puerta
{
	private:

	float tiempo_abierta;
	float tiempo_abierta_actual;

	public:

	Obstaculo_puerta_disparar(unsigned int);
	virtual ~Obstaculo_puerta_disparar();
	virtual void implementacion_procesar_turno(float);

	//Esta es propia.
	bool procesar_colision_proyectil(Proyectil *);

	//Todas estas deben ser implementadas.
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL_NO_RESTA_COMBO;}
	virtual unsigned short int actuar_sonido();
	virtual void cuentame() {std::cout<<"SOY OBSTACULO PUERTA DISPARAR"<<std::endl;}
	virtual short unsigned int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_ACTIVIDAD_PUERTA_DISPARAR;}
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
