#include "controlador_juego.h"
#include "controlador_juego.utils.cpp"
#include "controlador_juego.representaciones.cpp"
#include "controlador_juego.controles.cpp"
#include "controlador_juego.loop.cpp"
#include "controlador_juego.acciones.cpp"
#include "controlador_juego.niveles.cpp"
#include "controlador_juego.actividades.cpp"

Controlador_juego::Controlador_juego(
		unsigned int p_control,
		DLibV::Pantalla& p, 
		DLibI::Controles_SDL& c,
		Configuracion_controles& config_controles, 
		Lector_puntuaciones& l, 
		Campo_estrellas& ce, 
		DLibH::Controlador_fps_SDL& cf, 
		DLibV::Representacion_texto_fija_estatica& rfps, 
		DLibA::Controlador_audio_SDL& caudio, 
		DLibA::Cola_sonido& csonido, 
		Localizador &clocalizador,
		Controlador_perfiles& p_controlador_perfiles,
		Controlador_retos& p_controlador_retos)
		:
		LOG_G(	
			DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), 
			DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0, 600, 200),
			LOG),
		esquema_control(p_control), pantalla(p), controles_sdl(c), 
		configuracion_controles(config_controles),
		lector_puntuaciones(l), escritor(c), 
		campo_estrellas(ce), control_frames(cf), 
		rep_fps(rfps), controlador_audio(caudio), 
		cola_sonido(csonido),
		localizador(clocalizador),
		controlador_perfiles(p_controlador_perfiles),
		controlador_retos(p_controlador_retos),
		tutorial("", "", 0.0f, 0.0f),
		camara(0,0, Medidas::ANCHO_VISTA, Medidas::ALTO_VISTA ,0,0),
		mirilla_gamepad(0,0,0,0,0),
		mirilla_gamepad_color_preparado(DLibV::Gestor_color::color(0,128,0)),
		mirilla_gamepad_color_no_preparado(DLibV::Gestor_color::color(128,0,0)),
		barra_tiempo(),
		barra_canon(),
		barra_edu(),
		barra_escudo(),
		barra_combo()
{
	LOG_G.mut_color_fondo(DLibV::Gestor_color::color(64, 64, 128));
	LOG_G.mut_alpha(128);
	mirilla_gamepad.establecer_alpha(192);
	actualizar_barra_combo();
}

Controlador_juego::~Controlador_juego()
{

}

