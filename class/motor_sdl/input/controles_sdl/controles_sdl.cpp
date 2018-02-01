#include "controles_sdl.h"

using namespace DLibI;

Controles_SDL::Controles_SDL():
	senal_salida(false), joystick_defecto(0), cantidad_joysticks(0)
{
	//Esto traga memoria, pero que le den.
	this->teclas_up=new char[SDLK_LAST];
	this->teclas_down=new char[SDLK_LAST];

	this->inicializar_teclas();
	this->inicializar_joysticks();

	this->hay_eventos_movimiento_raton=false;
	this->hay_eventos_boton_raton=false;
	this->hay_eventos_teclado=false;
	this->hay_eventos_eje_joystick=false;
	this->hay_eventos_boton_joystick=false;
}

Controles_SDL::~Controles_SDL()
{
	if(this->teclas_up) delete[] this->teclas_up;
	if(this->teclas_up) delete[] this->teclas_down;
	this->cerrar_joysticks();

}

void Controles_SDL::inicializar_joysticks()
{
	this->cantidad_joysticks=SDL_NumJoysticks();

	unsigned int i=0;
	if(this->cantidad_joysticks > 0)
	{
		SDL_JoystickEventState(SDL_ENABLE);
		Joystick * temp=NULL;

		for(;i<this->cantidad_joysticks; i++)
		{
			temp=new Joystick(i);
			temp->inicializar();
			this->joysticks.push_back(temp);	
			temp=NULL;
		}
	}
}

void Controles_SDL::cerrar_joysticks()
{
	unsigned int l=this->joysticks.size(), i=0;
	for(;i<l; i++)
	{
		delete(this->joysticks[i]);
	}

	this->joysticks.clear();
}

void Controles_SDL::inicializar_teclas()
{
	memset(this->teclas_up, 0, SDLK_LAST);
	memset(this->teclas_down, 0, SDLK_LAST);
}

bool Controles_SDL::bombear_eventos_manual(SDL_Event &p_evento, bool p_procesar)
{
	bool resultado=SDL_PollEvent(&p_evento);
	if(p_procesar && resultado) this->procesar_evento(p_evento);
	return resultado;
}

/*
A esto se le llama una vez por cada evento que haya en "recoger". Lo hemos
incluido para poderlo llamar desde otros puntos como el "Escritor_texto" y 
poder seguir usando la clase de Controles desde fuera de la última.
*/

void Controles_SDL::procesar_evento(SDL_Event& eventos)
{
	switch(eventos.type)
	{
		case SDL_QUIT:
			this->senal_salida=true; 
		break;

		case SDL_MOUSEMOTION:

			this->hay_eventos_movimiento_raton=true;

			raton.x=eventos.motion.x; 
			raton.y=eventos.motion.y; 
			raton.movimiento=true;
		break;

		case SDL_MOUSEBUTTONDOWN:

			this->hay_eventos_boton_raton=true;

			this->raton.botones_down[eventos.button.button]=1;
			this->raton.botones_pulsados[eventos.button.button]=1;
		break;

		case SDL_MOUSEBUTTONUP:

			this->hay_eventos_boton_raton=true;

			this->raton.botones_up[eventos.button.button]=1;
			this->raton.botones_pulsados[eventos.button.button]=0;
		break;

		case SDL_JOYBUTTONDOWN:
			this->hay_eventos_boton_joystick=true;
			this->joysticks[eventos.jbutton.which]->registrar_boton(0, eventos.jbutton.button);
		break;

		case SDL_JOYBUTTONUP:
			this->hay_eventos_boton_joystick=true;
			this->joysticks[eventos.jbutton.which]->registrar_boton(1, eventos.jbutton.button);
		break;

		case SDL_JOYAXISMOTION:
			if(eventos.jaxis.value < -MIN_RUIDO || eventos.jaxis.value > MAX_RUIDO)
			{
				this->hay_eventos_eje_joystick=true;
				this->joysticks[eventos.jaxis.which]->registrar_eje(eventos.jaxis.axis, eventos.jaxis.value);
			}
			else
			{
				this->joysticks[eventos.jaxis.which]->registrar_eje(eventos.jaxis.axis, 0);
			}
		break;

		case SDL_ACTIVEEVENT:

			this->evento_actividad.recibir_input((bool) eventos.active.gain, eventos.active.state);

		break;

		case SDL_KEYDOWN:

			this->hay_eventos_teclado=true;
			this->teclas_down[eventos.key.keysym.sym]=1;
		break;

		case SDL_KEYUP:

			this->hay_eventos_teclado=true;
			this->teclas_up[eventos.key.keysym.sym]=1;
		break;

		default: break;
	}
}

void Controles_SDL::limpiar_estado_joysticks()
{
	unsigned int l=this->joysticks.size(), i=0;
	for(;i<l; i++)
		this->joysticks[i]->inicializar_estado();
}

