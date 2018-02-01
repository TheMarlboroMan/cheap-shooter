/*
Este método sólo se va a ejecutar una vez por cada instancia del Controlador
de juego que se cree desde el Controlador. Por así decirlo, esto representa a
cada una de las partidas que vayamos a echar.
*/

/*
ESTRUCTURA DEL LOOP:

loop principal
	cargar_condiciones

	loop

		procesar_controles_modo_juego
		(cargar_condiciones)

		procesar_turno_jugador
		procesar_turno_no_jugador
		procesar_turno_proyectiles
		procesar_turno_decoracion

		procesar_resultados
			comprobar_proyectiles_fuera_juego
			
			actores: 
				comprobar_procesar_colision_actor_proyectiles
				comprobar_procesar_colision_actor_jugador
					game_over y otros.

	fin loop					
*/

void Controlador_juego::loop_principal()
{	
	//Sumamos una partida iniciada...
	this->controlador_perfiles.iniciar_partida_con_perfil();

	/*Simplemente para que quede referencia: el juego se inicia en el 
	estado E_JUEGO de modo que es empezar y ya directamente entramos en
	el nivel 1...*/

	//Es seguro inicializar algunos valores del juego aquí porque ya se ha inicializado.
	this->cargar_condiciones_nivel_e_iniciar(this->nivel_actual);
	bool procesar_resultados=true;	
	float delta=0;
	float tiempo_max_delta=Definiciones::T_MAX_DELTA; //0.1500f;

	while(!this->salir)	//Salir se produce cuando te liquidan o cuando lo quitas.
	{	
		//Aquí se recogen todas las pulsaciones.
		this->recoger_controles();

		Uint32 medida_control=SDL_GetTicks();

		delta=this->control_frames.obtener_delta();
		if(delta >= tiempo_max_delta) delta=tiempo_max_delta;

		if(pausado)
		{
			this->tiempo_pausa+=delta;
		}
		else if(tutorial.es_mostrable())	//Siempre hay un objeto tutorial en la pila, ya ves.
		{	
			tutorial.procesar_turno(delta);

			if(tutorial.espera_input())
			{			
				if(controles_sdl.recibe_eventos_boton_raton() 
				|| controles_sdl.recibe_eventos_teclado()
				|| controles_sdl.recibe_eventos_boton_joystick() )
				{
					tutorial.desvanecer();
				}
			}
		}
		else
		{
			//Procesar control.
			switch(this->estado_actual)
			{
				case E_PREPARANDO_JUGADOR_INICIAR_NIVEL: //Esta es la pantalla de mostrar estadísticas y fin de nivel.

					if(this->procesar_controles_modo_entre_niveles())
					{
						if(es_entrenamiento) salir=true;
						else this->cargar_condiciones_nivel_e_iniciar(this->nivel_actual);
					}					
				break;

				case E_GAME_OVER:
					this->tiempo_game_over+=delta;

					if(this->tiempo_game_over >= this->max_tiempo_game_over && this->procesar_controles_modo_game_over())
					{
						this->salir=true;
						break;
					}
				break;

				case E_NUEVA_PUNTUACION_MAXIMA:
					this->procesar_controles_modo_nueva_puntuacion_maxima();
				break;

				case E_REINICIANDO_NIVEL:
				case E_REINICIANDO_NIVEL_DESVANECIENDO:
				case E_ANIMACION_FIN_JUEGO:
					//Realmente no hay control que valga aquí.
				break;

				default:					
					this->procesar_controles_modo_juego(delta);
				break;
			}

			//Turnos.
			this->procesar_turno_marcadores_combo(delta);
			this->proceso_turno_nivel_estructura_info(delta);

			procesar_resultados=true; 

			switch(this->estado_actual)
			{
				case E_PREPARANDO_JUGADOR_INICIAR_NIVEL:

					//En este estado estamos esperando al input del jugador...
					this->procesar_turno_jugador(delta);
					this->procesar_turno_proyectiles(delta);
					this->procesar_turno_decoraciones(delta);
					this->procesar_turno_acciones(delta);
				break;

				case E_REINICIANDO_NIVEL:
					this->reiniciar_nivel_preparado();
					procesar_resultados=false;
				break;

				case E_GAME_OVER:
				case E_NUEVA_PUNTUACION_MAXIMA:

					No_jugador::establecer_divisor_movimiento(1);
					this->campo_estrellas.establecer_divisor_movimiento(1);
					this->procesar_turno_no_jugador(delta);
					this->procesar_turno_proyectiles(delta);
					this->procesar_turno_decoraciones(delta);
					this->procesar_turno_acciones(delta);
				break;

				case E_REINICIANDO_PRE_CUENTA_DESVANECIENDO:

					contador_pre_desvanecer_al_perder_vida-=delta;

					//Cuando la cuenta llega a cero ya desvanecemos.

					if(contador_pre_desvanecer_al_perder_vida <= 0)
					{
						this->pantalla.adjuntar_proceso(DLibV::Proceso_superficie_fundido::generar(this->pantalla.acc_superficie(), 0,0,0, 0, 255, 1));
						this->cambiar_estado(E_REINICIANDO_NIVEL_DESVANECIENDO);
					}

					procesar_resultados=false;
					this->procesar_turno_jugador(delta);
					this->procesar_turno_no_jugador(delta);
					this->procesar_turno_proyectiles(delta);
					this->procesar_turno_decoraciones(delta);
					this->procesar_turno_acciones(delta);
				break;

				case E_REINICIANDO_NIVEL_DESVANECIENDO:
					//Si no hay más filtros que aplicar a la pantalla 
					//es que ya hemos terminado de desvanecer, de modo
					//que cambiamos el estado.

					if(!this->pantalla.hay_procesos_en_marcha())
					{
						this->cambiar_estado(E_REINICIANDO_NIVEL);
					}

					procesar_resultados=false;

					this->procesar_turno_jugador(delta);
					this->procesar_turno_no_jugador(delta);
					this->procesar_turno_proyectiles(delta);
					this->procesar_turno_decoraciones(delta);
					this->procesar_turno_acciones(delta);
				break;

				case E_JUEGO:
				case E_FINALIZANDO_NIVEL:

					if(this->estado_actual==E_JUEGO)
					{
						if(this->estructura_info_nivel.acc_modo_juego()==Estructura_info_nivel::M_ALEATORIO)
						{
							this->poblar_juego_aleatorio();
						}
					}
					else
					{
	/*Esto es cuando estamos finalizando el nivel: esto es, se da por
	terminado pero aún no se ha iniciado el siguiente. Para el modo prehecho
	no pasa nada: si se da por terminado se ha terminado. Para el modo
	aleatorio se da por terminado al conseguir todas las gemas, pero se
	puede seguir jugando hasta que todos los bonus y obstáculos se hayan
	ido... OJO: en modo preparado no se cuentan obstáculos ni bonus!!!*/

						switch(this->estructura_info_nivel.acc_modo_juego())
						{
							case Estructura_info_nivel::M_PREPARADO: 
								this->aumentar_nivel(); 
							break;
							case Estructura_info_nivel::M_ALEATORIO:
								if(this->cont_obstaculos==0 && this->cont_bonus==0)
								{
									this->aumentar_nivel();
								}
							break;
						}
					}

					this->procesar_turno_jugador(delta);
					this->procesar_turno_no_jugador(delta);
					this->procesar_turno_proyectiles(delta);
					this->procesar_turno_decoraciones(delta);
					this->procesar_turno_acciones(delta);				
				break;

				case E_ANIMACION_FIN_JUEGO:
					this->procesar_animacion_fin_juego(delta);

					this->procesar_turno_jugador(delta);
					this->procesar_turno_no_jugador(delta);
					this->procesar_turno_proyectiles(delta);
					this->procesar_turno_decoraciones(delta);
					this->procesar_turno_acciones(delta);
				break;
			}

			//Resultado del turno.
			if(procesar_resultados) 
			{
				this->procesar_resultados();
			}

			controlador_audio.debug_estado_canales();
			this->cola_sonido.procesar_cola();

/*Esta comprobación sólo se da en modos de juego. El método de comprobar puede 
cambiar el estado y nos puede joder si el estado es algo que no sea "JUEGO".
A día de hoy sólo se hace en niveles aleatorios. En niveles preparados hay un 
obstáculo que al cruzarlo activa la comprobación.*/

			if(this->estado_actual==E_JUEGO)
			{
				//Para el modo aleatorio querremos comprobar en todo momento si ha finalizado.
				if(this->estructura_info_nivel.acc_modo_juego()==Estructura_info_nivel::M_ALEATORIO)
				{
					this->comprobar_finalizacion_nivel_aleatorio();
				}
				//Para el modo preparado, sólo cuando el nivel nos diga.
				else if(this->estructura_info_nivel.es_comprobar_finalizacion_nivel_preparado())
				{
					LOG<<"COMPROBADO FIN DE NIVEL PREPARADO: INICIANDO FORZADO"<<std::endl;
					this->forzar_finalizacion_nivel_preparado();
				}
			}

			//Esto estaba antes bajo "control_frames.turno".
			this->controlador_perfiles.procesar_crono_perfiles(this->datos_actualizables, this->controlador_retos); //Procesamos el control de perfiles, para guardar los datos que puedan haber cambiado si se ha cumplido el crono.

		} //Fin de "No pausa..."

		this->ms_logica=this->control_frames.obtener_delta_para_tiempo(medida_control);
		Uint32 medida_render=SDL_GetTicks();

		this->representar(delta);

		this->ms_render=this->control_frames.obtener_delta_para_tiempo(medida_render);

		this->control_frames.turno();	//Pausado o no, esto siempre hace algo.
	}

	this->finalizar_loop_principal();
}

