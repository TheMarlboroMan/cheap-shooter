#ifndef CAJA_TEXTO_H
#define CAJA_TEXTO_H

#include <string>
#include "../../motor_sdl/libDanSDL.h"

/*
La caja de texto es una representación compuesta para mostrar textos en 
pantalla... Se establece un texto, una posición X e Y y un ancho y alto deseado.
A ese ancho se le sumará un borde y un márgen interno. El borde
tendrá un fondo hecho con varias representaciones de patrón  mientras que el 
fondo del texto tendrá un color. No vamos a hacer muchas historias de evitar 
overdraw: el borde será más grande y luego debajo irán el fondo y el texto.
*/

class Caja_texto
{
	private:

	std::string cadena;
	std::string cadena_fin;
	Uint32 color_fondo;
	Uint32 color_borde;
	unsigned int ancho;
	unsigned int alto;
	unsigned int borde;
	unsigned int margen;

	DLibV::Representacion_texto_fija_estatica rep_texto;
	DLibV::Representacion_texto_fija_estatica rep_texto_fin;
	DLibV::Representacion_compuesta_estatica rep_marco;
	DLibV::Representacion_agrupada_estatica rep;

	public:

	Caja_texto(const std::string& p_cadena, const std::string& p_cadena_fin, const DLibV::Recurso_grafico * p_recurso_fuente, const DLibV::Recurso_grafico * p_recurso_borde, const SDL_Rect p_patron, Uint32 p_fondo, Uint32 p_color_borde, unsigned int p_ancho, unsigned int p_alto, unsigned int p_borde, unsigned int p_margen);
	Caja_texto(const Caja_texto&);
	Caja_texto& operator=(const Caja_texto&);
	~Caja_texto(){}

	void establecer_cadena(const std::string& p_cadena)
	{
		cadena=p_cadena;
		rep_texto.asignar(cadena);
	}

	void establecer_cadena_fin(const std::string& p_cadena)
	{
		cadena_fin=p_cadena;
		rep_texto_fin.asignar(cadena);
	}

	void establecer_posicion(const SDL_Rect& p_caja) {rep.establecer_posicion(p_caja);}
	void habilitar_mensaje_fin() {rep_texto_fin.hacer_visible();}
	void establecer_alpha(Uint8 p_valor){rep.establecer_alpha(p_valor);}
	DLibV::Representacion * acc_representacion() {return &rep;}

	void test_intercambiar_visibilidad(unsigned int t)
	{
		switch(t)
		{
			case 0: rep_texto.intercambiar_visibilidad(); break;
			case 1: rep_texto_fin.intercambiar_visibilidad(); break;
			case 2: rep_marco.intercambiar_visibilidad(); break;
		}

	}
};

#endif
