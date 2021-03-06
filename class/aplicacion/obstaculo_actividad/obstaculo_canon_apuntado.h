#ifndef OBSTACULO_CANON_APUNTADO_H
#define OBSTACULO_CANON_APUNTADO_H

#include "obstaculo_actividad.h"
#include "../proyectil/proyectil.h"
#include "../visitante/visitante.h"

class Obstaculo_canon_apuntado:public Obstaculo_actividad
{
	private:
	
	static const float INTERLUDIO_ENTRE_DISPAROS;

	protected: 
	void establecer_representacion();
	void establecer_caracteristicas();
	float vel_max() {return 0;}
	float vel_min() {return 0;}

	float tiempo_descanso;
	float tiempo_activo;

	float tiempo_descanso_actual;
	float tiempo_activo_actual;
	float interludio_entre_disparos;

	bool activo;
	bool encendido;

	public:

	Obstaculo_canon_apuntado();
	virtual ~Obstaculo_canon_apuntado();

	void procesar_turno(float=0);
	bool es_activo() const;
	Proyectil * disparar(const Actor&);
	unsigned short int actuar_sonido() {return Recursos::S_DISPARO;}
	short int puntuacion_por_destruir() {return 0;}
	bool es_afectado_por_edu() const {return false;}
	virtual bool es_fuera_de_juego() const {return false;}

	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL;}

	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	void cuentame() {std::cout<<"SOY OBSTACULO CANON APUNTADO"<<std::endl;}
	short unsigned int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_ACTIVIDAD_CANON_APUNTADO;}
	bool recibir_senal_trigger(const Accion_trigger_datos&);
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
