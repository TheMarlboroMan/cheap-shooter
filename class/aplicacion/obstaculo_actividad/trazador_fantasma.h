#ifndef TRAZADOR_FANTASMA_H
#define TRAZADOR_FANTASMA_H

#include "obstaculo_actividad.h"
#include "../visitante/visitante.h"

/*El trazador del fantasma en un actor que viaja del punto A al punto B soltando
chispas. Es invisible... La idea es que al activar el teletransporte aparezca
rápidamente y desaparezca tan pronto como haya llegado a su destino. Para
ver cuando llega a su destino calculamos la distancia entre puntos, la velocidad
y mediamos su "duración".

Es del tipo Obstáculo_actividad para que podamos hacer que genere una actividad,
que parece que es como estamos haciendo últimamente las cosas.*/


class Trazador_fantasma:public Obstaculo_actividad
{
	protected:

	float duracion;
	static const float pixeles_segundo;

	static const int CONTADOR_REFLEJO=20;
	float contador_reflejo;

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	float vel_max() {return 0;}
	float vel_min() {return 0;}

	Trazador_fantasma();
	virtual ~Trazador_fantasma();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_TRAZADOR_FANTASMA;}
	bool es_afectado_por_edu() const {return false;}
	bool desaparece_tras_colision() const {return false;}
	bool es_parametrizable() const {return false;}
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor& a){}	//Ya es la segunda vez que tenemos que escribir este método cuando no se necesita...
	void cuentame() {std::cout<<"SOY TRAZADOR DE FANTASMA"<<std::endl;}
	unsigned int tipo_a_colision_jugador() const {return T_NADA;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_JUGADOR;}
	unsigned int tipo_a_cuenta() const {return T_NADA;}
	void procesar_turno(float);

	bool es_representable() const {return false;}
	bool es_activo() const {return true;}
	unsigned short int actuar_sonido() {return 0;}
	
	void configurar(const Actor * const, const Actor * const);
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
	bool debe_dejar_reflejo();
};


#endif