bool Controlador_juego::inicializar(unsigned int p_nivel_actual, bool p_es_entrenamiento)
{
	LOG<<DLibH::Log_base_n(10)<<"INICIALIZANDO SESION DE JUEGO"<<std::endl;

	this->J=new Jugador();
	this->F=new Fantasma();
	this->M=new Mirilla();
	this->E=new Escudo();

	//Le damos a las barras sus valores iniciales. Sólo cuando cambien se actualizará.
	this->barra_tiempo.establecer_color_fondo(Barra_hud::COLOR_FONDO);
	this->barra_tiempo.establecer_valor_maximo(J->acc_carga_maxima_canon());

	this->barra_canon.establecer_color_fondo(Barra_hud::COLOR_FONDO);
	this->barra_canon.establecer_valor_maximo(this->J->acc_carga_maxima_canon());
	this->barra_canon.establecer_color_auxiliar(Barra_hud::COLOR_AUXILIAR);
	
	this->barra_edu.establecer_color_fondo(Barra_hud::COLOR_FONDO);
	this->barra_edu.establecer_valor_maximo(this->J->acc_max_cantidad_edu());

	this->barra_escudo.establecer_color_fondo(Barra_hud::COLOR_FONDO);
	this->barra_escudo.establecer_valor_maximo(this->E->acc_max_carga_escudo());

	this->rep_txt_gemas_nivel=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_3), "", 200, 20);
	this->rep_txt_gemas_nivel->establecer_posicion(100, 17);
	this->rep_txt_gemas_nivel->mut_interletra(1);
		
	this->rep_txt_puntuacion=new DLibV::Representacion_texto_auto_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "");
	this->rep_txt_puntuacion->establecer_posicion(X_REPRESENTACION_PUNTUACION, Y_REPRESENTACION_PUNTUACION);

	this->rep_txt_vidas=new DLibV::Representacion_texto_auto_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "");
	this->rep_txt_vidas->establecer_posicion(X_REPRESENTACION_VIDAS, Y_REPRESENTACION_VIDAS);

	rep_caja_fondo_game_over=new DLibV::Primitiva_grafica_caja_estatica(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA_FONDO_GAME_OVER, Y_CAJA_FONDO_GAME_OVER, W_CAJA_FONDO_GAME_OVER, H_CAJA_FONDO_GAME_OVER), 
		COLOR_CAJA_FONDO_GAME_OVER);

	rep_caja_fondo_game_over->establecer_alpha(64);

	this->rep_txt_gameover=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), this->localizador.obtener(Localizador::C_GAME_OVER_01), 400, 40);
	this->rep_txt_gameover->establecer_posicion(340, 260);

	this->rep_txt_pausa=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), this->localizador.obtener(Localizador::C_PAUSA), 200, 20);
	this->rep_txt_pausa->establecer_posicion(0, 500);

	this->rep_txt_cargando=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), this->localizador.obtener(Localizador::C_CARGANDO), 200, 20);
	this->rep_txt_cargando->establecer_posicion(0, 550);


	rep_caja_fondo_puntuacion=new DLibV::Primitiva_grafica_caja_estatica(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA_FONDO_PUNTUACION, Y_CAJA_FONDO_PUNTUACION, W_CAJA_FONDO_PUNTUACION, H_CAJA_FONDO_PUNTUACION), 
		COLOR_CAJA_FONDO_PUNTUACION);

	rep_caja_fondo_puntuacion->establecer_alpha(64);

	this->rep_txt_nueva_puntuacion=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), this->localizador.obtener(Localizador::C_NUEVO_RECORD), 400, 20);
	this->rep_txt_nueva_puntuacion->establecer_posicion(340, 320);

	this->rep_txt_nueva_puntuacion_nombre=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "", 400, 20);
	this->rep_txt_nueva_puntuacion_nombre->establecer_posicion(340, 340);

	this->rep_txt_nueva_puntuacion_local=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "", 400, 20);
	this->rep_txt_nueva_puntuacion_local->establecer_posicion(340, 380);

	this->rep_txt_nueva_puntuacion_online=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "", 400, 20);
	this->rep_txt_nueva_puntuacion_online->establecer_posicion(340, 400);

	rep_caja_fondo_fin_nivel=new DLibV::Primitiva_grafica_caja_estatica(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA_FONDO_FIN_NIVEL, Y_CAJA_FONDO_FIN_NIVEL, W_CAJA_FONDO_FIN_NIVEL, H_CAJA_FONDO_FIN_NIVEL),
		COLOR_CAJA_FONDO_FIN_NIVEL);
	rep_caja_fondo_fin_nivel->establecer_alpha(64);

	rep_fin_nivel=new DLibV::Representacion_compuesta_estatica(W_CAJA_FONDO_FIN_NIVEL, H_CAJA_FONDO_FIN_NIVEL, 0, 255, 255);
	rep_fin_nivel->establecer_clave_color(0, 255, 255);
	rep_fin_nivel->establecer_posicion(X_CAJA_FONDO_FIN_NIVEL, Y_CAJA_FONDO_FIN_NIVEL);

	this->vaciar_actores_mapa();
	this->vaciar_proyectiles();
	this->vaciar_decoraciones();

	this->cont_obstaculos=0;
	this->cont_bonus=0;
	this->extra_velocidad_actores=0;
	this->salir=false;
	this->debe_introducir_puntuacion=false;
	this->posicion_online=0;
	this->posicion_local=0;
	this->superficie_ocupada=0;
	this->max_superficie_ocupada=20480;
	this->cont_gemas=0;
	this->pausado=false;
	this->tiempo_pausa=0;
	this->tiempo_cargando=0;
	this->tiempo_game_over=0;
	this->excedida_superficie_maxima=false;
	this->juego_finalizado=false;
	this->actualizar_combo=false;

	this->paso_animacion_final=PAF_NADA;
	this->contador_animacion_final=0;
	contador_pre_desvanecer_al_perder_vida=0;
	this->cola_combo.push_back(Marcador_combo(Controlador_puntuacion::X1, DLibV::Gestor_recursos_graficos::obtener(Recursos::G_COMBO)));

	this->nivel_actual=p_nivel_actual;
	this->es_entrenamiento=p_es_entrenamiento;

	this->cambiar_estado(E_JUEGO);	

	return true;
}

void Controlador_juego::desmontar()
{
	delete this->J;
	delete this->F;
	delete this->M;
	delete this->E;

	delete this->rep_txt_gemas_nivel;
	delete this->rep_txt_puntuacion;
	delete this->rep_txt_vidas;
	delete this->rep_txt_gameover;
	delete rep_caja_fondo_game_over;
	delete this->rep_txt_pausa;
	delete this->rep_txt_cargando;
	delete rep_caja_fondo_puntuacion;
	delete this->rep_txt_nueva_puntuacion;
	delete this->rep_txt_nueva_puntuacion_nombre;
	delete this->rep_txt_nueva_puntuacion_local;
	delete this->rep_txt_nueva_puntuacion_online;
	delete rep_caja_fondo_fin_nivel;
	delete rep_fin_nivel;

	this->vaciar_actores_mapa();
	this->vaciar_proyectiles();
	this->vaciar_decoraciones();

	this->campo_estrellas.restaurar();
}
