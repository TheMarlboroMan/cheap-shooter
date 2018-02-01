#include "caja_texto.h"

Caja_texto::Caja_texto(const std::string& p_cadena, const std::string& p_cadena_fin, 
	const DLibV::Recurso_grafico * p_recurso_fuente, const DLibV::Recurso_grafico * p_recurso_borde, 
	const SDL_Rect p_patron, 
	Uint32 p_fondo, Uint32 p_color_borde, 
	unsigned int p_ancho, unsigned int p_alto, unsigned int p_borde, unsigned int p_margen)

		:cadena(p_cadena), cadena_fin(p_cadena_fin), 
	color_fondo(p_fondo), color_borde(p_color_borde), ancho(p_ancho), alto(p_alto), 
	borde(p_borde), margen(p_margen),
	rep_texto(p_recurso_fuente, cadena, ancho, alto+margen),
	rep_texto_fin(p_recurso_fuente, cadena_fin, ancho, alto),	//En realidad las dimensiones aquí nos dan un poco igual.
	rep_marco(ancho+(2*(borde+margen)), alto+(2*(margen+borde))),

	rep(false)
{
	DLibV::Primitiva_grafica_caja_estatica rep_fondo(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(borde, borde, ancho+(2*margen), alto+(2*margen)), color_fondo);
	rep_fondo.establecer_alpha(64);

	DLibV::Primitiva_grafica_caja_estatica rep_borde_sup(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, ancho+(2*(borde+margen)), borde), color_borde);

	DLibV::Primitiva_grafica_caja_estatica rep_borde_der(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(ancho+(2*margen)+borde, borde, borde, alto+(2*margen)), color_borde);

	DLibV::Primitiva_grafica_caja_estatica rep_borde_inf(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0, alto+(2*margen)+borde, ancho+(2*(borde+margen)), borde), color_borde);

	DLibV::Primitiva_grafica_caja_estatica rep_borde_izq(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0, borde, borde, alto+(2*margen)), color_borde);

/*	DLibV::Representacion_patron_estatica rep_borde_sup(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, ancho+(2*(borde+margen)), borde), 
		p_patron, p_fondo, p_recurso_borde);

	DLibV::Representacion_patron_estatica rep_borde_der(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(ancho+(2*margen)+borde, borde, borde, alto+(2*margen)),
		p_patron, p_fondo, p_recurso_borde);

	DLibV::Representacion_patron_estatica rep_borde_inf(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0, alto+(2*margen)+borde, ancho+(2*(borde+margen)), borde), 
		p_patron, p_fondo, p_recurso_borde);

	DLibV::Representacion_patron_estatica rep_borde_izq(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0, borde, borde, alto+(2*margen)), 
		p_patron, p_fondo, p_recurso_borde);
*/

	rep_marco.volcar_representacion(rep_borde_sup, DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, ancho+(2*(borde+margen)), borde));
	rep_marco.volcar_representacion(rep_borde_der, DLibH::Herramientas_SDL::nuevo_sdl_rect(ancho+(2*margen)+borde, borde, borde, alto+(2*margen)));
	rep_marco.volcar_representacion(rep_borde_inf, DLibH::Herramientas_SDL::nuevo_sdl_rect(0, alto+(2*margen)+borde, ancho+(2*(borde+margen)), borde));
	rep_marco.volcar_representacion(rep_borde_izq, DLibH::Herramientas_SDL::nuevo_sdl_rect(0, borde, borde, alto+(2*margen)));
	rep_marco.volcar_representacion(rep_fondo, DLibH::Herramientas_SDL::nuevo_sdl_rect(borde, borde, ancho+(2*margen), alto+(2*margen)));

	rep_texto_fin.hacer_invisible();
	rep_texto.establecer_posicion(borde+margen, borde+(margen/2)); //El margen por encima es más pequeño.
	rep_texto_fin.establecer_posicion(borde+margen, alto+margen+borde);

	rep.no_poseer_representaciones();
	rep.insertar_representacion(&rep_marco);
	rep.insertar_representacion(&rep_texto);
	rep.insertar_representacion(&rep_texto_fin);
}

Caja_texto::Caja_texto(const Caja_texto& p_otra):
	cadena(p_otra.cadena), cadena_fin(p_otra.cadena_fin), color_fondo(p_otra.color_fondo), ancho(p_otra.ancho), 
	alto(p_otra.alto), borde(p_otra.borde), margen(p_otra.margen),
	rep_texto(p_otra.rep_texto),
	rep_texto_fin(p_otra.rep_texto_fin),
	rep_marco(p_otra.rep_marco),
	rep(false)
{
	rep_texto_fin.hacer_invisible();

	rep.no_poseer_representaciones();
	rep.insertar_representacion(&rep_marco);
	rep.insertar_representacion(&rep_texto);
	rep.insertar_representacion(&rep_texto_fin);
}

Caja_texto& Caja_texto::operator=(const Caja_texto& p_otra)
{
	cadena=p_otra.cadena;
	cadena_fin=p_otra.cadena_fin;
	color_fondo=p_otra.color_fondo;
	ancho=p_otra.ancho;
	alto=p_otra.alto;
	borde=p_otra.borde;
	margen=p_otra.margen;
	rep_texto=p_otra.rep_texto;
	rep_texto_fin=p_otra.rep_texto_fin;
	rep_marco=p_otra.rep_marco;

	rep.no_poseer_representaciones();
	rep.insertar_representacion(&rep_marco);
	rep.insertar_representacion(&rep_texto);
	rep.insertar_representacion(&rep_texto_fin);

	return *this;
}
