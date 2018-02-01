#include "solicitud_servidor_perfil.h"

const std::string Solicitud_servidor_perfil::URL_COMPROBACIONES="http://www.caballorenoir.net/servicios/perfiles.php?&modo=";
const std::string Solicitud_servidor_perfil::MODO_EXISTE_PERFIL="t0";
const std::string Solicitud_servidor_perfil::MODO_OBTENER_IDENTIFICADOR_PERFIL="t1";
const std::string Solicitud_servidor_perfil::MODO_CREAR_PERFIL="t2";
const std::string Solicitud_servidor_perfil::MODO_LOGIN_PERFIL="t3";
const std::string Solicitud_servidor_perfil::MODO_VALIDAR_PERFIL="t4";
const std::string Solicitud_servidor_perfil::MODO_SUBIR_RETOS="t5";
const std::string Solicitud_servidor_perfil::MODO_BAJAR_RETOS="t6";

const std::string Solicitud_servidor_perfil::CAMPO_LOGIN="login";
const std::string Solicitud_servidor_perfil::CAMPO_EMAIL="email";
const std::string Solicitud_servidor_perfil::CAMPO_PASS="pass";
const std::string Solicitud_servidor_perfil::CAMPO_ID="id";
const std::string Solicitud_servidor_perfil::CAMPO_ID_ORIGEN="id_juego_origen";
const std::string Solicitud_servidor_perfil::CAMPO_RETO_01="reto[";
const std::string Solicitud_servidor_perfil::CAMPO_RETO_02="]";

Respuesta_servidor Solicitud_servidor_perfil::leer_resultado(std::string const& p_respuesta, std::string const &p_tipo)
	throw (Excepcion_solicitud_perfil)
{
	Respuesta_servidor resultado;

	//Creamos un stream con la respuesta y una cadena para contener el resultado...
	std::istringstream stream(p_respuesta);

	std::string linea;
	
	//Leer el tipo.
	std::getline(stream, resultado.tipo);

	//Leer resultado.
	std::getline(stream, linea);
	resultado.resultado=DLibH::Herramientas::cadena_a_entero(linea.c_str());

	//Leer código.
	std::getline(stream, linea);
	resultado.codigo=DLibH::Herramientas::cadena_a_entero(linea.c_str());

	//Leer cadena...
	while(std::getline(stream, linea))
	{
		resultado.texto+=linea+"\n";
	}

	if(resultado.tipo!=p_tipo)
	{
		throw Excepcion_solicitud_perfil(Excepcion_solicitud_perfil::ERROR_TIPOS, "Ha ocurrido un error al solicitar los datos: los tipos no se corresponden");
	}
	else
	{
		return resultado;
	}
}

bool Solicitud_servidor_perfil::existe_perfil(std::string const& p_login, std::string const& p_email)
	throw(Excepcion_solicitud_perfil)
{
	Solicitud_CURL C;
	C.inicializar(construir_url(MODO_EXISTE_PERFIL));
	C.post(CAMPO_LOGIN, p_login);
	C.post(CAMPO_EMAIL, p_email);

	if(!C.lanzar())
	{
		throw Excepcion_solicitud_perfil(Excepcion_solicitud_perfil::ERROR_SOLICITUD, "Ha ocurrido un error al solicitar los datos");
	}
	else
	{
		Respuesta_servidor R=leer_resultado(C.acc_respuesta(), MODO_EXISTE_PERFIL);
		return (bool) R.resultado;
	}
}

unsigned long int Solicitud_servidor_perfil::obtener_identificador_perfil(
	std::string const& p_login, std::string const& p_email, 
	std::string const& p_pass) 
	throw (Excepcion_solicitud_perfil)
{
	Solicitud_CURL C;
	C.inicializar(construir_url(MODO_OBTENER_IDENTIFICADOR_PERFIL));
	C.post(CAMPO_LOGIN, p_login);
	C.post(CAMPO_EMAIL, p_email);
	C.post(CAMPO_PASS, p_pass);

	if(!C.lanzar())
	{
		throw Excepcion_solicitud_perfil(Excepcion_solicitud_perfil::ERROR_SOLICITUD, "Ha ocurrido un error al solicitar los datos");
	}
	else
	{
		Respuesta_servidor R=leer_resultado(C.acc_respuesta(), MODO_OBTENER_IDENTIFICADOR_PERFIL);
		if(!R.resultado) return 0;
		else return R.codigo;
	}
}

/*La solicitud devolverá:

Resultado: 1
	Código: id
Resultado: o
	Código 0: error al crear.
	Código -1 has superado el límite de perfiles.
	Código -2 el perfil ya existe.

*/

long int Solicitud_servidor_perfil::crear_perfil(
	std::string const& p_login, std::string const& p_email, 
	std::string const& p_pass)
	throw (Excepcion_solicitud_perfil)
{
	Solicitud_CURL C;
	C.inicializar(construir_url(MODO_CREAR_PERFIL));
	C.post(CAMPO_LOGIN, p_login);
	C.post(CAMPO_EMAIL, p_email);
	C.post(CAMPO_PASS, p_pass);
	C.post(CAMPO_ID_ORIGEN, ID_JUEGO);

	if(!C.lanzar())
	{
		throw Excepcion_solicitud_perfil(Excepcion_solicitud_perfil::ERROR_SOLICITUD, "Ha ocurrido un error al solicitar los datos");
	}
	else
	{
		Respuesta_servidor R=leer_resultado(C.acc_respuesta(), MODO_CREAR_PERFIL);
//		if(R.resultado) return 0;
//		else return R.codigo;
		return R.codigo;
	}
}

