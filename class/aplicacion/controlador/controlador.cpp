#include "controlador.h"
#include "controlador.config.cpp"
#include "controlador.menu.cpp"
#include "controlador.perfiles.cpp"
#include "controlador.controles.cpp"
#include "controlador.instrucciones.cpp"
#include "controlador.retos.cpp"
#include "controlador.seleccion_nivel.cpp"

Controlador::Controlador()
	:
	controles_sdl(),
	controlador_perfiles(),
	controles_teclado_raton("teclado_raton"),
	controles_joystick("joystick"),
	entrenando(false),
	nivel_inicio(0)

	 //:flags_video(SDL_SWSURFACE) ///*SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_ANYFORMAT*/
{
	this->estado=E_SELECCION_PERFIL;

	this->pagina_puntuaciones_online=0;
	this->total_paginas_puntuaciones_online=0;

	this->idioma=0;
	this->flags_video=0;
//	this->conectar_online=false;
	this->volumen_audio=64;
	this->volumen_musica=64;
}

Controlador::~Controlador()
{
	if(this->montado)
	{
		this->desmontar();	
	}	
}

void Controlador::iniciar_y_procesar_editor(std::string p_fichero)
{
	this->estado=E_EDITOR;

	Controlador_editor C(p_fichero,	this->pantalla, this->controles_sdl);

	C.loop_principal();
}

void Controlador::iniciar_y_procesar_juego()
{
	this->estado=E_JUEGO;	//No es que esto signifique mucho pero...

	Controlador_juego C(
			this->esquema_control,
			this->pantalla, 
			this->controles_sdl, 
			(this->esquema_control==Controlador_juego::C_GAMEPAD ? this->controles_joystick : this->controles_teclado_raton),
			this->lector_puntuaciones, 
			*this->campo_estrellas, 
			this->control_frames, 
			*this->rep_fps, 
			*this->controlador_audio, 
			*this->cola_sonido,
			this->localizador,
			this->controlador_perfiles,
			this->controlador_retos);

	if(!C.es_lista_niveles_valida())
	{
		throw Excepcion_salida(Excepcion_salida::E_LISTA_NIVELES_CORRUPTA);
	}

	C.inicializar(nivel_inicio, entrenando);
	C.loop_principal();
	C.desmontar();

	this->estado=E_MENU;
}

