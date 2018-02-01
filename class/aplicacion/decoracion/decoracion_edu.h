#ifndef DECORACION_EDU_H
#define DECORACION_EDU_H

/*
*/

#include "decoracion.h"

class Decoracion_edu:public Decoracion
{
	private:

	float static const FACTOR_CRECIMIENTO;	//Cada segundo crecerá 200px.
	float duracion;
	float duracion_original;
	float factor;
	float radio_actual;
	Uint32 color_actual;

	void actualizar_representacion();
	static Uint32 calcular_color(float, float);

	/*Hay una razón para que tenga su propia representación: se llamarán a 
	varios métodos que son únicos para la circunferencia de modo que el 
	puntero báse de actor - del tipo Representacion - no nos servirá. 
	Simplemente crearemos la representación y la enlazaremos con el puntero 
	base para que el código de fuera haga lo que tenga que hacer y, desde 
	dentro, podamos manipular la representación*/

	DLibV::Primitiva_grafica_circunferencia_dinamica * representacion_propia;

	protected:

	void establecer_representacion();
	void establecer_caracteristicas();

	public:

	unsigned short int tipo_decoracion() {return Decoracion::D_EDU;}

	Decoracion_edu(float, float, float, unsigned short int);
	~Decoracion_edu();

	void procesar_turno(float=0);
	bool es_finalizado();
	void cuentame() {std::cout<<"SOY DECORACION EDU"<<std::endl;}
};

#endif
