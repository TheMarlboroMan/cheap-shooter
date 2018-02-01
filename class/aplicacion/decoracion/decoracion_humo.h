#ifndef DECORACION_HUMO_H
#define DECORACION_HUMO_H

#include "decoracion.h"

class Decoracion_humo:public Decoracion
{
	public:
	static const float MAX_DURACION;

	private:

	float duracion;	//La duración que le queda. A cada turno se resta y a cada turno se escoge un frame u otro.
	float duracion_original;
	void actualizar_representacion();
	unsigned short int parte;	
	bool auto_actualizar_parte;

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	unsigned short int tipo_decoracion() {return Decoracion::D_HUMO;}

	Decoracion_humo(float, float, unsigned short int=0);
	~Decoracion_humo();

	void establecer_duracion(float d)
	{
		duracion=d;
		duracion_original=d;
	}

	void activar_auto_parte() {auto_actualizar_parte=true;}
	void desactivar_auto_parte() {auto_actualizar_parte=false;}

	void procesar_turno(float=0);
	bool es_finalizado();
	void cuentame() {std::cout<<"SOY HUMO DECORATIVO"<<std::endl;}
};

#endif
