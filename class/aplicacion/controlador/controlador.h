#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <string>
#include <vector>
#include "../../motor_sdl/libDanSDL.h"
#include "../campo_estrellas/campo_estrellas.h"
#include "../lector_puntuaciones/lector_puntuaciones.h"
#include "../controlador_juego/controlador_juego.h"
#include "../controlador_editor/controlador_editor.h"
#include "../localizador/localizador.h"
#include "../configuracion/configuracion.h"
#include "../item_configuracion/item_configuracion.h"
#include "../recursos/recursos.h"
#include "../controlador_perfiles/controlador_perfiles.h"
#include "../reto/reto.h"
#include "../definiciones/definicion_retos.h"
#include "../controlador_retos/controlador_retos.h"
#include "../configuracion_controles/configuracion_controles.h"
#include "../estructura_animacion_menu/estructura_animacion_menu.h"
#include "../estructura_instrucciones/estructura_instrucciones.h"

class Controlador
{
	private:

	DLibV::Pantalla pantalla;
	DLibI::Controles_SDL controles_sdl;
	Lector_puntuaciones lector_puntuaciones;
	Campo_estrellas * campo_estrellas;
	DLibH::Controlador_fps_SDL control_frames;
	DLibV::Representacion_texto_fija_estatica * rep_fps;

	DLibA::Controlador_audio_SDL * controlador_audio;
	DLibA::Cola_sonido * cola_sonido;
	Localizador localizador;
	Configuracion configuracion;
	Controlador_perfiles controlador_perfiles;
	Controlador_retos controlador_retos;
	Configuracion_controles controles_teclado_raton;
	Configuracion_controles controles_joystick;

	std::string cadena_error;
	std::vector<Estructura_instrucciones> instrucciones;

	//Datos de configuracion...
	unsigned short int esquema_control;
	Uint32 flags_video;
//	bool conectar_online;
	short int volumen_audio;
	short int volumen_musica;
	unsigned short int idioma;

	bool montado;

	bool entrenando;		//Indica si estamos empezando desde "seleccionar nivel".
	unsigned short int nivel_inicio;	//Indica el nivel desde el que empezamos a jugar.

		////////////////////////////////////////////////////////////////
		//Configuracion del menú.

	static const int X_CAJA_MENU=266;
	static const int Y_CAJA_MENU=528;
	static const int W_CAJA_MENU=257;
	static const int H_CAJA_MENU=25;
	static const int VECTOR_X_MENU=400;
	static const int Y_ITEMS_MENU=534;
	int menu_memoria_seleccion;

	enum Items_menu{
		M_INICIAR, 
		M_CONTROLES,
		M_CONFIGURACION,
		M_TUTORIALES,
		M_AYUDA,
		M_SELECCION_NIVEL,
		M_RETOS,
			M_MAX_MENU};

		////////////////////////////////////////////////////////////////
		//Para los retos.

	static const int RETOS_POR_PAGINA=8;
	static const int X_RETOS_INICIO=47;
	static const int Y_RETOS_INICIO=278;
	static const int SALTO_X_RETOS=376;
	static const int SALTO_Y_RETOS=67;
	static const int X_CLIP_RETOS=38;
	static const int Y_CLIP_RETOS=270;
	static const int W_CLIP_RETOS=730;
	static const int H_CLIP_RETOS=282;
	static const int VECTOR_X_RETOS=1200;

	void mostrar_retos(std::vector<DLibV::Representacion *> &, std::vector<DLibV::Representacion *> &);
	void procesar_estado_retos(std::vector<DLibV::Representacion *> &);

		////////////////////////////////////////////////////////////////
		//Métodos varios.

	void componer_flags_video();
	void obtener_datos_de_configuracion();
	void aplicar_datos_de_configuracion();
//	bool puede_conectar();
	bool preparar_recursos_graficos();
//	void generar_recurso_alterando_color(unsigned int, unsigned int, Uint32, Uint32);
	void generar_recurso_alterando_color(unsigned int, unsigned int, std::vector<Uint32>, std::vector<Uint32>);
	void generar_recurso_alterando_canales(unsigned int, unsigned int, int, int, int);
	bool preparar_recursos_audio();
	void iniciar_y_procesar_juego();
	void desmontar();
	void mostrar_estados_generico(std::vector<DLibV::Representacion *>);

	//Una utilidad para generar vectores de colores rápidamente.
	class Pila_colores
	{
		private:

		std::vector<Uint32> colores;
		Pila_colores() {}

		public:

		static Pila_colores nueva() {return Pila_colores();}

		std::vector<Uint32> obtener() {return colores;}
		Pila_colores& color(Uint32 c) //Interfaz fluida, se podría hacer Pila_colores::nueva().color(c1).color(c2).color(c3).obtener();
		{
			colores.push_back(c);
			return *this;
		}
	};

		////////////////////////////////////////////////////////////////
		//Métodos y propiedades de las instrucciones...

