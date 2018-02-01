/* Cosas del loop de menú*/

/*Se procesa el estado completo del menú, desde el input hasta el mostrado de
cosas en la pantalla... Se mantiene en este loop mientras estemos en el estado
de menu, devuelve "true" al loop principal cuando salgamos del juego. False en
el resto de casos. */

bool Controlador::procesar_estado_menu()
{
	bool salir=false;
	bool resultado=false;
	bool es_perfil_anonimo=this->controlador_perfiles.es_perfil_anonimo();

	std::string cadena_bienvenida="";

	if(es_perfil_anonimo)
	{
		cadena_bienvenida=this->localizador.obtener(Localizador::C_MENU_BIENVENIDA_ANONIMO);	
	}
	else
	{
		cadena_bienvenida=this->localizador.obtener(Localizador::C_MENU_BIENVENIDA_PERFIL);
		cadena_bienvenida+=this->controlador_perfiles.obtener_nombre_perfil();
	}

	//Creamos el texto de info y bienvenida...
	DLibV::Representacion_texto_auto_estatica txt_bienvenida(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), cadena_bienvenida);
	txt_bienvenida.establecer_posicion(10, 80);

	DLibV::Representacion_texto_auto_estatica txt_menu(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), this->localizador.obtener(Localizador::C_MENU_01));
	txt_menu.establecer_posicion(10, 110);

	std::string cadena_info_puntuaciones=(char)127+this->localizador.obtener(Localizador::C_MENU_INFO_PUNTUACIONES); 	//Esto es una medalla.
 
	//Creamos los textos de puntuaciones...
	DLibV::Representacion_texto_fija_estatica txt_puntuaciones(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), "", 250, 200);
	txt_puntuaciones.establecer_posicion(0, 0);

	DLibV::Representacion_texto_fija_estatica txt_puntuaciones_online(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), "", 250, 200);
	txt_puntuaciones_online.establecer_posicion(0, 0);

	DLibV::Representacion_texto_fija_estatica txt_puntuaciones_info(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), cadena_info_puntuaciones, 400, 200);
	txt_puntuaciones_info.establecer_posicion(100, 500);

	txt_puntuaciones.mut_interlineado(2);
	txt_puntuaciones_online.mut_interlineado(2);

	//Este texto sólo dice si tienes los tutoriales activados o no. Vamos a meter algo de lambda, que me gusta, 
	//para cuando haya cambios :).

	DLibV::Representacion_texto_auto_estatica txt_tutoriales(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), "...");
	txt_tutoriales.establecer_posicion(100, 540);

	auto cambiar_texto_tutoriales=[&txt_tutoriales, this](bool activo)
	{
		std::string cadena_tutoriales=activo ? 
			this->localizador.obtener(Localizador::C_MENU_TUTORIALES_ACTIVOS) : 
			this->localizador.obtener(Localizador::C_MENU_TUTORIALES_INACTIVOS);
		txt_tutoriales.asignar(cadena_tutoriales);
	};

	cambiar_texto_tutoriales(this->controlador_perfiles.es_mostrar_tutoriales());

	//Vamos a crear una representación compuesta para mostrar los trofeos.
	int indice_reto=0;
	DLibV::Representacion_compuesta_estatica rep_retos=DLibV::Representacion_compuesta_estatica(400, 60, 0, 0, 0);

	rep_retos.cambiar_visibilidad(!this->controlador_perfiles.es_perfil_anonimo());
	rep_retos.establecer_posicion(400, 520);
	this->modificar_trofeo_mostrado_menu(indice_reto, rep_retos);

	//Recargamos los valores de las puntuaciones...
	this->auxiliar_recargar_texto_puntuaciones(this->lector_puntuaciones.acc_puntuaciones(), txt_puntuaciones, 60, 300);

//	if(this->puede_conectar()) 
//	{
		this->auxiliar_recargar_texto_puntuaciones(this->lector_puntuaciones.acc_puntuaciones_online(), txt_puntuaciones_online, 400, 300);
		this->lector_puntuaciones.cargar_online(this->pagina_puntuaciones_online, this->total_paginas_puntuaciones_online);
