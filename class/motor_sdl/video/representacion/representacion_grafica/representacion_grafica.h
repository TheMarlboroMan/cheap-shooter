#ifndef REPRESENTACION_GRAFICA_H
#define REPRESENTACION_GRAFICA_H

#include "../representacion.h"
#include "../../recurso_grafico/recurso_grafico.h"
#include "../../gestor_color/gestor_color.h"

/*
Es la clase base de todas las representaciones que usan un SDL_Surface 
(encapsulado en un recurso) para ser representado. La superficie puede tener
cargada una imagen o estar preparada para su manipulación directa por parte
del código.

Provee de los métodos para manipular la clave de color del recurso
*/

namespace DLibV
{

class Representacion_grafica:public Representacion
{
	private:

	Recurso_grafico * recurso;	//Este es el puntero a su superficie de memoria.
	mutable bool preparada;	//Indica si la pantalla puede volcar o tiene que hacer una preparación propia.

	protected:

	void marcar_como_preparada() {this->preparada=true;}
	void marcar_como_no_preparada() {this->preparada=false;}
	void recorte_a_medidas_recurso();
	void liberar_recurso();
	void anular_recurso() {recurso=NULL;}
//	void copiar_recurso(const Representacion_grafica& o) {*this->recurso=*o.recurso;}

	virtual bool volcado(SDL_Surface *);
	virtual bool volcado(SDL_Surface *, const SDL_Rect&, const SDL_Rect&, int=0, int=0);

	public:

	bool es_preparada() const {return this->preparada;}

	Recurso_grafico * ref_recurso() const {return this->recurso ? this->recurso : NULL;}
	SDL_Surface * ref_superficie() const {return this->recurso ? this->recurso->acc_superficie() : NULL;}
	Representacion_grafica();
	Representacion_grafica(const Representacion_grafica&);
	Representacion_grafica& operator=(const Representacion_grafica &);
	virtual ~Representacion_grafica();


	virtual void establecer_recurso(Recurso_grafico const * p_recurso) {this->recurso=const_cast <Recurso_grafico *> (p_recurso);}
	virtual void preparar()=0;
	void rellenar(Uint8, Uint8, Uint8);
	void rellenar(Uint32);
};

} //Fin namespace DLibV

#endif
