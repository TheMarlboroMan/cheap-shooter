#ifndef CONTROLADOR_PERFILES_H
#define CONTROLADOR_PERFILES_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring> //Memset...
#include <vector>

#include "../solicitud_servidor_perfil/solicitud_servidor_perfil.h"
#include "../../motor_sdl/herramientas/hilo_sdl/hilo_sdl.h"
#include "../../motor_sdl/herramientas/controlador_fps_sdl/controlador_fps_sdl.h"
#include "../perfil/perfil.h"
#include "../estructura_datos_perfil/estructura_datos_perfil.h"
#include "../controlador_retos/controlador_retos.h" //Es horrible que esto esté aquí, pero es lo más cómodo.

class Controlador_perfiles
{
	private:

	Perfil perfil;
	Perfil_datos * datos_perfil_juego; //OJO: Es propiedad del perfil, no del controlador.
	DLibH::Cronometro_SDL crono_perfil;

	static const unsigned short int INTERVALO_CRONO_PERFIL=10; 
	static const std::string ARCHIVO_PERFILES;

	//Esta estructura se rellenará leyendo un poco de cada archivo y se
	//usará como índice de perfiles.
 
	struct Entrada_indice_perfil_datos
	{
		std::string fichero;
		std::string nombre;
		bool online;
		unsigned int indice;

		Entrada_indice_perfil_datos(std::string p_fichero, std::string p_nombre, bool p_online, unsigned int p_indice)
			:fichero(p_fichero), nombre(p_nombre), online(p_online), indice(p_indice)
		{} 
	};

	typedef std::vector<Entrada_indice_perfil_datos> t_vector_datos_perfil;

	//Datos...
	t_vector_datos_perfil indice;
	bool indice_ok;
	std::string clave_perfil; //Almacena la clave del perfil...
	
	public:

	/*Esta estructurae usará para realizar la petición de comprobación
	de validez a la hora de crear un perfil en un hilo. Contiene todo lo
	necesario para funcionar.*/

	struct Estructura_comprobar_perfil
	{
		std::string nombre;
		std::string email;
		std::string clave;
		int id;
		Controlador_perfiles * cp;

		Estructura_comprobar_perfil()
			:nombre(""), email(""), clave(""), id(0), cp(NULL)
		{}

		void preparar(Controlador_perfiles * p_cp, std::string const& p_n, std::string const& p_e, std::string const& p_c, int p_id=0)
		{
			cp=p_cp;
			id=p_id;
			nombre=p_n;
			email=p_e;
			clave=p_c;
		}	
	};

	static const unsigned short int PERFILES_MOSTRADOS_POR_PAGINA=5;

	private:

	//Métodos...
	bool leer_perfil_en_indice(std::string);
	bool comprobar_y_crear_archivo_indice();
	void vaciar_indice();
	void leer_indice();
	void sanear_indice(std::vector<std::string>);

	void asignar_perfil(Perfil const&);
	int procesar_resultado_comprobacion_perfil(int resultado_hilo, Estructura_comprobar_perfil const& est);
	Perfil obtener_por_indice(unsigned int) throw (Perfil_excepcion); //const;
	bool existe_perfil(std::string) const;

	void inline activar_crono_perfiles() {this->crono_perfil.activar();}
	void inline desactivar_crono_perfiles() {this->crono_perfil.desactivar();}

	public:
	
	enum comprobaciones_perfil{
		CPERFIL_DATOS_INCOMPLETOS,
		CPERFIL_OFFLINE_YA_EXISTE,
		CPERFIL_ONLINE_YA_EXISTE,
		CPERFIL_ONLINE_YA_EXISTE_ERROR_PASS,
		CPERFIL_ONLINE_DATOS_INCORRECTOS,
		CPERFIL_ONLINE_ERROR,
		CPERFIL_OFFLINE_OK,
		CPERFIL_ONLINE_OK, 
		CPERFIL_NOMBRE_INCORRECTO,
		CPERFIL_ONLINE_EMAIL_INCORRECTO,
		CPERFIL_ONLINE_CLAVE_INCORRECTA,
		CPERFIL_ONLINE_ERROR_CREAR,
		CPERFIL_ONLINE_SUPERADO_LIMITE_POR_EMAIL};

