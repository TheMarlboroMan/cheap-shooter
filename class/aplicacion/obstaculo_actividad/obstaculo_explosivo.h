#ifndef OBSTACULO_EXPLOSIVO_H
#define OBSTACULO_EXPLOSIVO_H

#include "obstaculo_actividad.h"
#include "../proyectil/proyectil.h"
#include "../visitante/visitante.h"

/*
Una mina. Cuando se pasa su tiempo explota en X trozos. Cuando el jugador la
derriba explotaría en menos.
*/

class Obstaculo_explosivo:public Obstaculo_actividad
{
	private:
	bool activo;
	void escoger_recorte_representacion();

	protected: 
	void establecer_representacion();
	void establecer_caracteristicas();
	float vel_max() {return 200;}
	float vel_min() {return 50;}
	float puntos_turno_actual;
	float puntos_turno_total;
	float puntos_1_3;
	float puntos_2_3;
	unsigned int total_disparos;
	float angulo_inicial;
	float salto_angulo;

	void procesar_tiempo_herido_ok() {escoger_recorte_representacion();}
	void procesar_tiempo_herido_herido() {Representable::establecer_recorte(48,240,16,16);}

	public:

	Obstaculo_explosivo(unsigned int);
	virtual ~Obstaculo_explosivo();

	void procesar_turno(float=0);
	bool es_activo() const;
	bool procesar_colision_proyectil(Proyectil *);

	Proyectil * explotar();
	void dividir_cantidad(unsigned int);
	unsigned short int actuar_sonido() {return Recursos::S_DISPARO;}
	short int puntuacion_por_destruir() {return 300;}
	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR_GENERA_PROYECTILES;}

	virtual bool es_fuera_de_juego() const {return false;} //Este no sale fuera de juego porque tarde o temprano explotará.
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	void cuentame() {std::cout<<"SOY OBSTACULO EXPLOSIVO"<<std::endl;}
	short unsigned int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_ACTIVIDAD_EXPLOSIVO;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_NO_PREPARADO;}
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
	unsigned int acc_columnas_humo_explosion() const {return 3;}
	virtual bool es_cuenta_destruible() const {return true;}

};

#endif
