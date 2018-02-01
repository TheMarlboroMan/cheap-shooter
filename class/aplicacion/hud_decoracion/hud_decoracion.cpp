#include "hud_decoracion.h"
 
const Uint32 Hud_decoracion::COLOR=16777215; //Gestor_color::color(255,255,255);
	
Hud_decoracion::Hud_decoracion()
	:rep(false),
	linea_combo(X1_LINEA_COMBO, Y_LINEA_COMBO, X2_LINEA_COMBO, Y_LINEA_COMBO, COLOR),
	linea_puntos(X_LINEA_PUNTOS, Y_LINEA_PUNTOS, X_LINEA_PUNTOS, Y_LINEA_PUNTOS, COLOR),
	caja_puntos(DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, ANCHO_CAJA_PUNTOS, ALTO_CAJA_PUNTOS), COLOR),
	caja_fondo(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA_FONDO, Y_CAJA_FONDO, W_CAJA_FONDO, H_CAJA_FONDO), COLOR),
	cantidad_digitos(0)
{
	establecer_cantidad_digitos(1);

	caja_fondo.establecer_alpha(64);

	rep.establecer_posicion(0,0);
	rep.insertar_representacion(&linea_combo);
	rep.insertar_representacion(&linea_puntos);
	rep.insertar_representacion(&caja_puntos);
	rep.insertar_representacion(&caja_fondo);
}

Hud_decoracion::~Hud_decoracion()
{

}

void Hud_decoracion::establecer_cantidad_digitos(unsigned int c)
{
	if(c!=cantidad_digitos)
	{
		cantidad_digitos=c;
		int x2=ANCHO_INICIAL_LINEA_PUNTOS + (cantidad_digitos * ANCHO_DIGITOS);
		linea_puntos.mut_x2(x2);
		caja_puntos.establecer_posicion(x2, Y_LINEA_PUNTOS-ALTO_CAJA_PUNTOS+1);
		caja_fondo.establecer_posicion(X_CAJA_FONDO, Y_CAJA_FONDO, x2+ANCHO_CAJA_PUNTOS+2, H_CAJA_FONDO);
	}
}
