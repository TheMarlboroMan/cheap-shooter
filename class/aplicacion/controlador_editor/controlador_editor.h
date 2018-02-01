#ifndef CONTROLADOR_EDITOR_H
#define CONTROLADOR_EDITOR_H

#include <memory>
#include <algorithm>
#include <map>
#include "../../motor_sdl/libDanSDL.h"
#include "../factorias/factoria_actores_mapa.h"	//Aquí estarían incluidos todos los actores de mapa.
#include "../recursos/recursos.h"
#include "../actor_serializado/actor_serializado.h"
#include "../accion_trigger/accion_trigger.h"
#include "../definiciones/definiciones.h"
#include "../estructura_info_nivel/estructura_info_nivel.h"

class Controlador_editor
{
	private:

	enum filtros{
		F_FILTRO_FONDO=1,
		F_FILTRO_NORMAL=2,
		F_FILTRO_FRENTE=4};

	enum estados{
		E_EDITAR_NIVEL=0, 
		E_EDITAR_PROPIEDADES, 
		E_EDITAR_TABLA_OBSTACULOS, 
		E_EDITAR_TABLA_BONUS, 
		E_EDITAR_TABLA_BONUS_CAJA, 
		E_EDITAR_TABLA_BONUS_OBSTACULO,
		E_EDITAR_PARAMETROS,
		E_EDITAR_TAG,
		E_EDITAR_ACCIONES,
		E_LISTADO,
		E_INFO,
			E_ESTADO_MAX};

	enum propiedades{
		P_NIVEL_VECTOR_Y=0, 
		P_NIVEL_FLAGS_ARTEFACTOS_CANON,
		P_NIVEL_FLAGS_ARTEFACTOS_TELETRANSPORTE,
		P_NIVEL_FLAGS_ARTEFACTOS_EDU,
		P_NIVEL_FLAGS_ARTEFACTOS_ESCUDO,
		P_NIVEL_MAX_OBSTACULOS,
		P_NIVEL_MAX_BONUS,
		P_NIVEL_GEMAS_EN_NIVEL,
		P_NIVEL_GEMAS_MAX_NIVEL,
		P_PROXIMO_NIVEL,
		P_PROXIMO_NIVEL_SECRETO,
		P_INDICE_MUSICA,
			P_MAX_PROPIEDADES};

	struct Estructura_parametros
	{
		std::string param1;
		std::string param2;
		std::string param3;
		std::string param4;
		std::string param5;
		std::string param6;
		std::string param7;
		std::string param8;

		Estructura_parametros(const char * p1, const char * p2="N/A", const char * p3="N/A",
			const char * p4="N/A" , const char * p5="N/A", const char * p6="N/A",
			const char * p7="N/A", const char * p8="N/A"):
			param1(p1), param2(p2), param3(p3),
			param4(p4), param5(p5), param6(p6), 
			param7(p7), param8(p8)
		{}

		Estructura_parametros():
			param1(""), param2(""), param3(""),
			param4(""), param5(""), param6(""), 
			param7(""), param8("")
		{}

		Estructura_parametros& operator=(const Estructura_parametros & otra)
		{
			this->param1=otra.param1;
			this->param2=otra.param2;
			this->param3=otra.param3;
			this->param4=otra.param4;
			this->param5=otra.param5;
			this->param6=otra.param6;
			this->param7=otra.param7;
			this->param8=otra.param8;

			return *this;
		}

	};

	static const int TOTAL_PARAMS=8;

	std::vector<Accion_trigger> acciones;
	std::vector<std::string> cadenas_propiedades;
	std::map<unsigned int, Estructura_parametros> mapa_parametros;
	
	struct Tipo_actor
	{
		std::string nombre;
		unsigned int tipo;

		Tipo_actor(std::string p_nombre, unsigned int p_tipo):nombre(p_nombre), tipo(p_tipo) {}
	};

	struct Grupo_tipo
	{
		std::string nombre;
		std::vector<Tipo_actor> TIPOS;

		Grupo_tipo(std::string p_nombre, std::vector<Tipo_actor> p_vector):nombre(p_nombre), TIPOS(p_vector) {}
	};