/*Esta es la función de limpieza del loop principal. Grabará la información del 
jugador en disco y se procede hará una sincronización de datos con el servidor.
Durante todo el proceso tan sólo se mostrará el campo de estrellas y un icono
de carga.

Una parte importante es determinar si tenemos o no que lanzar un 
hilo para sincronizar los datos con el servidor - mayormente el 
tema de los trofeos. Esto sólo ocurrirá con un perfil online validado.
resulta que para validar un perfil también tenemos que lanzar un 
hilo de modo que el proceso completo sería de dos hilos, uno después
del otro. Lo que haremos será abrir un sólo hilo donde se hacen todas las
comprobaciones y procesas.

Esperaremos que una bandera indique que el proceso está finalizado: ya
sea por medio de hilos, ya sea forzando porque algo falle.	

Como siempre que trabajamos con hilos, el código engorda bastante, pero no 
es complicado si lo miramos con detenimiento.
*/

void Controlador_juego::finalizar_loop_principal()
{
	float delta=0;
	float acum=0;
	bool finalizadas_operaciones_online=false;

	tutorial.desactivar();

	this->cambiar_estado(E_FINALIZANDO_LOOP_PRINCIPAL);

	//Al finalizar la partida guardamos los datos...
	this->controlador_perfiles.actualizar_perfil_jugador(this->datos_actualizables);
	this->controlador_perfiles.refrescar_cuenta_niveles_finalizados();

	//Liberamos los mapas de colision... 
	Gestor_mapas_colision::finalizar();

	//Empiezan las operaciones con hilos.
	DLibH::Hilo_SDL * hilo_finalizar=NULL;

	//Lo primero es ver si es online. Si no es online podemos ignorar todo
	//el tema de los hilos. En caso contrario, lanzaremos el hilo de 
	//validación, que se propagará al interior del bucle.

	if(!this->controlador_perfiles.es_perfil_online()) finalizadas_operaciones_online=true; //Si no es online, forzamos.
	else
	{
		//Lanzamos el hilo...
		hilo_finalizar=new DLibH::Hilo_SDL();
		if(!hilo_finalizar->iniciar(Controlador_juego::hilo__finalizar_loop_principal, &this->controlador_perfiles)) 
		{
			finalizadas_operaciones_online=true;
			DLibH::Hilo_SDL::destruir_hilo(hilo_finalizar);
		}
	}	
	
	//Iniciamos el bucle de control...

	while(acum < TIEMPO_CARGANDO_MINIMO || !finalizadas_operaciones_online)
	{
		//Si el proceso ha terminado entonces ya no nos queda nada
		//que hacer.

		if(hilo_finalizar && hilo_finalizar->es_finalizado())
		{
			hilo_finalizar->completar();
			DLibH::Hilo_SDL::destruir_hilo(hilo_finalizar);
			finalizadas_operaciones_online=true;
		}

		//Sumamos el tiempo acumulado y representamos.

		delta=this->control_frames.obtener_delta();
		acum+=delta;	
		this->representar(delta);
		this->control_frames.turno();
	}

	//Recordamos: el hilo nunca se puede quedar abierto!.
	if(hilo_finalizar) DLibH::Hilo_SDL::finalizar_y_destruir_hilo(hilo_finalizar);
}

