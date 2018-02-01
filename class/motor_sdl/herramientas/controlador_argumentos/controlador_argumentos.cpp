#include "controlador_argumentos.h"

using namespace DLibH;

Controlador_argumentos::Controlador_argumentos()
{

}

Controlador_argumentos::Controlador_argumentos(int argc, char ** argv)
{
	this->inicializar(argc, argv);
}

void Controlador_argumentos::inicializar(int argc, char ** argv)
{
	int i=0;
	for(; i<argc; i++)
	{
		this->argumentos.push_back(t_argumento(argv[i]));
	}
}

Controlador_argumentos::~Controlador_argumentos()
{
	this->argumentos.clear();
}

unsigned int Controlador_argumentos::obtener_cantidad() const 
{
	return this->argumentos.size();
}

const Controlador_argumentos::t_argumento Controlador_argumentos::acc_argumento(unsigned int p_arg) const
{
	try
	{
		return this->argumentos[p_arg];
	}
	catch (...)
	{
		std::cout<<"Controlador_argumentos::se intentó acceder a un argumento no existente"<<std::endl;
		return t_argumento("ERROR");
	}
}

const Controlador_argumentos::t_lista_argumentos& Controlador_argumentos::acc_argumentos() const 
{
	return this->argumentos;
}

/*
Busca un argumento. Devuelve -1 si no encuentra nada y el índice del argumento si la ha encontrado.
*/

int Controlador_argumentos::buscar(const char * p_char) const {return this->buscar(t_argumento(p_char));}
int Controlador_argumentos::buscar(t_argumento const& p_buscar) const
{
	//Manual del todo..
	int resultado=-1;
	int i=0;

	t_lista_argumentos::const_iterator 	ini=this->argumentos.begin(),
						fin=this->argumentos.end();

	for(;ini<fin; ini++, i++)
	{
		if( (*ini).compare(p_buscar)==0)
		{
			resultado=i;
			break;
		}
	}

	return resultado;
}
