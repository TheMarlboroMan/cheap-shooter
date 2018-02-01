#ifndef DECORACION_FANTASMA_H
#define DECORACION_FANTASMA_H

#include "decoracion.h"

class Decoracion_fantasma:public Decoracion
{
	private:

	float duracion;	//La duración que le queda. A cada turno se resta y a cada turno se escoge un frame u otro.
	void actualizar_representacion();
	unsigned short int parte;	

	static const float MAX_DURACION;

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	unsigned short int tipo_decoracion() {return Decoracion::D_FANTASMA;}

	Decoracion_fantasma(float, float);
	~Decoracion_fantasma();

	void procesar_turno(float=0);
	bool es_finalizado();
	void cuentame() {std::cout<<"SOY FANTASMA DECORATIVO"<<std::endl;}
};

#endif
