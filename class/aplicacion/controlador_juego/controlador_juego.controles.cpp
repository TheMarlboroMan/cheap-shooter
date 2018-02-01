void Controlador_juego::recoger_controles()
{
	if(this->debe_introducir_puntuacion) //Si hay que escribir la puntuación máxima queremos otro tipo de loop.
	{
		this->escritor.loop_control(); //El loop del escritor bombea, además, los eventos a los controles_SDL.
	}
	else
	{
		this->controles_sdl.recoger();
	}

	if(this->controles_sdl.es_senal_salida() || this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
	{
		cancelar_partida();
	}

	//TODO: Quitar en versión final.				
	else if(this->controles_sdl.es_tecla_down(SDLK_F4))
	{
		if(Actor_mapa::EN_EDITOR()) Actor_mapa::desactivar_en_editor();
		else Actor_mapa::activar_en_editor();
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_r))
	{
		localizador.recargar();
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_q))
	{
		LOG_G.intercambiar_visibilidad();
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_PAGEUP))
	{
		LOG_G.retroceder();
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_PAGEDOWN))
	{
		LOG_G.avanzar();
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_w))
	{
		J->intercambiar_invulnerabilidad_infinita();
	}
	//Controlamos aquí si perdemos el del input, para pausar el juego... Esto
	//puede pasar jugando con el ratón, si hacemos click fuera...
	else if(this->controles_sdl.es_evento_actividad())
	{
		//TODO: Reactivar... Lo tenemos así para poder usar debugueadores.
/*		if(
			this->controles_sdl.acc_estado_evento_actividad()==DLibI::Controles_SDL::E_FOCO_INPUT
			&& !this->controles_sdl.es_foco_evento_actividad()
			&& !this->pausado)
		{
			this->procesar_pausa();		
		}
*/
	}
	else if(this->configuracion_controles.es_pausa(this->controles_sdl, Configuracion_controles::T_DOWN))
	{
		this->procesar_pausa();
	}
}

/*Este método procesa los controles del modo de juego en si: volar, disparar,
cambiar de artefacto...
En el modo "preparando nivel" se puede mover con total normalidad y cambiar
de artefacto pero si devuelve true indicará que el nuevo nivel puede comenzar.

Se devuelve true cuando se intenta usar algún artefacto o habilidad. 
*/

bool Controlador_juego::procesar_controles_modo_juego(float p_delta)	
{	
	float input_x=0, input_y=0, input_eje_b_x=0, input_eje_b_y=0;
	unsigned short int input_tiempo=0, ciclar_artefacto=0;
	int raton_x=-1, raton_y=-1;	//-1 es el valor en que no hace nada.

	bool input_artefacto_pulsado=false;
	bool input_artefacto_down=false;
	bool input_artefacto_up=false;

	switch(this->esquema_control)
	{
		case C_GAMEPAD:

			this->procesar_controles_acciones_jugador(input_artefacto_up, input_artefacto_down, input_artefacto_pulsado, input_tiempo, ciclar_artefacto);
			this->procesar_controles_movimiento_jugador(input_x, input_y, input_eje_b_x, input_eje_b_y);
	
		break;

		case C_TECLADO_RATON:
			this->procesar_controles_acciones_jugador(input_artefacto_up, input_artefacto_down, input_artefacto_pulsado, input_tiempo, ciclar_artefacto);
			this->procesar_controles_movimiento_jugador(input_x, input_y, input_eje_b_x, input_eje_b_y);

			if(this->controles_sdl.es_movimiento_raton())
			{	
				raton_x=this->controles_sdl.acc_raton().acc_x();
				raton_y=this->controles_sdl.acc_raton().acc_y();
			}
		break;

		default: break;
	}

	//La señal del tiempo se procesa estés usando al fantasma o al propio
	//jugador. De este modo se puede combinar usar la teleportación con
	//detener el tiempo.

	this->J->recibir_senal_tiempo(input_tiempo);

	if(this->F->es_visible())
	{
		//Input para el fantasma...
		this->procesar_input_fantasma(input_x, input_y, p_delta, input_artefacto_up, input_artefacto_down, input_artefacto_pulsado);	
	}
	else
	{
		if(ciclar_artefacto) 
		{
			unsigned short int artefacto_siguiente=estructura_info_nivel.ciclar_artefacto(ciclar_artefacto, J->acc_artefacto_actual());
			J->establecer_artefacto_actual(artefacto_siguiente);
			actualizar_datos_hud_artefactos();
			procesar_post_seleccion_artefacto();
		}
		
		this->procesar_input_jugador(input_x, input_y, p_delta, input_artefacto_up, input_artefacto_down, input_artefacto_pulsado);

		//Cuando estamos en niveles preparados hay cambios en la forma
		//de procesar el input de la mirilla: sumamos el "offset" 
		//indicado por la estructura de nivel preparado. No comprobamos
		//el modo de juego, cuando no es nivel preparado simplemente 
		//vale 0.

		raton_y+=this->estructura_info_nivel.acc_distancia();
		
		this->procesar_input_mirilla(raton_x, raton_y, input_eje_b_x, input_eje_b_y);
	}

	return input_artefacto_pulsado || input_artefacto_down || input_artefacto_up;
}

