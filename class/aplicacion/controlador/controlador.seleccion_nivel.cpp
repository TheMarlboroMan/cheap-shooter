/*El estado de selección de nivel se apoya en los perfiles: dentro del perfil
se guarda la información de qué niveles se han finalizado y con qué 
estadisticas.

Los niveles aleatorios no muestran estadísticas de porcentajes.
*/

void Controlador::procesar_estado_seleccion_nivel(std::vector<DLibV::Representacion *> & v_rep)
{
	//Generar los elementos gráficos...	
	DLibV::Primitiva_grafica_caja_estatica rep_caja_fondo(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 258, 800, 302), 
		DLibV::Gestor_color::color(255, 255,255));
	rep_caja_fondo.establecer_alpha(64);

	DLibV::Representacion_bitmap_estatica rep_flecha_izq(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_MENU));
	rep_flecha_izq.establecer_recorte(13,0,13,25);
	rep_flecha_izq.establecer_posicion(15, 396);

	DLibV::Representacion_bitmap_estatica rep_flecha_der(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_MENU));
	rep_flecha_der.establecer_recorte(0,0,13,25);
	rep_flecha_der.establecer_posicion(774, 396);

	//Todo lo representable va en este vector.
	std::vector<DLibV::Representacion *> vector_rep;

	//Lo primero es añadir lo que hemos recibido.
	for(DLibV::Representacion * r : v_rep) vector_rep.push_back(r);

	//Y ahora añadir lo que ya hay.
	vector_rep.push_back(&rep_caja_fondo);
	vector_rep.push_back(&rep_flecha_der);
	vector_rep.push_back(&rep_flecha_izq);

	//Vamos a crear todas las representaciones de nivel que
	//correspondan. La selección del nivel 1 está SIEMPRE disponible, pase
	//lo que pase.

	std::vector<Estructura_seleccion_nivel *> vector_nivel;
	int i=0;
	int x=X_SELECCION_NIVEL_INICIO;

	int max_pag=controlador_perfiles.acc_cuenta_niveles_finalizados()+1; //El primer nivel está siempre accesible.
	if(max_pag==Definicion_retos::TOTAL_NIVELES) --max_pag; //Restamos uno si la operación anterior se ha pasado.

	for(; i < max_pag; ++i) 
	{
		Estructura_seleccion_nivel * temp=new Estructura_seleccion_nivel(i, x);
		x+=SALTO_X_SELECCION_NIVEL;
		Perfil_datos::Info_nivel in;
		try{in=controlador_perfiles.obtener_datos_nivel(i);}
		catch(Perfil_datos::Info_nivel_excepcion e) {}	//En el peor de los casos se rellenan en blanco.

		temp->actualizar(in, localizador);
		vector_nivel.push_back(temp);
	}

	//Ahora a por los niveles secretos.
	for(i=Definicion_retos::TOTAL_NIVELES; i<Definicion_retos::TOTAL_NIVELES+Definicion_retos::TOTAL_NIVELES_SECRETOS; ++i)
	{
		Perfil_datos::Info_nivel in;
		try{in=controlador_perfiles.obtener_datos_nivel(i);}
		catch(Perfil_datos::Info_nivel_excepcion e) {}	//En el peor de los casos se rellenan en blanco.

		if(in.es_finalizado())
		{
			Estructura_seleccion_nivel * temp=new Estructura_seleccion_nivel(i, x);
			x+=SALTO_X_SELECCION_NIVEL;
			temp->actualizar(in, localizador);
			vector_nivel.push_back(temp);
			++max_pag;
		}
	}


	Grupo_animacion * animacion=NULL;
	bool salir=false;

	int pag=0;

	while(!salir)
	{
		float delta=this->control_frames.obtener_delta();
		this->campo_estrellas->procesar_turno(delta);

		if(animacion && animacion->es_finalizado())
		{
			delete animacion;
			animacion=NULL;
		}
		
		this->controles_sdl.recoger();

		//TODO: Un botón de joystick para volver.
		if(this->controles_sdl.es_senal_salida() || this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
		{
			salir=true;
		}

		if(animacion)
		{
			animacion->turno(delta);
		}
		//Sólo cuando no estamos en movimiento.
		else
		{
			if(
				this->controles_sdl.es_tecla_down(SDLK_LEFT) 
				|| this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JIZQUIERDA) 
				|| this->controles_sdl.es_tecla_down(SDLK_RIGHT) 
				|| this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JDERECHA)
			)
			{
				int direccion=(this->controles_sdl.es_tecla_down(SDLK_LEFT) || this->controles_sdl.es_joystick_boton_pulsado(Controlador_juego::JIZQUIERDA)) ? -1 : 1;
				pag+=direccion;
		
				if(pag < 0 || pag==max_pag) //Topes del listado.
				{
					pag-=direccion;
				}
				else
				{
					animacion=new Grupo_animacion();
					for(Estructura_seleccion_nivel * r : vector_nivel) animacion->asignar_animacion(r->generar_animacion(direccion));
				}
			}
			else if(
				this->controles_teclado_raton.es_usar_artefacto(this->controles_sdl, Configuracion_controles::T_DOWN)
				|| this->controles_joystick.es_usar_artefacto(this->controles_sdl, Configuracion_controles::T_DOWN)
				|| this->controles_sdl.es_tecla_down(SDLK_RETURN) 
			)
			{
				entrenando=true;
	
				nivel_inicio=vector_nivel[pag]->acc_nivel();
				mostrar_animacion_nivel_seleccionado(1, vector_rep, vector_nivel[pag]);
				iniciar_y_procesar_juego();
				mostrar_animacion_nivel_seleccionado(-1, vector_rep, vector_nivel[pag]);

				//Cuando se termina el estado de juego se actualiza la info.
				Perfil_datos::Info_nivel in;
				try{in=controlador_perfiles.obtener_datos_nivel(nivel_inicio);}
				catch(Perfil_datos::Info_nivel_excepcion e) {}	//En el peor de los casos se rellenan en blanco.
				vector_nivel[pag]->actualizar(in, localizador);
			}
		}

		if(!salir)
		{
			this->mostrar_seleccion_nivel(vector_rep, vector_nivel);
			this->control_frames.turno();
		}
	}

	//Si la animación sigue en marcha, destruirla para no dejar la memoria por ahí.
	if(animacion)
	{
		animacion->forzar_finalizacion();
		delete animacion;
		pantalla.reiniciar_clip_completo();
	}

	for(Estructura_seleccion_nivel * r : vector_nivel) delete r;
}

