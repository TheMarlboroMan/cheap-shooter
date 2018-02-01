#include "cola_sonido.h"

using namespace DLibA;

/* TODO: 
Lo ideal es que esto no sea sólo una cola de sonidos sino una cola de bloques
de sonido que incluyan cosas como el canal, el volumen o lo que sea. De momento
lo podemos dejar así pero me gustaría estudiarlo en el futuro.
*/

Cola_sonido::Cola_sonido()
{
	this->inicializar();
}

Cola_sonido::~Cola_sonido()
{
	this->vaciar();
}

/*
OJO: El mero de hecho de llamar a este método (que se hace sólo nada más 
construir un objeto de esta clase) implica que se inicializará el controlador
de audio sdl.
*/

void Cola_sonido::inicializar()
{
	this->controlador=Controlador_audio_SDL::obtener();
}

void Cola_sonido::vaciar()
{
	while(!this->cola.empty())
	{
		this->cola.pop();
	}		
}

//Los sonidos se insertan, preparados o no.

void Cola_sonido::insertar(const Estructura_sonido& p_e)
{
	this->cola.push(p_e);
}
	
Estructura_sonido Cola_sonido::obtener_primero()
{
	return this->cola.front();
}

void Cola_sonido::eliminar_primero()
{
	this->cola.pop();	
}

void Cola_sonido::procesar_cola(bool descartar)
{
	if(this->controlador)
	{
		//Los sonidos que NO se pueden reproducir se descartan o se
		//insertan en la parte trasera de la cola de nuevo. 
		//Simplemente hacemos un bucle por los sonidos que había al 
		//principio y lo iteramos una vez.

		Estructura_sonido actual=NULL;
		unsigned int t=this->cola.size(), i=0;

		for(; i<t; i++)
		{
			actual=this->cola.front();
			this->cola.pop();

			if(this->controlador->reproducir_sonido(actual) == -1 && !descartar)
			{
				this->cola.push(actual);
			}

			actual=NULL;
		}
	}
}
