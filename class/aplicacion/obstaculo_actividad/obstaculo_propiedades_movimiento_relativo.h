#ifndef OBSTACULO_PROPIEDADES_MOVIMIENTO_RELATIVO_H
#define OBSTACULO_PROPIEDADES_MOVIMIENTO_RELATIVO_H

/*Se usa para almacenar la información de movimiento relativo, como el obstáculo
vector almacena las propiedades de un vector.*/

#include "obstaculo_actividad.h"
#include "../visitante/visitante.h"

class Obstaculo_propiedades_movimiento_relativo:public Obstaculo_actividad
{
	private:

	float desplazamiento_x;
	float desplazamiento_y;
	float velocidad;

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	float vel_max() {return 0;}
	float vel_min() {return 0;}

	Obstaculo_propiedades_movimiento_relativo();
	virtual ~Obstaculo_propiedades_movimiento_relativo();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_PROPIEDADES_MOVIMIENTO_RELATIVO;}
	unsigned int tipo_a_colision_jugador() const {return T_NADA;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_TRIGGER;}

	bool es_afectado_por_edu() const {return false;}
	bool desaparece_tras_colision() const {return false;}
	bool es_parametrizable() const {return true;}
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	void cuentame() {std::cout<<"SOY OBSTACULO DE MOVIMIENTO RELATIVO"<<std::endl;}
	virtual bool es_fuera_de_juego() const {return false;}
//	float acc_nuevo_vector_x() {return this->nuevo_vector_x;}
//	float acc_nuevo_vector_y() {return this->nuevo_vector_y;}

	bool es_representable() const {return false;}
	bool es_activo() const {return true;}
	unsigned short int actuar_sonido() {return 0;}
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
	float acc_desplazamiento_x() const {return desplazamiento_x;}
	float acc_desplazamiento_y() const {return desplazamiento_y;}
	float acc_velocidad() const {return velocidad;}
};

#endif