	void preparar_instrucciones();
	void procesar_estado_instrucciones();
	void mostrar_animacion_salir_instrucciones(unsigned int);
	void mostrar_animacion_pasar_pagina_instrucciones(unsigned int, short int);

		////////////////////////////////////////////////////////////////
		//Métodos y propiedades de menú.

	unsigned int pagina_puntuaciones_online;
	unsigned int total_paginas_puntuaciones_online;

	void mostrar_animacion_titulo(DLibV::Representacion_bitmap &, DLibV::Representacion_bitmap &);
	void mostrar_animacion_abandonar_menu(DLibV::Representacion_bitmap &, DLibV::Representacion_bitmap &);
	void mostrar_menu(std::vector<DLibV::Representacion *> &, Grupo_animacion *, DLibV::Representacion *, bool);
	void mostrar_salida_menu(std::vector<DLibV::Representacion *> &, bool);
	void auxiliar_recargar_texto_puntuaciones(std::vector<Lector_puntuaciones::struct_puntuacion>, DLibV::Representacion_texto &, int, int);
	void recargar_texto_puntuaciones();
	void recargar_texto_puntuaciones_online();
	unsigned int pasar_pagina_puntuaciones_online(int);
	bool procesar_estado_menu();
//	void modificar_trofeo_mostrado_menu(unsigned int, DLibV::Representacion_compuesta_estatica&);
	static int hilo__recargar_puntuaciones_online(void *);

		////////////////////////////////////////////////////////////////
		//Métodos de selección de nivel...

	static const int X_SELECCION_NIVEL_INICIO=125;
	static const int Y_SELECCION_NIVEL_INICIO=336;
	static const int SALTO_X_SELECCION_NIVEL=800;
	static const int X_CLIP_SELECCION_NIVEL=38;
	static const int Y_CLIP_SELECCION_NIVEL=270;
	static const int W_CLIP_SELECCION_NIVEL=730;
	static const int H_CLIP_SELECCION_NIVEL=282;
	static const int VECTOR_X_SELECCION_NIVEL=1200;
	static const int VECTOR_Y_SELECCION_NIVEL=800;
	static const int W_REP_SELECCION_NIVEL=548;
	static const int H_REP_SELECCION_NIVEL=148;

	static const int X_TITULO_SEL_NIVEL=260;
	static const int Y_TITULO_SEL_NIVEL=15;
	static const int W_TITULO_SEL_NIVEL=800;
	static const int H_TITULO_SEL_NIVEL=133;

	static const int X_VALOR_SEL_NIVEL=440;
	static const int Y_VALOR_SEL_NIVEL=15;
	static const int W_VALOR_SEL_NIVEL=800;
	static const int H_VALOR_SEL_NIVEL=133;

	static const int X_IMAGEN_SEL_NIVEL=0;
	static const int Y_IMAGEN_SEL_NIVEL=0;
	static const int W_IMAGEN_SEL_NIVEL=216;
	static const int H_IMAGEN_SEL_NIVEL=148;

	class Estructura_seleccion_nivel
	{
		private:

		DLibV::Representacion_compuesta_estatica * representacion;
		unsigned int nivel;

		public:
		Estructura_seleccion_nivel(unsigned int, int);
		~Estructura_seleccion_nivel();

		unsigned int acc_nivel() const {return nivel;}
		void actualizar(Perfil_datos::Info_nivel&, Localizador&);
		DLibV::Representacion * acc_representacion() {return representacion;}
		Estructura_animacion_movimiento * generar_animacion(int); 
	};

	void procesar_estado_seleccion_nivel(std::vector<DLibV::Representacion *> &);
	void mostrar_seleccion_nivel(std::vector<DLibV::Representacion *> &, std::vector<Estructura_seleccion_nivel *> &);
	void mostrar_animacion_nivel_seleccionado(int, std::vector<DLibV::Representacion *> &, Estructura_seleccion_nivel *);

		////////////////////////////////////////////////////////////////
		//Métodos de configuracion...

	static const int X_CURSOR_CONFIG=0;
	static const int Y_CURSOR_CONFIG=366;
	static const int W_CURSOR_CONFIG=800;
	static const int H_CURSOR_CONFIG=18;
	static const int X_TXT_INFO_CONFIG=220;
	static const int Y_TXT_INFO_CONFIG=277; 
	static const int X_CLAVES_CONFIG=212;
	static const int X_VALORES_CONFIG=452;
	static const int Y_CONFIG=366;
	static const int X_CAJA_FONDO_CONFIG=0;
	static const int Y_CAJA_FONDO_CONFIG=258;
	static const int W_CAJA_FONDO_CONFIG=800;
	static const int H_CAJA_FONDO_CONFIG=302;

	void procesar_estado_configuracion(std::vector<DLibV::Representacion *> &);
	std::vector<Item_configuracion> preparar_vector_configuracion();

		////////////////////////////////////////////////////////////////
		//Métodos para la configuración de controles...

