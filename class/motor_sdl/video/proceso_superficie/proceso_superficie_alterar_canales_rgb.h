#ifndef PROCESO_SUPERFICIE_ALTERAR_CANALES_RGB_H
#define PROCESO_SUPERFICIE_ALTERAR_CANALES_RGB_H

#include "proceso_superficie_base_por_pixel.h"

namespace DLibV
{

class Proceso_superficie_alterar_canales_rgb:public Proceso_superficie_base_por_pixel
{
	private:
	
	int cambio_r, cambio_g, cambio_b;
	int tr, tg, tb; 
	Uint8 r, g, b, a;

	bool respetar_transparencia;
	Proceso_superficie_alterar_canales_rgb(int, int, int, bool);

	protected:
	bool procesar_pixel(Uint32, Uint32&, Uint32);

	public:

	~Proceso_superficie_alterar_canales_rgb();
	static Proceso_superficie_alterar_canales_rgb * generar(int, int, int, bool=false);
};

} //Fin namespace DLibV

#endif