bool Controlador::preparar_recursos_graficos()
{
	DLibV::Gestor_color::establecer_formato(this->pantalla.obtener_formato_pixeles());

	unsigned int i=0;
	std::string temp;
	std::string archivo;
	DLibV::Imagen * img=NULL;

	for(;i<Recursos::G_FIN_COMUNES; i++)
	{
		temp="data/imagenes/"+Recursos::grafico(i);
		img=new DLibV::Imagen(temp.c_str());

		if(DLibV::Gestor_recursos_graficos::insertar(i, img)==-1)
		{
			this->cadena_error="ERROR AL INSERTAR RECURSO "+temp;
			return false;
		}	
	}

	i=Recursos::G_INI_NIVELES+1;

	for( ;i<Recursos::G_FIN_NIVELES; i++)
	{
		temp="data/imagenes/"+Recursos::grafico(i);
		img=new DLibV::Imagen(temp.c_str());

		if(DLibV::Gestor_recursos_graficos::insertar(i, img)==-1)
		{
			this->cadena_error="ERROR AL INSERTAR RECURSO "+temp;
			return false;
		}
	}

	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1)->establecer_transparencia(0x00,0x00,0x00);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2)->establecer_transparencia(0x00,0x00,0x00);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_3)->establecer_transparencia(0x00,0x00,0x00);

	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_ITEMS)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUGADOR)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUEGO)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_01)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TITULO_1)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TITULO_2)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_MENU)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_COMBO)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FONDO_RETOS)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_ICONO_RETOS)->establecer_transparencia(0x00,0xff,0xff);
	
	//Estos están aquí porque al aplicar los procesos ya necesitaríamos saber
	//el colorkey de los originales...

	
	this->generar_recurso_alterando_color(Recursos::G_FUENTE_3, Recursos::G_FUENTE_4, 
		Pila_colores::nueva().color(DLibV::Gestor_color::color(255,255,255)).obtener(),	
		Pila_colores::nueva().color(DLibV::Gestor_color::color(255,0,0)).obtener()
	);

	this->generar_recurso_alterando_color(Recursos::G_FUENTE_1, Recursos::G_FUENTE_5, 
		Pila_colores::nueva().color(DLibV::Gestor_color::color(255,255,255)).obtener(),	
		Pila_colores::nueva().color(DLibV::Gestor_color::color(255,0,0)).obtener()
	);

	this->generar_recurso_alterando_canales(Recursos::G_TILE_NIV_01_A, Recursos::G_TILE_NIV_01_A_ALT_01, -100, -100, -100);
	this->generar_recurso_alterando_canales(Recursos::G_TILE_NIV_01_B, Recursos::G_TILE_NIV_01_B_ALT_01, -100, -100, -100);

	this->generar_recurso_alterando_canales(Recursos::G_TILE_NIV_02_A, Recursos::G_TILE_NIV_02_A_ALT_01, -80, -80, -80);
	this->generar_recurso_alterando_canales(Recursos::G_TILE_NIV_02_B, Recursos::G_TILE_NIV_02_B_ALT_01, -80, -80, -80);
	this->generar_recurso_alterando_canales(Recursos::G_TILE_NIV_02_C, Recursos::G_TILE_NIV_02_C_ALT_01, -80, -80, -80);

	this->generar_recurso_alterando_canales(Recursos::G_TILE_NIV_04_A, Recursos::G_TILE_NIV_04_A_ALT_01, -140, -140, -140);
	this->generar_recurso_alterando_canales(Recursos::G_TILE_NIV_04_B, Recursos::G_TILE_NIV_04_B_ALT_01, -140, -140, -140);
	this->generar_recurso_alterando_canales(Recursos::G_TILE_NIV_04_C, Recursos::G_TILE_NIV_04_C_ALT_01, -140, -140, -140);

	this->generar_recurso_alterando_color(Recursos::G_TILE_NIV_01_A, Recursos::G_TILE_NIV_03_A, 
		Pila_colores::nueva().
				color(DLibV::Gestor_color::color(210, 179, 173)).
				color(DLibV::Gestor_color::color(160, 125, 116)).
				color(DLibV::Gestor_color::color(58, 39, 35)).obtener(),
		Pila_colores::nueva().
				color(DLibV::Gestor_color::color(155, 192, 125)).
				color(DLibV::Gestor_color::color(109, 152, 73)).
				color(DLibV::Gestor_color::color(33, 47, 22)).obtener()
	);
	this->generar_recurso_alterando_color(Recursos::G_TILE_NIV_01_B, Recursos::G_TILE_NIV_03_B, 
		Pila_colores::nueva().
				color(DLibV::Gestor_color::color(210, 179, 173)).
				color(DLibV::Gestor_color::color(160, 125, 116)).
				color(DLibV::Gestor_color::color(58, 39, 35)).obtener(),
		Pila_colores::nueva().
				color(DLibV::Gestor_color::color(155, 192, 125)).
				color(DLibV::Gestor_color::color(109, 152, 73)).
				color(DLibV::Gestor_color::color(33, 47, 22)).obtener()
	);

	this->generar_recurso_alterando_canales(Recursos::G_TILE_NIV_03_A, Recursos::G_TILE_NIV_03_A_ALT_01, -100, -100, -100);
	this->generar_recurso_alterando_canales(Recursos::G_TILE_NIV_03_B, Recursos::G_TILE_NIV_03_B_ALT_01, -100, -100, -100);

	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_NIV_01_B)->establecer_transparencia(0x00,0xff,0xff); //Establecemos una vez copiado. No hace bien la copia con colorkey.
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_NIV_01_B_ALT_01)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_NIV_02_B)->establecer_transparencia(0x00,0xff,0xff); 
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_NIV_02_B_ALT_01)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_NIV_03_B)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_NIV_03_B_ALT_01)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_NIV_04_B)->establecer_transparencia(0x00,0xff,0xff);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_NIV_04_B_ALT_01)->establecer_transparencia(0x00,0xff,0xff);

	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_4)->establecer_transparencia(0x00,0x00,0x00);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_5)->establecer_transparencia(0x00,0x00,0x00);

	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_3)->establecer_alpha(128);
	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_4)->establecer_alpha(128);

	return true;

}

void Controlador::generar_recurso_alterando_canales(unsigned int recurso_original, unsigned int recurso_nuevo, int r, int g, int b) 
{
	DLibV::Imagen * img=NULL;
	SDL_Surface * superficie=DLibV::Utilidades_graficas_SDL::copiar_superficie(
			DLibV::Gestor_recursos_graficos::obtener(recurso_original)->acc_superficie());

	DLibV::Proceso_superficie_alterar_canales_rgb * pr=DLibV::Proceso_superficie_alterar_canales_rgb::generar(r, g, b, true);
	pr->mut_color_transparente(DLibV::Gestor_color::color(0, 255, 255));
	pr->actuar(superficie);
	img=new DLibV::Imagen(superficie);	
	if(DLibV::Gestor_recursos_graficos::insertar(recurso_nuevo, img)==-1)
	{
		this->cadena_error="ERROR AL GENERAR RECURSO ALTERANDO CANALES";
	}
}

