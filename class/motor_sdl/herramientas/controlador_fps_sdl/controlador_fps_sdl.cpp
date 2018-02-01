#include "controlador_fps_sdl.h"

using namespace DLibH;

Controlador_fps_SDL::Controlador_fps_SDL(unsigned int p_f):
	aplicar(true), 
	ticks_cuenta(0), 
	ticks_fin(0), 
	ticks_inicio(0), 
	dif(0), 
	frames_contados(0), 
	frames_contados_interno(0), 
	duracion_frame(0),
	frames_deseados(p_f),
	delta(0)
{
	this->calcular_duracion_frame();
	this->inicializar();
}
Controlador_fps_SDL::~Controlador_fps_SDL()
{
	this->vaciar_cronos();
}

void Controlador_fps_SDL::calcular_duracion_frame()
{
	this->duracion_frame=1000 / this->frames_deseados;	
}

void Controlador_fps_SDL::incrementar_framerate()
{
	this->frames_deseados++;
	this->establecer_framerate(this->frames_deseados);
}

void Controlador_fps_SDL::decrementar_framerate()
{
	this->frames_deseados--;
	this->establecer_framerate(this->frames_deseados);
}

void Controlador_fps_SDL::establecer_framerate(unsigned int p_param)
{
	this->frames_deseados=p_param;
	this->calcular_duracion_frame();
}

void Controlador_fps_SDL::mut_aplicar(bool valor) 
{
	this->aplicar=valor;
}

void Controlador_fps_SDL::inicializar()
{
	this->ticks_cuenta=SDL_GetTicks();
	this->ticks_inicio=this->ticks_cuenta;
	this->frames_contados_interno=0;
	this->frames_contados=0;
}

float Controlador_fps_SDL::obtener_delta_para_tiempo(Uint32 p_ticks)
{
	Uint32 ahora=SDL_GetTicks();
	Uint32 dif=ahora - p_ticks;
	float delta=dif / 1000.f;
	return delta;
}

void Controlador_fps_SDL::turno()
{
	if(!this->aplicar) return;

	this->ticks_fin=SDL_GetTicks();  //Cortar FPS aquí.
	this->frames_contados_interno++;

	if( (this->ticks_fin - this->ticks_cuenta) > 1000)
	{
		this->frames_contados=this->frames_contados_interno;
		this->frames_contados_interno=0;
		this->ticks_cuenta=SDL_GetTicks();
	}

	this->dif=this->ticks_fin - this->ticks_inicio;
	this->delta=this->dif / 1000.f;
	if(this->cronos.size()) this->procesar_cronos();

	this->ticks_inicio=SDL_GetTicks();
}

void Controlador_fps_SDL::limitar_fps()
{
	if(this->dif < this->duracion_frame) SDL_Delay(this->duracion_frame-this->dif);
}

float Controlador_fps_SDL::obtener_delta()
{
	return this->delta;
}

void Controlador_fps_SDL::procesar_cronos()
{
	std::vector<Cronometro_SDL *>::iterator 	ini=this->cronos.begin(),
							fin=this->cronos.end();

	while(ini < fin)
	{
		if((*ini)->es_activo())
		{
			(*ini)->turno(this->delta);
		}
		++ini;
	}
}

Cronometro_SDL * Controlador_fps_SDL::insertar_crono(Cronometro_SDL & p_crono)
{
	return this->insertar_crono(&p_crono);
}

Cronometro_SDL * Controlador_fps_SDL::insertar_crono(Cronometro_SDL * p_crono)
{
	std::vector<Cronometro_SDL *>::iterator 	ini=this->cronos.begin(),
							fin=this->cronos.end(),
							res;

	res=find(ini, fin, p_crono);
	if(res==fin) this->cronos.push_back(p_crono);
	return p_crono;
}

bool Controlador_fps_SDL::retirar_crono(Cronometro_SDL & p_crono)
{
	return this->retirar_crono(&p_crono);
}

bool Controlador_fps_SDL::retirar_crono(Cronometro_SDL * p_crono)
{
	std::vector<Cronometro_SDL *>::iterator 	ini=this->cronos.begin(),
							fin=this->cronos.end();

	unsigned int cantidad_anterior=cronos.size();
	remove(ini, fin, p_crono);
	return cantidad_anterior != cronos.size();
}

void Controlador_fps_SDL::vaciar_cronos()
{
	this->cronos.clear();
}
