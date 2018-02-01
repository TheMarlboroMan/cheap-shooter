#ifndef REPRESENTACION_H
#define REPRESENTACION_H

#include <SDL/SDL.h>
#include "../camara/camara.h"
#include "../pantalla/pantalla.h"
#include "../../herramientas/herramientas_sdl/herramientas_sdl.h"

namespace DLibV
{

class Representacion
{
	private:

	Uint8 alpha;

	SDL_Rect posicion; 	//Lugar en que se muestra de la pantalla.
	SDL_Rect recorte;	//Considerando la dimensión total de la representación, la parte que mostramos.
	SDL_Rect clip_volcado;	//El clip de volcado se aplica a la superficie a la que se haga el método "volcado". Típicamente la pantalla.
	bool con_clip_volcado;

	protected:

	void reiniciar_posicion();
	void reiniciar_recorte();
	bool visible;

	Representacion();
	Representacion(Uint8);
	Representacion(const Representacion&);
	Representacion& operator=(const Representacion &);


/*
Donde p_vista es camara.caja y p_posicion es camara.posicion.
p_vista es el rectángulo hipotético al que se está enfocando.
p_posicion es la caja que actualiza la cámara, es decir, dónde se ve en pantalla.
Desp_x es el desplazamiento extra x de la cámara.
Desp_y es el desplazamiento extra y de la cámara.
*/
	virtual bool volcado(SDL_Surface *, const SDL_Rect&, const SDL_Rect&, int=0, int=0)=0;

	//Manipulación normal y corriente. No se harían comprobaciones de si cabe o no cabe.
	virtual bool volcado(SDL_Surface *)=0;
	
	public:
	
	virtual ~Representacion();

	enum FLAGS_RECT{FRECT_X=1, FRECT_Y=2, FRECT_W=4, FRECT_H=8};

	virtual bool es_estatica() const=0; 	//Indica si sería afectada por la cámara. Es la única diferencia entre algunas clases.
					//Hay que entenderlo en el contexto de que puede dibujarse "dentro" de una cámara
					//pero por más que esta mueva su enfoque se mantendrá siempre en el mismo lugar.

	bool en_toma(const SDL_Rect& p_caja) const {return DLibH::Herramientas_SDL::rectangulos_superpuestos(p_caja, this->posicion, true);} 
	bool es_en_posicion(Sint16 p_x, Sint16 p_y) const 
	{
		return this->posicion.x==p_x && 
		this->posicion.y==p_y;
	}

	const SDL_Rect& acc_posicion() const {return this->posicion;}
	void establecer_posicion(int, int, int=-1, int=-1, int=15);
	void establecer_posicion(const SDL_Rect &);	

	const SDL_Rect& acc_recorte() const {return this->recorte;}
	void establecer_recorte(Sint16, Sint16, Uint16, Uint16, int=15);
	void establecer_recorte(const SDL_Rect&);

	void establecer_dimensiones_posicion_por_recorte();

	virtual void ir_a(int x, int y){this->establecer_posicion(x,y);} //Es virtual porque algunas igual redefinen el comportamiento (especialmente las primitivas....
	void desplazar(Sint16 p_x, Sint16 p_y);					
	void hacer_invisible() {this->visible=false;}
	void hacer_visible() {this->visible=true;}
	void intercambiar_visibilidad() {this->visible=!this->visible;}
	void cambiar_visibilidad(bool p_valor) {this->visible=p_valor;}
	bool es_visible() const {return this->visible;}

	//Se pasa el rectángulo de pantalla... Básicamente se comprueba si está dentro. Estática o no.
	bool volcar(SDL_Surface *, const SDL_Rect&);
	bool volcar(SDL_Surface *, const Camara&);
	bool volcar(const Pantalla&, const Camara&);
	bool volcar(const Pantalla&);
	bool volcar(SDL_Surface *, const SDL_Rect&, const SDL_Rect&, int=0, int=0);
	bool volcar(SDL_Surface *);

	void establecer_alpha(Uint8 p_valor) {this->alpha=p_valor;}
	Uint8 acc_alpha() const {return this->alpha;}
};

} //Fin namespace DLibV

#endif
