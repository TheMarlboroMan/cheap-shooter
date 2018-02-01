#ifndef OBSTACULO_TUTORIAL_H
#define OBSTACULO_TUTORIAL_H

/*Este obstáculo activa el modo tutorial cuando es activado al aparecer en la 
pantalla.
Al activarse, generará un tutorial en el controlador y se marcará como usado para
no volver a activarse. A partir de ese momento el controlador se encargará de
manejar el tutorial y se olvidará de que este obstáculo existe.
*/

#include "obstaculo_actividad.h"
#include "../visitante/visitante.h"

class Obstaculo_tutorial:public Obstaculo_actividad
{
	protected:

	unsigned int codigo_mensaje;	//El código determinará la composición del mensaje a mostrar.
	unsigned int ms_tiempo;		//Este será el tiempo mínimo que aparecerá el mensaje.
	bool finaliza_tutoriales;
	bool usado;

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	float vel_max() {return 0;}
	float vel_min() {return 0;}

	Obstaculo_tutorial();
	virtual ~Obstaculo_tutorial();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_TUTORIAL;}
	unsigned int tipo_a_colision_jugador() const {return T_NADA;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_TRIGGER;}
	bool es_afectado_por_edu() const {return false;}
	bool desaparece_tras_colision() const {return false;}
	bool es_parametrizable() const {return true;}
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	void cuentame() {std::cout<<"SOY OBSTACULO TUTORIAL"<<std::endl;}
	unsigned int acc_codigo_mensaje() const {return codigo_mensaje;}
	unsigned int acc_ms_tiempo() const {return ms_tiempo;}
	bool es_representable() const {return false;}
	bool es_finaliza_tutoriales() const {return finaliza_tutoriales;}
	bool es_activo() const {return !usado && es_en_pantalla();}
	void usar() {usado=true;}
	unsigned short int actuar_sonido() {return 0;}
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