//	}

	//Creamos los títulos...
	DLibV::Representacion_bitmap_dinamica rep_bmp_titulo_01(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TITULO_1));
	DLibV::Representacion_bitmap_dinamica rep_bmp_titulo_02(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TITULO_2));

	//Mostramos la animación de título.
	this->mostrar_animacion_titulo(rep_bmp_titulo_01, rep_bmp_titulo_02);

	//Vamos a preparar un hilo para pasar de página las puntuaciones online...
	DLibH::Hilo_SDL * hilo_puntuaciones=NULL;

	std::vector<DLibV::Representacion *> vector_rep;
	vector_rep.push_back(&txt_puntuaciones);
	vector_rep.push_back(&rep_retos);
	vector_rep.push_back(&rep_bmp_titulo_01);
	vector_rep.push_back(&rep_bmp_titulo_02);
	vector_rep.push_back(&txt_bienvenida);
	vector_rep.push_back(&txt_puntuaciones_info);
	vector_rep.push_back(&txt_menu);
	vector_rep.push_back(&txt_puntuaciones_online);
	vector_rep.push_back(&txt_tutoriales);

	while(!salir)
	{
		//Si hay un hilo en marcha...
		if(hilo_puntuaciones && hilo_puntuaciones->es_finalizado())
		{
			hilo_puntuaciones->completar();
			delete hilo_puntuaciones;
			hilo_puntuaciones=NULL;

			//Como hemos terminado la carga, mostramos los resultados.
			this->auxiliar_recargar_texto_puntuaciones(this->lector_puntuaciones.acc_puntuaciones_online(), txt_puntuaciones_online, 400, 300);
		}

		this->controles_sdl.recoger();

		//Salir del juego...
		if(this->controles_sdl.es_senal_salida() || this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
		{
			resultado=true;
			salir=true;
		}
		//Entrar en configuración...
		else if(this->controles_sdl.es_tecla_down(SDLK_1))
		{
			this->estado=E_CONFIGURACION;
			this->mostrar_animacion_abandonar_menu(rep_bmp_titulo_01, rep_bmp_titulo_02);
			salir=true;
		}
		else if(this->controles_sdl.es_tecla_down(SDLK_2))
		{
			this->estado=E_CONFIGURAR_CONTROLES;
			this->mostrar_animacion_abandonar_menu(rep_bmp_titulo_01, rep_bmp_titulo_02);
			salir=true;
		}
		else if(this->controles_sdl.es_tecla_down(SDLK_3))
		{
			this->controlador_perfiles.intercambiar_tutoriales_en_perfil();
			cambiar_texto_tutoriales(this->controlador_perfiles.es_mostrar_tutoriales());
			
		}
		//Instrucciones
		else if(this->controles_sdl.es_tecla_down(SDLK_i))
		{
			this->estado=E_INSTRUCCIONES;
			this->mostrar_animacion_abandonar_menu(rep_bmp_titulo_01, rep_bmp_titulo_02);
			salir=true;
		}

		//Navegar por las paginas de maximas puntuaciones online...
		else if(
			this->controles_sdl.es_tecla_down(SDLK_RIGHT) ||
			this->controles_sdl.es_tecla_down(SDLK_LEFT) ||
			this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JDERECHA) ||
			this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JIZQUIERDA))
		{
			if(!hilo_puntuaciones) // && this->puede_conectar())
			{
				unsigned int pagina_pre=this->pagina_puntuaciones_online;
				int direccion=this->controles_sdl.es_tecla_down(SDLK_RIGHT) || this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JDERECHA) ? 1 : -1;
				unsigned int pag_original=this->pasar_pagina_puntuaciones_online(direccion);

				if(pag_original != this->pagina_puntuaciones_online)
				{
					hilo_puntuaciones=new DLibH::Hilo_SDL();
					if(!hilo_puntuaciones->iniciar(hilo__recargar_puntuaciones_online, this))
					{
						this->pagina_puntuaciones_online=pagina_pre;
						delete hilo_puntuaciones;
						hilo_puntuaciones=NULL;
					}
				}
			}
		}
		//Navegar por los trofeos
		else if(
			this->controles_sdl.es_tecla_down(SDLK_UP) ||
			this->controles_sdl.es_tecla_down(SDLK_DOWN) ||
			this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JARRIBA) ||
			this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JABAJO))
		{
			int direccion=this->controles_sdl.es_tecla_down(SDLK_DOWN) || this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JABAJO) ? 1 : -1;
			indice_reto+=direccion;

			if(indice_reto < 0) indice_reto=Definicion_retos::TOTAL_RETOS-1;
			else if(indice_reto >= Definicion_retos::TOTAL_RETOS) indice_reto=0;
		
			this->modificar_trofeo_mostrado_menu(indice_reto, rep_retos);
		}
		//Iniciar juego...
		else if(
			this->controles_teclado_raton.es_usar_artefacto(this->controles_sdl, Configuracion_controles::T_DOWN)
			|| this->controles_joystick.es_usar_artefacto(this->controles_sdl, Configuracion_controles::T_DOWN)
			)
		{
			//Nunca dejamos el hilo abierto...
			if(hilo_puntuaciones)
			{	
				DLibH::Hilo_SDL::finalizar_y_destruir_hilo(hilo_puntuaciones);
			}
		
			this->mostrar_animacion_abandonar_menu(rep_bmp_titulo_01, rep_bmp_titulo_02);

			this->estado=E_JUEGO;
			salir=true;
		}
	

		if(!salir)
		{
			this->mostrar_menu(vector_rep, hilo_puntuaciones!=NULL);
			this->control_frames.turno();
			//this->controlador_perfiles.procesar_crono_perfiles(); No procesamos el turno: aqui no se producen cambios en el perfil que queramos guardar.
		}
	}

	//Nunca dejamos el hilo abierto...
	if(hilo_puntuaciones) DLibH::Hilo_SDL::finalizar_y_destruir_hilo(hilo_puntuaciones);
	
	//Si devolvemos true aquí vamos a oscurecer finalmente la pantalla antes de salir.

	if(resultado)
	{
		this->mostrar_salida_menu(vector_rep, hilo_puntuaciones!=NULL);
	}

	return resultado;
}

