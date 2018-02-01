#ifndef OBSTACULO_GENERADOR_DECORACION_H
#define OBSTACULO_GENERADOR_DECORACION_H

#include "obstaculo_actividad.h"
#include "../visitante/visitante.h"

/*Este obstáculo genera decoraciones en la posición en la que se encuentre 
cuando sea activado. Poco más. Hay varios tipos de decoraciones y algunos
parámetros para definir el comportamiento.*/

class Obstaculo_generador_decoracion:public Obstaculo_actividad
{
	protected:

	bool activo;

	unsigned int explosion;
	unsigned int chispas;
	unsigned int chatarra;
	unsigned int humo_explosion;

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	float vel_max() {return 0;}
	float vel_min() {return 0;}

	Obstaculo_generador_decoracion();
	virtual ~Obstaculo_generador_decoracion();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_ACTIVIDAD_GENERADOR_DECORACION;}
	bool es_afectado_por_edu() const {return false;}
	bool desaparece_tras_colision() const {return false;}
	bool es_parametrizable() const {return true;}
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	void cuentame() {std::cout<<"SOY OBSTACULO GENERADOR DECORACION"<<std::endl;}
	unsigned int tipo_a_colision_jugador() const {return T_NADA;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_TRIGGER;}

	bool es_representable() const {return false;}
	bool recibir_senal_trigger(const Accion_trigger_datos&);
	bool es_activo() const {return this->activo;}
	void activar() {this->activo=true;}
	void desactivar() {this->activo=false;}
	unsigned short int actuar_sonido() {return 0;}
	virtual bool es_fuera_de_juego() const {return false;}
	
	unsigned int acc_explosion() const {return this->explosion;}
	unsigned int acc_chispas() const {return this->chispas;}
	unsigned int acc_chatarra() const {return this->chatarra;}
	unsigned int acc_humo_explosion() const {return this->humo_explosion;}
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