bool Controles_SDL::comprobacion_boton_joystick(unsigned int p_joystick, unsigned int p_boton) const
{
	if(!this->joysticks.size()) return false;
	else if(p_joystick > this->joysticks.size()) return false;
	else if(p_boton > this->joysticks[p_joystick]->botones) return false;
	else return true;
}

bool Controles_SDL::es_joystick_boton_down(unsigned int p_joystick, unsigned int p_boton) const
{
	if(!this->comprobacion_boton_joystick(p_joystick, p_boton)) return false;
	else 
	{
		return this->joysticks[p_joystick]->botones_down[p_boton];
	}
}

bool Controles_SDL::es_joystick_boton_up(unsigned int p_joystick, unsigned int p_boton) const
{
	if(!this->comprobacion_boton_joystick(p_joystick, p_boton)) return false;
	else 
	{
		return this->joysticks[p_joystick]->botones_up[p_boton];	
	}
}

bool Controles_SDL::es_joystick_boton_pulsado(unsigned int p_joystick, unsigned int p_boton) const
{
	if(!this->comprobacion_boton_joystick(p_joystick, p_boton)) return false;
	else 
	{
		return this->joysticks[p_joystick]->botones_pulsados[p_boton];
	}
}

bool Controles_SDL::es_joystick_boton_soltado(unsigned int p_joystick, unsigned int p_boton) const
{
	if(!this->comprobacion_boton_joystick(p_joystick, p_boton)) return false;
	else 
	{
		return this->joysticks[p_joystick]->botones_soltados[p_boton];	
	}
}

bool Controles_SDL::comprobacion_eje_joystick(unsigned int p_joystick, unsigned int p_eje) const
{
	if(!this->joysticks.size()) return false;
	else if(p_joystick > this->joysticks.size()) return false;
	else if(p_eje > this->joysticks[p_joystick]->cantidad_ejes) return false;
	else return true;
}

Sint16 Controles_SDL::joystick_eje(unsigned int p_joystick, unsigned int p_eje) const
{
	if(!this->comprobacion_eje_joystick(p_joystick, p_eje)) return false;
	else
	{
		return this->joysticks[p_joystick]->ejes[p_eje];
	}
}

bool Controles_SDL::es_joystick_boton_down(unsigned int p_boton) const {return this->es_joystick_boton_down(this->joystick_defecto, p_boton);}
bool Controles_SDL::es_joystick_boton_up(unsigned int p_boton) const {return this->es_joystick_boton_up(this->joystick_defecto, p_boton);}
bool Controles_SDL::es_joystick_boton_pulsado(unsigned int p_boton) const {return this->es_joystick_boton_pulsado(this->joystick_defecto, p_boton);}
bool Controles_SDL::es_joystick_boton_soltado(unsigned int p_boton) const {return this->es_joystick_boton_soltado(this->joystick_defecto, p_boton);}
Sint16 Controles_SDL::joystick_eje(unsigned int p_eje) const {return this->joystick_eje(this->joystick_defecto, p_eje);}

void Controles_SDL::limpiar_estado_eventos_actividad()
{
	this->evento_actividad.reiniciar();
}

void Controles_SDL::limpiar_para_nueva_recogida()
{
	this->hay_eventos_movimiento_raton=false;
	this->hay_eventos_boton_raton=false;
	this->hay_eventos_teclado=false;
	this->hay_eventos_eje_joystick=false;
	this->hay_eventos_boton_joystick=false;

	//Alimentamos las teclas pulsadas y trabajamos con ellas.
	this->teclado.mut_teclas_pulsadas(SDL_GetKeyState(NULL));
	this->inicializar_teclas();
	this->limpiar_estado_joysticks();
	this->limpiar_estado_eventos_actividad();
	this->raton.inicializar_estado();
}

void Controles_SDL::recoger()
{
	this->limpiar_para_nueva_recogida();
			
	//Eventos...
	while(SDL_PollEvent(&eventos))
	{
		this->procesar_evento(eventos);
	}

//	raton.manejador();
}

bool Controles_SDL::es_evento_quit(SDL_Event &p_evento) const {return p_evento.type==SDL_QUIT;}
bool Controles_SDL::es_evento_mousemotion(SDL_Event &p_evento) const {return p_evento.type==SDL_MOUSEMOTION;}
bool Controles_SDL::es_evento_mousebuttondown(SDL_Event &p_evento) const {return p_evento.type==SDL_MOUSEBUTTONDOWN;}
bool Controles_SDL::es_evento_mousebuttonup(SDL_Event &p_evento) const {return p_evento.type==SDL_MOUSEBUTTONUP;}
bool Controles_SDL::es_evento_keydown(SDL_Event &p_evento) const {return p_evento.type==SDL_KEYDOWN;}
bool Controles_SDL::es_evento_keyup(SDL_Event &p_evento) const {return p_evento.type==SDL_KEYUP;}
