//Aquí está todo lo relacionado con los niveles.

/*
Estructura de finalización de nivel:

loop_principal: estado E_JUEGO : comprobar_finalización_nivel_aleatorio / forzar_finalizacion_nivel_preparado

	comprobar_finalización_nivel_aleatorio -> Estado a E_FINALIZANDO_NIVEL
	forzar_finalizacion_nivel_preparado -> Estado a E_FINALIZANDO_NIVEL

loop_principal: estado E_FINALIZANDO_NIVEL: aumentar_nivel

	iniciar_animacion_fin_juego
	cambio de estado a E_PREPARANDO_JUGADOR_INICIAR_NIVEL

loop_principal: estado E_PREPARANDO_JUGADOR_INICIAR_NIVEL -> esperando input.
	
	cargar_condiciones_nivel_e_iniciar() -> Estado a E_JUEGO
*/

/*Este otro se encarga de alimentar el turno de "Estructura_info_nivel" y 
de mover la cámara y el terreno de juego si procede. Sólo lo llamamos una vez
a fecha 25-abr-2013, pero por si acaso lo distribuimos por otros lados lo
meteremos en algún método...*/

void Controlador_juego::proceso_turno_nivel_estructura_info(float p_delta)
{
	this->estructura_info_nivel.turno(p_delta);

	//En game over detenemos la cámara.

	if(this->estado_actual==E_GAME_OVER || this->estado_actual==E_NUEVA_PUNTUACION_MAXIMA)
	{

	}
	else
	{
		this->camara.movimiento_relativo(0, -(this->estructura_info_nivel.acc_avance_este_turno()));
		Actor::establecer_terreno_juego(this->camara.acc_caja()); //OJO: Este va SIEMPRE después de mover la cámara.
	}
}

void Controlador_juego::comprobar_finalizacion_nivel_aleatorio()
{
	if(this->cont_gemas>=this->estructura_info_nivel.acc_gemas_en_nivel())
	{
		this->cambiar_estado(E_FINALIZANDO_NIVEL);
	}
}

/*Fuerza la finalización de un nivel preparado. Lo que hace es forzar la 
finalización de todas las acciones, borrar todos los actores (las acciones ya
no pueden referirse a ellos) y luego hacer las comprobaciones de turno para
ver si el nivel puede o no finalizarse.*/

void Controlador_juego::forzar_finalizacion_nivel_preparado()
{
	//Forzar finalización de todas las acciones (esto incluirá a la que ha llamado).

	LOG<<"INICIANDO FORZADO FIN ACCIONES"<<std::endl;

	auto finalizar_accion=[](Accion_trigger_compuesta& accion) {accion.forzar_finalizacion();};
	for_each(vector_acciones_corrientes.begin(), vector_acciones_corrientes.end(), finalizar_accion);

	//Eliminar todos los actores. Estén o no en pantalla.
	LOG<<"INICIANDO BORRADO DE ACTORES, RUTAS Y DECORACIONES"<<std::endl;
	vaciar_actores_mapa();
	vaciar_mapa_rutas();
	vaciar_decoraciones();

	CONTROLADOR_PUNTUACION.igualar_representada_a_real();

	//Aunque hayas finalizado el nivel es posible que no hayas obtenido todas
	//las gemas requeridas.

	LOG<<"INICIANDO COMPROBACIONES"<<std::endl;

	if(this->cont_gemas>=this->estructura_info_nivel.acc_gemas_en_nivel())
	{
		LOG<<"CON GEMAS SUFICIENTES"<<std::endl;
		this->cambiar_estado(E_FINALIZANDO_NIVEL);
	}
	else
	{
		LOG<<"SIN GEMAS SUFICIENTES"<<std::endl;
		this->restar_vida_jugador();
	}
}

/*Simplemente se encarga de subir el nivel y poner las gemas de nuevo a 0.
Luego se pone en estado de iniciar nivel.*/

