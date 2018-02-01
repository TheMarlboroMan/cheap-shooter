#ifndef SONIDO_H
#define SONIDO_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <string>
#include "../../herramientas/log_base/log_base.h"

/* Se definen dos cosas: la clase Sonido es un wrapper para un chunk de 
SDL_Mixer. La clase Estructura sonido envuelve a Sonido y le añade volumen,
repeticiones y otras historias para poder pasarlo a un canal y que haga 
todo lo que tenga que hacer.*/

namespace DLibA
{
class Sonido
{
	private:

	Mix_Chunk * p_sonido;
	std::string ruta;
	bool preparado;
	void liberar();

	public:

	void cargar(const char *);
	Sonido();
	Sonido(const char *);
	~Sonido();

	std::string acc_ruta() const {return this->ruta;}
	bool es_preparado() const {return preparado;}
	Mix_Chunk * acc_datos() {return this->p_sonido;}
};

struct Estructura_sonido
{
	Sonido * sonido;
	short int volumen;	//El volumen -1 es "no hagas cambios".
	short int repeticiones;
	int vol_izq;
	int vol_der;
	int ms_fade;
	
	bool es_preparado() {return sonido && sonido->es_preparado();}

	Estructura_sonido(Sonido * s, int v=-1, int r=0, int pvi=127, int pvd=127, int msf=0):
		sonido(s), volumen(v), repeticiones(r), 
		vol_izq(pvi), vol_der(pvd), ms_fade(msf) {}
	Estructura_sonido(Sonido& s, int v=-1, int r=0, int pvi=127, int pvd=127, int msf=0):
		sonido(&s), volumen(v), repeticiones(r), 
		vol_izq(pvi), vol_der(pvd), ms_fade(msf) {}
};

}

#endif
