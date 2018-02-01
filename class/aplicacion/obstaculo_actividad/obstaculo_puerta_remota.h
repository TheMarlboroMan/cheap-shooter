#ifndef OBSTACULO_PUERTA_REMOTA_H
#define OBSTACULO_PUERTA_REMOTA_H

#include "obstaculo_puerta.h"
#include "../visitante/visitante.h"

class Obstaculo_puerta_remota:public Obstaculo_puerta
{
	private:

	float tiempo_abierta;
	float tiempo_abierta_actual;
	bool autocierre;
	bool iniciado_movimiento;

	public:

	Obstaculo_puerta_remota(unsigned int);
	virtual ~Obstaculo_puerta_remota();
	virtual void implementacion_procesar_turno(float);

	//Todas estas deben ser implementadas.
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	virtual unsigned short int actuar_sonido();
	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL;}
	virtual void cuentame() {std::cout<<"SOY OBSTACULO PUERTA REMOTA"<<std::endl;}
	virtual short unsigned int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_ACTIVIDAD_PUERTA_REMOTA;}
	bool recibir_senal_trigger(const Accion_trigger_datos&);
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
