#ifndef PANTALLA_H
#define PANTALLA_H

#include <SDL/SDL.h>
#include <iostream>
#include <vector>
#include "../utilidades_graficas_sdl/utilidades_graficas_sdl.h"
#include "../camara/camara.h"
#include "../gestor_color/gestor_color.h"
//#include "../representacion/representacion.h"
#include "../proceso_superficie/proceso_superficie.h"

namespace DLibV
{


class Pantalla
{
	private:

	//Propiedades...	

	typedef std::vector<SDL_Rect > vector_sdl_rect;
	typedef std::vector<Proceso_superficie *> t_vector_procesos;

	SDL_Surface * superficie;
	SDL_Rect simulacro_caja; //El simulacro de caja es un rectángulo con las medidas de la pantalla, para controlar lo que se muestra o no.
	unsigned int volcados;	//Volcados desde el último flipe.

	int w;
	int h;
	int bpp;
	int flags;
	vector_sdl_rect cajas_para_actualizar;
	t_vector_procesos vector_procesos; 
	bool comprobar_en_toma;

	//Métodos privados.

	void optimizar_cajas_para_actualizar();
	void cortar_caja_a_pantalla(SDL_Rect *);
	void establecer_modo_video();
	
	public:

	//Interfaz pública.

	Pantalla(int=800, int=600, int=8, int=SDL_SWSURFACE);
	~Pantalla();

	vector_sdl_rect const& acc_cajas_para_actualizar() {return this->cajas_para_actualizar;}
	SDL_Surface * acc_superficie() const {return this->superficie;}
	unsigned int acc_volcados() const {return this->volcados;}
	void establecer_titulo(const char * p_cadena);

	void inicializar();
	void inicializar(int=800, int=600, int=8, int=SDL_SWSURFACE /*| SDL_ANYFORMAT*/);
	void rellenar(Uint8, Uint8, Uint8);
	void rellenar(Uint32);
	void rellenar(Uint8, Uint8, Uint8, SDL_Rect const&);
	void rellenar(Uint8, Uint8, Uint8, Uint16, Uint16, Sint16, Sint16);
	void rellenar(Uint32, SDL_Rect const&);
	void rellenar(Uint8, Uint8, Uint8, const Camara &);
	void rellenar(Uint32, const Camara &);
	void rellenar(Uint8, Uint8, Uint8, SDL_Rect const&, const Camara &);
	void rellenar(Uint8, Uint8, Uint8, Uint16, Uint16, Sint16, Sint16, const Camara &);
	void rellenar(Uint32, SDL_Rect const&, const Camara &);
	void flipar();
	void insertar_caja_para_actualizar(SDL_Rect const&);
	void limpiar_cajas_para_actualizar(){this->cajas_para_actualizar.clear();}
	SDL_PixelFormat * obtener_formato_pixeles() {return this->superficie->format;}
	void eliminar_caja_para_actualizar(SDL_Rect const&);
	bool comprobar(){return this->superficie!=NULL;}
	void preparar_para_camara(Camara const&);
	void establecer_clip_para_camara(Camara const&);
	void establecer_clip(SDL_Rect);
	void reiniciar_clip_completo();
	void adjuntar_proceso(Proceso_superficie *);
	void procesar_procesos(float);
	void eliminar_procesos();
	bool hay_procesos_en_marcha() {return this->vector_procesos.size()!=0;}
	const SDL_Rect acc_simulacro_caja() {return this->simulacro_caja;}
};

} //Fin namespace DLibV

#endif