/*Este es el hilo que se usará para validar y guardar en el servidor los datos
del perfil del jugador cuando se sale del loop principal. 
En la estructura de datos incluye una referencia al controlador de perfiles.*/

int Controlador_juego::hilo__finalizar_loop_principal(void * datos_recibidos)
{
	//Desempaquetamos los datos... primero desempaquetamos el hilo que necesitaremos para "finalizarlo".
	DLibH::Hilo_SDL::Hilo_estructura_datos * estructura_datos=static_cast<DLibH::Hilo_SDL::Hilo_estructura_datos*>(datos_recibidos);
	DLibH::Hilo_SDL * h=estructura_datos->ref_hilo;

	//Desempaquetamos el controlador de perfiles enviado.
	Controlador_perfiles * cc=static_cast<Controlador_perfiles *>(estructura_datos->datos);

	cc->guardar_datos_perfil_en_servidor();

	h->finalizar();
	return 1;
}

/*Una vez recibidos y aplicados los inputs y turnos se procesan los resultados
de las acciones que han ocurrido. Por cada actor de mapa se procesa contra los
proyectiles, contra el jugador y contra el EDU **obsoleto**, no necesariamente en este 
orden. Al finalizar es posible que haya que eliminar algún actor, en cuyo caso
se procesa.*/

void Controlador_juego::procesar_resultados()
{
	std::vector<Actor_mapa *>::iterator 	ini=this->actores_mapa.begin(),
						fin=this->actores_mapa.end();

	//Procesar los proyectiles para ver si están fuera de juego.
	this->comprobar_proyectiles_fuera_juego();
	
	this->comprobar_proyectiles_contra_escudo();

	//Comprobar los proyectiles contra el jugador...
	this->comprobar_procesar_colision_proyectiles_jugador();	

	bool jugador_destruido=false; //Vamos a almacenar aquí si el jugador se destruye en este turno.	
					//lo haremos porque, si se destruye, se generan nuevos actores
					//para la explosión y pueden joder el while que tenemos abajo.
					//Sacamos la generación de actores y la ponemos fuera del bucle.

	//Comprobar cada actor contra proyectiles, el jugador y el EDU.

	while(ini < fin)	//Hay una razón para NO tener un bucle for, mirar el final...
	{

		//Algunos obstáculos se borrarán, otros no... Por ejemplo,
		//los preparados se quedan siempre, lo mismo que las decoraciones.
		//Los bonus y gemas se destruirían.
		//Es importante que no se cree ni se destruya nada en el vector
		//de actores mientras lo estamos iterando!.

		if((*ini)->es_fuera_de_juego() )
		{
			(*ini)->marcar_para_borrar();
		}	
		else
		{
			if((*ini)->es_colisionable())
			{
				//Comprobamos primero contra los proyectiles...
				if(!this->proyectiles.empty())
				{				
					this->comprobar_procesar_colision_actor_proyectiles((*ini)); 
				}

				//Luego comprobamos contra el jugador, si procede, esto es, si no hay que borrarlos.

				if(!(*ini)->es_marcado_para_borrar() && this->J->es_jugable())
				{
					jugador_destruido=this->comprobar_procesar_colision_actor_jugador((*ini));	
					if(this->F->es_visible()) this->comprobar_procesar_colision_actor_fantasma((*ini));
				}
			}
		}

		/*Esta es la razón para no tener un bucle for: el for
		haría siempre la acción ini++ pero en este caso sólo
		queremos hacerla cuando NO borremos. Al borrar se coloca
		ini en una nueva posición y hacer ini++ haría que nos 
		saltasemos posiciones válidas*/

		if((*ini)->es_marcado_para_borrar())
		{			
			/*De la documentación de std::vector::erase
			Return value: A random access iterator pointing to the new location of the element that followed the last element erased by the function call, which is the vector end if the operation erased the last element in the sequence.*/

			this->eliminar_actor_mapa(*ini);
			ini=this->actores_mapa.erase(ini);
			fin=this->actores_mapa.end();	//Se invalidarían todos los iteradores cuando borramos uno!!!.
		}
		else
		{
			++ini;
		}

		if(jugador_destruido)	//No merecería la pena seguir.
		{
			break;
		}
	}				

	if(jugador_destruido)
	{
		this->generar_explosion(this->J, true, 8);
	}

	if(actualizar_combo) 
	{
		this->actualizar_marcador_combo();
	}
}

void Controlador_juego::cambiar_estado(unsigned int p_estado)
{
	this->estado_actual=p_estado;
}

/*En función del input se procesa el turno completo del jugador.*/

