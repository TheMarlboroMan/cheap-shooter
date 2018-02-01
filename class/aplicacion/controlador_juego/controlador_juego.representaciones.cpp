/*Método maestro de representación. Llama a otros métodos auxiliares y hace
volcados propios en función del estado actual del juego.*/

void Controlador_juego::representar(float p_delta)
{
	this->campo_estrellas.acc_representacion()->volcar(this->pantalla.acc_superficie());

	bool tutorial_mostrable=tutorial.es_mostrable();

	if(! (pausado || tutorial_mostrable))
	{
		this->campo_estrellas.procesar_turno(p_delta);
	}

	switch(this->estado_actual)
	{
		case E_JUEGO:
		case E_FINALIZANDO_NIVEL:
		case E_PREPARANDO_JUGADOR_INICIAR_NIVEL:

			this->representar_entidades_mapa_fondo();			
			this->representar_entidades_jugador();
			this->representar_entidades_mapa_frente();
			this->representar_entidades_jugador_frente();
			this->representar_mirilla();
			this->representar_hud();

		break;

		case E_ANIMACION_FIN_JUEGO:
			this->representar_entidades_mapa_fondo();			
			this->representar_entidades_jugador(true);
			this->representar_entidades_mapa_frente();
		break;

		case E_NUEVA_PUNTUACION_MAXIMA:

			this->representar_entidades_mapa_fondo();
			this->representar_entidades_mapa_frente();

			rep_caja_fondo_puntuacion->volcar(pantalla);

			if(this->rep_txt_nueva_puntuacion_nombre->acc_cadena()!=this->escritor.acc_cadena())
				this->rep_txt_nueva_puntuacion_nombre->asignar(this->escritor.acc_cadena());

			if(this->posicion_local) this->rep_txt_nueva_puntuacion_local->volcar(pantalla);
			if(this->posicion_online) this->rep_txt_nueva_puntuacion_online->volcar(pantalla);

			this->rep_txt_nueva_puntuacion->volcar(pantalla);
			this->rep_txt_nueva_puntuacion_nombre->volcar(pantalla);
			this->representar_hud(true);
		break;

		case E_REINICIANDO_PRE_CUENTA_DESVANECIENDO:
		case E_REINICIANDO_NIVEL_DESVANECIENDO:
			this->representar_entidades_mapa_fondo();
			this->representar_entidades_mapa_frente();
			this->representar_hud(true);
		break;

		case E_GAME_OVER:

			this->representar_entidades_mapa_fondo();
			this->representar_entidades_mapa_frente();
			rep_caja_fondo_game_over->volcar(pantalla);
			rep_txt_gameover->volcar(pantalla);
		break;

		case E_FINALIZANDO_LOOP_PRINCIPAL:

			{ //Guardia para errores de cross-initialization.
				this->tiempo_cargando+=p_delta;
				float v=this->tiempo_cargando*100;
				this->rep_txt_cargando->ref_recurso()->establecer_alpha((int) v % 255);	
				this->rep_txt_cargando->volcar(pantalla);
			}
		break;

		case E_REINICIANDO_NIVEL: //Esto apenas dura un segundo...
		default: break;
	}

	//Lo del estado finalizando es porque puedes pausar y salir, y se quedaría la pausa marcada.
	if(this->pausado && this->estado_actual != E_FINALIZANDO_LOOP_PRINCIPAL)
	{
		float v=this->tiempo_pausa*100;
		this->rep_txt_pausa->ref_recurso()->establecer_alpha((int) v % 255);	
		this->rep_txt_pausa->volcar(pantalla);
	}

	//Tutoriales...
	if(tutorial_mostrable)
	{
		tutorial.acc_caja_texto().acc_representacion()->volcar(pantalla);
	}

	//Control de frames...
	std::string c1("");

	c1=DLibH::Herramientas::entero_a_cadena(J->acc_x())+
	" ,"+
	DLibH::Herramientas::entero_a_cadena(J->acc_y())+
	" "+
	DLibH::Herramientas::entero_a_cadena(this->control_frames.acc_frames_contados())+
	" L"+
	DLibH::Herramientas::float_a_cadena(ms_logica)+
	" R"+
	DLibH::Herramientas::float_a_cadena(ms_render)+
	" D"+
	DLibH::Herramientas::float_a_cadena(p_delta)+
	" E"+
	DLibH::Herramientas::entero_a_cadena(actores_mapa.size());

	this->rep_fps.asignar(c1);
	this->rep_fps.volcar(pantalla);

	//El log consume bastantes FPS, pero es temporal :).
	LOG_G.procesar();	
	LOG_G.acc_representacion().volcar(pantalla);

	//Trofeos...

	//Vemos si hay algún reto que mostrar y le pasamos el turno.
	Reto * r=this->controlador_retos.turno_controlador(p_delta);
	if(r)
	{
		r->acc_representacion()->volcar(pantalla);
	}	

	//Por si los hubiera...
	this->pantalla.procesar_procesos(p_delta);
	this->pantalla.flipar();
}

