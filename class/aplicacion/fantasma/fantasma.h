#ifndef FANTASMA_H
#define FANTASMA_H

#include "../actor_jugable/actor_jugable.h"

class Fantasma:public Actor_jugable
{
	protected:

	bool visible;
	
	float factor_retraccion_vectores(){return 600.f;} //por segundo...
	float maximo_vector_desplazamiento() {return 600.f;}
	float factor_multiplicador_input() {return 400.f;}

	void establecer_representacion();
	
	public:

	bool es_visible() {return this->visible;}

	Fantasma();
	virtual ~Fantasma();
	void hacer_visible(float, float);
	void hacer_invisible();
	void procesar_turno(float=0);

	void cuentame() {std::cout<<"SOY FANTASMA"<<std::endl;}
};

#endif