void Controlador_juego::procesar_turno_jugador(float p_delta)
{
	//Movemos al jugador, el fantasma y la mirilla según el desplazamiento del nivel... 
	this->J->ir_a(this->J->acc_x(), this->J->acc_y()-this->estructura_info_nivel.acc_avance_este_turno());
	this->F->ir_a(this->F->acc_x(), this->F->acc_y()-this->estructura_info_nivel.acc_avance_este_turno());
	this->M->ir_a(this->M->acc_x(), this->M->acc_y()-this->estructura_info_nivel.acc_avance_este_turno());

	//El escudo va a la misma posición del jugador.
	this->E->establecer_posicion_y_angulo_desde_actores(*this->J, *this->M);

	this->J->reiniciar_senal_sonido();
	this->J->procesar_turno(p_delta);
	this->E->procesar_turno(p_delta);

	if(this->J->es_parando_tiempo())
	{
		this->datos_actualizables.sumar_tiempo_detenido(p_delta);
	}

	float centro_x=0;
	float centro_y=0;

	if(this->F->es_visible()) 
	{
		this->F->procesar_turno(p_delta);
	}
	else 
	{
	//Esto es el proceso de la mirilla. Según el esquema de control 
	//querremos procesar los valores que se han guardado anteriormente o no.

		switch(this->esquema_control)
		{
			case C_GAMEPAD:
				centro_x=this->J->acc_x()+(this->J->acc_w()/2);
				centro_y=this->J->acc_y()+(this->J->acc_h()/2);
				this->M->procesar_valores_gamepad(centro_x, centro_y);
			break;

			case C_TECLADO_RATON:
				this->M->procesar_turno(p_delta);
			break;
		}
	}

	unsigned short int sonidos_jugador=this->J->acc_senales_sonido();

	if(sonidos_jugador)
	{
	
		int x=J->acc_x();
		int max_x=Medidas::ANCHO_VISTA-J->acc_w();
		int r=(x * 255) / max_x;		
		int l=255-r;
		
		if(sonidos_jugador & Jugador::S_DISPARO) 
		{
			this->cola_sonido.insertar(
				DLibA::Estructura_sonido(
					DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_DISPARO),
					90, 0, l, r, 0)
			);
		}
		//if(sonidos_jugador & Jugador::S_RECARGA) this->cola_sonido.insertar(DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_RECARGA), 90);

		//Estos dos sonidos deberían ir por un canal dedicado de forma 
		//que uno se pare si se inicia el otro. En concreto, si dejamos
		//rápidamente de frenar el tiempo tiene que parar el sonido e
		//iniciarse el otro.

		if(sonidos_jugador & Jugador::S_INICIO_PARAR_TIEMPO) 
		{
			try
			{
				DLibA::Canal_audio canal=controlador_audio.obtener_canal_libre();
				J->establecer_canal_sonido_tiempo(canal.acc_indice());

				canal.iniciar_reproduccion(
					DLibA::Estructura_sonido(
						DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_WHOOSH_01),
						-1, 0, l, r, 400)
					);
			}
			catch(const DLibA::Excepcion_controlador_audio& e)
			{
			}
		}
		else if(sonidos_jugador & Jugador::S_FIN_PARAR_TIEMPO) 
		{
			try
			{
				int indice_canal=J->obtener_canal_sonido_tiempo();
				DLibA::Canal_audio canal=controlador_audio.obtener_canal(indice_canal);
				canal.forzar_parada();
				canal.iniciar_reproduccion(
					DLibA::Estructura_sonido(
						DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_WHOOSH_02),
						-1, 0, l, r, 400)
					);
			}
			catch(const DLibA::Excepcion_controlador_audio& e) 
			{
			}
		}

	}
}

/*Se procesa el turno de cualquier elemento del mapa. Por cada uno de ellos se
comprueba si tiene una actividad y, si es el caso, se inserta en un vector
para procesar su actividad individualmente más adelante.*/

void Controlador_juego::procesar_turno_no_jugador(float p_delta)
{
	//Divisores de movimiento.

	float divisor=this->J->acc_divisor_tiempo();

	No_jugador::establecer_divisor_movimiento(divisor);
	this->campo_estrellas.establecer_divisor_movimiento(divisor);
	this->estructura_info_nivel.establecer_divisor_movimiento(divisor);

	//Turnos en si.

	std::vector<Actor_mapa*>::iterator 	ini=this->actores_mapa.begin(),
						fin=this->actores_mapa.end();

	std::vector<Obstaculo_actividad *> con_actividad;

	for(;ini < fin; ini++) 
	{
		if((*ini)->procesa_turno()) (*ini)->procesar_turno(p_delta);

		/*Si algún actor tiene alguna actividad que realizar este es el
		momento. En cualquier caso puede ser problemático ya que como
		consecuencia de la actividad puede crearse otro actor y los
		iteradores ya no serían válidos. Lo que haremos será guardarlos
		en un vector y hacerlos todos al final.
		"realiza_actividad" incluye comprobaciones de estar dentro de 
		la pantalla.		
		*/

		if((*ini)->realiza_actividad())	//Realiza actividad es sinónimo de "es obstáculo actividad". Mala planificación.
		{
			//S-S-S-Static cast :S.
			con_actividad.push_back(static_cast <Obstaculo_actividad *>(*ini));
		}
	}

	/*Al hacer su actividad una vez terminado el trabajo con el vector
	de actores ya no hay problemas con los iteradores*/

	if(con_actividad.size())
	{
		std::vector<Obstaculo_actividad *>::iterator 	ac_ini=con_actividad.begin(),
								ac_fin=con_actividad.end();
		while(ac_ini < ac_fin)
		{
			this->procesar_turno_actividad_no_jugador(*ac_ini);
			ac_ini++;
		}
	}
}

/*Se procesa el turno de los proyectiles, sin más novedad.*/

void Controlador_juego::procesar_turno_proyectiles(float p_delta)
{
	std::vector<Proyectil*>::iterator 	ini=this->proyectiles.begin(),
						fin=this->proyectiles.end();

	for(;ini < fin; ini++) 
	{
		(*ini)->procesar_turno(p_delta);
	}
}

