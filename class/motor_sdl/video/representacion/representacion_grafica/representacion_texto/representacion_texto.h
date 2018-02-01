#ifndef REPRESENTACION_TEXTO_H
#define REPRESENTACION_TEXTO_H

#include "../representacion_manual.h"
#include "../../../recurso_grafico/lienzo.h"
#include "../../../../herramientas/herramientas/herramientas.h"
#include <SDL/SDL.h>
#include <string>
#include <cmath>

/*
El constructor está "incompleto", por así decirlo. Debe llamar a "asignar"
e "iniciar_superficie_texto_renderizada" pero "asignar" es un método que puede
variar en clases derivadas e "iniciar_superficie_texto_renderizada" depende
en gran medida de que se haya "iniciado"

Deriva de la representación manual porque hay una versión que te da las 
dimensiones en el constructor, pero hay otra que no. En resumen, mejor no 
tocar el tinglao.
*/

namespace DLibV
{


class Representacion_texto:public Representacion_manual
{
	protected:

	enum T_GLIFOS{T_GLIFOS_FILA=16};	//Hay 16 glifos for fila.

	unsigned short int ancho_glifo;
	unsigned short int alto_glifo;
	int interlineado;	//Es un entero con signo, por si queremos hacer efectos raros.
	int interletra;		//Es un entero con signo, por si queremos hacer efectos raros.
	std::string cadena;
	
	unsigned int ancho_area;
	unsigned int alto_area;
	
	Uint32 color_fondo;

	SDL_Surface * superficie_alfabeto;

	private:

	bool establecer_recorte_para_glifo(unsigned char, SDL_Rect &);
	void establecer_posicion_para_glifo(unsigned char, SDL_Rect &, unsigned int);
	void establecer_recurso_fuente(Recurso_grafico const *);

	protected:

	virtual void destruir_recurso();

	public:

	virtual void iniciar_recurso();

	std::string acc_cadena() const {return this->cadena;}
	void establecer_dimensiones_area(unsigned int, unsigned int);
	void preparar();
	void mut_interlineado(int p_valor) 
	{
		this->interlineado=p_valor;
		asignar(cadena);
	}
	void mut_interletra(int p_valor) 
	{
		this->interletra=p_valor;
		asignar(cadena);
	}
	unsigned short int acc_alto_glifo() const {return this->alto_glifo;}
	unsigned short int acc_ancho_glifo() const {return this->ancho_glifo;}
	unsigned int acc_ancho_area() const {return this->ancho_area;}
	unsigned int acc_alto_area() const {return this->alto_area;}
	void modificar_fuente(Recurso_grafico const * p_rep);
	void mut_color_fondo(Uint32 c) {this->color_fondo=c;}

	////////////////////////////////////////////////////////////////////////

	virtual void asignar(const char);
	virtual void asignar(const char *);
	virtual void asignar(const std::string& p_param);
	virtual bool es_estatica() const=0;

	////////////////////////////////////////////////////////////////////////
	//Constructor y destructor.

	Representacion_texto(const Recurso_grafico *);
	Representacion_texto(const Representacion_texto&);
	virtual ~Representacion_texto();
	Representacion_texto& operator=(const Representacion_texto&);
};

} //Fin namespace DLibV

#endif