	static const int X_CURSOR_CONTROLES=0;
	static const int Y_CURSOR_CONTROLES=311;
	static const int W_CURSOR_CONTROLES=800;
	static const int H_CURSOR_CONTROLES=11;
	static const int X_COL_IZQ_CONTROLES=101;
	static const int Y_COL_IZQ_CONTROLES=290;
	static const int X_COL_DER_CONTROLES=401;
	static const int Y_COL_DER_CONTROLES=290;
	static const int INTERLINEADO_CONTROLES=2;
	static const int SALTO_Y_BLOQUE_CONTROLES=33;
	static const int SALTO_Y_CONTROLES=11;
	static const int X_CAJA_FONDO_CONTROLES=0; 
	static const int Y_CAJA_FONDO_CONTROLES=258;
	static const int W_CAJA_FONDO_CONTROLES=800;
	static const int H_CAJA_FONDO_CONTROLES=302; 

	void procesar_estado_configuracion_controles(std::vector<DLibV::Representacion *> &);	
	bool aprender_controles(unsigned int);
	std::string actualizar_datos_configuracion_controles();
	enum config_controles{	//Enumeración con el orden de los controles configurables.
	CC_0_PAUSA,
	CC_0_ARTEFACTO,
	CC_0_TIEMPO,
	CC_0_SIGUIENTE,
	CC_0_ANTERIOR,
	CC_0_ARRIBA,
	CC_0_DERECHA,
	CC_0_ABAJO,
	CC_0_IZQUIERDA,
	CC_1_PAUSA,
	CC_1_ARTEFACTO,
	CC_1_TIEMPO,
	CC_1_SIGUIENTE,
	CC_1_ANTERIOR,
	CC_1_ARRIBA,
	CC_1_DERECHA,
	CC_1_ABAJO,
	CC_1_IZQUIERDA,
		CC_MAX_CONTROLES
	};

		////////////////////////////////////////////////////////////////
		//Métodos de perfil...

	static const int X_CAJA_FONDO_PERFILES=0;
	static const int Y_CAJA_FONDO_PERFILES=307;
	static const int W_CAJA_FONDO_PERFILES=800;
	static const int H_CAJA_FONDO_PERFILES=202;

	static const int X_TXT_INFO_PERFILES=20;
	static const int Y_TXT_INFO_PERFILES=20;		

	static const int X_TXT_PASS_PERFILES=20;
	static const int Y_TXT_PASS_PERFILES=400;		

	static const int Y_OPCION_NUEVO_PERFIL=400;
	static const int Y_OPCION_INICIAR_SIN_PERFIL=440;

	static const int Y_PERFILES=362;
	static const int Y_FLECHA_PERFILES=360;
	static const int MARGEN_X_FLECHA_PERFILES=20;

	static const int X_CAJA_PERFILES=300;
	static const int Y_CAJA_PERFILES=360;
	static const int W_CAJA_PERFILES=200; 	//20 (máx caracteres+ márgen) * 8 (ancho glifo)
	static const int H_CAJA_PERFILES=30;

	static const int X_CURSOR_PERFILES=0;
	static const int Y_CURSOR_PERFILES=358;
	static const int W_CURSOR_PERFILES=800;
	static const int H_CURSOR_PERFILES=29;
	static const int SALTO_Y_CURSOR_PERFILES=38;

	static const int VECTOR_X_PERFIL=500;

	bool procesar_estado_seleccion_perfil();
	int obtener_representacion_perfiles(std::vector<DLibV::Representacion_texto *>&);
	int obtener_centro_texto_perfiles(DLibV::Representacion_texto *);
	void mostrar_estado_perfiles(std::vector<DLibV::Representacion* >&, std::vector<DLibV::Representacion_texto* >&);
	void procesar_estado_creacion_perfil();
	void procesar_estado_creacion_perfil_aux_actualizar_posicion(DLibI::Escritor_texto&, std::string&, std::string&, std::string&, int, int);
	std::string procesar_estado_creacion_perfil_aux_cadena_datos(DLibI::Escritor_texto&, std::string &, std::string &, std::string &, int);
	void procesar_estado_login_perfil();

		///////////////////////////////////////////////////////
		// Estados y atributos del motor.


	int estado;
	enum estados{
	E_MENU, 
	E_JUEGO, 
	E_EDITOR, 
	E_SELECCION_PERFIL, 
	E_CREACION_PERFIL, 
	E_VALIDAR_LOGIN, 
	E_INSTRUCCIONES};

	enum config{
	C_IDIOMA, 
	C_ESQUEMA_CONTROL, 
//	C_PUNTUACIONES_ONLINE, 
	C_VOLUMEN_AUDIO, 
	C_VOLUMEN_MUSICA, 
	C_MODO_VIDEO, 
	C_PANTALLA_COMPLETA};
	
	public:

	Controlador();
	~Controlador();

	bool inicializar();
	void loop_principal();
	void iniciar_y_procesar_editor(std::string);
	std::string acc_cadena_error() const {return this->cadena_error;}
};

#endif