	enum login_perfil{
		CLOGIN_NO_EXISTE=-2,
		CLOGIN_ERROR=-1,
		CLOGIN_NO_VALIDO=0};

	Controlador_perfiles();
	~Controlador_perfiles();

	std::vector<std::string> obtener_vector_indice();

	unsigned int obtener_total_perfiles() const {return this->indice.size();}
	void insertar_perfil(Perfil &);	
	bool eliminar_por_indice(unsigned int);
	bool es_indice_ok() const {return this->indice_ok;}
	void inline asignar_perfil_anonimo() {this->asignar_perfil(Perfil::generar_perfil_anonimo());}
	bool asignar_perfil_por_indice(unsigned int) throw (Perfil_excepcion); 
	static int hilo__comprobar_perfil(void *);
	static int hilo__login_perfil_online(void *);
	bool es_perfil_online() const {return this->perfil.es_online();}
	bool es_perfil_anonimo() const {return this->perfil.es_anonimo();}
	std::string obtener_nombre_perfil() const {return this->perfil.acc_nombre();}
	std::string obtener_email_perfil() const {return this->perfil.acc_email();}
	std::string obtener_clave_perfil() const {return this->clave_perfil;}
	unsigned long int obtener_id_perfil() const {return this->perfil.acc_id_bd();}
	static void preparar_estructura_para_login(Controlador_perfiles *, Estructura_comprobar_perfil &, std::string const&);

	void asignar_clave_perfil(std::string p_valor) {this->clave_perfil=p_valor;}

	void actualizar_perfil_jugador(Estructura_datos_perfil &) throw (Perfil_excepcion);
	void actualizar_perfil_jugador() throw (Perfil_excepcion);
	void actualizar_informacion_nivel(Estructura_datos_perfil_nivel&, unsigned int, bool);
	Perfil_datos::Info_nivel obtener_datos_nivel(unsigned int p_nivel) throw() {return datos_perfil_juego->obtener_datos_nivel(p_nivel);}
	void inline iniciar_partida_con_perfil() {this->datos_perfil_juego->iniciar_partida();}
	void inline perder_partida_con_perfil()	{this->datos_perfil_juego->perder_partida();}
	void inline ganar_partida_con_perfil() {this->datos_perfil_juego->ganar_partida();}
	void inline abortar_partida_con_perfil() {this->datos_perfil_juego->abortar_partida();}
	void procesar_crono_perfiles(Estructura_datos_perfil &, Controlador_retos &);
	void inline inicializar_crono_perfiles()
	{
		this->crono_perfil.establecer_limite(INTERVALO_CRONO_PERFIL);
		this->desactivar_crono_perfiles(); //Se inicia desconectado. Se conectará cuando haya un perfil disponible.
	}

	bool es_finalizado_todos_niveles_secretos() const {return this->datos_perfil_juego->es_finalizado_todos_niveles_secretos();}
	bool es_reto_conseguido(unsigned int p_indice) const {return this->datos_perfil_juego->es_reto_conseguido(p_indice);}
	bool es_mostrar_tutoriales() const {return this->datos_perfil_juego->es_mostrar_tutoriales();}
	void disparar_reto(unsigned int, Controlador_retos &);
	void comprobar_retos(Controlador_retos &);

	void desactivar_tutoriales_en_perfil() {this->datos_perfil_juego->desactivar_tutoriales();}
	void activar_tutoriales_en_perfil() {this->datos_perfil_juego->activar_tutoriales();}
	void intercambiar_tutoriales_en_perfil() 
	{
		if(es_mostrar_tutoriales()) desactivar_tutoriales_en_perfil();
		else activar_tutoriales_en_perfil();
	}

	unsigned short int acc_cuenta_niveles_finalizados() {return datos_perfil_juego->acc_cuenta_niveles_finalizados();}
	void refrescar_cuenta_niveles_finalizados() {datos_perfil_juego->refrescar_cuenta_niveles_finalizados();}
	int guardar_datos_perfil_en_servidor();
	int cargar_datos_perfil_de_servidor(const std::string &, const std::string &, const std::string &, unsigned long int);

	DLibH::Cronometro_SDL &obtener_crono_perfil() {return this->crono_perfil;}
};

#endif