void Controlador_juego::aumentar_nivel()
{
	//Comprobamos el trofeo de terminar el nivel sin perder gemas...
	if(!this->datos_actualizables.nivel_gemas_perdidas)
	{
		this->disparar_reto(Definicion_retos::R_NIVEL_CON_TODAS_GEMAS);
	}

	//Y el trofeo de terminar un nivel sin disparar...
	if(!this->datos_actualizables.nivel_disparos_efectuados)
	{
		this->disparar_reto(Definicion_retos::R_NIVEL_PACIFISTA);
	}

	//Comprobamos: si se ha destruido asteroide secreto podemos terminar
	//en un nivel extraño. También, si salimos de un nivel secreto iremos
	//a parar a otro (y añadiremos que se ha terminado ese nivel secreto).
	//Lo guardamos en esta variable antes de reiniciar los datos.

	bool destruido_asteroide_secreto=this->estructura_info_nivel.es_destruido_asteroide_secreto();
	unsigned int nivel_original=nivel_actual;
	
	this->datos_actualizables.reiniciar_nivel();
	this->campo_estrellas.restaurar();

	this->nivel_actual=destruido_asteroide_secreto && this->estructura_info_nivel.acc_proximo_nivel_secreto()
		? this->estructura_info_nivel.acc_proximo_nivel_secreto() 
		: this->estructura_info_nivel.acc_proximo_nivel();
	
	//Guardar estadísticas del nivel...
	Estructura_datos_perfil_nivel estructura_fin_nivel(
		estructura_info_nivel.total_recogible(),
		estructura_info_nivel.cuenta_recogible(),
		estructura_info_nivel.total_destruible(),
		estructura_info_nivel.cuenta_destruible(),
		estructura_info_nivel.acc_vidas_perdidas(),
		estructura_info_nivel.acc_puntuacion_acumulada());

	controlador_perfiles.actualizar_informacion_nivel(estructura_fin_nivel, nivel_actual, true);

	//Si estamos saliendo de un nivel secreto queremos comprobar retos...

	if(LISTA_NIVELES.es_nivel_secreto_indice(nivel_original))
	{
		//Los niveles secretos están al final de la lista luego el primero de 
		//ellos viene realmente tras los normales. Además, hay una posibilidad 
		//de conseguir un nuevo reto!.

		if(this->controlador_perfiles.es_finalizado_todos_niveles_secretos())
		{
			this->disparar_reto(Definicion_retos::R_NIVELES_SECRETOS);
		}
	}


	//Este es el modelo por defecto para pasar de un nivel a otro:
	//Si el siguiente índice de nivel es cero, hemos terminado el 
	//juego.

	if(!this->nivel_actual)
	{	
		this->iniciar_animacion_fin_juego();
	}
	else
	{
		componer_representacion_estadisticas_fin_nivel(
			nivel_original, 
			this->estructura_info_nivel.acc_modo_juego()==Estructura_info_nivel::M_PREPARADO, 
			LISTA_NIVELES.es_nivel_secreto_indice(nivel_actual));
		this->cambiar_estado(E_PREPARANDO_JUGADOR_INICIAR_NIVEL);
	}
}

/*Este método se encarga de establecer la cantidad de gemas y otras historias
que haya por cada nivel. Luego pone al controlador en modo de juego.
Ocurre al finalizar un nivel y al pulsar alguna tecla.
*/

void Controlador_juego::cargar_condiciones_nivel_e_iniciar(unsigned short int p_nivel)
{
LOG<<"CARGAR CONDICIONES E INICIAR NIVEL "<<p_nivel<<std::endl;

	//Necesitamos guardar momentáneamente estos valores...
	float distancia_recorrida_nivel_anterior=this->estructura_info_nivel.acc_distancia();
	unsigned int modo_nivel_anterior=this->estructura_info_nivel.acc_modo_juego();

	//Reiniciamos ahora. Sin reiniciar dirá que el archivo está cargado.
	this->estructura_info_nivel.reiniciar();

	if(!(this->estructura_info_nivel.es_archivo_cargado()))
	{
		try
		{	
			std::string cadena=LISTA_NIVELES.obtener_ruta_para_indice(p_nivel);

LOG<<"CARGANDO FICHERO "<<cadena<<std::endl;

			if(!(this->estructura_info_nivel.abrir_fichero(cadena)))
			{
LOG<<"ERROR AL ABRIR FICHERO"<<std::endl;
				throw Excepcion_salida(Excepcion_salida::E_IMPOSIBLE_ABRIR_FICHERO_NIVEL);
			}
		}
		catch(Lista_niveles_excepcion e)
		{
			throw Excepcion_salida(Excepcion_salida::E_IMPOSIBLE_ABRIR_FICHERO_NIVEL);
		}
	}

	this->estructura_info_nivel.cargar_valores_para_nivel(p_nivel);

	this->max_superficie_ocupada+=5120;
	this->superficie_ocupada=0;
	this->excedida_superficie_maxima=false;
	this->cont_obstaculos=0;
	this->cont_bonus=0;
	this->cont_gemas=0;
	this->actualizar_datos_hud_gemas();
	this->extra_velocidad_actores=this->estructura_info_nivel.acc_vector_y(); //Esto sólo se usa en modo aleatorio. Pero no molesta :D.
	No_jugador::mut_permitir_vector_cero(this->estructura_info_nivel.acc_modo_juego()==Estructura_info_nivel::M_PREPARADO);

	//Miramos si el artefacto en uso está permitido y si no lo está,
	//lo cambiamos por el siguiente permitido que encuentre.

	establecer_artefacto_jugador_auto();


	//Es posible que el nivel anterior sea preparado y eso significa que el jugador tendrá una posición Y que corregir.

	if(modo_nivel_anterior==Estructura_info_nivel::M_PREPARADO)
	{
		this->restaurar_posiciones_post_nivel_preparado(distancia_recorrida_nivel_anterior);
		this->camara.enfocar_a(0,0);
	}

	if(this->estructura_info_nivel.acc_modo_juego()==Estructura_info_nivel::M_PREPARADO)
	{
		//Cargar el nivel en si... Reinicia tb las estadísticas.
		this->poblar_juego_preparado();
	}
	else
	{
		//Si no es preparado vamos a reiniciar las estadísticas igualmente.
		this->estructura_info_nivel.reiniciar_info_perfil();
	}
	
	//Establecemos el estado.
	this->cambiar_estado(E_JUEGO);
}

