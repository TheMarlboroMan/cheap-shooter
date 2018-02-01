#include "localizador_base.h"

using namespace DLibH;

Localizador_base::Localizador_base(unsigned short int p_idioma)
{
	this->idioma=p_idioma;
}

Localizador_base::~Localizador_base()
{
	this->limpiar_cadenas();
}

void Localizador_base::limpiar_cadenas()
{
	this->cadenas.clear();
}

void Localizador_base::cambiar_idioma(unsigned short int p_idioma)
{
	this->idioma=p_idioma;
	this->inicializar();
}

void Localizador_base::insertar_cadena(unsigned int p_indice, t_cadena const& p_cadena)
{
	this->cadenas.insert(std::pair<unsigned int, t_cadena>(p_indice, p_cadena));
}

t_cadena Localizador_base::generar_nombre_archivo(t_cadena const& p_original)
{
	t_cadena nombre_archivo(p_original);

	nombre_archivo.append(".");
	nombre_archivo.append(Herramientas::entero_a_cadena(this->idioma));
	nombre_archivo.append(".dat");

	return nombre_archivo;
}

void Localizador_base::inicializar()
{
	t_lista_nombres_archivo lista=this->obtener_lista_archivos();
	t_lista_nombres_archivo::iterator ini=lista.begin(), fin=lista.end();

	//Limpiar mapa e ir asignando.
	this->limpiar_cadenas();

	for(;ini<fin; ini++)
	{
		this->procesar_fichero(*ini);
	}

}

void Localizador_base::procesar_fichero(t_cadena const& nombre_archivo)
{
	t_stream_in archivo(this->generar_nombre_archivo(nombre_archivo).c_str(), std::ios::in | std::ios::binary);

	unsigned int indice; //, pos;
	int indice_aux;
	t_char_buffer * buffer=new t_char_buffer[1024];
	t_cadena_stream cadena, cadena_def;
	bool leyendo=false;

	if(archivo)
	{
		//Leemos de línea en línea, 1024 como mucho. Por cada línea leida procesamos.
		while(!archivo.eof())
		{
			//Leer línea en cadena.
			memset(buffer, '\0', 1024);
			archivo.getline(buffer, 1024);
			cadena.assign(buffer);

			//La cadena esta vacia?	Si no estamos leyendo saltar al siguiente con "continue".
			if(!cadena.size() && !leyendo) continue;
			else if(buffer[0]=='#') continue;	//Es un comentario????

			//Delimitador de inicio encontrado?
		
			indice_aux=this->delimitador_inicio_en_cadena(cadena);

			if(indice_aux!=-1)
			{
				leyendo=true; //Marcar leyendo como true.
				indice=indice_aux; //Obtener índice.
				cadena=cadena.substr(3+Herramientas::digitos_en_entero(indice)); //Cortar delimitador inicio. + 3 por el < y el $>
			}

			//Delimitador de fin encontrado?
			if(this->delimitador_fin_en_cadena(cadena))
			{
				leyendo=false; //Marcar leyendo como false.
				cadena=cadena.substr(0, cadena.size()-3); //Cortar resto cadena. -3 es por <#>
				cadena_def.append(cadena);

				//Insertar en mapa.

				this->insertar_cadena(indice, cadena_def);
				cadena_def.clear();
				cadena.clear();
			}		

			//Estamos leyendo?
			if(leyendo)
			{
				cadena.append("\n"); //Insertar nueva línea.
				cadena_def.append(cadena); //Insertar en cadena actual.	
				cadena.clear();	//Limpiar buffer.
			}
		}	
	
		archivo.close();
	}

	delete [] buffer;
}

/*
Busca <n#> al principio de la cadena donde n# es un número positivo. Si lo
encuentra devuelve n#. Si no devolverá -1.
*/
int Localizador_base::delimitador_inicio_en_cadena(std::string const& p_cadena)
{
	unsigned int pos=p_cadena.find("$>", 1);
	int indice=-1;
	std::string cad_indice("");

	if(pos!=std::string::npos)
	{
		cad_indice.assign(p_cadena.substr(1, pos-1));
		indice=Herramientas::cadena_a_entero(cad_indice.c_str());
	}

	return indice;
}

bool Localizador_base::delimitador_fin_en_cadena(std::string const& p_cadena)
{
	bool resultado=false;
	unsigned int pos;
	pos=p_cadena.find("<#>");

	if(pos!=std::string::npos)
	{
		resultado=true;
	}

	return resultado;
}

t_cadena const& Localizador_base::obtener(unsigned int p_indice)
{
	if(!this->cadenas.size())
	{
		return this->cadena_no_cargado();
	}
	else
	{
		t_mapa::iterator it;
		it=this->cadenas.find(p_indice);

		if(it==this->cadenas.end())
		{
			return this->cadena_no_encontrado();
		}
		else
		{
			//Esto de second, debe ser de los pairs.
			return it->second;
		}
	}
}