/*Representa todas las entidades relacionadas con el jugador: esto es, fantasma
y jugador. La mirilla va luego.*/

void Controlador_juego::representar_mirilla()
{
	//Artefactos mostrables...
	switch(this->J->acc_artefacto_actual())
	{
		case Definiciones::A_CANON:
			switch(this->esquema_control)
			{
				case C_TECLADO_RATON:
					this->M->establecer_estado_disparo(J->es_canon_usable());
					this->M->acc_representacion()->volcar(pantalla, this->camara);
				break;
	
				case C_GAMEPAD:
					if(this->M->es_activa_gamepad()) this->representar_puntero_gamepad(J->es_canon_usable());
				break;
			}
		break;

		case Definiciones::A_ESCUDO:

			switch(this->esquema_control)
			{
				case C_TECLADO_RATON:
					this->M->mostrar_como_escudo();
					this->M->acc_representacion()->volcar(pantalla, this->camara);
				break;
			}

		break;
	}
}

void Controlador_juego::representar_entidades_jugador(bool solo_jugador)
{
	if(!solo_jugador)
	{		
		//Artefactos mostrables...
		if(this->J->acc_artefacto_actual()==Definiciones::A_ESCUDO)
		{
			this->E->reiniciar_pasos_representacion();
			DLibV::Representacion * rep_escudo=this->E->acc_representacion();

			while(rep_escudo)
			{
				rep_escudo->volcar(pantalla, this->camara);
				rep_escudo=this->E->acc_representacion();
			}
		}
	}

	this->J->acc_representacion()->volcar(pantalla, this->camara);
}

void Controlador_juego::representar_entidades_jugador_frente()
{
	if(this->F->es_visible())
	{
		this->F->acc_representacion()->volcar(pantalla, this->camara);
	}
}

/*Método auxiliar: si jugamos con gamepad el puntero se representa como una 
línea. EL parámetro es para ver el color que vamos a usar en la línea.
Teniendo en cuenta que la línea del gamepad es una representación estática
vamos a ajustar la posiciones para compensar los niveles preparados (que tienen
desplazamiento real.*/

void Controlador_juego::representar_puntero_gamepad(bool preparado)
{
	float px1, py1, px2, py2;

	px1=this->J->acc_x()+(this->J->acc_w()/2);
	py1=-this->J->acc_y()-(this->J->acc_h()/2);

	px2=this->M->acc_x()+(this->M->acc_w()/2);
	py2=-this->M->acc_y()-(this->M->acc_h()/2);

	//Compensamos por la distancia de desplazamiento. Ojo, es un valor
	//negativo!!.
	int dist=(int) this->estructura_info_nivel.acc_distancia();

	py1+=dist;
	py2+=dist;

	DLibH::Linea linea=DLibH::Linea::crear_desde_puntos(px1, py1, px2, py2);

	//En función de la dirección...
	float dx1, dy1, dx2, dy2, dist1, dist2, defx, defy;

	if(linea.es_horizontal())
	{
		defx=px1 < px2 ? 799 : 0;
		defy=py1;
	}
	else if(linea.es_vertical())
	{
		defx=px1;
		defy=py1 > py2 ? -599 : 0;
	}
	else
	{	
		/*En este punto vamos a buscar la intersección de nuestra
		línea con los límites de la pantalla por ambos lados (arriba/abajo
		e izquierda/derecha). La línea más corta de las dos
		será la deseable porque no dibujaría nada fuera de la
		pantalla...*/

		dx1=px1 < px2 ? 799 : 0;
		dy1=linea.obtener_y_para_x(dx1);

		dy2=py1 > py2 ? -599 : 0;
		dx2=linea.obtener_x_para_y(dy2);

		dist1=abs(DLibH::Linea::distancia_entre_puntos(px1, py1, dx1, dy1));
		dist2=abs(DLibH::Linea::distancia_entre_puntos(px1, py1, dx2, dy2));

		if(dist1 < dist2)
		{
			defx=dx1;
			defy=dy1;
		}
		else
		{
			defx=dx2;
			defy=dy2;
		}
	}

	this->mirilla_gamepad.mut_puntos(px1, -py1, defx, -defy);

	if(preparado) this->mirilla_gamepad.mut_pixel_color(this->mirilla_gamepad_color_preparado);
	else this->mirilla_gamepad.mut_pixel_color(this->mirilla_gamepad_color_no_preparado);
	this->mirilla_gamepad.volcar(pantalla);
}