bool Solicitud_servidor_perfil::login_perfil(
	std::string const& p_login, std::string const& p_email, 
	std::string const& p_pass, unsigned long int p_id) 
	throw (Excepcion_solicitud_perfil)
{
	Solicitud_CURL C;
	C.inicializar(construir_url(MODO_LOGIN_PERFIL));
	C.post(CAMPO_LOGIN, p_login);
	C.post(CAMPO_EMAIL, p_email);
	C.post(CAMPO_PASS, p_pass);
	C.post(CAMPO_ID, p_id);

	if(!C.lanzar())
	{
		throw Excepcion_solicitud_perfil(Excepcion_solicitud_perfil::ERROR_SOLICITUD, "Ha ocurrido un error al solicitar los datos");
	}
	else
	{
		Respuesta_servidor R=leer_resultado(C.acc_respuesta(), MODO_LOGIN_PERFIL);
		if(!R.resultado) return false;
		else return true;
	}
}

bool Solicitud_servidor_perfil::validar_perfil(
	std::string const& p_login, std::string const& p_email, 
	std::string const& p_pass, unsigned long int p_id) 
	throw (Excepcion_solicitud_perfil)
{
	Solicitud_CURL C;
	C.inicializar(construir_url(MODO_VALIDAR_PERFIL));
	C.post(CAMPO_LOGIN, p_login);
	C.post(CAMPO_EMAIL, p_email);
	C.post(CAMPO_PASS, p_pass);
	C.post(CAMPO_ID, p_id);

	if(!C.lanzar())
	{
		throw Excepcion_solicitud_perfil(Excepcion_solicitud_perfil::ERROR_SOLICITUD, "Ha ocurrido un error al solicitar los datos");
	}
	else
	{
		Respuesta_servidor R=leer_resultado(C.acc_respuesta(), MODO_VALIDAR_PERFIL);
		if(!R.resultado) return false;
		else return true;
	}
}

std::string Solicitud_servidor_perfil::construir_url(std::string const& p_modo)
{
	std::string resultado=URL_COMPROBACIONES;
	resultado+=p_modo;
	return resultado;
}

bool Solicitud_servidor_perfil::guardar_datos_retos(
	std::string const& p_login, std::string const& p_email, 
	std::string const& p_pass, unsigned long int p_id,
	const Estructura_intercambio_retos & est)
	throw (Excepcion_solicitud_perfil)
{
	Solicitud_CURL C;
	C.inicializar(construir_url(MODO_SUBIR_RETOS));

	C.post(CAMPO_ID_ORIGEN, ID_JUEGO);
	C.post(CAMPO_LOGIN, p_login);
	C.post(CAMPO_EMAIL, p_email);
	C.post(CAMPO_PASS, p_pass);
	C.post(CAMPO_ID, p_id);

	std::string temp="";

	unsigned int total=est.datos.size();
	unsigned int i=0;

	while(i<total)
	{
		temp=CAMPO_RETO_01+DLibH::Herramientas::entero_a_cadena(Definicion_retos::reto_a_id(i))+CAMPO_RETO_02;
		C.post(temp, (int) est.datos[i]);
		i++;
	}

	if(!C.lanzar())
	{
		throw Excepcion_solicitud_perfil(Excepcion_solicitud_perfil::ERROR_SOLICITUD, "Ha ocurrido un error al solicitar los datos");
	}
	else
	{
		Respuesta_servidor R=leer_resultado(C.acc_respuesta(), MODO_SUBIR_RETOS);
		if(!R.resultado) return false;
		else return true;
	}


	return true;
}

Estructura_intercambio_retos Solicitud_servidor_perfil::obtener_datos_retos(
	std::string const& p_login, std::string const& p_email, 
	std::string const& p_pass, unsigned long int p_id)
	throw (Excepcion_solicitud_perfil)
{
	Estructura_intercambio_retos est;

	Solicitud_CURL C;
	C.inicializar(construir_url(MODO_BAJAR_RETOS));

	C.post(CAMPO_ID_ORIGEN, ID_JUEGO);
	C.post(CAMPO_LOGIN, p_login);
	C.post(CAMPO_EMAIL, p_email);
	C.post(CAMPO_PASS, p_pass);
	C.post(CAMPO_ID, p_id);

	if(!C.lanzar())
	{
		throw Excepcion_solicitud_perfil(Excepcion_solicitud_perfil::ERROR_SOLICITUD, "Ha ocurrido un error al solicitar los datos");
	}
	else
	{
		Respuesta_servidor R=leer_resultado(C.acc_respuesta(), MODO_BAJAR_RETOS);
		if(!R.resultado) 
		{
			est.carga_correcta=false;
		}
		else 
		{
			est.carga_correcta=true;
			est.cargar_desde_cadena(R.texto);
		}
	}

	return est;
}