/*Al finalizar un nivel preparado el jugador se ha desplazado muchas unidades
en el eje "Y"... Ahora tocaría volverlo a poner de nuevo en la posición inicial
para poder iniciar un nuevo nivel... La distancia está expresada como un número
negativo.
*/

void Controlador_juego::restaurar_posiciones_post_nivel_preparado(float p_dist)
{
	this->J->ir_a(this->J->acc_x(), this->J->acc_y()-p_dist);	
	this->M->ir_a(this->M->acc_x(), this->M->acc_y()-p_dist);
	this->F->ir_a(this->F->acc_x(), this->F->acc_y()-p_dist);

	//Y ahora los proyectiles que hubiera.
	std::vector<Proyectil*>::iterator 	ini_pr=this->proyectiles.begin(),
						fin_pr=this->proyectiles.end();

	while(ini_pr < fin_pr)
	{
		(*ini_pr)->ir_a((*ini_pr)->acc_x(), (*ini_pr)->acc_y()-p_dist);
		ini_pr++;
	}
}


/*Cuando nos liquidan en un nivel preparado y nos quedan vidas, ponemos en
marcha la animación de reiniciar el nivel... Básicamente es fundido a negro.
Justo antes hay que hacer una cuenta atrás, para que no se vea tan brusco.*/

void Controlador_juego::preparar_reinicio_nivel()
{
	contador_pre_desvanecer_al_perder_vida=(float) MS_CONTADOR_PRE_DESVANECER_PERDER_VIDA / 1000;
	cambiar_estado(E_REINICIANDO_PRE_CUENTA_DESVANECIENDO);
}

/*Cuando nos liquidan en un nivel preparado y nos quedan vidas se reinicia
el nivel por completo. Lo que hacemos es vaciar todos los contenedores, 
rebobinar el nivel, mover de nuevo al jugador al inicio y establecer el estado
como E_JUEGO...*/ 

void Controlador_juego::reiniciar_nivel_preparado()
{
	this->vaciar_actores_mapa();
	this->vaciar_vector_de_acciones();
	this->vaciar_proyectiles();
	this->vaciar_decoraciones();
	this->vaciar_mapa_rutas();
	this->estructura_info_nivel.rebobinar();
	CONTROLADOR_PUNTUACION.igualar_representada_a_real();

	establecer_artefacto_jugador_auto();

	this->datos_actualizables.reiniciar_nivel();

	controlador_audio.detener_sonido();

	this->poblar_juego_preparado(); //Puebla y reinicia también las estadísticas.

	this->cont_obstaculos=0;
	this->cont_bonus=0;
	this->cont_gemas=0;

	this->J->detener_vector();
	this->J->ir_a(400, 300);

	this->F->ir_a(400, 300);
	this->J->detener_vector();

	this->M->ir_a(400, 300);
	this->J->detener_vector();

	this->camara.enfocar_a(0,0);

	this->actualizar_datos_hud_gemas();
	this->CONTROLADOR_PUNTUACION.reiniciar_combo();
	this->actualizar_marcador_combo();

	this->cambiar_estado(E_JUEGO);
	this->campo_estrellas.restaurar();


}

/*Introduce nuevos actores en el juego siempre que se den las condiciones. Las
condiciones son, básicamente, que el número de obstáculos y bonus presentes sea 
suficientemente pequeño. Se sirve de dos métodos de ayuda.*/


void Controlador_juego::poblar_juego_aleatorio()
{
	if( (rand() % 10) + this->cont_obstaculos < this->estructura_info_nivel.acc_max_obstaculos() && !this->excedida_superficie_maxima)
	{
		this->insertar_obstaculo();		
	}

	if( (rand() % 20) + this->cont_bonus < this->estructura_info_nivel.acc_max_bonus())
	{		
		short int tirada=this->estructura_info_nivel.generar_tirada_tabla(Estructura_info_nivel::T_BONUS);
		this->insertar_bonus(tirada);
	}
}

