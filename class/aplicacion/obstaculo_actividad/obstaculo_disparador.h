#ifndef OBSTACULO_DISPARADOR_H
#define OBSTACULO_DISPARADOR_H

#include "obstaculo_actividad.h"
#include "../proyectil/proyectil.h"
#include "../visitante/visitante.h"

/*
Obstáculo que puede disparar al jugador. Aparece con ciertos puntos por turno
y dispara cuando llega a cero. A partir de ese momento se recupera.
*/

class Obstaculo_disparador:public Obstaculo_actividad
{
	private:

	enum MODOS_ACTUACION{
	M_AUTO=0,
	M_VECTORES_MANUAL=1
	};

	mutable bool activo;
	bool activado_modo_actuacion;
	DLibH::Vector_2d v_actuacion;
	unsigned int modo_actuacion;
	unsigned int ruido_punteria;

	void activar_modo_actuacion();

	protected: 
	
	void procesar_tiempo_herido_herido() {this->establecer_recorte(96,224,32,32);}
	void procesar_tiempo_herido_ok() {this->establecer_recorte(64,224,32,32);}

	void establecer_representacion();
	void establecer_caracteristicas();
	float vel_max() {return 100;}
	float vel_min() {return 30;}
	float puntos_turno_actual;
	float puntos_turno_total;

	public:

	Obstaculo_disparador(unsigned int);
	virtual ~Obstaculo_disparador();

	void procesar_turno(float=0);
	bool es_activo() const;
	bool procesar_colision_proyectil(Proyectil *);
	Proyectil * disparar(const Actor&);
	unsigned short int actuar_sonido() {return Recursos::S_DISPARO;}
	short int puntuacion_por_destruir() {return 200;}

	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	void cuentame() {std::cout<<"SOY OBSTACULO DISPARADOR"<<std::endl;}
	short unsigned int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_ACTIVIDAD_DISPARADOR;}
	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_NO_PREPARADO;}
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
	unsigned int acc_columnas_humo_explosion() const {return 6;}
	virtual bool es_cuenta_destruible() const {return true;}

};

#endif