/*Procesa el input recibido cuando el juego está en modo de "Nueva puntuación
máxima. Lo único que hace es esperar a que pulsemos enter para introducir
la nueva puntuación y luego cambiar al modo game over.*/

void Controlador_juego::procesar_controles_modo_nueva_puntuacion_maxima()
{
	if(this->debe_introducir_puntuacion)
	{
		if(this->controles_sdl.es_tecla_down(SDLK_RETURN) || this->controles_sdl.es_joystick_boton_down(JX))	//El Escritor_texto tiene un alias de this->controles de modo que ha ciclado normalmente.
		{
			this->enviar_nueva_puntuacion();
			this->estado_actual=E_GAME_OVER;
		}
	}
}

/*En el modo game over no hay nada que hacer salvo pulsar 
cualquier tecla o botón.*/

bool Controlador_juego::procesar_controles_modo_game_over()
{
	return this->configuracion_controles.es_usar_artefacto(this->controles_sdl, Configuracion_controles::T_DOWN);
}

bool Controlador_juego::procesar_controles_modo_entre_niveles()
{
	return this->configuracion_controles.es_usar_artefacto(this->controles_sdl, Configuracion_controles::T_DOWN);
}


/*Este modo procesa todos los comandos de movimiento introducidos para el 
jugador. Recibe por referencia un par de floats que son los que usarán luego
para controlar al jugador en si.*/

void Controlador_juego::procesar_controles_movimiento_jugador(float &input_x, float &input_y, float &input_eje_b_x, float &input_eje_b_y)
{
	float input_eje_a_x=0, input_eje_a_y=0; //, input_eje_b_x=0, input_eje_b_y=0;

	//Estos son los controles comunes a joystick y teclado...

	if(this->configuracion_controles.es_movimiento_derecha(this->controles_sdl, Configuracion_controles::T_PULSADO)) input_x+=1;
	if(this->configuracion_controles.es_movimiento_izquierda(this->controles_sdl, Configuracion_controles::T_PULSADO)) input_x-=1;
	if(this->configuracion_controles.es_movimiento_arriba(this->controles_sdl, Configuracion_controles::T_PULSADO)) input_y-=1;
	if(this->configuracion_controles.es_movimiento_abajo(this->controles_sdl, Configuracion_controles::T_PULSADO)) input_y+=1;			

	//Además, para joystick, controlamos también los ejes.
	if(this->esquema_control==C_GAMEPAD)
	{
		input_eje_a_x=this->controles_sdl.joystick_eje(0), 
		input_eje_a_y=this->controles_sdl.joystick_eje(1);
		input_eje_b_x=this->controles_sdl.joystick_eje(2), 
		input_eje_b_y=this->controles_sdl.joystick_eje(3);

		if(input_eje_a_x > 0) input_x=input_eje_a_x / DLibI::Controles_SDL::MAX_EJE;
		else if(input_eje_a_x < 0) input_x=input_eje_a_x / -DLibI::Controles_SDL::MIN_EJE;

		if(input_eje_a_y > 0) input_y=input_eje_a_y / DLibI::Controles_SDL::MAX_EJE;
		else if(input_eje_a_y < 0) input_y=input_eje_a_y / -DLibI::Controles_SDL::MIN_EJE;

		if(input_eje_b_x > 0) input_eje_b_x/=DLibI::Controles_SDL::MAX_EJE;
		else if(input_eje_b_x < 0) input_eje_b_x/=-DLibI::Controles_SDL::MIN_EJE;

		if(input_eje_b_y > 0) input_eje_b_y /= DLibI::Controles_SDL::MAX_EJE;
		else if(input_eje_b_y < 0) input_eje_b_y /= -DLibI::Controles_SDL::MIN_EJE;
	}
}

/*Procesa cualquier acción del jugador que no sea movimiento: activar artefactos,
armas, controlar el tiempo o ciclar entre artefactos.*/

