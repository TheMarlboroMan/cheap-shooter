#ifndef HUD_DECORACION_H
#define HUD_DECORACION_H

/*No confundir con una decoración, básicamente son un par de representaciones
que decoran el hud con rayas y cuadrados.*/

#include "../../motor_sdl/libDanSDL.h"

class Hud_decoracion
{
	private:

	static const Uint32 COLOR;

	static const int X1_LINEA_COMBO=730;
	static const int X2_LINEA_COMBO=800;
	static const int Y_LINEA_COMBO=52;

	static const int X_LINEA_PUNTOS=0;
	static const int Y_LINEA_PUNTOS=580;
	static const int ANCHO_CAJA_PUNTOS=4;
	static const int ALTO_CAJA_PUNTOS=22;
	static const int ANCHO_DIGITOS=12;
	static const int ANCHO_INICIAL_LINEA_PUNTOS=106;

	static const int X_CAJA_FONDO=0;
	static const int Y_CAJA_FONDO=556;
	static const int W_CAJA_FONDO=ANCHO_CAJA_PUNTOS+ANCHO_INICIAL_LINEA_PUNTOS+ANCHO_DIGITOS+2;
	static const int H_CAJA_FONDO=ALTO_CAJA_PUNTOS+6;

	DLibV::Representacion_agrupada_estatica rep;

	DLibV::Primitiva_grafica_linea_estatica linea_combo;
	DLibV::Primitiva_grafica_linea_estatica linea_puntos;
	DLibV::Primitiva_grafica_caja_estatica caja_puntos;
	DLibV::Primitiva_grafica_caja_estatica caja_fondo;

	unsigned int cantidad_digitos;

	public:

	Hud_decoracion();
	~Hud_decoracion();

	DLibV::Representacion_agrupada_estatica& acc_representacion() {return rep;}
	void establecer_cantidad_digitos(unsigned int);
};

#endif
