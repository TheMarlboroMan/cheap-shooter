#ifndef PERFIL_H
#define PERFIL_H

#include <string>
#include <fstream>
#include <cstring>
#include "../../motor_sdl/herramientas/herramientas/herramientas.h"
#include "../perfil_datos/perfil_datos.h"
#include "../estructura_datos_perfil/estructura_datos_perfil.h"

struct Perfil_excepcion
{
	enum codigos
	{
		C_NO_ARCHIVO_CARGAR=0,	
		C_PERFIL_MAL_PREPARADO,
		C_NO_ARCHIVO_GUARDAR,
		C_CONTROLADOR_VACIO,
		C_CONTROLADOR_INDICE_INVALIDO
	};

	unsigned short int codigo;
	std::string mensaje;

	Perfil_excepcion(unsigned short int p_c, std::string p_m)
		:codigo(p_c), mensaje(p_m)
	{}
};

class Perfil
{
	private:

	typedef uint8_t U_ENTERO_8;
	typedef uint32_t U_ENTERO_32;

	Perfil_datos datos_juego;

	std::string nombre_archivo;
	std::string nombre;
	std::string email;
	unsigned long int id_bd;
	int posicion_inicio_datos_juego;
	bool anonimo;	//Esto controla si el perfil debe hacer operaciones en disco y con trofeos.
	static const unsigned int VERSION_CORRIENTE=1;

	static void guardar_string_en_fichero(std::fstream &, std::string);
	static std::string leer_string_de_fichero(std::ifstream &);

	void cargar_datos_perfil(std::ifstream &);
	void guardar_datos_perfil(std::fstream &);

	public:

	static const unsigned int MAX_CARACTERES_NOMBRE=20;
	static const unsigned int MIN_CARACTERES_NOMBRE=4;
	static const unsigned int MAX_CARACTERES_EMAIL=120;
	static const unsigned int MAX_CARACTERES_CLAVE=10;
	static const unsigned int MIN_CARACTERES_CLAVE=4;
	static const std::string RUTA_PERFILES;
	static const std::string EXTENSION_PERFILES;

	Perfil& operator=(const Perfil &);
	Perfil();
	~Perfil();

	Perfil_datos * obtener_datos_juego() {return &this->datos_juego;}

	static Perfil cargar_desde_archivo(std::string) throw (Perfil_excepcion);
	static Perfil generar_perfil_anonimo();
	void guardar() throw(Perfil_excepcion);
	void actualizar_datos_juego(Estructura_datos_perfil &) throw (Perfil_excepcion);
	bool eliminar();

	std::string acc_nombre() const {return this->nombre;}
	std::string acc_email() const {return this->email;}
	std::string acc_nombre_archivo() const {return this->nombre_archivo;}
	unsigned long int acc_id_bd() const {return this->id_bd;}
	bool es_online() const {return this->id_bd;}
	bool es_anonimo() const {return this->anonimo;}
	static std::string generar_nombre_fichero_para_nombre(std::string);
	static Perfil crear_perfil_desde_datos(std::string, std::string, unsigned long int);

	
};
#endif