void Controlador::mostrar_animacion_nivel_seleccionado(int p_dir, std::vector<DLibV::Representacion *> &v_rep, Estructura_seleccion_nivel * niv)
{
	float delta;
	int vector=p_dir==1 ? VECTOR_Y_SELECCION_NIVEL : -VECTOR_Y_SELECCION_NIVEL, x=0, y=0, desp=Medidas::ALTO_VISTA * p_dir;
	Grupo_animacion animacion;

	for(DLibV::Representacion * r : v_rep) 
	{
		x=r->acc_posicion().x; 
		y=r->acc_posicion().y+desp;
		animacion.asignar_animacion(new Estructura_animacion_movimiento(r, DLibH::Vector_2d(0, vector), x, y));
	}

	x=niv->acc_representacion()->acc_posicion().x; 
	y=niv->acc_representacion()->acc_posicion().y+desp;
	animacion.asignar_animacion(new Estructura_animacion_movimiento(niv->acc_representacion(), DLibH::Vector_2d(0, vector), x, y));
	
	while(!animacion.es_finalizado()) 
	{
		animacion.turno(delta);

		delta=this->control_frames.obtener_delta();
		this->campo_estrellas->procesar_turno(delta);

		this->campo_estrellas->acc_representacion()->volcar(this->pantalla);
		for(DLibV::Representacion * r : v_rep) r->volcar(pantalla);
		niv->acc_representacion()->volcar(pantalla);

		this->pantalla.flipar();
	}
}

void Controlador::mostrar_seleccion_nivel(std::vector<DLibV::Representacion *> & v_rep, std::vector<Estructura_seleccion_nivel *> & v_nivel)
{
	this->campo_estrellas->acc_representacion()->volcar(this->pantalla);

	for(DLibV::Representacion * r : v_rep) r->volcar(pantalla);
	pantalla.establecer_clip(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CLIP_SELECCION_NIVEL, Y_CLIP_SELECCION_NIVEL, W_CLIP_SELECCION_NIVEL, H_CLIP_SELECCION_NIVEL));
	for(Estructura_seleccion_nivel *  r : v_nivel) r->acc_representacion()->volcar(pantalla);
	pantalla.reiniciar_clip_completo();

	this->pantalla.flipar();
}

Controlador::Estructura_seleccion_nivel::Estructura_seleccion_nivel(unsigned int p_nivel, int px):
	nivel(p_nivel)
{
	representacion=new DLibV::Representacion_compuesta_estatica(W_REP_SELECCION_NIVEL, H_REP_SELECCION_NIVEL, 0, 255, 255);
	representacion->establecer_clave_color(0, 255, 255);
	representacion->establecer_posicion(px, Y_SELECCION_NIVEL_INICIO);
}

Controlador::Estructura_seleccion_nivel::~Estructura_seleccion_nivel()
{
	delete representacion;	
}

