#ifndef OBSTACULO_VECTOR_H
#define OBSTACULO_VECTOR_H

/*Tiene una posición fija y sirve para almacenar dos datos, vector x e y. Se 
usará para aplicar vectores a los actores por medio de acciones.*/

#include "obstaculo_actividad.h"
#include "../visitante/visitante.h"

class Obstaculo_vector:public Obstaculo_actividad
{
	protected:

	float nuevo_vector_x;
	float nuevo_vector_y;

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	float vel_max() {return 0;}
	float vel_min() {return 0;}

	Obstaculo_vector();
	virtual ~Obstaculo_vector();

	unsigned short int tipo_actor_mapa() {return Actor_mapa::T_OBSTACULO_VECTOR;}
	unsigned int tipo_a_colision_jugador() const {return T_NADA;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_TRIGGER;}

	bool es_afectado_por_edu() const {return false;}
	bool desaparece_tras_colision() const {return false;}
	bool es_parametrizable() const {return true;}
	void parametrizar_actor_en_nivel_preparado(const Parametros_actor&);
	void cuentame() {std::cout<<"SOY OBSTACULO DE VECTOR"<<std::endl;}
	virtual bool es_fuera_de_juego() const {return false;}
	float acc_nuevo_vector_x() {return this->nuevo_vector_x;}
	float acc_nuevo_vector_y() {return this->nuevo_vector_y;}

	bool es_representable() const {return false;}
	bool es_activo() const {return true;}
	unsigned short int actuar_sonido() {return 0;}
	void aceptar_visitante(Visitante& v) {v.visitar(*this);}
};

#endif