/*Se procesan los turnos de las decoraciones. Las decoraciones tienen un método
que indica si han finalizado su actividad. Si es el caso, se eliminan. Antes
de ser eliminados comprobamos el tipo por si hay que llevar a cabo alguna
acción especial.*/

void Controlador_juego::procesar_turno_decoraciones(float p_delta)
{
	std::vector<Decoracion*>::iterator 	ini=this->decoraciones.begin(),
						fin=this->decoraciones.end();

	for(;ini < fin; ini++) 
	{
		(*ini)->procesar_turno(p_delta);

		if( (*ini)->es_finalizado())
		{
			delete (*ini);
			ini=this->decoraciones.erase(ini);
			fin=this->decoraciones.end();	//Se invalidarían todos los iteradores cuando borramos uno!!!.
		}
	}

	ini=this->decoraciones_frente.begin(),
	fin=this->decoraciones_frente.end();

	for(;ini < fin; ini++) 
	{
		(*ini)->procesar_turno(p_delta);

		if( (*ini)->es_finalizado())
		{
			if((*ini)->tipo_decoracion()==Decoracion::D_PUNTUACION)
			{
				this->sumar_puntuacion_representada( static_cast<Decoracion_puntuacion *>(*ini));
			}

			delete (*ini);
			ini=this->decoraciones_frente.erase(ini);
			fin=this->decoraciones_frente.end();	//Se invalidarían todos los iteradores cuando borramos uno!!!.
		}
	}
}


/*La única finalidad de este método es comprobar la colisión de todos los 
proyectiles con el jugador. comprobar_procesar_colision_actor_proyectiles 
necesita además un actor_mapa y no es válido.*/

void Controlador_juego::comprobar_procesar_colision_proyectiles_jugador()
{
	std::vector<Proyectil *>::iterator 	pr_ini=this->proyectiles.begin(),
						pr_fin=this->proyectiles.end();

	while(pr_ini < pr_fin)
	{
		if((*pr_ini)->es_disparado_por(this->J))
		{
			pr_ini++;
			continue;
		}
		else if(this->J->en_colision_con(**(pr_ini)))
		{			
			(*pr_ini)->marcar_para_borrar();

			//Sólo te afectarán si no eres invulnerable y si 	
			//el estado es "Juego". Esto es para que cuando estés
			//con la máxima puntuación no te puedan disparar, ponerte
			//a -1 vidas y joderte vivo.

			if(!this->J->es_invulnerable() && estado_actual==E_JUEGO)
			{	
//DEBUG<<"RESTAR VIDA POR PROYECTIL"<<std::endl;
				this->restar_vida_jugador();
				this->cola_sonido.insertar(DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_DISPARO));
			}
		}

		if((*pr_ini)->es_marcado_para_borrar())
		{
			this->eliminar_proyectil(*pr_ini);
			pr_ini=this->proyectiles.erase(pr_ini);
			pr_fin=this->proyectiles.end();	
		}
		else
		{
			pr_ini++;
		}
	}
}

/*El actor se compara con cada uno de los proyectiles del jugador en la 
pantalla. Si hay colisión se comprueba el tipo de colisión generada (en función
de unas tablas estáticas) y se llevan a cabo las acciones pertinentes.*/

