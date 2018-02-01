#include "perfil.h"

const std::string Perfil::RUTA_PERFILES="data/perfiles/";
const std::string Perfil::EXTENSION_PERFILES=".dat";

Perfil::Perfil():
	datos_juego(VERSION_CORRIENTE), 
	posicion_inicio_datos_juego(-1),
	anonimo(false)
{

}

Perfil::~Perfil()
{

}

Perfil& Perfil::operator=(const Perfil& p_otro)
{
	this->nombre_archivo=p_otro.nombre_archivo;
	this->nombre=p_otro.nombre;
	this->email=p_otro.email;
	this->id_bd=p_otro.id_bd;
	this->anonimo=p_otro.anonimo;
	this->datos_juego=p_otro.datos_juego;
	this->posicion_inicio_datos_juego=p_otro.posicion_inicio_datos_juego;

	return *this;
}
 
/*Estos dos son métodos auxiliares para guardar y cargar cadenas desde un 
fichero.*/

void Perfil::guardar_string_en_fichero(std::fstream &fichero, std::string cadena)
{
	unsigned short int i=0;
	unsigned short int l=cadena.size();

	//Guardamos la longitud del nombre y el nombre.
	fichero.write((char *) &l, sizeof(U_ENTERO_8));
	for(;i<l; i++) fichero.write((char *) &cadena[i], sizeof(U_ENTERO_8));
}

std::string Perfil::leer_string_de_fichero(std::ifstream &fichero)
{
	unsigned short l=0;
	fichero.read((char *) &l, sizeof(U_ENTERO_8));

	char * buffer=new char[l+1];
	fichero.read(buffer, sizeof(U_ENTERO_8)*l);
	buffer[l]='\0';	

	std::string resultado=buffer;

	delete [] buffer;

	return resultado;

}

/*Este método se encarga de guardar los datos propios del perfil en el fichero.
Al final se llamará también al método de la clase Perfil_datos que se encarga
de guardar los datos que no son propiamente del perfil (trofeos y cosas así).*/

void Perfil::guardar()
	throw(Perfil_excepcion)
{
	//Un perfil anónimo no realiza operaciones de disco.
	if(this->anonimo)
	{
		return;
	}	
	//Si no hay nombre de archivo sacamos una excepción.
	else if(!this->nombre_archivo.size())
	{
		throw Perfil_excepcion(Perfil_excepcion::C_PERFIL_MAL_PREPARADO, "EL NOMBRE DE FICHERO NO TIENE LONGITUD");
	}
	//Proceso normal.
	else
	{
		std::string ruta=RUTA_PERFILES+this->nombre_archivo+EXTENSION_PERFILES;
//		std::ofstream archivo(ruta.c_str(), std::ios::out | std::ios::binary);	
		std::fstream archivo(ruta.c_str(), std::ios::out | std::ios::binary);	

		if(!archivo)
		{
			std::string error="IMPOSIBLE ABRIR FICHERO PARA GUARDAR "+ruta;
			throw Perfil_excepcion(Perfil_excepcion::C_NO_ARCHIVO_GUARDAR, error);
		}
		else
		{
			guardar_string_en_fichero(archivo, this->nombre);
			guardar_string_en_fichero(archivo, this->email);
			archivo.write( (char *) &this->id_bd, sizeof(U_ENTERO_32));			

			this->posicion_inicio_datos_juego=archivo.tellp();

			this->guardar_datos_perfil(archivo);
			archivo.close();
		}
	}
}

/*Este método guardaría sólo los datos de Perfil_datos sin afectar al fichero
en lo que se refiere a los datos propios del perfil. Tendríamos que hacerlo
habitualmente según se juega, para mantener los datos de los trofeos y tal
al día.*/

