/* Cosas del loop de configuración...*/

std::vector<Item_configuracion> Controlador::preparar_vector_configuracion()
{
	std::vector<Item_configuracion> vector_config;
	std::vector<Item_configuracion>::iterator ini;

	//OJO: El orden REALMENTE importa. Deben estar en el orden del ENUM "config".
	vector_config.push_back(Item_configuracion(this->localizador.obtener(Localizador::C_CONFIG_IDIOMA)));
	vector_config.push_back(Item_configuracion(this->localizador.obtener(Localizador::C_CONFIG_METODO_CONTROL)));
//	vector_config.push_back(Item_configuracion(this->localizador.obtener(Localizador::C_CONFIG_PUNTUACIONES_ONLINE)));
	vector_config.push_back(Item_configuracion(this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_AUDIO)));
	vector_config.push_back(Item_configuracion(this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_MUSICA)));
	vector_config.push_back(Item_configuracion(this->localizador.obtener(Localizador::C_CONFIG_VIDEO_HARDWARE)));
	vector_config.push_back(Item_configuracion(this->localizador.obtener(Localizador::C_CONFIG_PANTALLA_COMPLETA)));

	//Configuración de idioma...
	vector_config[C_IDIOMA].insertar_opcion(0, this->localizador.obtener(Localizador::C_CONFIG_IDIOMA_00));
	vector_config[C_IDIOMA].insertar_opcion(1, this->localizador.obtener(Localizador::C_CONFIG_IDIOMA_01));
	vector_config[C_IDIOMA].establecer_actual(this->idioma);

	//Configuración de controles...
	vector_config[C_ESQUEMA_CONTROL].insertar_opcion(0, this->localizador.obtener(Localizador::C_CONFIG_METODO_CONTROL_00));
	vector_config[C_ESQUEMA_CONTROL].insertar_opcion(1, this->localizador.obtener(Localizador::C_CONFIG_METODO_CONTROL_01));
	vector_config[C_ESQUEMA_CONTROL].establecer_actual(this->esquema_control);

	//Configuracion de puntuaciones online...
//	vector_config[C_PUNTUACIONES_ONLINE].insertar_opcion(0, this->localizador.obtener(Localizador::C_CONFIG_PUNTUACIONES_ONLINE_00));
//	vector_config[C_PUNTUACIONES_ONLINE].insertar_opcion(1, this->localizador.obtener(Localizador::C_CONFIG_PUNTUACIONES_ONLINE_01));
//	vector_config[C_PUNTUACIONES_ONLINE].establecer_actual(this->conectar_online);

	//Configuracion de volumen de audio...
	vector_config[C_VOLUMEN_AUDIO].insertar_opcion(0, this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_AUDIO_00));
	vector_config[C_VOLUMEN_AUDIO].insertar_opcion(16, this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_AUDIO_01));
	vector_config[C_VOLUMEN_AUDIO].insertar_opcion(32, this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_AUDIO_02));
	vector_config[C_VOLUMEN_AUDIO].insertar_opcion(64, this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_AUDIO_03));
	vector_config[C_VOLUMEN_AUDIO].insertar_opcion(92, this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_AUDIO_04));
	vector_config[C_VOLUMEN_AUDIO].insertar_opcion(128, this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_AUDIO_05));
	vector_config[C_VOLUMEN_AUDIO].establecer_actual(this->volumen_audio);

	//Configuracion de volumen de música...
	vector_config[C_VOLUMEN_MUSICA].insertar_opcion(0, this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_AUDIO_00));
	vector_config[C_VOLUMEN_MUSICA].insertar_opcion(16, this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_AUDIO_01));
	vector_config[C_VOLUMEN_MUSICA].insertar_opcion(32, this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_AUDIO_02));
	vector_config[C_VOLUMEN_MUSICA].insertar_opcion(64, this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_AUDIO_03));
	vector_config[C_VOLUMEN_MUSICA].insertar_opcion(92, this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_AUDIO_04));
	vector_config[C_VOLUMEN_MUSICA].insertar_opcion(128, this->localizador.obtener(Localizador::C_CONFIG_VOLUMEN_AUDIO_05));
	vector_config[C_VOLUMEN_MUSICA].establecer_actual(this->volumen_musica);

	//Configuración de flags de video...
	vector_config[C_MODO_VIDEO].insertar_opcion(0, this->localizador.obtener(Localizador::C_CONFIG_VIDEO_HARDWARE_00));
	vector_config[C_MODO_VIDEO].insertar_opcion(1, this->localizador.obtener(Localizador::C_CONFIG_VIDEO_HARDWARE_01));
	vector_config[C_MODO_VIDEO].establecer_actual(this->flags_video & SDL_SWSURFACE ? 1 : 0);

	//Configuracion de flags de video (2)...
	vector_config[C_PANTALLA_COMPLETA].insertar_opcion(0, this->localizador.obtener(Localizador::C_CONFIG_PANTALLA_COMPLETA_00));
	vector_config[C_PANTALLA_COMPLETA].insertar_opcion(1, this->localizador.obtener(Localizador::C_CONFIG_PANTALLA_COMPLETA_01));
	vector_config[C_PANTALLA_COMPLETA].establecer_actual(this->flags_video & SDL_FULLSCREEN ? 1 : 0);

	return vector_config;
}

