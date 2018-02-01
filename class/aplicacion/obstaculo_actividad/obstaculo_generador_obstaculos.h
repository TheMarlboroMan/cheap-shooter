#ifndef OBSTACULO_GENERADOR_OBSTACULOS_H
#define OBSTACULO_GENERADOR_OBSTACULOS_H

#include "obstaculo_actividad.h"
#include "../visitante/visitante.h"

/*Este obstáculo genera otros obstáculos... No los genera en una posición
concreta, sino en toda su dimensión.*/

class Obstaculo_generador_obstaculos:public Obstaculo_actividad
{
	private:

	void calcular_nuevo_tiempo_descanso();

	unsigned int tipo_obstaculo; //Tipo de obstáculo que genera.
	bool toda_dimension;		//Lo genera en cualquier punto de su dimensión o no.
	unsigned int angulo_inicial;	//Vector inicial.
	unsigned int angulo_final;	//Vector final.
	float tiempo_descanso;		//Tiempo inactivo... 
	unsigned int factor_aleatorio;		//Factor que se suma al tiempo inactivo.
	unsigned int multiplicador_vector;
	bool encendido;

	float tiempo_descanso_actual; 	//Tiempo de descanso...

	bool activo;

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	enum tipos_obstaculo{
	TO_26_26,
	TO_32_64,
	TO_64_26,
	TO_128_32
	};

	float vel_max() {return 0;}
	float vel_min() {return 0;}

	Obstaculo_generador_obstaculos();
	virtual ~Obstaculo_generador_obstaculos();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_ACTIVIDAD_GENERADOR_OBSTACULOS;}
	bool es_afectado_por_edu() const {return false;}
	bool desaparece_tras_colision() const {return false;}
	bool es_parametrizable() const {return true;}
	bool es_redimensionable() const {return true;}
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	unsigned int tipo_a_colision_jugador() const {return T_NADA;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_TRIGGER;}

	void cuentame() {std::cout<<"SOY OBSTACULO GENERADOR DE OBSTACULOS"<<std::endl;}

	virtual bool es_fuera_de_juego() const {return false;}
	void procesar_turno(float);
	bool es_representable() const {return false;}
	bool recibir_senal_trigger(const Accion_trigger_datos&);
	bool es_activo() const {return this->activo;}
	void activar() {this->activo=true;}
	void desactivar() {this->activo=false;}
	unsigned short int actuar_sonido() {return 0;}
	
	unsigned int acc_tipo_obstaculo() const {return this->tipo_obstaculo;}
	bool es_encendido() const {return this->encendido;}
	DLibH::Vector_2d obtener_vector() const;
	void obtener_posicion(float&, float&);
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}

};

#endif