void Controlador::mostrar_menu(std::vector<DLibV::Representacion *> &p_vector, bool cargando_online)
{
	//Esto no hace falta siempre que haya campo de estrellas...
	float delta=this->control_frames.obtener_delta();

	this->campo_estrellas->acc_representacion()->volcar(this->pantalla);
	this->campo_estrellas->procesar_turno(delta);

	std::vector<DLibV::Representacion *>::iterator	ini=p_vector.begin(),
						fin=p_vector.end(),
						primero=ini;

	while(ini < fin)
	{
		if(ini==primero && !cargando_online) (*ini)->volcar(pantalla); //El primero es el de las puntuaciones.
		else (*ini)->volcar(pantalla);		
		++ini;
	}

	this->rep_fps->asignar(DLibH::Herramientas::entero_a_cadena(this->control_frames.acc_frames_contados()));
	this->rep_fps->volcar(pantalla);

	this->pantalla.flipar();
}

/*Esta es la animación al iniciarse el juego: las letras se apartan a gran
velocidad...*/

void Controlador::mostrar_animacion_abandonar_menu(DLibV::Representacion_bitmap &rep_bmp_titulo_01, DLibV::Representacion_bitmap &rep_bmp_titulo_02)
{
	//Por si acaso, ponemos el título en la posición en la que se inició...
	rep_bmp_titulo_01.establecer_posicion(340, 100);
	rep_bmp_titulo_02.establecer_posicion(370, 180);

	//Generar animaciones..
	Grupo_animacion grupo=Grupo_animacion();
	grupo.asignar_animacion(new Estructura_animacion_movimiento(&rep_bmp_titulo_01, DLibH::Vector_2d(0, -500), 340, -100));
	grupo.asignar_animacion(new Estructura_animacion_movimiento(&rep_bmp_titulo_02, DLibH::Vector_2d(0, Medidas::ANCHO_VISTA), 370, 700));

	float delta=0;

	//Mientras la animación no haya finalizado...
	while(!grupo.es_finalizado())
	{
		this->controles_sdl.recoger();
		delta=this->control_frames.obtener_delta();

		//Volcar el campo de estrellas.
		this->campo_estrellas->procesar_turno(delta);
		this->campo_estrellas->acc_representacion()->volcar(pantalla);
	
		grupo.turno(delta);
		grupo.mostrar(this->pantalla);

		this->pantalla.flipar();
		this->control_frames.turno();
		//this->controlador_perfiles.procesar_crono_perfiles(); No lo hacemos tampoco, esto es una animación!!!.
	}
}

