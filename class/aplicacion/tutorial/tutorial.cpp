#include "tutorial.h"

Tutorial::Tutorial(const std::string& cadena, const std::string& cadena_fin, float p_tiempo, float p_opacidad, bool p_mostrable)
	:opacidad(p_opacidad),
	tiempo_restante(p_tiempo / 1000),
	mostrable(p_mostrable),
	estado(E_MOSTRANDO),
	caja_texto(
		cadena, cadena_fin,
		DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1),
		DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUEGO),
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,16,16),
		DLibV::Gestor_color::color(0xff, 0xff, 0xff),
		DLibV::Gestor_color::color(0xff, 0xff, 0xff),
		400, 300, 16, 32)
{
	//Esto más o menos la centra en la pantalla.
	caja_texto.establecer_posicion(DLibH::Herramientas_SDL::nuevo_sdl_rect(160,90,0,0));
}

Tutorial::Tutorial(const Tutorial& p_otro)
	:opacidad(p_otro.opacidad),
	tiempo_restante(p_otro.tiempo_restante),
	mostrable(p_otro.mostrable),
	estado(E_MOSTRANDO),
	caja_texto(p_otro.caja_texto)
{

}

Tutorial::~Tutorial()
{

}

Tutorial& Tutorial::operator=(const Tutorial& p_otro)
{
	opacidad=p_otro.opacidad;
	tiempo_restante=p_otro.tiempo_restante;
	caja_texto=p_otro.caja_texto;
	estado=p_otro.estado;
	mostrable=p_otro.mostrable;

	return *this;
}

void Tutorial::procesar_turno(float p_delta)
{
	switch(estado)
	{
		case E_MOSTRANDO:

			if(tiempo_restante > 0.0f)
			{
				tiempo_restante-=p_delta;
			}

			if(tiempo_restante <= 0.0f )
			{
				caja_texto.habilitar_mensaje_fin();
				estado=E_ESPERANDO;
			}

		break;

		case E_DESVANECIENDO:
			opacidad-=p_delta*200.0f;	//200 sería la cantidad de opacidad que pierde en un segundo.
//			caja_texto.establecer_alpha((Uint8)opacidad);	
			caja_texto.establecer_alpha(ceil(opacidad));	

			if(opacidad <= 0.0f)
			{
				opacidad=0.0f;
				estado=E_FINALIZADO;
				caja_texto.establecer_alpha(SDL_ALPHA_OPAQUE);	//Esto está así para que la próxima vez que se muestre vuelva a estar a full.
				mostrable=false;
			}
		break;	
	}
}

void Tutorial::desvanecer()
{
	this->estado=E_DESVANECIENDO;

}
