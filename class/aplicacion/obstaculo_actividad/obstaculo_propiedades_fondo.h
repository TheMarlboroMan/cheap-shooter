#ifndef OBSTACULO_PROPIEDADES_FONDO_H
#define OBSTACULO_PROPIEDADES_FONDO_H

/*Este obstáculo ocupa todo el ancho y afecta a los parámetros del fondo de 
estrellas.*/

#include "obstaculo_actividad.h"
#include "../visitante/visitante.h"

class Obstaculo_propiedades_fondo:public Obstaculo_actividad
{
	protected:

	unsigned int rojo;
	unsigned int verde;
	unsigned int azul;
	unsigned int velocidad;	//Velocidad del cambio de color.
	unsigned int estrellas;	//Nueva cantidad de estrellas. Un valor de 401 es "No cambies". 

	bool activo;
	bool iniciado_cambio;
	bool finalizado_cambio;

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	float vel_max() {return 0;}
	float vel_min() {return 0;}

	Obstaculo_propiedades_fondo();
	virtual ~Obstaculo_propiedades_fondo();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_PROPIEDADES_FONDO;}
	bool es_afectado_por_edu() const {return false;}
	bool desaparece_tras_colision() const {return false;}
	bool es_parametrizable() const {return true;}
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	void cuentame() {std::cout<<"SOY OBSTACULO PROPIEDADES FONDO"<<std::endl;}
	bool es_representable() const {return false;}
	bool es_activo() const {return this->activo;}
	virtual bool es_fuera_de_juego() const {return false;}

	unsigned int tipo_a_colision_jugador() const {return T_NADA;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_TRIGGER;}
	unsigned short int actuar_sonido() {return 0;}

	bool es_finalizado_cambio() const {return this->finalizado_cambio;}
	void finalizar_proceso() 
	{
		this->activo=false;
		this->iniciado_cambio=false;
		this->finalizado_cambio=true;
	}

	bool es_iniciado_cambio() const {return this->iniciado_cambio;}
	void iniciar_cambio() {this->iniciado_cambio=true;}

	bool recibir_senal_trigger(const Accion_trigger_datos&);
	unsigned int acc_rojo() const {return this->rojo;}
	unsigned int acc_verde() const {return this->verde;}
	unsigned int acc_azul() const {return this->azul;}
	unsigned int acc_velocidad() const {return this->velocidad;}
	unsigned int acc_estrellas() const {return this->estrellas;}
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