void Controlador_juego::comprobar_procesar_colision_actor_proyectiles(Actor_mapa * p_actor)
{
	std::vector<Proyectil *>::iterator 	pr_ini=this->proyectiles.begin(),
						pr_fin=this->proyectiles.end();

	std::vector<Actor *> post_proceso_afecta_proyectiles;

	bool resultado_colision=false;
	unsigned int faccion=p_actor->tipo_a_faccion();
	Proyectil * pr=NULL;

	while(pr_ini < pr_fin)
	{
		/*Esto puede resultar arcano pero tiene sentido... Si el actor
		tiene un método especial de colisión queremos usar el del actor
		contra el proyectil y no el del proyectil contra el actor. Por
		ejemplo, las puertas: si hacemos proyectil vs actor directamente
		da colisión porque el proyectil dictamina que las posiciones 
		coinciden. Si lo hacemos al revés es el actor el que determina 
		si el proyectil chocaría - en este caso con las hojas de las
		puertas. */
	
		pr=(*pr_ini);

		if(p_actor->en_colision_con(*pr))
		{
			//No debe hacer nada si el proyectil es tuyo o de tu facción.

			if(pr->es_disparado_por(p_actor) || pr->es_de_faccion(faccion))
			{
				
			}
			else
			{
				//Esto del resultado sólo lo usaremos en algunas ocasiones puntuales.
				//Normalmente será true. En casos especiales puede ser false, por
				//ejemplo: true cuando un actor destruible pierde toda la resistencia,
				//false en caso contrario. True cuando se puede activar un interruptor,
				//false cuando no.

				resultado_colision=p_actor->procesar_colision_proyectil(pr);
				bool procesar_destruccion_actor=false; //Lo hacemos a true si vamos a procesar la destrucción del actor, con puntuación, explosión, combo...
				unsigned short int tipo_colision_proyectil=p_actor->tipo_a_colision_proyectil();
				
				//El proyectil no se marca para borrar simplemente por haber chocado: puede atravesar.
				bool borrar_proyectil=true;

				switch(tipo_colision_proyectil)
				{
					case Actor_mapa::T_DESTRUYE_PROYECTIL:
						evaluar_anulacion_combo_por_proyectil(pr);
					break;

					case Actor_mapa::T_DESTRUYE_PROYECTIL_NO_RESTA_COMBO: 
					break;

					case Actor_mapa::T_DESTRUYE_PROYECTIL_ACTOR: 	//Supuestamente esto es un bonus... Te corta el combo!.
						p_actor->marcar_para_borrar();
						this->generar_explosion(p_actor, p_actor->es_con_chatarra_explosion(), p_actor->acc_columnas_humo_explosion());
						evaluar_anulacion_combo_por_proyectil(pr);
					break;

					//Esto es sólo para las minas... Un caso especial.
					case Actor_mapa::T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR_GENERA_PROYECTILES:
				
						if(resultado_colision) //Devolverá true cuando no le quede resistencia.
						{
							//Este método puede resultar problemático ya que
							//manipula el mismo array sobre el que estamos 
							//iterando. Lo que haemos será guardar el actor
							//en un vector temporal y actuar sobre los
							//proyectiles cuando hayamos terminado.

							post_proceso_afecta_proyectiles.push_back(p_actor);
							procesar_destruccion_actor=true;
						}
					break;

					case Actor_mapa::T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR:
					case Actor_mapa::T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR_DA_ITEM:

						if(resultado_colision) //Devolverá true cuando no le quede resistencia.
						{
							procesar_destruccion_actor=true;

							if(tipo_colision_proyectil==Actor_mapa::T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR_DA_ITEM)
							{
								this->reemplazar_item(p_actor);					
							}
						}
					break;	

					//Esta es la implementación del asteroide secreto...
					case Actor_mapa::T_DESTRUYE_PROYECTIL_RESTA_RESISTENCIA_ACTOR_ES_SECRETO:

						if(resultado_colision) //Devolverá true cuando no le quede resistencia.
						{
							procesar_destruccion_actor=true;
							this->reemplazar_item(p_actor);

							this->disparar_reto(Definicion_retos::R_ASTEROIDE_SECRETO);
							this->estructura_info_nivel.marcar_asteroide_secreto_como_destruido();
							LOG<<"ENCONTRADO OBSTACULO SECRETO"<<std::endl;
						}
					break;	

					case Actor_mapa::T_DESTRUYE_PROYECTIL_LANZA_ACCION:

						if(resultado_colision) //Devolverá true cuando pueda activarse el interruptor.
						{		
							this->activar_accion_trigger( *(static_cast<Obstaculo_trigger *> (p_actor)));
						}

					case Actor_mapa::T_NADA:
					default: 
						borrar_proyectil=false;
					break;
				}	//Fin de switch por tipo.

				if(borrar_proyectil)
				{
					pr->marcar_para_borrar();
				}

				if(procesar_destruccion_actor)
				{
					actualizar_combo=actualizar_combo || this->CONTROLADOR_PUNTUACION.sumar_combo();
					actualizar_barra_combo();
					this->sumar_puntuacion(p_actor->puntuacion_por_destruir(), p_actor);
					this->generar_explosion(p_actor, p_actor->es_con_chatarra_explosion(), p_actor->acc_columnas_humo_explosion());
					p_actor->marcar_para_borrar();

					//Último detalle para el control de las estadísticas:
					if(p_actor->es_de_nivel_preparado())
					{
						if(p_actor->es_cuenta_destruible()) this->estructura_info_nivel.aumentar_destruible();
					}
				}
	
			}	//Fin de "afecta a facción". 
		}	//Fin de "Hay colisión?"


		if(pr->es_marcado_para_borrar())
		{
			//Sumamos un nuevo obstáculo destruido.
	
			this->eliminar_proyectil(pr);
			pr_ini=this->proyectiles.erase(pr_ini);
			pr_fin=this->proyectiles.end();	
		}
		else
		{
			pr_ini++;
		}
	}

	//Para el tema de trofeos y estadísticas...
	if(p_actor->es_marcado_para_borrar()) this->datos_actualizables.sumar_obstaculo_destruido();

	if(post_proceso_afecta_proyectiles.size())
	{
		std::vector<Actor *>::iterator 	ini_pp=post_proceso_afecta_proyectiles.begin(),
						fin_pp=post_proceso_afecta_proyectiles.end();

		while(ini_pp < fin_pp)
		{
			this->generar_destruccion_mina(static_cast<Obstaculo_explosivo *>(*ini_pp));
			ini_pp++;
		}
	}
}

/*Se comprueba si el actor está en colisón con el jugador. Si hay colisión se
obtiene el tipo de colisión de una tabla estática y se aplican las acciones
de turno (sumar puntos, quitar vidas...).

Devuelve true si el jugador ha sido destruido tras este turno.
*/

