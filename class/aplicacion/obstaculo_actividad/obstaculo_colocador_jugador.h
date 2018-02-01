#ifndef OBSTACULO_COLOCADOR_JUGADOR_H
#define OBSTACULO_COLOCADOR_JUGADOR_H

/*Esto no tiene más propósito que ser una ayuda al desarrollo. Al colocarlo
el jugador se transporta automáticamente a su posición.
*/

#include "obstaculo_actividad.h"
#include "../visitante/visitante.h"

class Obstaculo_colocador_jugador:public Obstaculo_actividad
{
	protected:

	bool usado;

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	float vel_max() {return 0;}
	float vel_min() {return 0;}

	Obstaculo_colocador_jugador();
	virtual ~Obstaculo_colocador_jugador();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_COLOCADOR_JUGADOR;}
	unsigned int tipo_a_colision_jugador() const {return T_NADA;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_TRIGGER;}
	bool es_afectado_por_edu() const {return false;}
	bool desaparece_tras_colision() const {return false;}
	bool es_parametrizable() const {return false;}
	void cuentame() {std::cout<<"SOY OBSTACULO COLOCADOR DE JUGADOR"<<std::endl;}
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor& a){}
	bool es_representable() const {return false;}
	bool es_activo() const {return !usado;}
	void usar() {usado=true;}
	unsigned short int actuar_sonido() {return 0;}
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
