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

class Log_base
{
	private:

	
	static std::stringstream sstream;
	std::ofstream s;	
	std::string c;
	unsigned int nivel_min;
	unsigned int nivel_actual;
	bool cadena_actualizada;
	bool activo;

	template <class X> void en_cadena(const X &input)
	{
		sstream.clear();
		sstream<<input;
		c+=sstream.str();
		sstream.str(" ");
		cadena_actualizada=true;
	}

	void en_cadena(std::ostream& (*pf)(std::ostream&))
	{
		sstream.clear();
		sstream<<pf;
		c+=sstream.str();
		sstream.str(" ");
		cadena_actualizada=true;
	}

	void en_cadena(std::ios& (*pf)(std::ios&))
	{
		sstream.clear();
		sstream<<pf;
		c+=sstream.str();
		sstream.str(" ");
		cadena_actualizada=true;
	}

	void en_cadena(std::ios_base& (*pf)(std::ios_base&))
	{
		sstream.clear();
		sstream<<pf;
		c+=sstream.str();
		sstream.str(" ");
		cadena_actualizada=true;
	}

	inline bool es_usable() {return activo && s.is_open();}

	public:

	const std::string& cadena()
	{
		cadena_actualizada=false;
		return c;
	}

	void activar() 
	{
		activo=true;
		if(es_usable())
		{
			s<<"ACTIVANDO SESION "<<time(NULL)<<std::endl;
		}
	}
	void desactivar() 
	{
		if(es_usable())
		{
			s<<"DESACTIVANDO SESION "<<time(NULL)<<std::endl;
		}

		activo=false;
	}

	void limpiar_cadena() {c="";}
	bool es_cadena_actualizada() const {return cadena_actualizada;}

	void inicializar(const char * nombre_fichero)
	{
		s.open(nombre_fichero);

		if(es_usable())
		{
			s<<"INICIANDO SESION "<<time(NULL)<<std::endl;
		}
	}

	Log_base()
		:s(), c(), nivel_min(10), nivel_actual(10),
		cadena_actualizada(false), activo(false) {}

	Log_base(const char * nombre_fichero)
		:s(), c(), nivel_min(10), nivel_actual(10),
		cadena_actualizada(false), activo(true) {inicializar(nombre_fichero);}

	~Log_base()
	{
		if(es_usable())
		{
			s<<std::endl<<"FINALIZANDO SESION "<<time(NULL);
			s.close();
		}
	}

//Esta es la entrada para todo lo que hay... Si el nivel actual es 
//mayor o igual que el nivel mínimo se logueará también en la cadena.

	template <class X> Log_base& operator<<(const X &input)
	{
		if(es_usable())
		{
			s<<input;
			if(this->nivel_actual >= this->nivel_min) en_cadena(input);
			s.flush();
		}
		return *this;
	}

	//Coloca la hora.
	Log_base& operator<<(const Log_base_t& n)
	{
		if(es_usable())
	 	{
			char * t=new char[14];
			memset(t, '\0', 14);
			time_t tiempo=time(NULL);
			strftime(t, 14, "%H:%M:%S >> ", localtime(&tiempo));
	
			s<<t;
			delete [] t ;
		}
		return *this;
	}

	//Establece un nuevo nivel actual de log
	Log_base& operator<<(const Log_base_n& n)
	{
		this->nivel_actual=n.niv;
		return *this;
	}

	//Establece un nuevo nivel minimo de log
	Log_base& operator<<(const Log_base_m& n)
	{
		this->nivel_min=n.niv;
		return *this;
	}

	Log_base& operator<<(std::ostream& (*)(std::ostream&));
	Log_base& operator<<(std::ios& (*)(std::ios&));
	Log_base& operator<<(std::ios_base& (*)(std::ios_base&));
};

/*Incluimos un singleton aquí, para uso del motor. Sería algo así
como DLibH::Log_motor::L()<<"aquí tu log";

Un detalle: cuando se inicia está inactivo. Hay que habilitarlo manualmente 
haciendo Log_motor::L().arrancar();
*/


class Log_motor
{
	private:

	static Log_base * l;
	
	public:

	static Log_base& L()
	{
		if(!l) 
		{
			l=new Log_base();
		}
		
		return *l;
	}

	static void arrancar()
	{
		if(!l) 
		{
			l=new Log_base();
		}

		l->inicializar("log_motor.log");
		l->activar();
	}

	static void finalizar()
	{
		if(l)
		{
			delete l;
			l=NULL;
		}
	}
};

}
#endif
