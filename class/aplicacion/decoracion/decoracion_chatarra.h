#ifndef DECORACION_CHATARRA_H
#define DECORACION_CHATARRA_H

/*
*/


#include "decoracion.h"

class Decoracion_chatarra:public Decoracion
{
	private:

	float duracion;	//La duración que le queda. A cada turno se resta y a cada turno se escoge un frame u otro.
	void actualizar_representacion();

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	unsigned short int tipo_decoracion() {return Decoracion::D_CHATARRA;}

	Decoracion_chatarra(float, float);
	~Decoracion_chatarra();

	void procesar_turno(float=0);
	bool es_finalizado();
	void cuentame() {std::cout<<"SOY CHATARRA DECORATIVA"<<std::endl;}
};

#endif
