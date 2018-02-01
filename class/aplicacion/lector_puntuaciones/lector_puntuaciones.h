#ifndef LECTOR_PUNTUACIONES_H
#define LECTOR_PUNTUACIONES_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <cmath>
//#include <curl/curl.h>
#include "../../motor_sdl/herramientas/herramientas/herramientas.h"
#include "../../herramientas/solicitud_curl/solicitud_curl.h"

class Lector_puntuaciones
{
	typedef uint16_t U_ENTERO_16;
	typedef uint8_t U_ENTERO_8;

	public:

	class struct_puntuacion
	{
		public:

		unsigned int id;
		unsigned int posicion;	//La posición no se graba de ninguna parte, simplemente es algo que leemos y asignamos con el controlador.	
		unsigned int puntuacion;
		unsigned short int nivel;
		std::string nombre;
		bool finalizado;

		struct_puntuacion()
			:id(0), posicion(0), puntuacion(0), nivel(0), nombre(""), finalizado(false)
		{}

		~struct_puntuacion()
		{

		}

		struct_puntuacion(const struct_puntuacion& otro)
		{
			this->id=otro.id;
			this->posicion=otro.posicion;
			this->nivel=otro.nivel;
			this->puntuacion=otro.puntuacion;
			this->finalizado=otro.finalizado;
			this->nombre.assign(otro.nombre);		
		}

		struct_puntuacion& operator=(const struct_puntuacion&otro)
		{
			this->id=otro.id;
			this->posicion=otro.posicion;
			this->nivel=otro.nivel;
			this->puntuacion=otro.puntuacion;
			this->finalizado=otro.finalizado;
			this->nombre.assign(otro.nombre);
			return *this;	
		}

		/*Esta es para rellenar con los datos que pasamos*/

		void rellenar(unsigned int p, unsigned short int niv, char const * n, unsigned int pos, bool finalizado, unsigned int p_id=0)
		{
			this->id=p_id;
			this->puntuacion=p;
			this->nivel=niv;
			this->nombre.assign(n);
			this->posicion=pos;
			this->finalizado=finalizado;
		}
	
		/*Esta es para rellenar desde lo que recibimos del servidor.*/

		void rellenar_desde_servidor(std::string const &cadena)
		{
			/*La cadena tiene este aspecto: 1|1900|0|Mr. Pani, donde es "posicion|puntos|nivel|nombre";
			serán pues cuatro trozos de los cuales el primero es la "posición".*/

			std::istringstream stream(cadena);
			std::string trozo;

			std::getline(stream, trozo, Lector_puntuaciones::SEPARADOR_CAMPO_SOLICITAR_PUNTUACIONES);
			this->posicion=DLibH::Herramientas::cadena_a_entero(trozo.c_str());

			std::getline(stream, trozo, Lector_puntuaciones::SEPARADOR_CAMPO_SOLICITAR_PUNTUACIONES);
			this->puntuacion=DLibH::Herramientas::cadena_a_entero(trozo.c_str());

			std::getline(stream, trozo, Lector_puntuaciones::SEPARADOR_CAMPO_SOLICITAR_PUNTUACIONES);
			this->nivel=DLibH::Herramientas::cadena_a_entero(trozo.c_str());

			std::getline(stream, trozo, Lector_puntuaciones::SEPARADOR_CAMPO_SOLICITAR_PUNTUACIONES);
			this->finalizado=(bool) DLibH::Herramientas::cadena_a_entero(trozo.c_str());

			std::getline(stream, trozo, Lector_puntuaciones::SEPARADOR_CAMPO_SOLICITAR_PUNTUACIONES);
			this->nombre.assign(trozo);
		}

	};

	private:

	static const char URL_SOLICITAR_POSICION_POR_PUNTUACION[];
	static const char URL_SOLICITAR_PUNTUACIONES[];
	static const char SEPARADOR_CAMPO_SOLICITAR_PUNTUACIONES;
	static const char NULO;
	static const char URL_INGRESAR_PUNTUACION[];
	static const char RUTA_PUNTUACION_LOCAL[];
	static const char CURL_EXPECT[];
	static const char SOLICITUD_CAMPO_ID[];
	static const char SOLICITUD_CAMPO_NOMBRE[];
	static const char SOLICITUD_CAMPO_PUNTUACION[];
	static const char SOLICITUD_CAMPO_NIVEL[];
	static const char SOLICITUD_CAMPO_FINALIZADO[];
	static const unsigned short int MAX_LONGITUD_PUNTUACION_LOCAL=31;
	const static unsigned int MAX_CANTIDAD_MOSTRAR=10;
	std::vector<struct_puntuacion> puntuaciones;
	std::vector<struct_puntuacion> puntuaciones_online;


	//Locales...
	void guardar();

	//Globales...
	bool guardar_servidor(struct_puntuacion const&);
//	static size_t solicitud_servidor_guardar_callback(void *, size_t, size_t, void *);
//	static size_t solicitud_servidor_leer_escribe_en_cadena(void *, size_t, size_t, void *);

	bool conectado;

	public:

	Lector_puntuaciones();
	~Lector_puntuaciones();
	
	bool es_conectado() {return this->conectado;}

	const std::vector<struct_puntuacion>& acc_puntuaciones() const {return this->puntuaciones;}
	const std::vector<struct_puntuacion>& acc_puntuaciones_online() const {return this->puntuaciones_online;}

	unsigned short int preguntar_nueva_puntuacion_local(unsigned int);
	void enviar_nueva_puntuacion(unsigned int, unsigned short int, std::string const&, bool, bool, unsigned int);
	unsigned long int solicitud_servidor_posicion_para_puntuacion(unsigned int);

	void iniciar();
	bool cargar_online(unsigned int, unsigned int&);
	void procesar_respuesta_cargar_online(std::string const&, unsigned int&);

	void cargar_local();
};

#endif
