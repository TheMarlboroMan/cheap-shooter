#ifndef GESTOR_RECURSOS_SONIDO_H
#define GESTOR_RECURSOS_SONIDO_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "../sonido/sonido.h"
#include "../musica/musica.h"
#include "../../herramientas/templates/gestor_mapa_static.h"

namespace DLibA
{

class Gestor_recursos_audio
{
	private:

	class Gestor_recursos_sonido:public DLibH::Gestor_mapa_static<int, Sonido *>
	{
		public:

		static int insertar(int, const char *);
		static Sonido * obtener(int);
	};

	class Gestor_recursos_musica:public DLibH::Gestor_mapa_static<int, Musica *>
	{
		public:

		static int insertar(int, const char *);
		static Musica * obtener(int);
	};

	Gestor_recursos_audio();

	static Gestor_recursos_sonido sonidos;
	static Gestor_recursos_musica musicas;
	
	public:

	static void liberar();
	static int insertar_sonido(int, const char *);
	static Sonido * obtener_sonido(int);
	static int insertar_musica(int, const char *);
	static Musica * obtener_musica(int);
};

}

#endif