bool Controlador_juego::comprobar_procesar_colision_actor_jugador(Actor_mapa * p_actor)
{
	bool resultado=false;

	if(p_actor->en_colision_con( *(this->J) ) )
	{
		if(p_actor->desaparece_tras_colision())
		{
			p_actor->marcar_para_borrar();
		}

		unsigned short int tipo_colision_jugador=p_actor->tipo_a_colision_jugador();

		switch(tipo_colision_jugador)
		{
			case Actor_mapa::T_DESTRUYE_JUGADOR:

				if(!this->J->es_invulnerable())
				{	
//DEBUG<<"RESTAR VIDA POR COLISION"<<std::endl;
					resultado=true;
					this->restar_vida_jugador();
					this->cola_sonido.insertar(DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_DISPARO));
				}
			break;

			case Actor_mapa::T_OTORGA_BONUS_JUGADOR:

				this->cola_sonido.insertar(DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_ITEM));

				switch(p_actor->tipo_actor_mapa())
				{
					case Actor_mapa::T_BONUS_DISPARO:
						if(!this->J->aumentar_disparo())
						{
							//Comprobamos si conseguimos el trofeo de maximizar el cañón...
							this->disparar_reto(Definicion_retos::R_MAXIMIZAR_CANON);
							this->sumar_puntuacion(500, p_actor);
						}
						else
						{				
							this->barra_canon.establecer_valor_maximo(this->J->acc_carga_maxima_canon());
						}
					break;

					case Actor_mapa::T_BONUS_TIEMPO:
						if(!this->J->aumentar_tiempo())
						{
							this->sumar_puntuacion(500, p_actor);
						}
						else
						{
							this->barra_tiempo.establecer_valor_maximo(this->J->acc_carga_maxima_parar_tiempo());
						}
					break;

					case Actor_mapa::T_BONUS_CARGA_ESCUDO:

						if(!this->E->sumar_duracion())
						{
							this->sumar_puntuacion(500, p_actor);
						}
						else
						{
							this->barra_escudo.establecer_valor_maximo(this->E->acc_max_carga_escudo());
						}
					break;

					case Actor_mapa::T_BONUS_COMPONENTE_ESCUDO:

						if(!this->E->sumar_escudos())
						{
							this->sumar_puntuacion(500, p_actor);
						}
						else
						{

						}
					break;

					case Actor_mapa::T_BONUS_VIDA:
						this->J->aumentar_vida();
					break;

					case Actor_mapa::T_BONUS_PIMIENTO:
						this->sumar_puntuacion(200, p_actor);
					break;

					default: break;
				}

				if(p_actor->es_de_nivel_preparado() && p_actor->es_cuenta_recogible())
				{
					this->estructura_info_nivel.aumentar_recogible();
				}				
			break;

			case Actor_mapa::T_AUMENTA_CONTADOR_NIVEL:

				this->cola_sonido.insertar(DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_ITEM));

				this->sumar_puntuacion(100, p_actor);
				this->recoger_gema(static_cast<Bonus_normal *> (p_actor));

				if(p_actor->es_de_nivel_preparado() && p_actor->es_cuenta_recogible())
				{
					this->estructura_info_nivel.aumentar_recogible();
				}
			break;

			case Actor_mapa::T_DISPARA_ACCION:
			{
				Obstaculo_trigger_contacto * otc=static_cast<Obstaculo_trigger_contacto *> (p_actor);

				if(otc->es_activable_por_jugador())
				{
					this->activar_accion_trigger(*otc);
				}

				otc=NULL;
			}
			break;

			default: break;
		}
	}

	return resultado;
}

/*Idem que el anterior, pero con el fantasma, que tiene menos cosas que le
afecten...*/

void Controlador_juego::comprobar_procesar_colision_actor_fantasma(Actor_mapa * p_actor)
{
	if(p_actor->en_colision_con( *(this->F) ) )
	{
		unsigned short int tipo_colision_jugador=p_actor->tipo_a_colision_jugador();

		switch(tipo_colision_jugador)
		{
			case Actor_mapa::T_DISPARA_ACCION:
			{
				Obstaculo_trigger_contacto * otc=static_cast<Obstaculo_trigger_contacto *> (p_actor);
				
				if(otc->es_activable_por_fantasma())
				{
					this->activar_accion_trigger(*otc);
				}
			}
			break;

			default: break;						
		}
	}
}

/*Inserta un item de bonus sacado de la tabla aleatoria de turno.*/

Actor_mapa * Controlador_juego::insertar_bonus(short int tirada)
{
	Actor_mapa * bonus=NULL;

	if(tirada!=-1)
	{
		bonus=Factoria_actores_mapa::fabricar(tirada, this->extra_velocidad_actores);

		if(bonus)
		{
			this->cont_bonus++;
			this->actores_mapa.push_back(bonus);				
		}
	}

	return bonus;
}

/*En caso de destruirse un actor que deja bonus, se crea el bonus en la misma
posición del actor. Nótese que el actor no de destruye.*/

void Controlador_juego::reemplazar_item(Actor_mapa * p_actor)
{
	int tipo_actor=p_actor->tipo_actor_mapa();
	short int tirada=-1;

	switch(tipo_actor)
	{
		case Actor_mapa::T_OBSTACULO_BONUS: 
		{	
			Obstaculo_bonus * ob=static_cast<Obstaculo_bonus *>(p_actor);
			tirada=ob->obtener_tirada();	//El obstáculo puede contener algo fijo o dar -1 cuando es aleatorio.
			if(tirada==-1) tirada=this->estructura_info_nivel.generar_tirada_tabla(Estructura_info_nivel::T_DESTRUIR_CAJA);
		}
		break;

		default: 
			tirada=this->estructura_info_nivel.generar_tirada_tabla(Estructura_info_nivel::T_DESTRUIR_OBSTACULO);
		break;
	}

	if(tirada!=Actor_mapa::T_NADA)
	{
		Actor_mapa * b=this->insertar_bonus(tirada);
	
		if(b)
		{
			b->establecer_vector_desde_actor(*p_actor);
			b->establecer_posicion_desde_actor(*p_actor);
		}
	}
}

/*Inserta un obstáculo sacado de la tabla aleatoria de turno.*/

void Controlador_juego::insertar_obstaculo()
{
	unsigned short int tirada=this->estructura_info_nivel.generar_tirada_tabla(Estructura_info_nivel::T_OBSTACULOS);

	if(tirada!=-1)
	{
		Actor_mapa * obstaculo=Factoria_actores_mapa::fabricar(tirada, this->extra_velocidad_actores);

		if(obstaculo)
		{		
			//Aunque el obstáculo se haya creado comprobamos si podemos tenerlo ahí...
			if(!this->insertar_obstaculo_comprobar(obstaculo))
			{
				delete obstaculo;
				obstaculo=NULL;
			}
			else
			{
				this->actores_mapa.push_back(obstaculo);
				this->cont_obstaculos++;
				this->superficie_ocupada+=obstaculo->acc_superficie();			
			}
		}
	}
}

/*Este es el único punto de borrado para los actores de mapa. Todas las cosas
que ocurran al borrar un actor ocurrirán aquí. No cuenta para proyectiles!!.*/