/*
Se procesa el estado de configuración. En primer lugar se crean una serie de
entidades temporales que se van a usar en este caso (textos y cosas de estas) y
luego entramos en el loop.
*/

void Controlador::procesar_estado_configuracion(std::vector<DLibV::Representacion *> & v_rep)
{
	bool salir=false, actualizar_cursor=false;
	int pos_cursor=Y_CURSOR_CONFIG, max_cursor=0, dimension_linea=0;

	//Caja de fondo.
	DLibV::Primitiva_grafica_caja_estatica rep_caja_fondo(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA_FONDO_CONFIG, Y_CAJA_FONDO_CONFIG, W_CAJA_FONDO_CONFIG, H_CAJA_FONDO_CONFIG),
		DLibV::Gestor_color::color(255, 255,255));
	rep_caja_fondo.establecer_alpha(64);

	//Generamos los textos de configuración. Vamos a organizarlos en dos
	//columnas: una para claves y otra para valores.

	DLibV::Representacion_texto_fija_estatica txt_claves(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "", 200, 500);
	txt_claves.establecer_posicion(X_CLAVES_CONFIG, Y_CONFIG);
	txt_claves.mut_interlineado(txt_claves.acc_alto_glifo() / 2);

	DLibV::Representacion_texto_fija_estatica txt_valores(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), "", 200, 500);
	txt_valores.establecer_posicion(X_VALORES_CONFIG, Y_CONFIG);	
	txt_valores.mut_interlineado(txt_valores.acc_alto_glifo() / 2);
	
	dimension_linea=txt_claves.acc_alto_glifo()+(txt_claves.acc_alto_glifo() / 2);

	DLibV::Primitiva_grafica_caja_estatica rep_cursor(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CURSOR_CONFIG, pos_cursor, W_CURSOR_CONFIG, H_CURSOR_CONFIG), 
		DLibV::Gestor_color::color(255, 255,255));
	rep_cursor.establecer_alpha(64);

	//Un texto de información pequeñito...
	DLibV::Representacion_texto_fija_estatica txt_info(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), "", 600, 100);
	txt_info.establecer_posicion(X_TXT_INFO_CONFIG, Y_TXT_INFO_CONFIG);
	txt_info.mut_interlineado(txt_valores.acc_alto_glifo() / 2);
	txt_info.asignar(this->localizador.obtener(Localizador::C_MENU_CONFIG));

	//Todos estos objetos van ahora a un vector. Incluimos los que hemos recibido.
	std::vector<DLibV::Representacion *> vector_rep;
	vector_rep.push_back(&rep_caja_fondo);
	vector_rep.push_back(&txt_claves);
	vector_rep.push_back(&txt_valores);
	vector_rep.push_back(&txt_info);
	vector_rep.push_back(&rep_cursor);
	for(DLibV::Representacion * r : v_rep) vector_rep.push_back(r);

	//Ahora prepararamos los objetos de configuración.
	std::vector<Item_configuracion> vector_config=this->preparar_vector_configuracion();
	std::vector<Item_configuracion>::iterator ini=vector_config.begin(), fin=vector_config.end();
	max_cursor=vector_config.size();

	bool volcar_textos=true;

	while(!salir)
	{
		float delta=this->control_frames.obtener_delta();
		this->campo_estrellas->procesar_turno(delta);

		this->controles_sdl.recoger();

		if(this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
		{
			salir=true;		
		}
		else if(this->controles_sdl.es_tecla_down(SDLK_DOWN))
		{
			pos_cursor++;
			actualizar_cursor=true;
		}
		else if(this->controles_sdl.es_tecla_down(SDLK_UP))
		{
			pos_cursor--;
			actualizar_cursor=true;
		}
		else if(this->controles_sdl.es_tecla_down(SDLK_LEFT))
		{
			vector_config[pos_cursor].cambiar_seleccion(-1);
			volcar_textos=true;
		}
		else if(this->controles_sdl.es_tecla_down(SDLK_RIGHT))
		{
			vector_config[pos_cursor].cambiar_seleccion(1);
			volcar_textos=true;
		}

		//Si procede, actualizar la posición del cursor...
		if(actualizar_cursor)
		{	
			if(pos_cursor >= max_cursor) pos_cursor=0;			
			else if(pos_cursor < 0)pos_cursor=max_cursor-1;

			rep_cursor.establecer_posicion(X_CURSOR_CONFIG, Y_CURSOR_CONFIG+(pos_cursor * dimension_linea));
			actualizar_cursor=false;
		}

		//Si procede, volcaremos los textos de configuración a las cadenas.

		if(volcar_textos)
		{
			std::string cadena_claves(""), cadena_valores("");
			ini=vector_config.begin();

			while(ini<fin)
			{
				cadena_claves+=(*ini).acc_titulo()+"\n";
				cadena_valores+=(*ini).acc_titulo_seleccionado()+"\n";
				ini++;
			}

			txt_claves.asignar(cadena_claves);
			txt_valores.asignar(cadena_valores);
			volcar_textos=false;
		}

		this->mostrar_estados_generico(vector_rep);
		this->control_frames.turno();
		//No vamos a guardar los datos del perfil: no tiene mucho sentido hacerlo aquí.
	}

	//Asignamos y guardamos los nuevos valores de configuración...
	this->configuracion.mut_esquema_control(vector_config[C_ESQUEMA_CONTROL].acc_valor_seleccionado());
//	this->configuracion.mut_conectar_online(vector_config[C_PUNTUACIONES_ONLINE].acc_valor_seleccionado());
	this->configuracion.mut_volumen_audio(vector_config[C_VOLUMEN_AUDIO].acc_valor_seleccionado());
	this->configuracion.mut_volumen_musica(vector_config[C_VOLUMEN_MUSICA].acc_valor_seleccionado());
	this->configuracion.mut_idioma(vector_config[C_IDIOMA].acc_valor_seleccionado());

	unsigned short int or_pantalla_completa=configuracion.acc_pantalla_completa();
	unsigned short int or_modo_hardware=configuracion.acc_modo_hardware();

	this->configuracion.mut_modo_hardware(vector_config[C_MODO_VIDEO].acc_valor_seleccionado());
	this->configuracion.mut_pantalla_completa(vector_config[C_PANTALLA_COMPLETA].acc_valor_seleccionado());
	this->configuracion.grabar();

	if(or_pantalla_completa != configuracion.acc_pantalla_completa()
		|| or_modo_hardware != configuracion.acc_modo_hardware())
	{
		componer_flags_video();
		pantalla.inicializar(Medidas::ANCHO_VISTA, Medidas::ALTO_VISTA, 0, flags_video);
	}

	//Aplicamos estos nuevos valores...
	this->obtener_datos_de_configuracion();
	this->aplicar_datos_de_configuracion();

	vector_config.clear();
}
