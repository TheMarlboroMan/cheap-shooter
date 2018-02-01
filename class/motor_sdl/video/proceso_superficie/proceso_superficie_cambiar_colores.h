#ifndef PROCESO_SUPERFICIE_CAMBIAR_COLORES_H
#define PROCESO_SUPERFICIE_CAMBIAR_COLORES_H

/*Convierte la superficie a blanco y negro. Si lo hacemos en tiempo real hay
una bajada de performance enorme (de 120fps a 20) de modo que no tiene mucho
futuro si no es para usarlo sobre alguna superficie concreta para 
transformarla una vez.*/

#include "proceso_superficie_base_por_pixel.h"

namespace DLibV
{

class Proceso_superficie_cambiar_colores:public Proceso_superficie_base_por_pixel
{
	private:
	
	Proceso_superficie_cambiar_colores();
	Uint32 color_original;
	Uint32 nuevo_color;

	protected:
	bool procesar_pixel(Uint32, Uint32&, Uint32);

	public:

	~Proceso_superficie_cambiar_colores();
	static Proceso_superficie_cambiar_colores * generar(Uint32, Uint32);
};

} //Fin namespace DLibV

#endif
