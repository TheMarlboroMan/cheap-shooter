#ifndef LOG_BASE_H
#define LOG_BASE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstring>

/*Esto es para sustituir y al guardián. Es más de lo mismo pero está un poco
más actualizado. Básicamente va guardando lo que se meta con el operador <<
pero adicionalmente permite recibir por medio de ese operador los dos structs
Log_base_n y Log_base_m, que van cambiando el nivel de "error".

Por defecto está en nivel de error 10. Todo aquello que esté por encima del 
nivel de error no sólo se guardará en un archivo, sino que se almacenará en una
cadena propia de la clase. Todo lo que esté por debajo no se almacenará en la
cadena.

La cadena la podemos usar para mostrarla por cout o por medio de alguna 
representación de SDL.

Se incluye además una clase Log_motor, que es una aplicación singleton de esta.
Se usará dentro del motor para notificar los posibles errores.
*/

namespace DLibH
{

struct Log_base_n
{
	public:
	unsigned int niv;
	Log_base_n(unsigned int n):niv(n) {}
};

struct Log_base_m
{
	public:
	unsigned int niv;
	Log_base_m(unsigned int n):niv(n) {}
};

struct Log_base_t
{
	//Yeppp..
};

/*Si se define la flag LOG_BASE_SILENCIOSO al compilar, se generará una clase
de log base que no hace absolutamente nada.*/

#ifdef LOG_BASE_SILENCIOSO
	#include "log_base_silencioso.h"
#else
	#include "log_base_normal.h"
#endif

//Incluimos un singleton aquí, para uso del motor. Sería algo así como DLibH::Log_motor::L()<<"aquí tu log";

class Log_motor
{
	private:

	static Log_base * l;
	
	public:

	static Log_base& L()
	{
		if(!l) l=new Log_base("log_motor.log");
		return *l;
	}

	static void finalizar()
	{
		if(!l) l=new Log_base("log_motor.log");
		delete l;
	}
};

}
#endif