void Controlador_juego::eliminar_actor_mapa(Actor_mapa * p_actor)
{
	unsigned short int tipo_cuenta=p_actor->tipo_a_cuenta();

	switch(tipo_cuenta)
	{
		case Actor_mapa::T_CUENTA_OBSTACULO:
			this->cont_obstaculos--;

			this->superficie_ocupada-=p_actor->acc_superficie();
			if(this->superficie_ocupada < this->max_superficie_ocupada)
			{
				this->excedida_superficie_maxima=false;
			}
		break;

		case Actor_mapa::T_CUENTA_BONUS:
			this->cont_bonus--;
		break;

		case Actor_mapa::T_CUENTA_GEMA:

			this->cont_bonus--;
			this->controlar_destruccion_gema(static_cast<Bonus_normal *> (p_actor));

			//Esta es la segunda condición para el fin del juego: que nos hayamos pasado de gemas.
			//Añadimos una nueva condición, y es que haya gemas que recoger. 
			//cuando no hay gemas que recoger se asume el valor de cero.
			if(
				this->estructura_info_nivel.acc_gemas_en_nivel() && 
				this->estructura_info_nivel.acc_gemas_max_nivel()<=0 && 
				this->estado_actual==E_JUEGO)
			{
				//En modo aleatorio se inicia el fin del juego.
				//En el modo preparado se resta una vida.

				switch(this->estructura_info_nivel.acc_modo_juego())
				{
					case Estructura_info_nivel::M_ALEATORIO: 
						this->iniciar_fin_juego(GO_NO_GEMAS); 
					break;
					case Estructura_info_nivel::M_PREPARADO: 

						if(this->cont_gemas < this->estructura_info_nivel.acc_gemas_en_nivel())
						{
//DEBUG<<"RETAR VIDA POR DESTRUCCION GEMAS"<<std::endl;
							this->restar_vida_jugador(); 
						}
					break;
				}
			}
		break;
	}

	//Y ahora lo podemos eliminar, finalmente.
	this->eliminar_actor_de_acciones(p_actor);

	delete p_actor;
}

/*Este es el único punto de borrado para los proyectiles. Todo lo que se haga
en ese momento se hace aquí.*/

void Controlador_juego::eliminar_proyectil(Proyectil * p_proyectil)
{
	this->generar_chispas(p_proyectil);
	delete p_proyectil;
}

bool Controlador_juego::insertar_obstaculo_comprobar(Actor_mapa * p_obstaculo)
{
	//Ahora mismo siempre devuelve true.

	bool resultado=true;

/*	
	//Antes se comprobaba si chocaba con otro. Not anymore.

	std::vector<Actor_mapa*>::iterator 	ini=this->actores_mapa.begin(),
						fin=this->actores_mapa.end();
	
	while(ini < fin)
	{
		if((*ini)->en_colision_con(*p_obstaculo))
		{
			resultado=false;
			break;
		}
		ini++;
	}
*/
	//Comprobamos que no ocuparía más de la cuenta... Aunque lo ocupase
	//lo meteríamos, para que puedan salir obstáculos grandes aunque haya
	//muchos pequeños PERO levantaremos una bandera para indicar que ya
	//no podemos meter más.

	if(this->superficie_ocupada+p_obstaculo->acc_superficie() > this->max_superficie_ocupada)
	{
		this->excedida_superficie_maxima=true;
	}

	return resultado;
}

/*Cuando se han terminado las vidas se inicia este proceso para indicar el nuevo
estado de juego (introducir puntuación o sólo pantalla de game over).*/

void Controlador_juego::iniciar_fin_juego(unsigned int tipo_fin)
{
	//El tipo de fin lo usaremos para determinar el mensaje de game over...
	switch(tipo_fin)
	{
		case GO_NO_VIDAS:
			this->controlador_perfiles.perder_partida_con_perfil();
			this->rep_txt_gameover->asignar(this->localizador.obtener(Localizador::C_GAME_OVER_01));
		break;

		case GO_NO_GEMAS:
			this->controlador_perfiles.perder_partida_con_perfil();
			this->rep_txt_gameover->asignar(this->localizador.obtener(Localizador::C_GAME_OVER_02));
		break;

		case GO_FIN_JUEGO:
			
			//Sumamos en primer lugar una partida ganada.
			this->controlador_perfiles.ganar_partida_con_perfil();

			//Disparamos el reto forzoso de terminar una vez el juego.
			this->disparar_reto(Definicion_retos::R_FINALIZAR_JUEGO);

			//Como acabamos de ganar, vamos a forzar no sea que consigamos un trofeo más!.
			this->controlador_perfiles.comprobar_retos(this->controlador_retos);
			
			this->juego_finalizado=true;
			this->rep_txt_gameover->asignar(this->localizador.obtener(Localizador::C_GAME_OVER_03));
		break;
	}	

	//Con que haya una puntuación online ya hay que introducir puntuación máxima nueva.
	//Siempre habrá puntuaciones online a no ser que estemos desconectados.

	if(es_entrenamiento)
	{
		this->cambiar_estado(E_GAME_OVER);
	}
	else
	{
		this->posicion_online=this->preguntar_nueva_maxima_puntuacion_online();
		this->posicion_local=this->preguntar_nueva_maxima_puntuacion_local();

		if(this->posicion_online || this->posicion_local)  
		{
			this->escritor.mut_cadena(this->controlador_perfiles.obtener_nombre_perfil());
			this->debe_introducir_puntuacion=true;
		}

		if(this->posicion_online)
		{
			std::string temp=this->localizador.obtener(Localizador::C_GAME_OVER_PUNTUACION_ONLINE)+DLibH::Herramientas::entero_a_cadena(this->posicion_online);
			this->rep_txt_nueva_puntuacion_online->asignar(temp);
		}

		if(this->posicion_local)
		{
			std::string temp=this->localizador.obtener(Localizador::C_GAME_OVER_PUNTUACION_LOCAL)+DLibH::Herramientas::entero_a_cadena(this->posicion_local);
			this->rep_txt_nueva_puntuacion_local->asignar(temp);
		}

		if(this->debe_introducir_puntuacion)
		{
			this->cambiar_estado(E_NUEVA_PUNTUACION_MAXIMA);
		}
		else
		{
			this->cambiar_estado(E_GAME_OVER);
		}
	}
}
