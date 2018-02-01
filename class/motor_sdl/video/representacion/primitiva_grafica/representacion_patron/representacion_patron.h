#ifndef REPRESENTACION_PATRON_H
#define REPRESENTACION_PATRON_H

/*Esto debe ser como un lienzo, pero que se vaya repitiendo y al que se le
puedan cambiar dinámicamente el tamaño. Tendrá dos entradas: el recurso en 
si, que es un lienzo, y el "patron", que será una superficie que se irá
repitiendo en el rectángulo marcado como tal.
Cada cambio en el tamaño o pincel se marcará como "debe prepararse", de forma
que "preparar" vuelva a dibujarlo todo.
*/

#include "../primitiva_grafica.h"
#include "../../../recurso_grafico/recurso_grafico.h"
#include "../../../../herramientas/herramientas_sdl/herramientas_sdl.h"


namespace DLibV
{
class Representacion_patron:public Primitiva_grafica
{
	private:

	SDL_Rect caja_patron;
	Recurso_grafico * patron;
	int alineamiento_x;
	int alineamiento_y;

	protected:

	void preparar_posicion();

	public:
	
	void preparar();
	bool es_estatica() const=0;
		
	bool volcado(SDL_Surface *);
	bool volcado(SDL_Surface *, const SDL_Rect&, const SDL_Rect&, int=0, int=0);

	Representacion_patron(const SDL_Rect&, const SDL_Rect&, Uint32, const Recurso_grafico*);
	Representacion_patron(const Representacion_patron&);
	virtual ~Representacion_patron();
	Representacion_patron& operator=(const Representacion_patron&);

	void actualizar_patron(const Recurso_grafico*);
	void actualizar_caja_patron(const SDL_Rect &);
	void actualizar_alineamiento(int, int);
	SDL_Rect acc_caja_patron() const {return this->caja_patron;}
	int acc_alineamiento_x() const {return this->alineamiento_x;}
	int acc_alineamiento_y() const {return this->alineamiento_y;}
};

class Representacion_patron_dinamica:public Representacion_patron
{
	public:

	bool es_estatica() const {return false;}

	Representacion_patron_dinamica(const SDL_Rect& p_pos, const SDL_Rect& p_caja, Uint32 p_color, const Recurso_grafico* p_rec):Representacion_patron(p_pos, p_caja, p_color, p_rec){}
	Representacion_patron_dinamica(const Representacion_patron_dinamica& p):Representacion_patron(p){}
	virtual ~Representacion_patron_dinamica() {}
	Representacion_patron_dinamica& operator=(const Representacion_patron_dinamica& p_otro)
	{
		Representacion_patron::operator=(p_otro);
		return *this;
	}
};

class Representacion_patron_estatica:public Representacion_patron
{
	public:

	bool es_estatica() const {return true;}

	Representacion_patron_estatica(const SDL_Rect& p_pos, const SDL_Rect& p_caja, Uint32 p_color, const Recurso_grafico* p_rec):Representacion_patron(p_pos, p_caja, p_color, p_rec){}
	Representacion_patron_estatica(const Representacion_patron_estatica& p):Representacion_patron(p){}
	virtual ~Representacion_patron_estatica() {}
	Representacion_patron_estatica& operator=(const Representacion_patron_estatica& p_otro)
	{
		Representacion_patron::operator=(p_otro);
		return *this;
	}
};

}

#endif