/*Esta es la animación de las letras del título moviéndose hasta su posición
y haciendo un flash.*/

void Controlador::mostrar_animacion_titulo(DLibV::Representacion_bitmap &rep_bmp_titulo_01, DLibV::Representacion_bitmap &rep_bmp_titulo_02)
{
	//Los colocamos en su posición inicial: fuera de la pantalla...
	rep_bmp_titulo_01.establecer_posicion(-400, 30);
	rep_bmp_titulo_02.establecer_posicion(1200, 109);

	//Generar animaciones..
	Grupo_animacion grupo=Grupo_animacion(15);	//15% minimo de animación antes de cancelar.
	grupo.asignar_animacion(new Estructura_animacion_movimiento(&rep_bmp_titulo_01, DLibH::Vector_2d(Medidas::ANCHO_VISTA, 0), 381, 30));
	grupo.asignar_animacion(new Estructura_animacion_movimiento(&rep_bmp_titulo_02, DLibH::Vector_2d(-Medidas::ANCHO_VISTA, 0), 405, 109));

	float delta=0;

	while(!grupo.es_finalizado())
	{
		this->controles_sdl.recoger();
		delta=this->control_frames.obtener_delta();

		//Ante cualquier tipo de input simplemente salimos
/*		if(this->controles_sdl.recibe_eventos_input())	
		{
			if(grupo.finalizar()) break;
		}
*/

		//Volcar el campo de estrellas.
		this->campo_estrellas->procesar_turno(delta);
		this->campo_estrellas->acc_representacion()->volcar(pantalla);
	
		grupo.turno(delta);
		grupo.mostrar(this->pantalla);

		this->pantalla.flipar();
		this->control_frames.turno();
		//this->controlador_perfiles.procesar_crono_perfiles(); No lo hacemos tampoco, esto es una animación!!!.
	}

	//Al final de la animación ponemos un destello ochentero :D.
	//ojo con el parámetro, es en segundos.	
	this->pantalla.adjuntar_proceso(DLibV::Proceso_superficie_fundido::generar(this->pantalla.acc_superficie(), 255,255,255, 255, 0, 1));

	//Sonido de explosión...
	this->cola_sonido->insertar(DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_DISPARO));
	this->cola_sonido->procesar_cola();
	
	while(this->pantalla.hay_procesos_en_marcha())
	{
		delta=this->control_frames.obtener_delta();
		//Volcar el campo de estrellas.

		this->campo_estrellas->acc_representacion()->volcar(pantalla);
		this->campo_estrellas->procesar_turno(delta);
	
		//Volcar el título...

		rep_bmp_titulo_01.volcar(pantalla);
		rep_bmp_titulo_02.volcar(pantalla);
	
		this->pantalla.procesar_procesos(delta);
		this->pantalla.flipar();
		this->control_frames.turno();
		//this->controlador_perfiles.procesar_crono_perfiles(); Idem que antes.
	}
}

/*Animación para la salida del juego. Simplemente se oscurece la pantalla
antes de salir.*/

