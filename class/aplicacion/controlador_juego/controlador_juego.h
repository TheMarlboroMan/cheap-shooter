#ifndef CONTROLADOR_JUEGO_H
#define CONTROLADOR_JUEGO_H	

#include <algorithm>
#include <map>
#include "../../motor_sdl/libDanSDL.h"
#include "../campo_estrellas/campo_estrellas.h"
#include "../mirilla/mirilla.h"
#include "../jugador/jugador.h"
#include "../escudo/escudo.h"
#include "../definiciones/definiciones.h"
#include "../fantasma/fantasma.h"
#include "../proyectil/proyectil.h"
#include "../controlador_puntuacion/controlador_puntuacion.h"

#include "../obstaculo_actividad/obstaculo_actividad.h"
#include "../visitante/visitante.h"

#include "../factorias/factoria_actores_mapa.h"	//Aquí estarían incluidos todos los actores de mapa.
#include "../barra_hud/barra_hud.h"
#include "../barra_combo/barra_combo.h"
#include "../hud_decoracion/hud_decoracion.h"
#include "../hud_artefactos/hud_artefactos.h"
#include "../marcador_combo/marcador_combo.h"
#include "../decoracion/decoracion.h"
#include "../decoracion/decoracion_explosion.h"
#include "../decoracion/decoracion_chatarra.h"
#include "../decoracion/decoracion_chispa.h"
#include "../decoracion/decoracion_puntuacion.h"
#include "../decoracion/decoracion_edu.h"
#include "../decoracion/decoracion_humo.h"
#include "../decoracion/decoracion_fantasma.h"
#include "../lector_puntuaciones/lector_puntuaciones.h"
#include "../localizador/localizador.h"
#include "../recursos/recursos.h"
#include "../actor_serializado/actor_serializado.h"
#include "../estructura_info_nivel/estructura_info_nivel.h"
#include "../controlador_perfiles/controlador_perfiles.h"
#include "../estructura_datos_perfil/estructura_datos_perfil.h"
#include "../controlador_retos/controlador_retos.h"
#include "../reto/reto.h"
#include "../configuracion_controles/configuracion_controles.h"
#include "../gestor_mapas_colision/gestor_mapas_colision.h"
#include "../trigger/trigger.h"
#include "../lista_niveles/lista_niveles.h"
#include "../tutorial/tutorial.h"

class Controlador_juego
{
	private:

	//El visitante para los Obstaculo_actividad

	class Visitante_actividades:public Visitante
	{
		private:

		Controlador_juego& c;
		unsigned int r;

		public:

		Visitante_actividades(Controlador_juego& cj):c(cj), r(0){}

		unsigned int acc_r() const {return r;}

		void visitar(Obstaculo_canon_apuntado& o) 	{r=c.actividad_no_jugador(o);};
		void visitar(Obstaculo_canon_giratorio& o) 	{r=c.actividad_no_jugador(o);};
		void visitar(Obstaculo_canon_programable& o) 	{r=c.actividad_no_jugador(o);};
		void visitar(Obstaculo_colocador_jugador& o) 	{r=c.actividad_no_jugador(o);};
		void visitar(Obstaculo_disparador& o) 		{r=c.actividad_no_jugador(o);};
		void visitar(Obstaculo_explosivo& o) 		{r=c.actividad_no_jugador(o);};
		void visitar(Obstaculo_generador_decoracion& o) {r=c.actividad_no_jugador(o);};
		void visitar(Obstaculo_generador_obstaculos& o) {r=c.actividad_no_jugador(o);};
		void visitar(Obstaculo_gravedad& o) 		{r=c.actividad_no_jugador(o);};
		void visitar(Obstaculo_propiedades& o) 		{r=c.actividad_no_jugador(o);};
		void visitar(Obstaculo_propiedades_fondo& o) 	{r=c.actividad_no_jugador(o);};
		void visitar(Obstaculo_trigger_auto& o) 	{r=c.actividad_no_jugador(o);};
		void visitar(Obstaculo_tutorial& o) 		{r=c.actividad_no_jugador(o);};
		void visitar(Trazador_fantasma& o) 		{r=c.actividad_no_jugador(o);};
		void visitar(Trazador_explosion& o) 		{r=c.actividad_no_jugador(o);};
		void visitar(Boss_01& o) 			{r=c.actividad_no_jugador(o);};
		void visitar(Boss_02& o) 			{r=c.actividad_no_jugador(o);};

