#ifndef LISTA_NIVELES_H
#define LISTA_NIVELES_H

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

/*La herramienta para leer la lista de niveles del juego. La lista de niveles
es un archivo de texto en el que cada línea representa a un nivel en la
siguiente forma:

[N/S]ruta/completa/del_fichero.dat

N o S indica que el nivel es "NORMAL" o "SECRETO". No hay por defecto, es 
obligatorio especificarlo. El número de línea será el índice del nivel. Tan
sencillo como eso.

*/

struct Lista_niveles_excepcion
{
	enum codigos
	{
		C_FUERA_INDICE=0
	};

	unsigned short int codigo;
	std::string mensaje;

	Lista_niveles_excepcion(unsigned short int p_c, std::string p_m)
		:codigo(p_c), mensaje(p_m) {}
};


class Lista_niveles
{
	private:

	static const std::string LOCALIZACION_POR_DEFECTO; //Indica la localización de la lista de niveles por defecto.

	struct Tipo_nivel
	{
		private:
			Tipo_nivel() {}; //Muahahahaahaaaa...

		public:

		bool secreto;
		std::string ruta_fichero;

		Tipo_nivel(const Tipo_nivel& otro):secreto(otro.secreto), ruta_fichero(otro.ruta_fichero){}
		Tipo_nivel(std::string linea)
		{
			secreto=linea.substr(0, 1)=="S" ? true : false;
			ruta_fichero=linea.substr(1);
		}
	};

	//Propiedades.

	typedef std::vector<Tipo_nivel> VECTOR_NIVELES;
	VECTOR_NIVELES vector_niveles;
	std::string localizacion;
	bool valida;
	bool externa; //Se refiere a que es un archivo que no es el original.

	//Métodos...
	
	void generar_lista();
	void validar_lista(); //Intentará ver si todos los ficheros apuntados por la lista existen.

	public:

	Lista_niveles();	//Creará la lista por defecto.
	Lista_niveles(const char *);	//Creará la lista con los que pueda cargar.
	~Lista_niveles();

	bool es_valida() const {return this->valida;}
	bool es_externa() const {return this->externa;}
	bool es_nivel_secreto_indice(unsigned int) const; //throw(Lista_niveles_excepcion)
	std::string obtener_ruta_para_indice(unsigned int) const; //throw(Lista_niveles_excepcion)	//Indicará la ruta donde leer el archivo.
};

#endif
