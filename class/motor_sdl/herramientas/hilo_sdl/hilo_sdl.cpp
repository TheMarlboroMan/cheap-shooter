#include "hilo_sdl.h"

/*Iniciar sólo se hace UNA vez y es lo que arranca el hilo...*/

using namespace DLibH;

bool Hilo_SDL::iniciar(int (*funcion)(void *), void * datos)
{
	if(this->iniciado) 
	{
		return false;
	}
	else
	{
		this->ed.ref_hilo=this;
		this->ed.datos=datos;
		this->hilo=SDL_CreateThread(funcion, &this->ed);
		if(!this->hilo)
		{
			return false;
		}
		else
		{
			this->iniciado=true;
			return true;
		}
	}
}

/*Finalizar debe ser llamado al final de la función 
que se ejecute en el hilo para permitir al código
cliente saber que la ejecución ha terminado y
que podemos pasar a la última fase...*/

void Hilo_SDL::finalizar()
{
	this->finalizado=true;
}

/*Este ya es el último paso a llamar... Espera a que
el hilo termine realmente de correr y guarda el resultado.*/

int Hilo_SDL::completar()
{
	if(!this->completo)
	{
		SDL_WaitThread(this->hilo, &this->resultado);
		this->iniciado=false;
		this->completo=true;	
	}
	
	return this->resultado;
}

/*Ojo con estos dos métodos: se hacen propietarios del puntero y lo vacían 
de modo que no deberíamos invocarlos a no ser que hayamos terminado de trabajar
con el hilo y no vayamos a usarlo sin reiniciarlo!!!.*/

void Hilo_SDL::destruir_hilo(Hilo_SDL *& p)
{
	delete p;
	p=NULL;
}

void Hilo_SDL::finalizar_y_destruir_hilo(Hilo_SDL *& p)
{
	p->finalizar();
	p->completar();
	delete(p);
	p=NULL;
}

void Hilo_SDL::completar_y_destruir_hilo(Hilo_SDL *& p)
{
	p->completar();
	delete(p);
	p=NULL;
}
