#ifndef OBSTACULO_TRIGGER_AUTO_H
#define OBSTACULO_TRIGGER_AUTO_H

/*Este es un obstáculo de actividad aunque podemos pensar en él como en un 
trigger mal hecho porque todo esto está pensado después de montada la
estructura del motor... Básicamente lanzará la acción asociada a su parámetro
tag_acción cuando entre en pantalla. Puede estar "encendido" o "apagado". En
este último caso, al entrar en pantalla no procesa su actividad...*/

#include "obstaculo_actividad.h"
#include "../trigger/trigger.h"
#include "../visitante/visitante.h"

class Obstaculo_trigger_auto:public Obstaculo_actividad, public Trigger
{
	private:

	bool encendido;

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	float vel_max() {return 0;}
	float vel_min() {return 0;}

	Obstaculo_trigger_auto();
	virtual ~Obstaculo_trigger_auto();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_TRIGGER_AUTO;}
	bool es_afectado_por_edu() const {return false;}
	bool desaparece_tras_colision() const {return false;}
	bool es_parametrizable() const {return true;}
	void cuentame() {std::cout<<"SOY OBSTACULO TRIGGER AUTO"<<std::endl;}
	unsigned int tipo_a_colision_jugador() const {return T_NADA;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_TRIGGER;}
	bool es_fuera_de_juego() const {return false;}
	bool es_representable() const {return false;}
	unsigned short int actuar_sonido() {return 0;}

	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	bool es_activo() const;
	bool recibir_senal_trigger(const Accion_trigger_datos&);
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
