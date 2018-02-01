#ifndef PROCESO_SUPERFICIE_BASE_POR_PIXEL_H
#define PROCESO_SUPERFICIE_BASE_POR_PIXEL_H

/*Define la base para cualquier proceso que actúe sobre todos los píxeles
de una superficie. Podemos usarlo para extenderlo a otros procesos que 
incorporen el método procesar_pixel(Uint32, Uint32&, Uint32)... Simplemente nos ahorraremos
el bucle en todos esos métodos y estará todo un poco más ordenado.

Está indicado para aquellas clases que pueden operar sobre píxeles individuales
con sus propios parámetros, sin necesitar nada más.

Las clases que lo extiendan deben implementar el método procesar_pixel, con
el prototipo:

virtual bool procesar_pixel(Uint32 color_original, Uint32& color_resultante, Uint32 color_transparente)

Que devolverá true si debe hacerse aplicarse el color resultante al pixel actual.

Desde dentro pueden acceder a this->acc_formato() para obtener el formato de la
superficie.
*/

#include "proceso_superficie.h"
#include "../utilidades_graficas_sdl/utilidades_graficas_sdl.h"
#include "../gestor_color/gestor_color.h"

namespace DLibV
{

class Proceso_superficie_base_por_pixel:public Proceso_superficie
{
	private:

	SDL_PixelFormat * formato;
	Uint32 color_transparente;
 
	protected:

	Proceso_superficie_base_por_pixel();
	void proceso(SDL_Surface * , float=0);
	void proceso_parcial(SDL_Surface *, SDL_Rect, float=0);
	virtual bool procesar_pixel(Uint32, Uint32&, Uint32)=0;
	const SDL_PixelFormat * acc_formato() const {return this->formato;}

	public:

	~Proceso_superficie_base_por_pixel();
	void mut_color_transparente(Uint32 p_valor) {this->color_transparente=p_valor;}

};

} //Fin namespace DLibV

#endif