		//Para estos, que se hace algo que no tiene nada que ver, no visitamos.	
		void visitar(Obstaculo_vector& o) {};
		void visitar(Obstaculo_propiedades_movimiento_relativo& o) {};
		void visitar(Obstaculo_puerta_automatica& o) {};
		void visitar(Obstaculo_actividad& o) {};
		void visitar(Obstaculo_puerta_disparar& o) {};
		void visitar(Obstaculo_puerta_remota& o) {};
	};

	friend class Visitante_acciones;


	DLibV::Log_grafico_enlazado LOG_G;

	static const unsigned int X_REPRESENTACION_PUNTUACION=12;
	static const unsigned int Y_REPRESENTACION_PUNTUACION=560;
	static const unsigned int X_REPRESENTACION_VIDAS=100;
	static const unsigned int Y_REPRESENTACION_VIDAS=560;
	static const unsigned int X_REPRESENTACION_ARTEFACTO=12;
	static const unsigned int Y_REPRESENTACION_ARTEFACTO=12;

	//Estas referencias las recibe del controlador.

	unsigned short int esquema_control;
	DLibV::Pantalla& pantalla;
	DLibI::Controles_SDL& controles_sdl;
	Configuracion_controles& configuracion_controles;
	Lector_puntuaciones& lector_puntuaciones;
	DLibI::Escritor_texto escritor;
	Campo_estrellas& campo_estrellas;
	DLibH::Controlador_fps_SDL& control_frames;
	DLibV::Representacion_texto_fija_estatica& rep_fps;
	DLibA::Controlador_audio_SDL& controlador_audio;
	DLibA::Cola_sonido& cola_sonido;
	Localizador& localizador;
	Controlador_perfiles &controlador_perfiles;
	Controlador_retos &controlador_retos;
	Estructura_info_nivel estructura_info_nivel;
	Estructura_datos_perfil datos_actualizables;
	Lista_niveles LISTA_NIVELES;
	Tutorial tutorial;			//Esto lo vamos a poner en la pila... Por diversión, para ver si funciona el tema de las copias.

	Controlador_puntuacion CONTROLADOR_PUNTUACION;
	DLibV::Camara camara;

	//Algunas constantes...

	enum estados{	
	E_PREPARANDO_JUGADOR_INICIAR_NIVEL=0,	//Esperando al input del jugador para empezar un nuevo nivel.
	E_JUEGO, 		//Juego normal y corriente.
	E_FINALIZANDO_NIVEL, 	//Finalizando un nivel, deja de poblarse el nivel.
	E_GAME_OVER,		//Te han liquidado.
	E_NUEVA_PUNTUACION_MAXIMA,	//Hay una nueva puntuación máxima.
	E_REINICIANDO_PRE_CUENTA_DESVANECIENDO, //En modo preparado, te liquidan y la pantalla se desvanece.
	E_REINICIANDO_NIVEL_DESVANECIENDO, //En modo preparado, te liquidan y la pantalla se desvanece.
	E_REINICIANDO_NIVEL,		//En modo preparado, te han liquidado y empezamos de nuevo.
	E_ANIMACION_FIN_JUEGO,
	E_FINALIZANDO_LOOP_PRINCIPAL
	};

	//Este enum guarda los tipos de game over que habrá. Se usarán para escoger el tipo de mensaje de game over.
	enum tipos_game_over{GO_NO_VIDAS, GO_NO_GEMAS, GO_FIN_JUEGO};
	
/*
DIAGRAMA DE ESTADOS:

-> E_JUEGO	-> [Supera nivel] -> E_FINALIZANDO_NIVEL -> [Ya no hay más obstaculos] -> E_PREPARANDO_JUGADOR_INICIAR_NIVEL -> [el jugador está listo] -> REPETIR.
		-> [No quedan vidas]
				

*/

	//Algunos valores numéricos...

	unsigned int cont_obstaculos, cont_bonus, extra_velocidad_actores;
	unsigned int max_superficie_ocupada, superficie_ocupada;
	bool excedida_superficie_maxima;
	unsigned int nivel_actual, cont_gemas, estado_actual;
	bool es_entrenamiento;
	bool salir;
	bool pausado;
	bool debe_introducir_puntuacion;	//Esta se usa como
	bool juego_finalizado;
	bool actualizar_combo;
	unsigned long int posicion_online;
	unsigned long int posicion_local;
	float tiempo_game_over; //Super auxiliar. Un máximo de tiempo que estamos en game over.
	float tiempo_pausa;	//Super auxiliar. La cantidad de tiempo que llevamos en pausa y que se usa para controlar la opacidad de la representación de la pausa.
	float tiempo_cargando;	//Idem que la anterior, la usaremos para controlar el tiempo de "cargando" al finalizar el juego...
	static const int max_tiempo_game_over=2;
	static const int TIEMPO_CARGANDO_MINIMO=2;
	static const int MS_CONTADOR_PRE_DESVANECER_PERDER_VIDA=800;

