#ifndef OBSTACULO_ALEATORIO_H
#define OBSTACULO_ALEATORIO_H

#include "obstaculo.h"

/*
Obstáculos de niveles aleatorios...
*/

class Obstaculo_aleatorio:public Obstaculo
{
	protected:

	unsigned int tipo_actor_aleatorio;
	bool desaparece_colision;
	bool afectado_edu;

	void establecer_representacion();
	void establecer_caracteristicas();
	void crear_representacion(const SDL_Rect&);
	void actualizar_representacion();

	public:

	Obstaculo_aleatorio(unsigned int, unsigned int, bool, bool);
	virtual ~Obstaculo_aleatorio();

	unsigned int tipo_a_colision_proyectil() const {return T_DESTRUYE_PROYECTIL;}
	unsigned int tipo_a_faccion() const {return Definiciones::F_NO_PREPARADO;}
	unsigned short int tipo_actor_mapa() {return this->tipo_actor_aleatorio;}
	bool es_afectado_por_edu() const {return this->afectado_edu;}
	bool desaparece_tras_colision() const {return this->desaparece_colision;}
	void cuentame() {std::cout<<"SOY ACTOR ALEATORIO "<<this->tipo_actor_aleatorio<<std::endl;}	
};

#endif
