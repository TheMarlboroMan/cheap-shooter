#include "sonido.h"
#include <iostream>

using namespace DLibA;

void Sonido::liberar()
{
	if(this->p_sonido)
	{
		Mix_FreeChunk(this->p_sonido);
		this->p_sonido=NULL;
		this->preparado=false;
	}
}

void Sonido::cargar(const char * p_ruta)
{
	this->p_sonido=Mix_LoadWAV(p_ruta);
	this->ruta=p_ruta;

	if(!this->p_sonido)
	{
		DLibH::Log_motor::L()<<DLibH::Log_base_n(1)<<DLibH::Log_base_t()<<"Sonido::cargar() : sonido no cargado:"<<p_ruta<<std::endl;
		this->preparado=false;
	}
	else
	{
		this->preparado=true;
	}	
}

Sonido::Sonido():preparado(false)
{

}

Sonido::Sonido(const char * p_ruta):preparado(false)
{
	this->cargar(p_ruta);
}

Sonido::~Sonido()
{
	this->liberar();	
}
