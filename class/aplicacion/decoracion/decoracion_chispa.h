#ifndef DECORACION_CHISPA_H
#define DECORACION_CHISPA_H

/*

*/


#include "decoracion.h"

class Decoracion_chispa:public Decoracion
{
	private:

	float duracion;	//La duración que le queda. A cada turno se resta y a cada turno se escoge un frame u otro.
	unsigned int gama;

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	enum gamas{
		TODAS=-1,
		AZULES=0,
		ROJOS=1,
		NARANJAS=2,
		AMARILLOS=3,
		VERDES=4,
			MAX_GAMA};

	unsigned short int tipo_decoracion() {return Decoracion::D_CHISPA;}

	Decoracion_chispa(float, float, short int=-1);
	~Decoracion_chispa();

	void establecer_vector(const DLibH::Vector_2d&);
	void establecer_duracion(float d) {duracion=d;}
	void procesar_turno(float=0);
	bool es_finalizado();
	void cuentame() {std::cout<<"SOY UNA CHISPA DECORATIVA"<<std::endl;}
};

#endif