/*Representa a todas las entidades del mapa que no son el jugador: esto es,
actores, proyectiles y decoraciones. Esta representa todo lo que se pinta
por detrás del jugador.*/

void Controlador_juego::representar_entidades_mapa_fondo()
{
	//Una lamba con captura de this, para representarlo todo.

	auto comprobar_y_volcar=[this](Representable& rep)
	{
		if(rep.es_representable())
		{
			rep.acc_representacion()->volcar(this->pantalla, this->camara);
		}
	};

	auto comprobar_mostrable_y_volcar=[this](Actor_mapa * act)
	{
		if(act->es_representable() && act->es_en_pantalla())
		{
			this->actores_mapa_representables.push_back(act);
		}
	};

	/*Por el tema de que los actores de mapa pueden estar representados en 
	varias capas tenemos que hacer aquí un arreglo... Recorreremos todos
	los actores de mapa y meteremos en un vector a aquellos que quedarían
	dentro de lo mostrable. Luego ordenaremos ese vector por la pronfundidad
	y lo iremos lanzando.
	
	Cuando vayan a ser dibujados se comprobará de nuevo si quedan dentro
	de los límites, pero creo que no hace un impacto en el rendimiento.*/	

	//Primero lo limpiamos...
	this->actores_mapa_representables.clear();

	//Si hay algo en el mapa...
	if(this->actores_mapa.size())
	{
		for_each(this->actores_mapa.begin(), this->actores_mapa.end(), comprobar_mostrable_y_volcar);		

		if(this->actores_mapa_representables.size())
		{
			//Ahora los ordenamos para empezar a dibujar. Sólo lo hacemos cuando es nivel preparado...

			if(this->estructura_info_nivel.acc_modo_juego()==Estructura_info_nivel::M_PREPARADO)
			{
				Ordenador_actor_mapa_por_profundidad ord;
				sort(this->actores_mapa_representables.begin(), 
					this->actores_mapa_representables.end(),
					ord);
			}

			//Apuntamos el iterador. Hay un iterador porque 
			//aquí vamos a dibujar sólo dos bloques: el fondo y 
			//el normal. Cuando hayamos dibujado al jugador
			//podremos añadir el frente.

			it_actores_mapa_representables=this->actores_mapa_representables.begin();
			auto it_actores_mapa_representables_fin=this->actores_mapa_representables.end();

			while(it_actores_mapa_representables < it_actores_mapa_representables_fin 
				&& (*it_actores_mapa_representables)->acc_profundidad_representacion()!=Actor_mapa::P_FRENTE)
			{
				(* it_actores_mapa_representables)->acc_representacion()->volcar(pantalla, this->camara);
				++it_actores_mapa_representables;
			}
		}
	}

	if(this->proyectiles.size())
	{
		std::vector<Proyectil*>::iterator 	ini_pr=this->proyectiles.begin(),
							fin_pr=this->proyectiles.end();
		for(;ini_pr < fin_pr; ini_pr++) 
			comprobar_y_volcar(* static_cast<Representable *> ((*ini_pr)));
	}

	if(this->decoraciones.size())
	{
		std::vector<Decoracion*>::iterator 	ini_dc=this->decoraciones.begin(),
							fin_dc=this->decoraciones.end();
		for(;ini_dc < fin_dc; ini_dc++) 
			comprobar_y_volcar(* static_cast<Representable *> ((*ini_dc)));
	}
}

void Controlador_juego::representar_entidades_mapa_frente()
{
	if(this->actores_mapa_representables.size())
	{
		//Teníamos un iterador, volvemos a usarlo.
		auto it_actores_mapa_representables_fin=this->actores_mapa_representables.end();

		while(it_actores_mapa_representables < it_actores_mapa_representables_fin)
		{
			(* it_actores_mapa_representables)->acc_representacion()->volcar(pantalla, this->camara);
			++it_actores_mapa_representables;
		}
	}

	for(Decoracion * d : decoraciones_frente)
	{
		if(d->es_representable())
			d->acc_representacion()->volcar(this->pantalla, this->camara);
	}

}

/*Representa el  en la pantalla con la puntuación, vidas y el estado de los
diferentes artefactos.*/