	struct Item_tabla_probabilidad
	{
		std::string nombre;
		unsigned int tipo;
		unsigned short int probabilidad;

		Item_tabla_probabilidad(std::string p_nombre, unsigned int p_tipo, unsigned short int p_prob)
			:nombre(p_nombre), tipo(p_tipo), probabilidad(p_prob) {}
	};

	//Usaremos esta estructura para conectar los actores con 
	//parámetros sin tener que tocar la clase. Será un poco desastroso,
	//pero servirá.

	struct Actor_parametrizado	
	{
		std::shared_ptr<Actor_mapa> actor;
		Parametros_actor parametros;
		Parametros_actor_flexible params_rep;
		Parametros_actor_redimensionable params_redim;
		Parametros_actor_ordenable_profundidad params_profundidad;

		Actor_parametrizado():actor(NULL), parametros(), params_rep(), params_profundidad() {}

		~Actor_parametrizado() {}

		Actor_parametrizado& operator=(const Actor_parametrizado& otro)
		{
			this->actor=otro.actor;
			this->parametros=otro.parametros;
			this->params_rep=otro.params_rep;
			this->params_redim=otro.params_redim;
			this->params_profundidad=otro.params_profundidad;
			return *this;
		}
	};

	struct Comparador_actor_parametrizado
	{
		bool operator()(const Actor_parametrizado * const a, const Actor_parametrizado * const b) const
		{ 
			bool resultado=false;
		
			if(a->actor->acc_y() < b->actor->acc_y()) resultado=false;
			else if(a->actor->acc_y() > b->actor->acc_y()) resultado=true;
			else 
			{
				if(a->actor->acc_x() < b->actor->acc_x()) resultado=false;
				else if(a->actor->acc_x() > b->actor->acc_x()) resultado=true;
				else 
				{
					return a->actor->tipo_actor_mapa() > b->actor->tipo_actor_mapa();
				}
			}
	
			return resultado;
		}
	};

	struct Comparador_igualdad_actor_parametrizado
	{
		bool operator()(const Actor_parametrizado * const a, const Actor_parametrizado * const b) const
		{ 
			return a->actor->tipo_actor_mapa()==b->actor->tipo_actor_mapa()
				&& a->actor->acc_y() == b->actor->acc_y()
				&& a->actor->acc_x() == b->actor->acc_x()
				&& a->actor->acc_w() == b->actor->acc_w()
				&& a->actor->acc_h() == b->actor->acc_h();
		}
	};

	struct Recurso_seleccionable
	{
		unsigned int indice;
		std::string nombre;

		Recurso_seleccionable(unsigned int a, const std::string& b):indice(a), nombre(b)
		{}
	};

	typedef Recurso_seleccionable T_RECURSO;
	typedef Actor_parametrizado T_TIPO_ACTOR;
	typedef uint32_t U_ENTERO_32;
	typedef uint16_t U_ENTERO_16;
	typedef uint8_t U_ENTERO_8;

	struct Ordenador_actores_profundidad_editor
	{
		bool operator()(T_TIPO_ACTOR * a, T_TIPO_ACTOR  * b)
		{
			return a->actor->acc_profundidad_representacion() < b->actor->acc_profundidad_representacion();
		}
	};

	struct Caja_seleccion_multiple
	{
		enum estados {
			E_INTRODUCIR_PUNTO_1,
			E_INTRODUCIR_PUNTO_2
		};

		struct Punto
		{
			int x, y;
			Punto():x(0), y(0){}
			Punto(int px, int py):x(px), y(py){}
		
			void introducir(int px, int py)
			{
				x=px;
				y=py;
			}
		};

		unsigned int estado;
		Punto punto1;
		Punto punto2;
		SDL_Rect caja;

		void reiniciar()
		{
			estado=E_INTRODUCIR_PUNTO_1;
			punto1.x=0;
			punto1.y=0;
			punto2.x=0;
			punto2.y=0;
			caja.x=0;
			caja.y=0;
			caja.w=0;
			caja.h=0;
		}

		void introducir_punto1(int px, int py)
		{
			punto1.introducir(px, py);
			estado=E_INTRODUCIR_PUNTO_2;
		}
		
		void introducir_punto2(int px, int py) {punto2.introducir(px, py);}