	float ms_logica;
	float ms_render;

	enum pasos_animacion_final
	{
		PAF_NADA=0,
		PAF_MOVERSE_A_INICIO,
		PAF_PAUSA_INTERMEDIA,
		PAF_SALIR_DISPARADO,
		PAF_PAUSA_FINAL,
		PAF_FIN
	};
	unsigned short int paso_animacion_final;
	float contador_animacion_final;
	float contador_pre_desvanecer_al_perder_vida;

	//Cosas del juego.
	Jugador * J;
	Fantasma * F;
	Mirilla * M;
	Escudo * E;

	//Cosas representables y de juego...
	std::vector<Actor_mapa*> actores_mapa;
	std::vector<Actor_mapa*> actores_mapa_representables;
	std::vector<Actor_mapa*>::iterator it_actores_mapa_representables;
	std::vector<Proyectil*> proyectiles;
	std::vector<Decoracion*> decoraciones;
	std::vector<Decoracion*> decoraciones_frente;
	std::vector<Accion_trigger_compuesta> vector_acciones_corrientes;

	typedef std::map<unsigned int, std::map<unsigned int, Punto_ruta *> > t_mapa_puntos_ruta;
	typedef std::pair<unsigned int, std::map<unsigned int, Punto_ruta *> > t_par_puntos_ruta;
	typedef std::pair<unsigned int, Punto_ruta *> t_par_punto_ruta_simple;

	t_mapa_puntos_ruta puntos_ruta;

	//Representaciones gráficas y demás.
	DLibV::Primitiva_grafica_linea_estatica mirilla_gamepad;
	Uint32 mirilla_gamepad_color_preparado;
	Uint32 mirilla_gamepad_color_no_preparado;
	
	Barra_hud barra_tiempo;
	Barra_hud barra_canon;
	Barra_hud barra_edu;
	Barra_hud barra_escudo;
	Barra_combo barra_combo; 
	Hud_artefactos hud_artefactos;	
	Hud_decoracion hud_decoracion;

	std::vector<Marcador_combo> cola_combo;

	DLibV::Representacion_texto_auto_estatica * rep_txt_puntuacion;
	DLibV::Representacion_texto_fija_estatica * rep_txt_gemas_nivel;
	DLibV::Representacion_texto_auto_estatica * rep_txt_vidas;
	DLibV::Representacion_texto_fija_estatica * rep_txt_pausa;
	DLibV::Representacion_texto_fija_estatica * rep_txt_cargando;

	static const int X_CAJA_FONDO_GAME_OVER=0;
	static const int Y_CAJA_FONDO_GAME_OVER=250;
	static const int W_CAJA_FONDO_GAME_OVER=800;
	static const int H_CAJA_FONDO_GAME_OVER=60;
	static const int COLOR_CAJA_FONDO_GAME_OVER=16777215;

	DLibV::Primitiva_grafica_caja_estatica * rep_caja_fondo_game_over;
	DLibV::Representacion_texto_fija_estatica * rep_txt_gameover;
	
	static const int X_CAJA_FONDO_PUNTUACION=0;
	static const int Y_CAJA_FONDO_PUNTUACION=300;
	static const int W_CAJA_FONDO_PUNTUACION=800;
	static const int H_CAJA_FONDO_PUNTUACION=160;
	static const int COLOR_CAJA_FONDO_PUNTUACION=16777215;

	DLibV::Primitiva_grafica_caja_estatica * rep_caja_fondo_puntuacion;
	DLibV::Representacion_texto_fija_estatica * rep_txt_nueva_puntuacion;
	DLibV::Representacion_texto_fija_estatica * rep_txt_nueva_puntuacion_nombre;
	DLibV::Representacion_texto_fija_estatica * rep_txt_nueva_puntuacion_local;
	DLibV::Representacion_texto_fija_estatica * rep_txt_nueva_puntuacion_online;

