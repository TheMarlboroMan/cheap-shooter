#include "musica.h"
#include <iostream>

using namespace DLibA;

void Musica::liberar()
{
	if(this->p_musica)
	{
		Mix_FreeMusic(this->p_musica);
		this->p_musica=NULL;
		this->preparada=false;
	}
}

void Musica::cargar(const char * p_ruta)
{
	this->p_musica=Mix_LoadMUS(p_ruta);

	if(!this->p_musica)
	{
		this->preparada=false;
	}
	else
	{
		this->preparada=true;
	}	
}

Musica::Musica():preparada(false)
{

}

Musica::Musica(const char * p_ruta):preparada(false)
{
	this->cargar(p_ruta);
}

Musica::~Musica()
{
	this->liberar();	
}
