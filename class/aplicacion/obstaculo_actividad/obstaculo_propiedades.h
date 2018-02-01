#ifndef OBSTACULO_PROPIEDADES_H
#define OBSTACULO_PROPIEDADES_H

/*Este obstáculo ocupa todo el ancho y afecta a los parámetros del nivel en
varias medidas: activa y desactiva los artefactos que se pueden usar y modifica
la velocidad del nivel.*/

#include "obstaculo_actividad.h"
#include "../visitante/visitante.h"

class Obstaculo_propiedades:public Obstaculo_actividad
{
	protected:

	unsigned int propiedades_artefactos;
	unsigned int nueva_velocidad;
	unsigned int velocidad_cambio;
	bool marca_fin_nivel;
	bool activo;
	bool usado;

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	float vel_max() {return 0;}
	float vel_min() {return 0;}

	Obstaculo_propiedades();
	virtual ~Obstaculo_propiedades();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_PROPIEDADES;}
	bool es_afectado_por_edu() const {return false;}
	bool desaparece_tras_colision() const {return false;}
	bool es_parametrizable() const {return true;}
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	void cuentame() {std::cout<<"SOY OBSTACULO PROPIEDADES"<<std::endl;}
	unsigned int acc_nueva_velocidad() const {return this->nueva_velocidad;}
	unsigned int acc_propiedades_artefactos() const {return this->propiedades_artefactos;}
	unsigned int acc_velocidad_cambio() const {return this->velocidad_cambio;}
	unsigned int tipo_a_colision_jugador() const {return T_NADA;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_TRIGGER;}

	bool es_representable() const {return false;}
	bool recibir_senal_trigger(const Accion_trigger_datos&);
	bool es_usado() const {return this->usado;}
	void usar() {this->usado=true;}
	bool es_activo() const {return this->activo;}
	void activar() {this->activo=true;}
	virtual bool es_fuera_de_juego() const {return false;}
	bool es_marca_fin_nivel() const {return this->marca_fin_nivel;}

	unsigned short int actuar_sonido() {return 0;}
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
