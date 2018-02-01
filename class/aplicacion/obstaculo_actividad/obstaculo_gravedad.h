#ifndef OBSTACULO_GRAVEDAD_H
#define OBSTACULO_GRAVEDAD_H

#include "obstaculo_actividad.h"
#include "../visitante/visitante.h"

/*
Obstáculo que tiene la capacidad de atraer al resto de proyectiles y obstáculos
del mapa (no atrae al jugador). Empieza con una carga de que se va vaciando
a cada turno de forma constante. Cuando la carga llega a la mitad empieza a 
atraer a otros obstáculos hasta que se agota.
*/

class Obstaculo_gravedad:public Obstaculo_actividad
{
	private:

	protected: 
	void establecer_representacion();
	void establecer_caracteristicas();
	float vel_max() {return 100;}
	float vel_min() {return 30;}
	float tiempo_turno_total;
	float tiempo_turno_actual;
	float tiempo_turno_mitad;
	bool ha_emitido_sonido;

	void procesar_tiempo_herido_herido() {this->establecer_recorte(96,128,32,32);}
	void procesar_tiempo_herido_ok() {this->establecer_recorte(64,128,32,32);}

	public:

	Obstaculo_gravedad(unsigned int);
	virtual ~Obstaculo_gravedad();

	void procesar_turno(float=0);
	bool es_activo() const;
	bool procesar_colision_proyectil(Proyectil *);
	short int puntuacion_por_destruir() {return 200;}
	unsigned short int actuar_sonido();

	void cuentame() {std::cout<<"SOY OBSTACULO GRAVEDAD"<<std::endl;}
	short unsigned int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_ACTIVIDAD_GRAVEDAD;}
	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR;}

	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	unsigned int tipo_a_faccion() const {return Definiciones::F_NO_PREPARADO;}
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
	unsigned int acc_columnas_humo_explosion() const {return 8;}
	virtual bool es_cuenta_destruible() const {return true;}
};

#endif