void Controlador::Estructura_seleccion_nivel::actualizar(Perfil_datos::Info_nivel& in, Localizador& localizador)
{
	representacion->rellenar(0, 255, 255);

	unsigned int p_nivel=(unsigned int)in.nivel;

	std::string ver_fecha_fin=in.es_finalizado() ? 
		DLibH::Herramientas::entero_a_cadena((int)in.dia_finalizacion)+"-"+
		DLibH::Herramientas::entero_a_cadena((int)in.mes_finalizacion)+"-"+
		DLibH::Herramientas::entero_a_cadena((int)in.anho_finalizacion) 
		: localizador.obtener(Localizador::C_SELECCION_NIVEL_SIN_FINALIZAR);

	//Esto es engañoso, pero nos vale :D!.
	bool es_nivel_preparado=(int)in.porcentaje_recogible || (int)in.porcentaje_destruible;

	std::string nombre_nivel=Controlador_juego::obtener_nombre_nivel(localizador, p_nivel);

	std::string texto_titulos=localizador.obtener(Localizador::C_SELECCION_NIVEL_NIVEL)+" "+
DLibH::Herramientas::entero_a_cadena(p_nivel+1)+": "+nombre_nivel+"\n\n"+
localizador.obtener(Localizador::C_SELECCION_NIVEL_MAX_PUNTUACION)+"\n"+
localizador.obtener(Localizador::C_SELECCION_NIVEL_VECES_INICIADO)+"\n"+
localizador.obtener(Localizador::C_SELECCION_NIVEL_VIDAS_PERDIDAS)+"\n"+
localizador.obtener(Localizador::C_SELECCION_NIVEL_FECHA_FIN)+"\n";

	if(es_nivel_preparado)
	{
		texto_titulos+=localizador.obtener(Localizador::C_SELECCION_NIVEL_ITEMS)+"\n"+
		localizador.obtener(Localizador::C_SELECCION_NIVEL_DESTRUIDOS);
	}

	std::string texto_valores="\n\n"+
	DLibH::Herramientas::entero_a_cadena((int)in.maxima_puntuacion)+"\n"+
DLibH::Herramientas::entero_a_cadena((int)in.veces_iniciado)+"\n"+
DLibH::Herramientas::entero_a_cadena((int)in.vidas_perdidas)+"\n"+
ver_fecha_fin+"\n";

	if(es_nivel_preparado)
	{
		texto_valores+=DLibH::Herramientas::entero_a_cadena((int)in.porcentaje_recogible)+"%\n"+
		DLibH::Herramientas::entero_a_cadena((int)in.porcentaje_destruible)+"%";
	}


	//Componemos ahora las representaciones...
	DLibV::Representacion_texto_fija_estatica titulos(
			DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), 
			texto_titulos, W_VALOR_SEL_NIVEL, H_VALOR_SEL_NIVEL);

	DLibV::Representacion_texto_fija_estatica valores(
			DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), 
			texto_valores, W_VALOR_SEL_NIVEL, H_VALOR_SEL_NIVEL);

	//Además, hay una imagen.
	DLibV::Representacion_bitmap_dinamica imagen(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_RESUMEN_NIVELES));
	imagen.establecer_recorte(X_IMAGEN_SEL_NIVEL, p_nivel * H_IMAGEN_SEL_NIVEL, W_IMAGEN_SEL_NIVEL, H_IMAGEN_SEL_NIVEL);
  
	//Cada uno de estos elementos va en una posición...
	SDL_Rect caja_imagen=DLibH::Herramientas_SDL::nuevo_sdl_rect(X_IMAGEN_SEL_NIVEL, Y_IMAGEN_SEL_NIVEL, W_IMAGEN_SEL_NIVEL, H_IMAGEN_SEL_NIVEL);
	SDL_Rect caja_texto_titulos=DLibH::Herramientas_SDL::nuevo_sdl_rect(X_TITULO_SEL_NIVEL, Y_TITULO_SEL_NIVEL, W_TITULO_SEL_NIVEL, H_TITULO_SEL_NIVEL);
	SDL_Rect caja_texto_valores=DLibH::Herramientas_SDL::nuevo_sdl_rect(X_VALOR_SEL_NIVEL, Y_VALOR_SEL_NIVEL, W_VALOR_SEL_NIVEL, H_VALOR_SEL_NIVEL);

	//Unirlo todo...
	representacion->volcar_representacion(imagen, caja_imagen);	
	representacion->volcar_representacion(titulos, caja_texto_titulos);
	representacion->volcar_representacion(valores, caja_texto_valores);
}

Estructura_animacion_movimiento * Controlador::Estructura_seleccion_nivel::generar_animacion(int direccion)
{
	int vector=direccion==1 ? -VECTOR_X_SELECCION_NIVEL : VECTOR_X_SELECCION_NIVEL;
	int x=representacion->acc_posicion().x+(-direccion * SALTO_X_SELECCION_NIVEL);
	int y=representacion->acc_posicion().y;
	return new Estructura_animacion_movimiento(representacion, DLibH::Vector_2d(vector, 0), x, y);
}
