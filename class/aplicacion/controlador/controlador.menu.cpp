/*El menú principal... Es enorme porque tiene que procesar muchos recursos
gráficos y luego tiene una lógica un poco descomunal, con hilos y demás.
No es muy difícil de seguir en todo caso.*/

bool Controlador::procesar_estado_menu()
{
	bool salir=false;
	bool resultado=false;
	bool es_perfil_anonimo=this->controlador_perfiles.es_perfil_anonimo();
	int seleccion_actual=menu_memoria_seleccion;

	//Generar los elementos gráficos...	

	//Primero la cadena de bienvenida...
	std::string cadena_bienvenida=es_perfil_anonimo 
		? this->localizador.obtener(Localizador::C_MENU_BIENVENIDA_ANONIMO)
		: this->localizador.obtener(Localizador::C_MENU_BIENVENIDA_PERFIL)+this->controlador_perfiles.obtener_nombre_perfil();
	DLibV::Representacion_texto_auto_estatica txt_bienvenida(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), cadena_bienvenida);
	txt_bienvenida.establecer_posicion(8, 238);

 	//Creamos los textos de puntuaciones...
	DLibV::Representacion_texto_fija_estatica txt_puntuaciones(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), "", 250, 200);
	txt_puntuaciones.establecer_posicion(0, 0);

	DLibV::Representacion_texto_fija_estatica txt_puntuaciones_online(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), "", 250, 200);
	txt_puntuaciones_online.establecer_posicion(0, 0);

	//Medalla de puntuaciones...
	std::string cadena_info_puntuaciones=(char)127+this->localizador.obtener(Localizador::C_MENU_INFO_PUNTUACIONES); 	//Esto es una medalla.
	DLibV::Representacion_texto_auto_estatica txt_puntuaciones_info(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), cadena_info_puntuaciones);
	txt_puntuaciones_info.establecer_posicion(277, 433);

	//Títulos de puntuaciones...
	DLibV::Representacion_texto_auto_estatica txt_titulo_puntuaciones_local(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), this->localizador.obtener(Localizador::C_MENU_PPAL_TITULO_OFFLINE));
	txt_titulo_puntuaciones_local.establecer_posicion(191, 269);

	std::string cadena_titulo_online=(char)16+this->localizador.obtener(Localizador::C_MENU_PPAL_TITULO_ONLINE); //Son dos flechitas de cursor.
	DLibV::Representacion_texto_auto_estatica txt_titulo_puntuaciones_online(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), cadena_titulo_online);
	txt_titulo_puntuaciones_online.establecer_posicion(509, 269);

	txt_puntuaciones.mut_interlineado(2);
	txt_puntuaciones_online.mut_interlineado(2);

	//Las puntuaciones van sobre una caja de fondo...
	DLibV::Primitiva_grafica_caja_estatica rep_caja_puntuaciones(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 258, 800, 199), 
		DLibV::Gestor_color::color(255, 255,255));
	rep_caja_puntuaciones.establecer_alpha(64);

	//Ahora el menú...
	DLibV::Primitiva_grafica_caja_estatica rep_caja_menu(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 522, 800, 38), 
		DLibV::Gestor_color::color(255, 255,255));
	rep_caja_menu.establecer_alpha(64);

	DLibV::Representacion_bitmap_estatica rep_flecha_menu_izq(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_MENU));
	rep_flecha_menu_izq.establecer_recorte(13,0,13,25);
	rep_flecha_menu_izq.establecer_posicion(249, 529);

	DLibV::Representacion_bitmap_estatica rep_flecha_menu_der(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_MENU));
	rep_flecha_menu_der.establecer_recorte(0,0,13,25);
	rep_flecha_menu_der.establecer_posicion(528, 529);

	//Recargamos los valores de las puntuaciones...
	this->auxiliar_recargar_texto_puntuaciones(this->lector_puntuaciones.acc_puntuaciones(), txt_puntuaciones, 138, 300);
	this->auxiliar_recargar_texto_puntuaciones(this->lector_puntuaciones.acc_puntuaciones_online(), txt_puntuaciones_online, 450, 300);
	this->lector_puntuaciones.cargar_online(this->pagina_puntuaciones_online, this->total_paginas_puntuaciones_online);

	//Creamos los títulos...
	DLibV::Representacion_bitmap_dinamica rep_bmp_titulo_01(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TITULO_1));
	DLibV::Representacion_bitmap_dinamica rep_bmp_titulo_02(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TITULO_2));

	std::string cadena_tutoriales=controlador_perfiles.es_mostrar_tutoriales() 
		? this->localizador.obtener(Localizador::C_MENU_PPAL_DESACTIVAR_TUTORIALES)
		: this->localizador.obtener(Localizador::C_MENU_PPAL_ACTIVAR_TUTORIALES);

	//Finalmente creamos todos los elementos seleccionables... Son simplemente textos, inicialmente fuera de la pantalla.
	DLibV::Representacion_texto_auto_estatica rep_menu_iniciar(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), this->localizador.obtener(Localizador::C_MENU_PPAL_INICIAR_PARTIDA));
	DLibV::Representacion_texto_auto_estatica rep_menu_controles(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), this->localizador.obtener(Localizador::C_MENU_PPAL_CONTROLES));
	DLibV::Representacion_texto_auto_estatica rep_menu_configuracion(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), this->localizador.obtener(Localizador::C_MENU_PPAL_CONFIGURACION));
	DLibV::Representacion_texto_auto_estatica rep_menu_tutoriales(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), cadena_tutoriales);
	DLibV::Representacion_texto_auto_estatica rep_menu_ayuda(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), this->localizador.obtener(Localizador::C_MENU_PPAL_AYUDA));
	DLibV::Representacion_texto_auto_estatica rep_menu_seleccion_nivel(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), this->localizador.obtener(Localizador::C_MENU_PPAL_SELECCIONAR_NIVEL));
	DLibV::Representacion_texto_auto_estatica rep_menu_retos(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), this->localizador.obtener(Localizador::C_MENU_PPAL_VER_RETOS));

	std::vector<DLibV::Representacion_texto_auto_estatica *> vector_menu;
	vector_menu.push_back(&rep_menu_iniciar);
	vector_menu.push_back(&rep_menu_controles);
	vector_menu.push_back(&rep_menu_configuracion);
	vector_menu.push_back(&rep_menu_tutoriales);
	vector_menu.push_back(&rep_menu_ayuda);
	vector_menu.push_back(&rep_menu_seleccion_nivel);
	vector_menu.push_back(&rep_menu_retos);
	for(DLibV::Representacion_texto_auto_estatica * r : vector_menu) r->establecer_posicion(0, -100);

	//Mostramos la animación de título.
	this->mostrar_animacion_titulo(rep_bmp_titulo_01, rep_bmp_titulo_02);

	//Vamos a preparar un hilo para pasar de página las puntuaciones online...
	DLibH::Hilo_SDL * hilo_puntuaciones=NULL;

	//Todo lo representable va en este vector.
	std::vector<DLibV::Representacion *> vector_rep;
	vector_rep.push_back(&rep_bmp_titulo_01);
	vector_rep.push_back(&rep_bmp_titulo_02);
	vector_rep.push_back(&txt_bienvenida);
	vector_rep.push_back(&rep_caja_puntuaciones);
	vector_rep.push_back(&txt_puntuaciones_info);
	vector_rep.push_back(&txt_titulo_puntuaciones_local);
	vector_rep.push_back(&txt_titulo_puntuaciones_online);
	vector_rep.push_back(&txt_puntuaciones);
	vector_rep.push_back(&txt_puntuaciones_online);
	vector_rep.push_back(&rep_caja_menu);
	vector_rep.push_back(&rep_flecha_menu_izq);
	vector_rep.push_back(&rep_flecha_menu_der);

	//Hay varios estados dentro de menú y todos ellos necesitan de estos recursos.
	std::vector<DLibV::Representacion *> vector_aux;
	vector_aux.push_back(&rep_bmp_titulo_01);
	vector_aux.push_back(&rep_bmp_titulo_02);
	vector_aux.push_back(&txt_bienvenida);

	auto calcular_destino_nuevo=[](int ancho) -> int
	{
		return X_CAJA_MENU+( (W_CAJA_MENU - ancho) / 2);
	};


	//El puntero de animacion_menu se reusará cada vez que cambiemos de opción de menú.
	//La primera animación es "iniciar partida" entrando.

	vector_menu[seleccion_actual]->establecer_posicion(X_CAJA_MENU+W_CAJA_MENU, Y_ITEMS_MENU);

	Grupo_animacion * animacion_menu=new Grupo_animacion();
	animacion_menu->asignar_animacion(new Estructura_animacion_movimiento(
		vector_menu[seleccion_actual], 
		DLibH::Vector_2d(-VECTOR_X_MENU, 0), 
		calcular_destino_nuevo(vector_menu[seleccion_actual]->acc_ancho_area()), 
		Y_ITEMS_MENU));


	//Aquí se inicia la lógica de control.

	while(!salir)
	{
		//Si hay un hilo en marcha...
		if(hilo_puntuaciones && hilo_puntuaciones->es_finalizado())
		{
			DLibH::Hilo_SDL::completar_y_destruir_hilo(hilo_puntuaciones);
			//Como hemos terminado la carga, mostramos los resultados.
			this->auxiliar_recargar_texto_puntuaciones(this->lector_puntuaciones.acc_puntuaciones_online(), txt_puntuaciones_online, 400, 300);
		}

		if(animacion_menu && animacion_menu->es_finalizado())
		{
			delete animacion_menu;
			animacion_menu=NULL;
		}
		
		this->controles_sdl.recoger();

		//Salir del juego...
		//TODO: Con botón de joystick tb.
		if(this->controles_sdl.es_senal_salida() || this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
		{
			resultado=true;
			salir=true;
		}

		//Navegar por las paginas de maximas puntuaciones online...
		else if(
			this->controles_sdl.es_tecla_down(SDLK_UP) ||
			this->controles_sdl.es_tecla_down(SDLK_DOWN) ||
			this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JARRIBA) ||
			this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JABAJO))
		{
			if(!hilo_puntuaciones) // && this->puede_conectar())
			{
				unsigned int pagina_pre=this->pagina_puntuaciones_online;
				int direccion=this->controles_sdl.es_tecla_down(SDLK_DOWN) || this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JABAJO) ? 1 : -1;
				unsigned int pag_original=this->pasar_pagina_puntuaciones_online(direccion);

				if(pag_original != this->pagina_puntuaciones_online)
				{
					hilo_puntuaciones=new DLibH::Hilo_SDL();
					if(!hilo_puntuaciones->iniciar(hilo__recargar_puntuaciones_online, this))
					{
						this->pagina_puntuaciones_online=pagina_pre;
						DLibH::Hilo_SDL::destruir_hilo(hilo_puntuaciones);	
					}
				}
			}
		}
		//Sólo cuando no estamos cambiando de menú.
		else if(!animacion_menu)
		{
			if(
				this->controles_sdl.es_tecla_down(SDLK_LEFT) 
				|| this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JIZQUIERDA) 
				|| this->controles_sdl.es_tecla_down(SDLK_RIGHT) 
				|| this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JDERECHA)
			)
			{
				int direccion=(this->controles_sdl.es_tecla_down(SDLK_LEFT) || this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JIZQUIERDA)) ? -1 : 1;
				unsigned int original=seleccion_actual;
				seleccion_actual+=direccion;
		
				if(seleccion_actual < 0) seleccion_actual=M_MAX_MENU-1;
				else if(seleccion_actual==M_MAX_MENU) seleccion_actual=0;

				//Esto vamos a controlarlo a mano y punto. Con el perfil anónimo no puedes 
				//hacer estas cosas.
				if(es_perfil_anonimo)
				{
					switch(seleccion_actual)
					{
						case M_SELECCION_NIVEL:
						case M_RETOS:					
							seleccion_actual=direccion==1 ? M_INICIAR : M_AYUDA;
						break;
					}
				}
				
				unsigned int destino_nuevo=calcular_destino_nuevo(vector_menu[seleccion_actual]->acc_ancho_area());
				int vector=direccion==1 ? -1*VECTOR_X_MENU : VECTOR_X_MENU;

				//El original debe llegar justo hasta el borde izquierdo o derecho 266 es el origen de la caja, 257 es el ancho..
				unsigned int destino_original=direccion==1 
					? X_CAJA_MENU-vector_menu[original]->acc_ancho_area() 
					: X_CAJA_MENU+W_CAJA_MENU;

				//El nuevo debe salir justo del borde izquierdo o derecho.
				unsigned int origen_nuevo=direccion==1 
					? X_CAJA_MENU+W_CAJA_MENU 
					: X_CAJA_MENU-vector_menu[seleccion_actual]->acc_ancho_area();
		
				animacion_menu=new Grupo_animacion();

				vector_menu[seleccion_actual]->establecer_posicion(origen_nuevo, Y_ITEMS_MENU);
				animacion_menu->asignar_animacion(new Estructura_animacion_movimiento(vector_menu[original], DLibH::Vector_2d(vector, 0), destino_original, Y_ITEMS_MENU));
				animacion_menu->asignar_animacion(new Estructura_animacion_movimiento(vector_menu[seleccion_actual], DLibH::Vector_2d(vector, 0), destino_nuevo, Y_ITEMS_MENU));
			}
			else if(
				this->controles_teclado_raton.es_usar_artefacto(this->controles_sdl, Configuracion_controles::T_DOWN)
				|| this->controles_joystick.es_usar_artefacto(this->controles_sdl, Configuracion_controles::T_DOWN)
				|| this->controles_sdl.es_tecla_down(SDLK_RETURN) 
			)
			{
				switch(seleccion_actual)
				{
					case M_INICIAR:
						this->mostrar_animacion_abandonar_menu(rep_bmp_titulo_01, rep_bmp_titulo_02);
						this->estado=E_JUEGO;
						salir=true;
					break;

					case M_AYUDA:
						this->estado=E_INSTRUCCIONES;
						this->mostrar_animacion_abandonar_menu(rep_bmp_titulo_01, rep_bmp_titulo_02);
						salir=true;
					break;

					case M_TUTORIALES:
						this->controlador_perfiles.intercambiar_tutoriales_en_perfil();

						cadena_tutoriales=controlador_perfiles.es_mostrar_tutoriales() 
							? this->localizador.obtener(Localizador::C_MENU_PPAL_DESACTIVAR_TUTORIALES)
							: this->localizador.obtener(Localizador::C_MENU_PPAL_ACTIVAR_TUTORIALES);
						rep_menu_tutoriales.asignar(cadena_tutoriales);
						rep_menu_tutoriales.establecer_posicion(calcular_destino_nuevo(rep_menu_tutoriales.acc_ancho_area()), Y_ITEMS_MENU);

					break;

					case M_SELECCION_NIVEL: this->procesar_estado_seleccion_nivel(vector_aux); break;
					case M_CONTROLES: this->procesar_estado_configuracion_controles(vector_aux); break;
					case M_CONFIGURACION: this->procesar_estado_configuracion(vector_aux); break;
					case M_RETOS: this->procesar_estado_retos(vector_aux); break;
				}
			}
			
			menu_memoria_seleccion=seleccion_actual;

		}

		if(!salir)
		{
			this->mostrar_menu(vector_rep, animacion_menu, !animacion_menu ? vector_menu[seleccion_actual] : NULL, hilo_puntuaciones!=NULL);
			this->control_frames.turno();
		}
	}

	//Nunca dejamos el hilo abierto...
	if(hilo_puntuaciones) DLibH::Hilo_SDL::finalizar_y_destruir_hilo(hilo_puntuaciones);
	
	//Si la animación sigue en marcha, destruirla para no dejar la memoria por ahí.
	if(animacion_menu)
	{
		animacion_menu->forzar_finalizacion();
		delete animacion_menu;
		pantalla.reiniciar_clip_completo();
	}

	//Si devolvemos true aquí vamos a oscurecer finalmente la pantalla antes de salir.
	if(resultado) this->mostrar_salida_menu(vector_rep, hilo_puntuaciones!=NULL);

	return resultado;
}

