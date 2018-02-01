#ifndef CAMARA_H
#define CAMARA_H

#include <SDL/SDL.h>
#include "../../herramientas/herramientas_sdl/herramientas_sdl.h"
//#include "../representacion/representacion_grafica/representacion_grafica.h"
//#include "../representacion/primitiva_grafica/primitiva_grafica.h"

namespace DLibV
{

class Camara
{
	private:

	//Propiedades...

	SDL_Rect caja;			//La caja a la que se apunta en el "mundo".
	SDL_Rect caja_ficticia;		//La caja desplazada con desp_x y desp_y, guardada aquí para no crearla again and again.
	unsigned int pos_x;		//La posición X e Y en la que aparece la cámara en pantalla.
	unsigned int pos_y;
	int desp_x;			//Desplazamiento adicional que da la cámara a todo lo que
	int desp_y;			//hay dentro.
	bool con_clip;			//Indica si no puede dibujar nada fuera de pos_x, pos_y, caja.w y caja.h.

	bool limitada;
	Sint16 limite_min_x;		//Límites para el movimiento de la cámara.
	Sint16 limite_min_y;
	Sint16 limite_max_x;
	Sint16 limite_max_y;

	const SDL_Rect * siguiendo_a;		//Una caja a la que la cámara sigue.
	int seguimiento_offset_x;
	int seguimiento_offset_y;

	void sincronizar_cajas();
	
	public:

	//Interfaz pública.

	Camara(Sint16=0, Sint16=0, Uint16=800, Uint16=600, unsigned int=0, unsigned int=0);
	~Camara();

	SDL_Rect acc_caja() const {return this->caja;}
	unsigned int acc_pos_x() const {return this->pos_x;}
	unsigned int acc_pos_y() const {return this->pos_y;}
	Sint16 acc_x() const {return this->caja.x;}
	Sint16 acc_y() const {return this->caja.y;}
	int acc_desp_x() const {return this->desp_x;}
	int acc_desp_y() const {return this->desp_y;}
	Uint16 acc_w() const {return this->caja.w;}
	Uint16 acc_h() const {return this->caja.h;}
	bool es_con_clip() const {return this->con_clip;}

	void mut_pos_x(unsigned int p_valor) {this->pos_x=p_valor;}
	void mut_pos_y(unsigned int p_valor) {this->pos_y=p_valor;}
	void mut_x(Sint16 p_valor) {this->caja.x=p_valor;}
	void mut_y(Sint16 p_valor) {this->caja.y=p_valor;}
	void mut_desp_x(int p_valor) {this->desp_x=p_valor;}
	void mut_desp_y(int p_valor) {this->desp_y=p_valor;}
	void mut_w(Uint16 p_valor) {this->caja.w=p_valor;}
	void mut_h(Uint16 p_valor) {this->caja.h=p_valor;}
	void mut_con_clip(bool p_valor) {this->con_clip=p_valor;}

	SDL_Rect acc_caja_pos() const;	

	void enfocar_a(Sint16, Sint16);
	void movimiento_relativo(Sint16=0, Sint16=0);
	void establecer_limites(Sint16, Sint16, Sint16, Sint16);
	void limpiar_limite();
//	void establecer_seguimiento(Representacion_grafica * const, int=0, int p_offset_y=0);
//	void establecer_seguimiento(Representacion_grafica const&, int=0, int p_offset_y=0);
	void establecer_seguimiento(const SDL_Rect&, int=0, int p_offset_y=0);
	void limpiar_seguimiento() {this->siguiendo_a=NULL;}
	void calcular_seguimiento();
	bool caja_en_toma (const SDL_Rect&) const;
//	bool representacion_en_toma (Primitiva_grafica const&) const;
};

} //Fin namespace DLibV

#endif