void Perfil::actualizar_datos_juego(Estructura_datos_perfil &datos_nuevos)
	throw(Perfil_excepcion)
{
	//Un perfil anónimo no realiza operaciones de disco.
	if(this->anonimo)
	{
		return;
	}	
	//Si no hay nombre de archivo sacamos una excepción.
	else if(!this->nombre_archivo.size())
	{
		throw Perfil_excepcion(Perfil_excepcion::C_PERFIL_MAL_PREPARADO, "EL NOMBRE DE FICHERO NO TIENE LONGITUD");
	}
	//Proceso normal.
	else
	{
		std::string ruta=RUTA_PERFILES+this->nombre_archivo+EXTENSION_PERFILES;

/*When creating an output stream using ofstream, any preexisting file by that name is automatically truncated

std::ofstream archivo(ruta.c_str(), std::ios::out | std::ios::in | std::ios::app | std::ios::binary);

No he terminado bien de entender lo que ha pasado aquí... Por alguna razón, si no le pongo el in parece que no haga bien el seekp o algo así. */

		std::fstream archivo(ruta.c_str(), std::ios::out | std::ios::in | std::ios::binary);
//		std::fstream archivo(ruta.c_str(), std::ios::out | std::ios::binary);

		if(!archivo)
		{
			std::string error="IMPOSIBLE ABRIR FICHERO PARA GUARDAR "+ruta;
			throw Perfil_excepcion(Perfil_excepcion::C_NO_ARCHIVO_GUARDAR, error);
		}
		else
		{
			//Buscamos el punto donde tiene que estar la información que necesitamos :D...
			archivo.seekp(this->posicion_inicio_datos_juego, std::ios::beg);
			this->datos_juego.actualizar_por_estructura(datos_nuevos);
			this->guardar_datos_perfil(archivo);
			archivo.close();
		}
	}
}

/*Esto es un mini-método auxiliar que vale sólo para guardar los datos de
Perfil_datos*/

void Perfil::guardar_datos_perfil(std::fstream &p_archivo)
{
	this->datos_juego.guardar(p_archivo);
}

/*Carga los datos del perfil desde el archivo correspondiente. Cuando ha 
finalizado carga del mismo archivo los datos de Perfil_datos.*/

Perfil Perfil::cargar_desde_archivo(std::string p_nombre_archivo)
	throw(Perfil_excepcion)
{
	std::string ruta=RUTA_PERFILES+p_nombre_archivo+EXTENSION_PERFILES;
	std::ifstream archivo(ruta.c_str(), std::ios::in | std::ios::binary);

	if(!archivo)
	{
		std::string error="IMPOSIBLE ABRIR FICHERO "+ruta;
		throw Perfil_excepcion(Perfil_excepcion::C_NO_ARCHIVO_CARGAR, error);
	}
	else
	{
		Perfil resultado;

		resultado.nombre_archivo=p_nombre_archivo;

		resultado.nombre=leer_string_de_fichero(archivo);
		resultado.email=leer_string_de_fichero(archivo);
		archivo.read( (char *) &resultado.id_bd, sizeof(U_ENTERO_32));

		resultado.posicion_inicio_datos_juego=archivo.tellg();

		resultado.cargar_datos_perfil(archivo);

		archivo.close();
		return resultado;
	}
}

/*Esto es un método auxiliar para sólo cargar los datos de Perfil_datos...*/

void Perfil::cargar_datos_perfil(std::ifstream & p_archivo)
{
	this->datos_juego.cargar(p_archivo);
}

/*Crea un perfil anónimo. Un perfil anónimo no tiene absolutaente
nada y se usa para cuando queremos jugar sin perfil. El perfil
anónimo nunca accede a disco para nada.*/

Perfil Perfil::generar_perfil_anonimo()
{
	Perfil resultado;

	resultado.nombre_archivo="";
	resultado.nombre="";
	resultado.email="";
	resultado.id_bd=0;
	resultado.anonimo=true;

	return resultado;
}

/*A partir del nombre del usuario se generará un nombre de archivo
que contenga tan sólo A-Z, a-z y 0-9.*/

std::string Perfil::generar_nombre_fichero_para_nombre(std::string p_nombre)
{
	std::string resultado=DLibH::Herramientas::cadena_a_alfanumerica_normalizada(p_nombre, '_');
	return resultado;
}

/*Devuelve un perfil a partir de los datos que se hayan pasado.*/

Perfil Perfil::crear_perfil_desde_datos(std::string p_nombre, std::string p_email, unsigned long int p_id)
{
	Perfil resultado;
	resultado.nombre=p_nombre;
	resultado.email=p_email;
	resultado.id_bd=p_id;
	resultado.nombre_archivo=generar_nombre_fichero_para_nombre(p_nombre);

	return resultado;
}

bool Perfil::eliminar()
{
	std::string ruta=RUTA_PERFILES+this->nombre_archivo+EXTENSION_PERFILES;
	return remove(ruta.c_str())==0;
}
