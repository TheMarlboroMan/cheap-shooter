#ifndef PRIMITIVA_GRAFICA_H
#define PRIMITIVA_GRAFICA_H

#include <SDL/SDL.h>
#include "../representacion.h"
#include "../../../herramientas/herramientas_sdl/herramientas_sdl.h"

/*Esto será una clase abstracta que podremos usar para volcar
en una pantalla... De por si no hay nada de código aquí, solo
los esqueletos de lo que vamos a usar más adelante.
*/

namespace DLibV
{

class Primitiva_grafica:public Representacion
{
	private:

	Uint32 pixel_color;
//	bool combinar_color;
	bool recomponer_recorte_con_posicion;

	//Esto es un puntero void a algo que podemos pasar al método de turno,
	//por si acaso queremos hacer algo más sofisticado. Todas las
	//clases que deriven de esta podrían hacer uso de este puntero.
	
	void * parametro_metodo_pixeles;

	protected:

	virtual void preparar_posicion()=0; //Este método es obligatorio implementarlo para decirle cómo de grande es la caja.

	bool determinar_caja_dibujo_final(SDL_Rect &, SDL_Rect const&, SDL_Rect const&);
	
	public:

	Primitiva_grafica(Uint32=0);
	Primitiva_grafica(const Primitiva_grafica&);
	Primitiva_grafica& operator=(const Primitiva_grafica&);
	virtual ~Primitiva_grafica();

	void establecer_parametro_metodo_pixeles(void * p_param) {this->parametro_metodo_pixeles=p_param;}
	Uint32 acc_pixel_color() const {return this->pixel_color;}
//	bool es_combinar_color() const {return this->combinar_color;}
	bool es_recomponer_recorte_con_posicion() const {return recomponer_recorte_con_posicion;}
	void * acc_parametro_metodo_pixeles() const {return this->parametro_metodo_pixeles;}

	void mut_recomponer_recorte_con_posicion(bool p_valor) {this->recomponer_recorte_con_posicion=p_valor;}	
	void mut_pixel_color(Uint32 param) {this->pixel_color=param;}
//	void mut_combinar_color(bool param) {this->combinar_color=param;}
	void recorte_a_posicion();


};

} //Fin namespace DLibV

#endif
