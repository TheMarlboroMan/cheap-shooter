#ifndef MUSICA_H
#define MUSICA_H

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <string>
#include <iostream>

namespace DLibA
{
class Musica
{
	private:

	Mix_Music * p_musica;
	std::string ruta;
	bool preparada;

	void liberar();

	public:

	void cargar(const char *);
	Musica();
	Musica(const char *);
	~Musica();

	std::string acc_ruta() const {return this->ruta;}
	bool es_preparada() const {return this->preparada;}
	Mix_Music * acc_datos() {return this->p_musica;}
};

}

#endif
