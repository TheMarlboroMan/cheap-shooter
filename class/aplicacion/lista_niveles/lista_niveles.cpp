#include "lista_niveles.h"

const std::string Lista_niveles::LOCALIZACION_POR_DEFECTO="data/niveles/lista.dat";

Lista_niveles::Lista_niveles()
	:localizacion(LOCALIZACION_POR_DEFECTO), valida(false), externa(false)
	
{
	generar_lista();
	validar_lista();
}

Lista_niveles::Lista_niveles(const char * p_ruta)
	:localizacion(p_ruta), valida(false), externa(true)
{
	generar_lista();
	validar_lista();
}

Lista_niveles::~Lista_niveles()
{

}

void Lista_niveles::generar_lista()
{
	//Buscar el archivo
	std::ifstream archivo(localizacion.c_str());

	if(!archivo)
	{

	}
	else
	{
		char * buffer=new char[80];
		std::string cadena;

		//Leer línea por línea y crear un nuevo Tipo_nivel por cada línea.

		while(true)
		{
			//Leer línea en cadena.
			memset(buffer, '\0', 80);
			archivo.getline(buffer, 80);	
			cadena=buffer;

			if(archivo.eof())
			{
				break;
			}
			else
			{
				vector_niveles.push_back(Tipo_nivel(cadena));
			}
		}

		delete [] buffer;
		archivo.close();	
	}		
}

void Lista_niveles::validar_lista()
{
	//Si no tiene tamaño es falso.
	if(!vector_niveles.size())
	{
		return;
	}
	else
	{
		//De lo contrario, intentar abrir cada uno de los archivos.
		std::ifstream archivo;
		unsigned int errores=0;

		auto comprobacion=[&archivo, &errores](const Tipo_nivel& n)
		{
			archivo.open(n.ruta_fichero);
			if(!archivo) ++errores;
			else archivo.close();
		};

		for_each(vector_niveles.begin(), vector_niveles.end(), comprobacion);
		valida=errores==0;
	}
}

std::string Lista_niveles::obtener_ruta_para_indice(unsigned int p_indice) const
{
	if(p_indice >= vector_niveles.size())
	{
		//Lanzar excepción si nos salimos de índice.
		throw Lista_niveles_excepcion(Lista_niveles_excepcion::C_FUERA_INDICE, "SOLICITUD DE NIVEL FUERA DE INDICE");
	}
	else
	{
		return vector_niveles[p_indice].ruta_fichero;
	}
}

bool Lista_niveles::es_nivel_secreto_indice(unsigned int p_indice) const
{
	if(p_indice >= vector_niveles.size())
	{
		//Lanzar excepción si nos salimos de índice.
		throw Lista_niveles_excepcion(Lista_niveles_excepcion::C_FUERA_INDICE, "SOLICITUD DE SECRETO FUERA DE INDICE");
	}
	else
	{
		return vector_niveles[p_indice].secreto;
	}
}
