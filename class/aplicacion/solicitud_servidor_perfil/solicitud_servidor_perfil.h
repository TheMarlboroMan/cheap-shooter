#ifndef SOLICITUD_SERVIDOR_PERFIL_H
#define SOLICITUD_SERVIDOR_PERFIL_H

#include <string>
#include <sstream>
#include <vector>
#include "../../herramientas/solicitud_curl/solicitud_curl.h"
#include "../../motor_sdl/herramientas/herramientas/herramientas.h"
#include "../definiciones/definicion_retos.h"

struct Excepcion_solicitud_perfil
{
	enum codigos
	{
		ERROR_SOLICITUD=0,
		ERROR_TIPOS
	};

	unsigned short int cod;
	std::string mensaje;

	Excepcion_solicitud_perfil(unsigned short int p_c, std::string p_m)
		:cod(p_c), mensaje(p_m)
	{}	
};

struct Respuesta_servidor
{
	std::string tipo;
	int resultado;
	int codigo;
	std::string texto;
};

class Solicitud_servidor_perfil
{
	private:
	
	Solicitud_servidor_perfil() {}

	static const unsigned short int ID_JUEGO=1;
	static const std::string URL_COMPROBACIONES;

	static const std::string MODO_EXISTE_PERFIL;
	static const std::string MODO_OBTENER_IDENTIFICADOR_PERFIL;
	static const std::string MODO_CREAR_PERFIL;
	static const std::string MODO_LOGIN_PERFIL;
	static const std::string MODO_VALIDAR_PERFIL;
	static const std::string MODO_SUBIR_RETOS;
	static const std::string MODO_BAJAR_RETOS;

	static const std::string CAMPO_LOGIN;
	static const std::string CAMPO_EMAIL;
	static const std::string CAMPO_PASS;
	static const std::string CAMPO_ID;
	static const std::string CAMPO_ID_ORIGEN;
	static const std::string CAMPO_RETO_01;
	static const std::string CAMPO_RETO_02;

	static Respuesta_servidor leer_resultado(std::string const&, std::string const&) throw (Excepcion_solicitud_perfil); 
	static std::string construir_url(std::string const&);

	public:

	//Métodos estáticos de solicitudes.
	static bool existe_perfil(std::string const&, std::string const&) throw (Excepcion_solicitud_perfil);
	static unsigned long int obtener_identificador_perfil(std::string const&, std::string const&, std::string const&) throw (Excepcion_solicitud_perfil);
	static long int crear_perfil(std::string const&, std::string const&, std::string const&) throw (Excepcion_solicitud_perfil);
	static bool login_perfil(std::string const&, std::string const&, std::string const&, unsigned long int) throw (Excepcion_solicitud_perfil);
	static bool validar_perfil(std::string const&, std::string const&, std::string const&, unsigned long int) throw (Excepcion_solicitud_perfil);
	static bool guardar_datos_retos(std::string const&, std::string const&, std::string const&, unsigned long int, const Estructura_intercambio_retos &) throw (Excepcion_solicitud_perfil);
	static Estructura_intercambio_retos obtener_datos_retos(std::string const&, std::string const&, std::string const&, unsigned long int) throw (Excepcion_solicitud_perfil);
};

#endif