		void actualizar_caja()
		{
			estado=E_INTRODUCIR_PUNTO_1;

			caja.x=punto1.x < punto2.x ? punto1.x : punto2.x;
			caja.w=punto1.x < punto2.x ? punto2.x-punto1.x : punto1.x-punto2.x;
			caja.y=punto1.y < punto2.y ? punto1.y : punto2.y;
			caja.h=punto1.y < punto2.y ? punto2.y-punto1.y : punto1.y-punto2.y;
		}

		Caja_seleccion_multiple() {reiniciar();}
	};

	Caja_seleccion_multiple CAJA_SELECCION_MULTIPLE;
	std::string nombre_fichero;
	DLibV::Pantalla& pantalla;
	DLibI::Controles_SDL& controles_sdl;
	DLibI::Escritor_texto escritor;
	DLibV::Camara camara;
	DLibV::Representacion_texto_fija_estatica * rep_txt_info;
	DLibV::Representacion_texto_fija_estatica * rep_txt_editar_propiedades;
	DLibV::Representacion_texto_fija_estatica * rep_txt_editar_tablas;
	DLibV::Representacion_texto_fija_estatica * rep_txt_parametros;
	DLibV::Representacion_texto_fija_estatica * rep_txt_tag;
	DLibV::Representacion_texto_fija_estatica * rep_txt_acciones;
	DLibV::Representacion_texto_fija_estatica * rep_txt_listado;
	DLibV::Representacion_texto_auto_estatica * rep_txt_ayuda;

	Info_base_nivel INFO_NIVEL; //Esto puede encontrarse en estructura_info_nivel.h

	long int posicion_y;
	int tipo_actor_actual;
	int grupo_tipo_actual;
	bool bloquear_a_rejilla;
	bool mostrar_rejilla;
	bool mostrar_info;
	unsigned int filtro_profundidad_actores;
	bool mostrar_ocultos;
	unsigned int indice_actual_listado;

	enum tipos_modificar_actor{
	T_NORMAL,
	T_REPRESENTACION_FLEXIBLE,
	T_TAMANO
	};

	std::string mensaje;
	std::string cadena_parametros;
	std::string cadena_tag;
	std::string cadena_acciones;
	unsigned int estado_actual;
	unsigned int tipo_modificar_actor;
	short int editando_propiedad;
	unsigned int indice_editando_tabla;
	short int indice_parametro;
	unsigned int indice_accion;
	unsigned int indice_parametro_accion;
	unsigned int maximo_tag;

	static unsigned int REJILLA_X;
	static unsigned int REJILLA_Y;
	static const unsigned int MIN_REJILLA=4;
	static const unsigned int MAX_REJILLA=256;
	static const unsigned int REJILLA_R=32;
	static const unsigned int REJILLA_G=32;
	static const unsigned int REJILLA_B=32;
	static const long int LIMITE_CAMARA_INFERIOR=-600;
	static const long int LIMITE_CAMARA_SUPERIOR=-2147483647;

	std::vector<T_TIPO_ACTOR *> actores_mapa;
	std::vector<T_TIPO_ACTOR *> seleccion_multiple;
	T_TIPO_ACTOR * seleccion_actual;
	std::vector<T_RECURSO> recursos;
	std::vector<Grupo_tipo> TIPOS;
	std::vector<Item_tabla_probabilidad> TABLA_OBSTACULOS;
	std::vector<Item_tabla_probabilidad> TABLA_BONUS;
	std::vector<Item_tabla_probabilidad> TABLA_DESTRUIR_CAJA;
	std::vector<Item_tabla_probabilidad> TABLA_DESTRUIR_OBSTACULO;