void Controlador::generar_recurso_alterando_color(unsigned int recurso_origen, unsigned int recurso_nuevo, std::vector<Uint32> color_original, std::vector<Uint32> nuevo_color) 
{
	//Alteraciones... Ojo con estos punteros: no hay que liberarlos!.
	DLibV::Imagen * img=NULL;
	SDL_Surface * superficie=DLibV::Utilidades_graficas_SDL::copiar_superficie(
			DLibV::Gestor_recursos_graficos::obtener(recurso_origen)->acc_superficie());

	unsigned int i=0, l=std::max(color_original.size(), nuevo_color.size());

	while(i < l)
	{
		DLibV::Proceso_superficie_cambiar_colores * pr=DLibV::Proceso_superficie_cambiar_colores::generar(color_original[i], nuevo_color[i]);
		pr->actuar(superficie);
		delete pr;
		++i;
	}

	img=new DLibV::Imagen(superficie);	
	if(DLibV::Gestor_recursos_graficos::insertar(recurso_nuevo, img)==-1)
	{
		this->cadena_error="ERROR AL GENERAR RECURSO CON CAMBIO DE COLOR";
	}
}

bool Controlador::preparar_recursos_audio()
{
	DLibA::Controlador_audio_SDL::configurar_ratio(configuracion.acc_audio_ratio());
	DLibA::Controlador_audio_SDL::configurar_salidas(configuracion.acc_audio_salidas());
	DLibA::Controlador_audio_SDL::configurar_buffers(configuracion.acc_audio_buffers());
	DLibA::Controlador_audio_SDL::configurar_canales_audio(configuracion.acc_audio_canales());

	this->controlador_audio=DLibA::Controlador_audio_SDL::obtener();

	if(!this->controlador_audio->iniciar())
	{
		this->cadena_error="IMPOSIBLE INICIAR CONTROLADOR AUDIO";
		return false;
	}
	else
	{
		unsigned int i=0;
		std::string temp;
		std::string archivo;

		for(i=0; i<Recursos::S_MAX; i++)
		{
			temp="data/sonidos/"+Recursos::sonido(i);
		
			if(DLibA::Gestor_recursos_audio::insertar_sonido(i, temp.c_str())==-1)
			{
				this->cadena_error="ERROR AL INSERTAR SONIDO "+temp;
				return false;
			}
		}

		for(i=0; i<Recursos::M_MAX; i++)
		{
			temp="data/musica/"+Recursos::musica(i);
		
			if(DLibA::Gestor_recursos_audio::insertar_musica(i, temp.c_str())==-1)
			{
				this->cadena_error="ERROR AL INSERTAR MUSICA "+temp;
				return false;
			}
		}

		this->cola_sonido=new DLibA::Cola_sonido();
	}

	return true;
}

void Controlador::obtener_datos_de_configuracion()
{
	this->esquema_control=this->configuracion.acc_esquema_control();
//	this->conectar_online=this->configuracion.acc_conectar_online();
	this->volumen_audio=this->configuracion.acc_volumen_audio();
	this->volumen_musica=this->configuracion.acc_volumen_musica();
	this->idioma=this->configuracion.acc_idioma();
	componer_flags_video();
}

void Controlador::aplicar_datos_de_configuracion()
{
	this->localizador.inicializar(this->idioma);
	this->controlador_audio->establecer_volumen(this->volumen_audio);
	this->controlador_audio->establecer_volumen_musica(this->volumen_musica);
}

void Controlador::componer_flags_video()
{
	flags_video=
		(this->configuracion.acc_modo_hardware() ? SDL_HWSURFACE : SDL_SWSURFACE) | 
		(this->configuracion.acc_pantalla_completa() ? SDL_FULLSCREEN : 0);
}