/*El modo de juego preparado son niveles guardados en un fichero. 

Al contrario que en el modo aleatorio: el jugador se mueve en el eje Y en lugar
de hacer que los obstáculos vengan desde -16 hacia abajo. Esto da todo tipo
de dolores de cabeza con los ajustes de posiciones de algunas cosas.

Se ejecuta una sóla vez.
*/

void Controlador_juego::poblar_juego_preparado()
{
	if(this->estructura_info_nivel.es_archivo_cargado())
	{
		int recogibles=0, destruibles=0;

		while(this->estructura_info_nivel.debe_cargar())
		{
			Actor_serializado act=this->estructura_info_nivel.obtener();

			if(act.valido)
			{
				Actor_mapa * actor_mapa=Factoria_actores_mapa::fabricar(act.tipo, 1.0);
				
				if(actor_mapa)
				{
					actor_mapa->preparar_actor_mapa_manual(act.x, act.y, 0, 0);
					actor_mapa->mut_tag(act.tag);

					if(actor_mapa->es_parametrizable())
						actor_mapa->parametrizar_actor_en_nivel_preparado(act.parametros);

					if(actor_mapa->es_flexible())
					{
						actor_mapa->recibir_valores_actor_flexible(act.params_rep);
					}

					if(actor_mapa->es_redimensionable())
					{
						actor_mapa->recibir_valores_actor_redimensionable(act.params_redim);
					}

					if(actor_mapa->es_ordenable_profundidad())
					{
						actor_mapa->recibir_valores_actor_ordenable_profundidad(act.params_profundidad);
					}

						
					this->actores_mapa.push_back(actor_mapa);
					unsigned short int tipo_cuenta=actor_mapa->tipo_a_cuenta();

					switch(tipo_cuenta)
					{
						case Actor_mapa::T_CUENTA_OBSTACULO:
							this->cont_obstaculos++;
							this->superficie_ocupada+=actor_mapa->acc_superficie();	//Esto realmente no importa nada porque ni se cuenta, pero por si acaso.
						break;

						case Actor_mapa::T_CUENTA_BONUS:
						case Actor_mapa::T_CUENTA_GEMA:
							this->cont_bonus++;
						break;
					}


					if(actor_mapa->es_cuenta_destruible()) ++destruibles;
					if(actor_mapa->es_cuenta_recogible()) ++recogibles;

					//Siendo del tipo de punto de ruta lo guardamos en el
					//mapa destinado para ello.

					if(actor_mapa->tipo_actor_mapa()==Actor_mapa::T_PUNTO_RUTA)
					{
						this->organizar_puntos_ruta( static_cast<Punto_ruta *> (actor_mapa));
					}
				}
			}
		}

		this->estructura_info_nivel.establecer_info_perfil(destruibles, recogibles);
		this->estructura_info_nivel.reiniciar_info_perfil();
	}
}

void Controlador_juego::organizar_puntos_ruta(Punto_ruta * p_punto)
{
	unsigned int agrupacion=p_punto->acc_agrupacion();

	t_mapa_puntos_ruta::iterator buscar;
	buscar=this->puntos_ruta.find(agrupacion);

	//Si no existe aún la agrupación la insertamos aquí...
	if(buscar == this->puntos_ruta.end())
	{
		std::map<unsigned int, Punto_ruta *> temp;
		temp.insert(t_par_punto_ruta_simple(p_punto->acc_identificador(), p_punto));
		this->puntos_ruta.insert(t_par_puntos_ruta(agrupacion, temp));
	}
	//Si la agrupación existe, extraemos el vector y le metemos el nuevo punto...
	else
	{
		buscar->second.insert(t_par_punto_ruta_simple(p_punto->acc_identificador(), p_punto));
	}
}

/*Cuando se cancela a la fuerza una partida se graba la información del nivel
en el perfil y se sale... No se graba ningún tipo de puntuación o items 
recogidos: tan sólo se guardan las vidas perdidas y, al guardar, se suma uno a 
las veces que se ha iniciado.*/

void Controlador_juego::cancelar_partida()
{
	Estructura_datos_perfil_nivel estructura_fin_nivel(
		estructura_info_nivel.total_recogible(),
		0,
		estructura_info_nivel.total_destruible(),
		0,
		estructura_info_nivel.acc_vidas_perdidas(),
		0);

	controlador_perfiles.actualizar_informacion_nivel(estructura_fin_nivel, nivel_actual, false);	

	this->controlador_perfiles.abortar_partida_con_perfil();
	this->salir=true;
}
