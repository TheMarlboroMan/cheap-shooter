#ifndef DECORACION_EXPLOSION_H
#define DECORACION_EXPLOSION_H

/*
Tipo de decoración que simplemente hace una explosión. La idea es que tiene
una duración determinada y en función de la cantidad de tiempo que le quede
mostraría un frame u otro.
*/


#include "decoracion.h"

class Decoracion_explosion:public Decoracion
{
	private:

	float duracion;	//La duración que le queda. A cada turno se resta y a cada turno se escoge un frame u otro.
	void actualizar_representacion();

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	unsigned short int tipo_decoracion() {return Decoracion::D_EXPLOSION;}

	Decoracion_explosion(float, float, DLibH::Vector_2d);
	~Decoracion_explosion();

	void procesar_turno(float=0);
	bool es_finalizado();
	void cuentame() {std::cout<<"SOY UNA EXPLOSION DECORATIVA"<<std::endl;}
};

#endif