/*Método de mostrado para el menú, muy genérico, sólo tiene de interesante el
tema de la animación: el menú puede estarse animando porque las opciones se
muevan al cambiar de una a otra...*/

void Controlador::mostrar_menu(std::vector<DLibV::Representacion *> &p_vector, 
		Grupo_animacion * animacion, 
		DLibV::Representacion * p_item_menu, bool cargando_online)
{
	//Si hay animación vamos a clipear en el futuro, así que desclipeamos ahora...
	if(animacion) pantalla.reiniciar_clip_completo();

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

	if(p_item_menu) p_item_menu->volcar(pantalla);

	this->rep_fps->asignar(DLibH::Herramientas::entero_a_cadena(this->control_frames.acc_frames_contados()));
	this->rep_fps->volcar(pantalla);

	if(animacion)
	{
		//El clip es para que el texto del menú venga de ninguna parte :D.	
		pantalla.establecer_clip(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA_MENU, Y_CAJA_MENU, W_CAJA_MENU, H_CAJA_MENU));
		animacion->turno(delta);
		animacion->mostrar(pantalla);

		//Si ha terminado reiniciamos el clip porque fuera de aquí se borrará.
		if(animacion->es_finalizado())
		{
			pantalla.reiniciar_clip_completo();
		}
	}

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

/*
void Controlador::modificar_trofeo_mostrado_menu(unsigned int p_indice, DLibV::Representacion_compuesta_estatica& rep)
{
	Reto * reto_apuntado=this->controlador_retos.obtener_por_indice(p_indice);
	rep.rellenar(0,0,0);

	if(this->controlador_perfiles.es_reto_conseguido(p_indice)) rep.volcar_representacion(*reto_apuntado->acc_representacion());
	else rep.volcar_representacion(*reto_apuntado->acc_representacion_off());
}
*/

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