	std::string traducir_tipo_actor(unsigned int);
	void generar_texto_info();
	void eliminar_actor_seleccionado();
	void insertar_actor_en_posicion(int, int, int, unsigned int, const Parametros_actor&, const Parametros_actor_flexible&, const Parametros_actor_redimensionable&, const Parametros_actor_ordenable_profundidad&, bool=false, bool=false);
	void procesar_click_raton();
	void representar();
	void representar_modo_editar_nivel();
	void representar_modo_editar_propiedades();
	void representar_modo_editar_tablas(unsigned int);
	void representar_modo_editar_parametros();
	void representar_modo_editar_tag();
	void representar_modo_editar_acciones();
	void representar_modo_listado();
	void representar_modo_info();
	void aux_representar_modo_editar_tablas(std::vector<Item_tabla_probabilidad> const &, std::string &, unsigned int=9999);
	void cambiar_dato_tabla(std::vector<Item_tabla_probabilidad> &, unsigned int, int);
	void dibujar_rejilla();
	void desplazar_camara(int);
	void escoger_tipo(unsigned int);
	void escoger_grupo(unsigned int);
	void modificar_rejilla(int);
	void modificar_vector(int);
	void modificar_max_obstaculos(int);
	void modificar_max_bonus(int);
	void modificar_gemas_en_nivel(int);
	void modificar_gemas_max_nivel(int);
	void modificar_artefactos(unsigned short int, bool);
	void actualizar_maximo_tag();
	bool guardar();
	void copiar_archivo();
	bool cargar();
	void reiniciar();
	void deserializar_actor_mapa(std::ifstream &);
	void deserializar_mapa_items(std::vector<Item_tabla_probabilidad> &, std::ifstream &);
	void aux_deserializar_mapa_items(std::vector<Item_tabla_probabilidad> &, unsigned int, unsigned short int);
	void desplazar_seleccion_actual(float, float);
	void modificar_profundidad_seleccion_actual(int);
	void seleccionar_actual(int);
	void seleccionar_indice(int);
	Actor_serializado serializar_actor_mapa(T_TIPO_ACTOR *);
	void serializar_mapa_items(std::vector<Item_tabla_probabilidad> const &, std::ofstream &);
	void vaciar_actores_mapa();
	void controles_general();
	void controles_modo_editar_nivel();
	void controles_modo_editar_nivel_normal();
	void controles_modo_editar_nivel_representacion_flexible();
	void controles_modo_editar_nivel_tamano();
	void controles_modo_editar_propiedades();
	void controles_modo_editar_tablas(unsigned int);
	void controles_modo_editar_parametros();
	void controles_modo_editar_tag();
	void controles_modo_editar_acciones();
	void controles_modo_listado();
	void controles_modo_info();
	void cambiar_estado(unsigned int);
	void cambiar_propiedad(short int);
	void insertar_nueva_accion();
	void eliminar_accion_actual();
	void cambiar_accion_actual(short int);
	void vaciar_acciones();
	void actualizar_representacion_flexible(short int, short int, short int, short int, short int, short int, short int);
	void actualizar_representacion_seleccion_actual(int, int, int, int, int, int, int=-1);
	void actualizar_tamano(short int, short int);
	void actualizar_tamano_actual(int, int);
	void actualizar_filtro_profundidad(unsigned int);
	void duplicar_actor_actual();
	unsigned int indice_para_patron(unsigned int);
	std::string traducir_recurso(unsigned int);
	bool existe_en_seleccion_multiple(T_TIPO_ACTOR *);
	void insertar_en_seleccion_multiple(T_TIPO_ACTOR *);
	void sacar_de_seleccion_multiple(T_TIPO_ACTOR *);
	void vaciar_seleccion_multiple();
	void seleccionar_actores_en_caja(const SDL_Rect&);
	void guardar_archivo_listado_actores();
	void eliminar_duplicados();
	void duplicar_seleccion_multiple_obstaculo_flexible_en_decoracion(unsigned int);
	bool actor_supera_filtro_profundidad(T_TIPO_ACTOR * ini);
	void exportar_imagen();

	T_TIPO_ACTOR * obtener_actor_en_posicion(int, int);

	public:

	bool inicializar();
	void desmontar();
	bool loop_principal();

	Controlador_editor(std::string, DLibV::Pantalla&, DLibI::Controles_SDL&);
	~Controlador_editor();

	struct Ordenador_T_TIPO_ACTOR
	{
		bool operator()(T_TIPO_ACTOR * i, T_TIPO_ACTOR * j) const
		{
			return i->actor->acc_profundidad_representacion() < j->actor->acc_profundidad_representacion();
		}
	};
	
};


#endif
