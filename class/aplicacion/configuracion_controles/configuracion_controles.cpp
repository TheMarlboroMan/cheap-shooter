#include "configuracion_controles.h"

std::string const Configuracion_controles::RUTA_ARCHIVO="data/controles_";
std::string const Configuracion_controles::EXTENSION_ARCHIVO=".dat";

Configuracion_controles::Configuracion_controles(const std::string& p_ruta)
		:nodo_pausa(), nodo_usar_artefacto(), nodo_parar_tiempo(),
		nodo_ciclar_artefacto_siguiente(), nodo_ciclar_artefacto_anterior(),
		nodo_movimiento_arriba(), nodo_movimiento_derecha(),
		nodo_movimiento_abajo(), nodo_movimiento_izquierda(),
		extra_ruta(p_ruta)
{

}

Configuracion_controles::~Configuracion_controles()
{

}

bool Configuracion_controles::comprobar_nodo(DLibI::Controles_SDL& p_controles_sdl, const Nodo_control& p_nodo, unsigned int p_tipo) const
{
	return p_nodo.comprobar(p_controles_sdl, p_tipo);
} 

/*Genera los controles por defecto para el modo de control de
teclado...*/

void Configuracion_controles::generar_por_defecto_teclado()
{
	this->nodo_pausa.configurar(T_TECLADO, SDLK_p);
	this->nodo_usar_artefacto.configurar(T_BOTON_RATON, SDL_BUTTON_LEFT);
	this->nodo_parar_tiempo.configurar(T_BOTON_RATON, SDL_BUTTON_RIGHT);
	this->nodo_ciclar_artefacto_siguiente.configurar(T_TECLADO, SDLK_RCTRL);
	this->nodo_ciclar_artefacto_anterior.configurar(T_TECLADO, SDLK_z);
	this->nodo_movimiento_arriba.configurar(T_TECLADO, SDLK_UP);
	this->nodo_movimiento_derecha.configurar(T_TECLADO, SDLK_RIGHT); 
	this->nodo_movimiento_abajo.configurar(T_TECLADO, SDLK_DOWN);
	this->nodo_movimiento_izquierda.configurar(T_TECLADO, SDLK_LEFT);
}

/*Genera los controles por defecto para el modo de control
de joystick.*/

void Configuracion_controles::generar_por_defecto_joystick()
{
	this->nodo_pausa.configurar(T_TECLADO, SDLK_p);
	this->nodo_usar_artefacto.configurar(T_BOTON_JOYSTICK, JR1);
	this->nodo_parar_tiempo.configurar(T_BOTON_JOYSTICK, JL1);
	this->nodo_ciclar_artefacto_siguiente.configurar(T_BOTON_JOYSTICK, JR2);
	this->nodo_ciclar_artefacto_anterior.configurar(T_BOTON_JOYSTICK, JL2);
	this->nodo_movimiento_arriba.configurar(T_BOTON_JOYSTICK, JARRIBA);
	this->nodo_movimiento_derecha.configurar(T_BOTON_JOYSTICK, JDERECHA); 
	this->nodo_movimiento_abajo.configurar(T_BOTON_JOYSTICK, JABAJO);
	this->nodo_movimiento_izquierda.configurar(T_BOTON_JOYSTICK, JIZQUIERDA);
}

std::string Configuracion_controles::generar_ruta()
{
	std::string resultado=RUTA_ARCHIVO+this->extra_ruta+EXTENSION_ARCHIVO;
	return resultado;
}

bool Configuracion_controles::cargar()
{
	std::string ruta=generar_ruta();
	std::ifstream archivo(ruta.c_str(), std::ios::in | std::ios::binary);

	if(!archivo.is_open())
	{
		return false;
	}
	else
	{
		this->nodo_pausa.cargar(archivo);
		this->nodo_usar_artefacto.cargar(archivo);
		this->nodo_parar_tiempo.cargar(archivo);
		this->nodo_ciclar_artefacto_siguiente.cargar(archivo);
		this->nodo_ciclar_artefacto_anterior.cargar(archivo);
		this->nodo_movimiento_arriba.cargar(archivo);
		this->nodo_movimiento_derecha.cargar(archivo);
		this->nodo_movimiento_abajo.cargar(archivo);
		this->nodo_movimiento_izquierda.cargar(archivo);

		archivo.close();

		return true;
	}
}

void Configuracion_controles::guardar()
{
	std::string ruta=generar_ruta();
	std::ofstream archivo(ruta.c_str(), std::ios::out | std::ios::binary);

	if(archivo.is_open())
	{
		this->nodo_pausa.guardar(archivo);
		this->nodo_usar_artefacto.guardar(archivo);
		this->nodo_parar_tiempo.guardar(archivo);
		this->nodo_ciclar_artefacto_siguiente.guardar(archivo);
		this->nodo_ciclar_artefacto_anterior.guardar(archivo);
		this->nodo_movimiento_arriba.guardar(archivo);
		this->nodo_movimiento_derecha.guardar(archivo);
		this->nodo_movimiento_abajo.guardar(archivo);
		this->nodo_movimiento_izquierda.guardar(archivo);

		archivo.close();
	}
}

bool Configuracion_controles::aprender(DLibI::Controles_SDL& p_controles_sdl, Nodo_control &p_nodo)
{
	SDL_Event evento;
	short int codigo=0;

	while(p_controles_sdl.bombear_eventos_manual(evento, false))
	{
		codigo=p_nodo.aprender(evento);

		switch(codigo)
		{
			case C_OK: return true;	break;
			case C_CANCELAR: return false; break;
		}
	}

	return false;
}

std::string Configuracion_controles::generar_cadena_configuracion() const
{
	std::string resultado=
		this->nodo_pausa.traducir()+"\n"+
		this->nodo_usar_artefacto.traducir()+"\n"+
		this->nodo_parar_tiempo.traducir()+"\n"+
		this->nodo_ciclar_artefacto_siguiente.traducir()+"\n"+
		this->nodo_ciclar_artefacto_anterior.traducir()+"\n"+
		this->nodo_movimiento_arriba.traducir()+"\n"+
		this->nodo_movimiento_derecha.traducir()+"\n"+
		this->nodo_movimiento_abajo.traducir()+"\n"+
		this->nodo_movimiento_izquierda.traducir()+"\n";

	return resultado;
}
