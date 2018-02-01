#ifndef PROCESO_SUPERFICIE_BLANCO_NEGRO_H
#define PROCESO_SUPERFICIE_BLANCO_NEGRO_H

/*Convierte la superficie a blanco y negro. Si lo hacemos en tiempo real hay
una bajada de performance enorme (de 120fps a 20) de modo que no tiene mucho
futuro si no es para usarlo sobre alguna superficie concreta para 
transformarla una vez.*/

#include "proceso_superficie_base_por_pixel.h"

namespace DLibV
{

class Proceso_superficie_blanco_negro:public Proceso_superficie_base_por_pixel
{
	private:
	
	Uint8 r, g, b, a;
	bool respetar_transparencia;
	Proceso_superficie_blanco_negro(bool);

	protected:
	bool procesar_pixel(Uint32, Uint32&, Uint32);

	public:

	~Proceso_superficie_blanco_negro();
	static Proceso_superficie_blanco_negro * generar(bool=false);
};

} //Fin namespace DLibV

#endif