bool Controlador::inicializar()
{
	if(!this->controlador_perfiles.es_indice_ok())
	{
		this->cadena_error="ARCHIVO INDICE DE PERFILES ERRONEO";
		return false;
	}

	//this->configuracion=Configuracion();
	this->obtener_datos_de_configuracion();

	this->lector_puntuaciones.iniciar();
	this->lector_puntuaciones.cargar_local();
	this->lector_puntuaciones.cargar_online(this->pagina_puntuaciones_online, this->total_paginas_puntuaciones_online);

	this->control_frames=DLibH::Controlador_fps_SDL(60);
	this->controlador_perfiles.inicializar_crono_perfiles();
	this->control_frames.insertar_crono(this->controlador_perfiles.obtener_crono_perfil());

	srand(time(NULL));

	this->pantalla.inicializar(Medidas::ANCHO_VISTA, Medidas::ALTO_VISTA, 0, this->flags_video);
	this->pantalla.establecer_titulo("CHEAP SHOOTER");	

	DLibV::Utilidades_graficas_SDL::mostrar_ocultar_cursor(false);

	if(!this->preparar_recursos_graficos()) return false;
	if(!this->preparar_recursos_audio()) return false;		

	this->campo_estrellas=new Campo_estrellas();		
	this->rep_fps=new DLibV::Representacion_texto_fija_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "", 400, 20);
	this->rep_fps->establecer_posicion(60, 0);

	this->montado=true;

	//Asignamos estos ahora...  Significa que lo hemos creado una vez al 
	//montar el objeto y ahora tiramos eso a la basura, pero necesitamos
	//esos controles que aún no existían y tienen que ser una referencia.

	if(!this->controles_teclado_raton.cargar())
	{
		this->controles_teclado_raton.generar_por_defecto_teclado();		
		this->controles_teclado_raton.guardar();
	}

	if(!this->controles_joystick.cargar())
	{
		this->controles_joystick.generar_por_defecto_joystick();		
		this->controles_joystick.guardar();
	}

	this->control_frames.inicializar();
	this->aplicar_datos_de_configuracion();
	
	//Esto lo hacemos ahora que el localizador ya está inicializado.
	this->controlador_retos.inicializar(this->localizador);
	this->preparar_instrucciones();

	return true;
}

void Controlador::loop_principal()
{
	this->controlador_audio->reproducir_musica(*DLibA::Gestor_recursos_audio::obtener_musica(Recursos::M_MUSICA_01));

	bool salir=false;

	//Eliminar este bloque para jugar normal...
//	this->controlador_perfiles.asignar_perfil_anonimo();
//	this->estado=E_MENU;
//	this->nivel_inicio=2;
//	this->entrenando=false;
//	this->iniciar_y_procesar_juego();
//	salir=true;

	menu_memoria_seleccion=M_INICIAR; //Esta es la "memoria" del menú, por si entramos y salimos volver al mismo sitio.

	while(!salir)
	{
		switch(this->estado)
		{
			case E_SELECCION_PERFIL:
				salir=this->procesar_estado_seleccion_perfil();
			break;

			case E_CREACION_PERFIL:
				this->procesar_estado_creacion_perfil();
			break;

			case E_VALIDAR_LOGIN:
				this->procesar_estado_login_perfil();
			break;

			case E_MENU:
				salir=this->procesar_estado_menu(); 
			break;

			case E_JUEGO:
				entrenando=false;
				nivel_inicio=0;
				this->iniciar_y_procesar_juego();
				this->estado=E_MENU;
			break;

			case E_INSTRUCCIONES:
				this->procesar_estado_instrucciones();
				this->estado=E_MENU;
			break;
		}
	}
}

/*
Esto simplemente devuelve si debe / puede conectar para las máximas puntuaciones.
Lo vamos a dejar de este modo por si acaso luego hay que añadir más condiciones.
*/

/*
bool Controlador::puede_conectar()
{
	return this->conectar_online;
}
*/

void Controlador::desmontar()
{
	this->control_frames.retirar_crono(this->controlador_perfiles.obtener_crono_perfil());
	this->controlador_audio->detener_musica();

	if(this->rep_fps) delete this->rep_fps;
	if(this->campo_estrellas) delete this->campo_estrellas;
	if(this->cola_sonido) delete this->cola_sonido;

	DLibA::Gestor_recursos_audio::liberar();
	DLibV::Gestor_recursos_graficos::liberar();
	
	//Esto es un hack... Ocurre que cuando sales del programa a pantalla
	//completa te cambia la configuración del sistema a la resolución que
	//tenga el programa. Esto lo resuelve.

	if(configuracion.acc_pantalla_completa())
	{
		this->flags_video=this->flags_video ^ SDL_FULLSCREEN;
		this->pantalla.inicializar(Medidas::ANCHO_VISTA, Medidas::ALTO_VISTA, 0, this->flags_video);
	}
	
	this->montado=false;
}


/*Este método se usa para mostrar todos los estados que podamos, que no dejan
de ser posicionar un vector de Representaciones y poner el campo de estrellas.
*/

void Controlador::mostrar_estados_generico(std::vector<DLibV::Representacion *> p_vector)
{
	this->campo_estrellas->acc_representacion()->volcar(pantalla);
	for(DLibV::Representacion * r : p_vector) r->volcar(pantalla);
	this->pantalla.flipar();
}