void Controlador_juego::representar_hud(bool solo_vidas)
{

	//Tenemos unos objetos para las barras del HUD. Vamos a usarlos aquí.
	//Parar tiempo... Primero escogemos el color y la posicionamos... Luego
	//actualizamos sus valores y las mostramos. Pero antes mostramos también
	//una cadena en pantalla con las gemas por nivel...
	//Sobre el cálculo de la posición: ambas son "estáticas" de modo que
	//para calcular su posición relativa al jugador restaremos el offset de
	//la distancia recorrida en modo "preparado".

	if(!solo_vidas)
	{
		const unsigned int espacio_extra=2; 
		int hud_x=this->J->acc_x()+this->J->acc_posicion().w+espacio_extra;

		//Ojo, en modo "No preparado" esa distancia valdrá cero :). 
		int dist=camara.acc_caja().y;
		int hud_y=this->J->acc_y()+10-dist;

		Uint32 color=0;

		if(hud_x + Barra_hud::ANCHO > Medidas::U_ANCHO_VISTA) hud_x=this->J->acc_x()-Barra_hud::ANCHO-espacio_extra;

		hud_artefactos.establecer_posicion(hud_x+28, hud_y+1);
		hud_artefactos.acc_representacion().volcar(pantalla);

		if(this->estado_actual==E_JUEGO)
		{
			this->rep_txt_gemas_nivel->establecer_posicion(hud_x, hud_y);
			this->rep_txt_gemas_nivel->volcar(pantalla);
		}	

		hud_y+=10;

		color=!this->J->es_parar_tiempo_usable() ? Barra_hud::COLOR_NO_DISPONIBLE : Barra_hud::COLOR_DISPONIBLE;
		this->barra_tiempo.establecer_posicion(hud_x, hud_y);
		this->barra_tiempo.establecer_color_barra(color);
		this->barra_tiempo.establecer_valor_actual(this->J->acc_carga_parar_tiempo());
		this->barra_tiempo.actualizar();
		this->barra_tiempo.acc_representacion()->volcar(pantalla, this->camara);
		hud_y+=4;

		switch(this->J->acc_artefacto_actual())
		{
			case Definiciones::A_CANON:
				color=!this->J->es_canon_usable() ? Barra_hud::COLOR_NO_DISPONIBLE : Barra_hud::COLOR_DISPONIBLE;

				this->barra_canon.establecer_posicion(hud_x, hud_y);
				this->barra_canon.establecer_color_barra(color);
				this->barra_canon.establecer_valor_actual(this->J->acc_carga_canon());
				this->barra_canon.establecer_valor_auxiliar(this->J->acc_carga_canon_acumulada());
				this->barra_canon.actualizar();
				this->barra_canon.acc_representacion()->volcar(pantalla, this->camara);
			break;

			case Definiciones::A_EMISOR_DENSIDAD_UNIFICADA:
				color=!J->es_usable_edu() ? Barra_hud::COLOR_NO_DISPONIBLE : Barra_hud::COLOR_DISPONIBLE;
				this->barra_edu.establecer_posicion(hud_x, hud_y);
				this->barra_edu.establecer_color_barra(color);
				this->barra_edu.establecer_valor_actual(J->acc_cantidad_edu());
				this->barra_edu.actualizar();
				this->barra_edu.acc_representacion()->volcar(pantalla, this->camara);
			break;

			case Definiciones::A_ESCUDO:
				color=!this->E->es_usable() ? Barra_hud::COLOR_NO_DISPONIBLE : Barra_hud::COLOR_DISPONIBLE;
				this->barra_escudo.establecer_posicion(hud_x, hud_y);
				this->barra_escudo.establecer_color_barra(color);
				this->barra_escudo.establecer_valor_actual(this->E->acc_carga_escudo_actual());
				this->barra_escudo.actualizar();
				this->barra_escudo.acc_representacion()->volcar(pantalla, this->camara);
			break;
		}
	}

	unsigned int puntuacion_representada=this->CONTROLADOR_PUNTUACION.acc_puntuacion_representada();
	this->rep_txt_puntuacion->asignar(DLibH::Herramientas::entero_a_cadena(puntuacion_representada));
	this->rep_txt_vidas->asignar(DLibH::Herramientas::entero_a_cadena(this->J->acc_vidas()));
	
	this->rep_txt_puntuacion->volcar(pantalla);
	this->rep_txt_vidas->volcar(pantalla);

	//Cuando estamos iniciando un nuevo nivel se muestran las estadísticas del nivel anterior.
	if(this->estado_actual==E_PREPARANDO_JUGADOR_INICIAR_NIVEL) 
	{
		rep_caja_fondo_fin_nivel->volcar(pantalla);
		rep_fin_nivel->volcar(pantalla);
	}

	barra_combo.acc_representacion()->volcar(pantalla, this->camara);

	std::vector<Marcador_combo>::iterator 	ini=this->cola_combo.begin(),
						fin=this->cola_combo.end();

	while(ini<fin)
	{
		(*ini).acc_representacion().volcar(pantalla);
		ini++;
	}

	//Esto es una jilipollez... Pensé que los puntos estaban a la derecha y por eso crecería... Lo cambio por las vidas y ahi se queda!.
	hud_decoracion.establecer_cantidad_digitos(DLibH::Herramientas::digitos_en_entero(J->acc_vidas()));
	hud_decoracion.acc_representacion().volcar(pantalla);
}