void Controlador_juego::procesar_controles_acciones_jugador(bool &input_artefacto_up, bool &input_artefacto_down, bool &input_artefacto_pulsado, unsigned short int &input_tiempo, unsigned short int &ciclar_artefacto)
{
	//Esto queda completamente unificado entre teclado y joystick :).

	if(this->configuracion_controles.es_parar_tiempo(this->controles_sdl, Configuracion_controles::T_DOWN)) input_tiempo=1;
	else if(this->configuracion_controles.es_parar_tiempo(this->controles_sdl, Configuracion_controles::T_UP)) input_tiempo=2;  

	if(this->configuracion_controles.es_usar_artefacto(this->controles_sdl, Configuracion_controles::T_DOWN)) input_artefacto_down=true;
	else if(this->configuracion_controles.es_usar_artefacto(this->controles_sdl, Configuracion_controles::T_UP)) input_artefacto_up=true;
	else if(this->configuracion_controles.es_usar_artefacto(this->controles_sdl, Configuracion_controles::T_PULSADO)) input_artefacto_pulsado=true;

	if(this->configuracion_controles.es_ciclar_artefacto_siguiente(this->controles_sdl, Configuracion_controles::T_DOWN)) ciclar_artefacto=1;
	if(this->configuracion_controles.es_ciclar_artefacto_anterior(this->controles_sdl, Configuracion_controles::T_DOWN)) ciclar_artefacto=-1;		
}


/*Este modo procesa el input de la mirilla en función del tipo de control que
tengamos activado. Con teclado y ratón movemos la mirilla con el puntero del 
ratón siempre que el valor recibido sea distinto de -1 (su valor por defecto
cuando se declara. En modo gamepad colocamos la mirilla en cierta posición
alrededor del jugador según el input recibido. Cuando no se recibe nada se
marca como "desactivada" simplemente para no mostrarla luego.*/

void Controlador_juego::procesar_input_mirilla(int p_raton_x, int p_raton_y, float input_x, float input_y)
{
	switch(this->esquema_control)
	{
		case C_GAMEPAD:
			if(input_x || input_y)
			{
				this->M->gamepad_activo(true);
		
	//Vamos a hacer que la mirilla almacene los valores del input del 
	//gamepad. Si lo procesamos ahora mismo aún no se ha procesado el 
	//turno del jugador y terminamos mirando al punto justo anterior. 
	//Eso es lo que hay en este código comentado.
	
				//this->M->ir_a( 
				//	(this->J->acc_x()+(this->J->acc_w()/2))-(this->M->acc_w()/2)+(64*input_x), 
				//	(this->J->acc_y()+(this->J->acc_h()/2))-(this->M->acc_h()/2)+(64*input_y)
				//);

				this->M->guardar_valores_gamepad(input_x, input_y);
			}
			else
			{
				this->M->gamepad_activo(false);
			}
		break;

		case C_TECLADO_RATON:

			//p_raton_y ya vendría con el offset y si se aplica en niveles en movimiento.

			if(p_raton_x != -1 && p_raton_y!=-1)
			{ 
				this->M->ir_a(p_raton_x, p_raton_y);
			}
		break;

		default: break;
	}		
}

/*Igual que el modo de jugador, pero sólo cuando este está en modo fantasma.*/

void Controlador_juego::procesar_input_fantasma(int input_x, int input_y, float p_delta, bool input_artefacto_up, bool input_artefacto_down, bool input_artefacto_pulsado)
{
	this->F->recibir_input(input_x, input_y, p_delta);
	this->J->recibir_input(0, 0, p_delta);

	if(input_artefacto_up && this->J->recibir_senal_artefacto_up())
	{
		switch(this->J->acc_artefacto_actual())
		{					
			case Definiciones::A_TELETRANSPORTADOR: this->finalizar_fantasma(); break;
			default: break;
		}
	}
}

/*Procesa el input que ha recibido el jugador... Básicamente implica realizar
las acciones con los artefactos.*/

void Controlador_juego::procesar_input_jugador(int input_x, int input_y, float p_delta, bool input_artefacto_up, bool input_artefacto_down, bool input_artefacto_pulsado)
{
	this->J->recibir_input(input_x, input_y, p_delta);

	if(this->J->puede_generar_chispas_propulsor()) this->generar_chispas_propulsor();

	//Las acciones de artefacto sólo se procesan en el modo de juego y salida de nivel.

	if(input_artefacto_down && this->J->recibir_senal_artefacto_down() 
		&& (this->estado_actual==E_JUEGO || this->estado_actual==E_FINALIZANDO_NIVEL ))
	{			
		switch(this->J->acc_artefacto_actual())
		{
			case Definiciones::A_CANON: J->iniciar_carga_canon(); break;
			case Definiciones::A_TELETRANSPORTADOR: this->iniciar_fantasma(); break;
			case Definiciones::A_EMISOR_DENSIDAD_UNIFICADA: this->activar_edu(); break;
			case Definiciones::A_ESCUDO: this->activar_escudo(); break;
			default: break;
		}
	}
	else if(input_artefacto_up && this->J->recibir_senal_artefacto_up())
	{
		switch(this->J->acc_artefacto_actual())
		{
			case Definiciones::A_CANON: this->generar_proyectil_jugador(); break;
			case Definiciones::A_ESCUDO: this->desactivar_escudo(); break;
			//OJO: El input para el fantasma está en procesar_input_fantasma.
		}
	}	
/*	{
	else if(input_artefacto_pulsado){}
	*/
}	