//void Controlador::mostrar_salida_menu(DLibV::Representacion_bitmap &rep_bmp_titulo_01, DLibV::Representacion_bitmap &rep_bmp_titulo_02 , Representacion_texto &txt_puntuaciones , Representacion_texto &txt_puntuaciones_online , Representacion_texto &txt_menu, bool cargando_online)
void Controlador::mostrar_salida_menu(std::vector<DLibV::Representacion *> &p_vector, bool cargando_online)
{
	//Oscurecemos....
	this->pantalla.adjuntar_proceso(DLibV::Proceso_superficie_fundido::generar(this->pantalla.acc_superficie(), 0,0,0, 0, 255, 1));
	float delta=0;

	std::vector<DLibV::Representacion *>::iterator	ini=p_vector.begin(),
						fin=p_vector.end(),
						primero=ini;

	while(this->pantalla.hay_procesos_en_marcha())
	{
		//Esto no hace falta siempre que haya campo de estrellas...
		delta=this->control_frames.obtener_delta();

		this->campo_estrellas->acc_representacion()->volcar(pantalla);
		this->campo_estrellas->procesar_turno(delta);

		while(ini < fin)
		{
			if(ini==primero && !cargando_online) (*ini)->volcar(pantalla); //El primero es el de las puntuaciones.
			else (*ini)->volcar(pantalla);		
			++ini;
		}

		this->pantalla.procesar_procesos(delta);
		this->pantalla.flipar();
		this->control_frames.turno();
		//this->controlador_perfiles.procesar_crono_perfiles(); AQuí no lo hacemos: estamos saliendo y se va a hacer sólo al salir.

		ini=p_vector.begin();
	}
}

/*Esto es una animación: el campo de estrellas siempre de fondo y el título
apareciendo en la pantalla, por ejemplo. Cuando termine la animación debería
quedarse en la misma posición para el menú principal*/

void Controlador::auxiliar_recargar_texto_puntuaciones(std::vector<Lector_puntuaciones::struct_puntuacion> puntuaciones, DLibV::Representacion_texto &txt, int x, int y)
{
	std::vector<Lector_puntuaciones::struct_puntuacion>::iterator ini=puntuaciones.begin(),
									fin=puntuaciones.end();
	std::string cadena("");

	for(;ini<fin; ini++)
	{
		if((*ini).puntuacion)
		{
			
			if((*ini).finalizado) cadena+=(char)127;	//Esto es una medalla.
			else cadena+=" ";
			cadena+=" ";		
			cadena+=DLibH::Herramientas::entero_a_cadena((*ini).posicion);
			cadena+=" - ";
			cadena+=DLibH::Herramientas::entero_a_cadena((*ini).puntuacion);
			cadena+=" [";
			cadena+=DLibH::Herramientas::entero_a_cadena((*ini).nivel+1);
			cadena+="] ";
			cadena+=(*ini).nombre;
			cadena+="\n";
		}
	}

	txt.asignar(cadena);
	txt.establecer_posicion(x, y);
}


unsigned int Controlador::pasar_pagina_puntuaciones_online(int direccion)
{
	unsigned int pag_original=this->pagina_puntuaciones_online;

	if(direccion==1 && this->pagina_puntuaciones_online < (this->total_paginas_puntuaciones_online-1))
	{
		this->pagina_puntuaciones_online++;
	}
	else if(direccion==-1 && this->pagina_puntuaciones_online > 0)
	{
		this->pagina_puntuaciones_online--;
	}

	return pag_original;
}

void Controlador::modificar_trofeo_mostrado_menu(unsigned int p_indice, DLibV::Representacion_compuesta_estatica& rep)
{
	Reto * reto_apuntado=this->controlador_retos.obtener_por_indice(p_indice);
	rep.rellenar(0,0,0);

	if(this->controlador_perfiles.es_reto_conseguido(p_indice)) rep.volcar_representacion(*reto_apuntado->acc_representacion());
	else rep.volcar_representacion(*reto_apuntado->acc_representacion_off());
}


int Controlador::hilo__recargar_puntuaciones_online(void * datos_recibidos)
{
	//Primero sacamos la estructura...
	DLibH::Hilo_SDL::Hilo_estructura_datos * estructura_datos=static_cast<DLibH::Hilo_SDL::Hilo_estructura_datos*>(datos_recibidos);

	//Ahora el hilo...
	DLibH::Hilo_SDL * h=NULL;
	h=estructura_datos->ref_hilo;
		
	//Y ahora casteamos el parámetro original.
	Controlador * c=static_cast<Controlador*>(estructura_datos->datos);
	c->lector_puntuaciones.cargar_online(c->pagina_puntuaciones_online, c->total_paginas_puntuaciones_online);

	//Preparar el hilo para finalizarlo...
	h->finalizar();

	return 1;
}