	static const int X_CAJA_FONDO_FIN_NIVEL=400;
	static const int Y_CAJA_FONDO_FIN_NIVEL=100;
	static const int W_CAJA_FONDO_FIN_NIVEL=400;
	static const int H_CAJA_FONDO_FIN_NIVEL=300;
	static const int COLOR_CAJA_FONDO_FIN_NIVEL=16777215;

	DLibV::Primitiva_grafica_caja_estatica * rep_caja_fondo_fin_nivel;
	DLibV::Representacion_compuesta_estatica * rep_fin_nivel;

	std::vector<Actor_mapa *> obtener_actores_por_tag(unsigned int);
	void componer_representacion_estadisticas_fin_nivel(unsigned int, bool, bool);
	void cambiar_estado(unsigned int);
	DLibH::Punto_2d<float> coordenadas_juego_a_pantalla(float, float);
	void procesar_post_seleccion_artefacto();
	void procesar_animacion_fin_juego(float);
	void cambiar_estado_animacion_fin_juego(unsigned int);
	void iniciar_animacion_fin_juego();
	void proceso_turno_nivel_estructura_info(float);
	void comprobar_finalizacion_nivel_aleatorio();
	void forzar_finalizacion_nivel_preparado();
	void restaurar_posiciones_post_nivel_preparado(float);
	void restar_vida_jugador();
	void reiniciar_nivel_preparado();
	void preparar_reinicio_nivel();
	void actualizar_marcador_combo();
	void procesar_turno_marcadores_combo(float);
	void actualizar_datos_hud_gemas();
	void actualizar_datos_hud_artefactos();
	void sumar_puntuacion(int, const Actor_mapa * const=NULL);
	unsigned short int preguntar_nueva_maxima_puntuacion_local();
	unsigned long int preguntar_nueva_maxima_puntuacion_online();
	void enviar_nueva_puntuacion();
	void vaciar_actores_mapa();
	void vaciar_proyectiles();
	void vaciar_decoraciones();
	void vaciar_mapa_rutas();
	void cancelar_partida();
	void reemplazar_item(Actor_mapa *);
	void generar_proyectil_jugador();
	void activar_edu();
	void activar_escudo();
	void desactivar_escudo();
//	void generar_decoracion_puntuacion(int);
	void generar_explosion(Actor *, bool=true, unsigned short int secciones_humo=0);
	void generar_explosion(float, float, float, float, bool=true, unsigned short int=0, float=0, float=0);
	void generar_humo(Actor *, float, float, float=Decoracion_humo::MAX_DURACION);
	void generar_humo(float, float, float, float, float=Decoracion_humo::MAX_DURACION);
	void generar_chispas(Actor *, unsigned int=50, unsigned int=Decoracion_chispa::TODAS);
	void generar_chispas_propulsor();
	void generar_chispas_teletransporte();
	void generar_chatarra(Actor *, unsigned int=200);
	void generar_chatarra(float, float, unsigned int=200);
	Actor_mapa * insertar_bonus(short int);
	void poblar_juego_aleatorio();
	void poblar_juego_preparado();
	void representar(float);
	void representar_pausa();
	void representar_hud(bool=false);
	void representar_entidades_mapa_fondo();
	void representar_entidades_jugador(bool=false);
	void representar_entidades_jugador_frente();
	void representar_entidades_mapa_frente();
	void representar_mirilla();
	void representar_puntero_gamepad(bool);
	void recoger_controles();
	void insertar_obstaculo();
	bool insertar_obstaculo_comprobar(Actor_mapa *);
	void procesar_pausa();
	bool procesar_controles_modo_juego(float);
//	bool procesar_controles_modo_preparando_nivel(float);
	bool procesar_controles_modo_game_over();
	bool procesar_controles_modo_entre_niveles();
	void procesar_controles_modo_nueva_puntuacion_maxima();
	void procesar_controles_movimiento_jugador(float &, float &, float &, float &);
	void procesar_controles_acciones_jugador(bool &, bool &, bool &,unsigned short int &, unsigned short int &);
	void procesar_resultados();
	void iniciar_fantasma();
	void finalizar_fantasma();
	void forzar_fin_fantasma();
	void procesar_input_fantasma(int, int, float, bool, bool, bool);
	void procesar_input_jugador(int, int, float, bool, bool, bool);
	void procesar_input_mirilla(int, int, float, float);
	unsigned int actividad_no_jugador(Obstaculo_canon_programable &);
	unsigned int actividad_no_jugador(Obstaculo_canon_giratorio &);
	unsigned int actividad_no_jugador(Obstaculo_canon_apuntado &);
	unsigned int actividad_no_jugador(Obstaculo_disparador &);
	unsigned int actividad_no_jugador(Obstaculo_explosivo &);
	unsigned int actividad_no_jugador(Obstaculo_gravedad &);
	unsigned int actividad_no_jugador(Obstaculo_propiedades_fondo &);
	unsigned int actividad_no_jugador(Obstaculo_propiedades &);
	unsigned int actividad_no_jugador(Obstaculo_generador_decoracion &);
	unsigned int actividad_no_jugador(Obstaculo_generador_obstaculos &);
	unsigned int actividad_no_jugador(Obstaculo_trigger_auto &);
	unsigned int actividad_no_jugador(Obstaculo_tutorial &);
	unsigned int actividad_no_jugador(Obstaculo_colocador_jugador &);
	unsigned int actividad_no_jugador(Trazador_fantasma &);
	unsigned int actividad_no_jugador(Trazador_explosion &);
	unsigned int actividad_no_jugador(Boss_01 &);
	unsigned int actividad_no_jugador(Boss_02 &);
	void activar_accion_trigger(Trigger&);
	void procesar_turno_acciones(float);
	void procesar_turno_no_jugador(float);
	void procesar_turno_actividad_no_jugador(Obstaculo_actividad *);
	void procesar_turno_jugador(float);
	void procesar_turno_proyectiles(float);
	void procesar_turno_decoraciones(float);
	void comprobar_proyectiles_fuera_juego();
	void comprobar_proyectiles_contra_escudo();
	void comprobar_procesar_colision_actor_proyectiles(Actor_mapa *);
	bool comprobar_procesar_colision_actor_jugador(Actor_mapa *);
	void comprobar_procesar_colision_actor_fantasma(Actor_mapa *);
	void comprobar_procesar_colision_proyectiles_jugador();
	void cargar_condiciones_nivel_e_iniciar(unsigned short int);
	void recoger_gema(Bonus_normal *, unsigned int=1);
	void controlar_destruccion_gema(Bonus_normal *);
	void aumentar_nivel();
	void evaluar_anulacion_combo_por_proyectil(const Proyectil * const);
	void actualizar_barra_combo();
	void iniciar_nuevo_nivel();
	void iniciar_fin_juego(unsigned int);
	void sumar_puntuacion_representada(Decoracion_puntuacion *);
	void generar_destruccion_mina(Obstaculo_explosivo *);
	void eliminar_actor_mapa(Actor_mapa *);
	void eliminar_proyectil(Proyectil *);
	void disparar_reto(unsigned int);
	void finalizar_loop_principal();
	void vaciar_vector_de_acciones();
	void eliminar_actor_de_acciones(Actor_mapa *);
	void instanciar_acciones_para_tag(unsigned int, Trigger * = NULL);
	void preparar_accion(Accion_trigger_compuesta&);
	void organizar_puntos_ruta(Punto_ruta*);
	void establecer_artefacto_jugador_auto();
	void generar_decoracion_desde_estructura_boss(Boss::Estructura_decoracion_boss&);
//	void ordenar_puntos_ruta();
	static int hilo__finalizar_loop_principal(void *);

	public:

	enum JOYSTICK{JABAJO=14, JIZQUIERDA=15, JARRIBA=12, JDERECHA=13, JX=2, JCIRCULO=1, JTRIANGULO=0, JCUADRADO=3, JSTART=8, JSELECT=9, JR1=5, JR2=7, JL1=4, JL2=6};
	enum ESQUEMAS_CONTROL{C_TECLADO_RATON, C_GAMEPAD, C_MAX_ESQUEMAS_CONTROL};

	Controlador_juego(unsigned int, DLibV::Pantalla&, DLibI::Controles_SDL&, Configuracion_controles&, Lector_puntuaciones&, Campo_estrellas&, DLibH::Controlador_fps_SDL&, DLibV::Representacion_texto_fija_estatica&, DLibA::Controlador_audio_SDL&, DLibA::Cola_sonido&, Localizador&, Controlador_perfiles&, Controlador_retos&);
	~Controlador_juego();

	bool inicializar(unsigned int=0, bool=false);
	bool es_lista_niveles_valida() const {return LISTA_NIVELES.es_valida();}
	void desmontar();
	void loop_principal();
	static std::string obtener_nombre_nivel(Localizador&, unsigned int);
};

#endif
