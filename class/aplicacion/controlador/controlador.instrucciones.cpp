void Controlador::procesar_estado_instrucciones()
{
	bool salir=false;
	float delta=0;
	const short int total_paginas=this->instrucciones.size();
	short int pagina_actual=0;

	//Reiniciar la posición de todos los objetos...

	std::vector<Estructura_instrucciones>::iterator ini=instrucciones.begin(), 
							fin=instrucciones.end();

	while(ini < fin)
	{
		(*ini).acc_representacion().establecer_posicion(0,0);
		++ini;
	}

	std::vector<DLibV::Representacion *> vector_rep;
	vector_rep.push_back(& (this->instrucciones[pagina_actual].acc_representacion()));

	while(!salir)
	{
		delta=this->control_frames.obtener_delta();
		this->campo_estrellas->procesar_turno(delta);

		this->controles_sdl.recoger();

		if(this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
		{
			this->mostrar_animacion_salir_instrucciones(pagina_actual);
			salir=true;		
		}
		else if(this->controles_sdl.es_tecla_down(SDLK_LEFT))
		{
			pagina_actual--;
			if(pagina_actual < 0) pagina_actual=total_paginas-1;
			this->mostrar_animacion_pasar_pagina_instrucciones(pagina_actual, -1);

			vector_rep.clear();
			vector_rep.push_back(& (this->instrucciones[pagina_actual].acc_representacion()));

		}
		else if(this->controles_sdl.es_tecla_down(SDLK_RIGHT))
		{
			pagina_actual++;
			if(pagina_actual==total_paginas) pagina_actual=0;
			this->mostrar_animacion_pasar_pagina_instrucciones(pagina_actual, 1);

			vector_rep.clear();
			vector_rep.push_back(& (this->instrucciones[pagina_actual].acc_representacion()));
		}	

		this->campo_estrellas->acc_representacion()->volcar(pantalla);
		this->mostrar_estados_generico(vector_rep);
		this->control_frames.turno();
	}
}

void Controlador::mostrar_animacion_salir_instrucciones(unsigned int p_pag)
{
	//Generar animaciones..
	Grupo_animacion grupo=Grupo_animacion();
	grupo.asignar_animacion(new Estructura_animacion_movimiento(&this->instrucciones[p_pag].acc_representacion(), DLibH::Vector_2d(0, Medidas::ALTO_VISTA), 0, Medidas::ALTO_VISTA));

	float delta=0;

	//Mientras la animación no haya finalizado...
	while(!grupo.es_finalizado())
	{
		delta=this->control_frames.obtener_delta();

		//Volcar el campo de estrellas.
		this->campo_estrellas->procesar_turno(delta);
		this->campo_estrellas->acc_representacion()->volcar(pantalla);
	
		grupo.turno(delta);
		grupo.mostrar(this->pantalla);

		this->pantalla.flipar();
		this->control_frames.turno();
	}
}

void Controlador::mostrar_animacion_pasar_pagina_instrucciones(unsigned int p_nueva_pag, short int p_dir)
{
	const short int total_paginas=this->instrucciones.size();
	short int indice_saliente=p_nueva_pag-p_dir;
	short int indice_entrante=p_nueva_pag;

	if(indice_entrante < 0) indice_entrante=total_paginas-1;
	else if(indice_entrante == total_paginas) indice_entrante=0;

	if(indice_saliente < 0) indice_saliente=total_paginas-1;
	else if(indice_saliente == total_paginas) indice_saliente=0;

	float vel=0;
	float x_origen_entrante=0;
	float x_destino_saliente=0;

	if(p_dir < 0)
	{
		vel=700;
		x_origen_entrante=-Medidas::ANCHO_VISTA;
		x_destino_saliente=Medidas::ANCHO_VISTA;
	}
	else
	{
		vel=-700;
		x_origen_entrante=Medidas::ANCHO_VISTA;
		x_destino_saliente=-Medidas::ANCHO_VISTA;
	}

	this->instrucciones[indice_entrante].acc_representacion().establecer_posicion(x_origen_entrante, 0);

	//Generar animaciones..
	Grupo_animacion grupo=Grupo_animacion();
	
	//El entrante SIEMPRE va a cero.
	grupo.asignar_animacion(new Estructura_animacion_movimiento(&this->instrucciones[indice_entrante].acc_representacion(), DLibH::Vector_2d(vel, 0), 0, 0));
	grupo.asignar_animacion(new Estructura_animacion_movimiento(&this->instrucciones[indice_saliente].acc_representacion(), DLibH::Vector_2d(vel, 0), x_destino_saliente, 0));

	float delta=0;

	//Mientras la animación no haya finalizado...
	while(!grupo.es_finalizado())
	{
		delta=this->control_frames.obtener_delta();

		//Volcar el campo de estrellas.
		this->campo_estrellas->procesar_turno(delta);
		this->campo_estrellas->acc_representacion()->volcar(pantalla);
	
		grupo.turno(delta);
		grupo.mostrar(this->pantalla);

		this->pantalla.flipar();
		this->control_frames.turno();
	}
}

void Controlador::preparar_instrucciones()
{
	DLibV::Recurso_grafico * fuente=	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1);
	DLibV::Recurso_grafico * tile_jugador=	DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUGADOR);

	//Preparar la página 1.

	//Imágenes...
	DLibV::Representacion_bitmap_dinamica foto_01_01=DLibV::Representacion_bitmap_dinamica(tile_jugador);
	foto_01_01.establecer_recorte(0,0, 32, 32);

	//Textos...
	DLibV::Representacion_texto_auto_dinamica txt_01_01=DLibV::Representacion_texto_auto_dinamica(fuente, this->localizador.obtener(Localizador::C_INSTRUCCIONES_01_01)); 
	DLibV::Representacion_texto_auto_dinamica txt_01_02=DLibV::Representacion_texto_auto_dinamica(fuente, this->localizador.obtener(Localizador::C_INSTRUCCIONES_01_02)); 
	DLibV::Representacion_texto_auto_dinamica txt_01_03=DLibV::Representacion_texto_auto_dinamica(fuente, this->localizador.obtener(Localizador::C_INSTRUCCIONES_01_03)); 

	//Composición.
	this->instrucciones.push_back(Estructura_instrucciones());
	this->instrucciones[0].componer(txt_01_01, DLibH::Herramientas_SDL::nuevo_sdl_rect(32, 10, 400, 32));
	this->instrucciones[0].componer(foto_01_01, DLibH::Herramientas_SDL::nuevo_sdl_rect(400, 10, 32, 32));

	this->instrucciones.push_back(Estructura_instrucciones());
	this->instrucciones[1].componer(txt_01_02, DLibH::Herramientas_SDL::nuevo_sdl_rect(32, 10, 400, 32));
	this->instrucciones[1].componer(foto_01_01, DLibH::Herramientas_SDL::nuevo_sdl_rect(400, 10, 32, 32));

	this->instrucciones.push_back(Estructura_instrucciones());
	this->instrucciones[2].componer(txt_01_03, DLibH::Herramientas_SDL::nuevo_sdl_rect(32, 10, 400, 32));
	this->instrucciones[2].componer(foto_01_01, DLibH::Herramientas_SDL::nuevo_sdl_rect(400, 10, 32, 32));
}
