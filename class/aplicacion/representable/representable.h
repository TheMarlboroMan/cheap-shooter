#ifndef REPRESENTABLE_H
#define REPRESENTABLE_H

#include "../../motor_sdl/libDanSDL.h"
#include "../recursos/recursos.h"

/*Esto queda separado formalmente del Actor: indica que algo se puede
representar y contiene todas las cosas que se necesitan para ese respecto.
Lo implementaremos mediante herencia múltiple donde haya que ponerlo.

*/

class Representable
{
	private:

	short int desp_rep_x;	//Cantidad en X que se desplaza la representación con respecto al origen.
	short int desp_rep_y;	//Esto es para que la caja pueda ser un poco más pequeña que la representación, por ejemplo.
	unsigned int indice_recurso;	//Indice del recurso gráfico usado.
	unsigned int indice_patron;
	bool representable;
//	SDL_Rect recorte;		//Este es el recorte que se coge...
	static const unsigned int RECURSO_POR_DEFECTO=Recursos::G_TILE_JUGADOR; //Simplemente porque de el cante si no se encuentra el recurso.
	DLibV::Representacion * representacion;	

	protected:

	virtual void establecer_representacion()=0;

	void establecer_desp_rep_x(short int param) {desp_rep_x=param;}
	void establecer_desp_rep_y(short int param) {desp_rep_y=param;}

	void establecer_recorte(const SDL_Rect&);
	void establecer_recorte(int, int, int, int);
	void establecer_posicion(const SDL_Rect&);
	void establecer_posicion(int, int, int, int);
	void establecer_posicion(int, int);

	void establecer_representacion(DLibV::Representacion *);
	void establecer_representacion_bitmap_dinamica();
	void establecer_representacion_bitmap_dinamica(DLibV::Recurso_grafico *);
	void establecer_representacion_patron_dinamica(const SDL_Rect&, const SDL_Rect&, Uint32, unsigned int);
	void establecer_representacion_agrupada_dinamica(bool);
	void insertar_en_representacion_agrupada(DLibV::Representacion *);
	
	public:

	void establecer_caja_patron(const SDL_Rect&);
	void establecer_caja_patron(int, int, int, int);
	void establecer_alineamiento(int, int);
	unsigned int acc_indice_recurso() const {return indice_recurso;}
	void mut_indice_recurso(unsigned int p_valor) {this->indice_recurso=p_valor;}
	unsigned int acc_indice_patron() const {return this->indice_patron;}
	void mut_indice_patron(unsigned int p_valor);
	void establecer_alpha(Uint8 a) {this->representacion->establecer_alpha(a);}
 
	virtual bool es_representable() const {return this->representable;}
	//Estas sólo afectarían a las que no redefinen "es_representable...".
	void hacer_representable() {this->representable=true;}
	void hacer_no_representable() {this->representable=false;}
	void intercambiar_representable() {this->representable=!this->representable;}
	
	bool con_representacion() const {return this->representacion ? true : false;}
	DLibV::Representacion * acc_representacion() {return this->representacion;}
	SDL_Rect acc_caja_patron() const;
	int acc_alineamiento_x() const;
	int acc_alineamiento_y() const;

	Representable();
	virtual ~Representable();
};

#endif
