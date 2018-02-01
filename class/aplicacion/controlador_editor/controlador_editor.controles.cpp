void Controlador_editor::controles_general()
{
	if(
		this->estado_actual==E_EDITAR_PARAMETROS ||
		this->estado_actual==E_EDITAR_TAG ||
		this->estado_actual==E_EDITAR_PROPIEDADES ||
		this->estado_actual==E_EDITAR_TABLA_OBSTACULOS ||
		this->estado_actual==E_EDITAR_TABLA_BONUS ||
		this->estado_actual==E_EDITAR_TABLA_BONUS_CAJA ||
		this->estado_actual==E_EDITAR_TABLA_BONUS_OBSTACULO ||
		this->estado_actual==E_EDITAR_ACCIONES) 
	{
		this->escritor.loop_control();
	}
	else
	{
		this->controles_sdl.recoger();

		//Cambios de estado.
		if(this->controles_sdl.es_tecla_down(SDLK_1)) this->cambiar_estado(E_EDITAR_NIVEL);
		else if(this->controles_sdl.es_tecla_down(SDLK_2)) this->cambiar_estado(E_EDITAR_PROPIEDADES);
		else if(this->controles_sdl.es_tecla_down(SDLK_3)) this->cambiar_estado(E_EDITAR_TABLA_OBSTACULOS);
		else if(this->controles_sdl.es_tecla_down(SDLK_4)) this->cambiar_estado(E_EDITAR_TABLA_BONUS);
		else if(this->controles_sdl.es_tecla_down(SDLK_5)) this->cambiar_estado(E_EDITAR_TABLA_BONUS_CAJA);
		else if(this->controles_sdl.es_tecla_down(SDLK_6)) this->cambiar_estado(E_EDITAR_TABLA_BONUS_OBSTACULO);
		else if(this->controles_sdl.es_tecla_down(SDLK_7)) this->cambiar_estado(E_EDITAR_ACCIONES);
		else if(this->controles_sdl.es_tecla_down(SDLK_8)) this->cambiar_estado(E_LISTADO);
		else if(this->controles_sdl.es_tecla_down(SDLK_9)) this->cambiar_estado(E_INFO);
		else if(this->controles_sdl.es_tecla_down(SDLK_F5)) this->exportar_imagen();
	}		

	//Controles.
	switch(this->estado_actual)
	{
		case E_EDITAR_NIVEL: this->controles_modo_editar_nivel(); break;
		case E_EDITAR_PROPIEDADES: this->controles_modo_editar_propiedades();break;
		case E_EDITAR_TABLA_OBSTACULOS:
		case E_EDITAR_TABLA_BONUS: 
		case E_EDITAR_TABLA_BONUS_CAJA: 
		case E_EDITAR_TABLA_BONUS_OBSTACULO:
			this->controles_modo_editar_tablas(this->estado_actual); break;
		break;
		case E_EDITAR_PARAMETROS: this->controles_modo_editar_parametros(); break;
		case E_EDITAR_TAG: this->controles_modo_editar_tag(); break;
		case E_EDITAR_ACCIONES: this->controles_modo_editar_acciones(); break;
		case E_LISTADO: this->controles_modo_listado(); break;
		case E_INFO: this->controles_modo_info(); break;
	}
}

void Controlador_editor::controles_modo_editar_nivel_representacion_flexible()
{
	if(! (this->seleccion_actual && this->seleccion_actual->actor->es_flexible()))
	{
		this->tipo_modificar_actor=T_NORMAL;
		return;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
	{
		this->tipo_modificar_actor=T_NORMAL;
		return;
	}
	//Mover textura izq.
	else if(this->controles_sdl.es_tecla_down(SDLK_LEFT)) this->actualizar_representacion_flexible(-1, 0, 0, 0, 0, 0, 0);
	//Mover textura der
	else if(this->controles_sdl.es_tecla_down(SDLK_RIGHT)) this->actualizar_representacion_flexible(1, 0, 0, 0, 0, 0, 0);
	//Mover textura arr.		
	else if(this->controles_sdl.es_tecla_down(SDLK_UP)) this->actualizar_representacion_flexible(0, -1, 0, 0, 0, 0, 0);
	//Mover textura abajo.
	else if(this->controles_sdl.es_tecla_down(SDLK_DOWN)) this->actualizar_representacion_flexible(0, 1, 0, 0, 0, 0, 0);
	//Recorte textura ancho menos...
	else if(this->controles_sdl.es_tecla_down(SDLK_a)) this->actualizar_representacion_flexible(0, 0, -1, 0, 0, 0, 0);
	//Recorte textura ancho mas...
	else if(this->controles_sdl.es_tecla_down(SDLK_d)) this->actualizar_representacion_flexible(0, 0, 1, 0, 0, 0, 0);
	//Recorte textura alto menos...
	else if(this->controles_sdl.es_tecla_down(SDLK_w)) this->actualizar_representacion_flexible(0, 0, 0, -1, 0, 0, 0);
	//Recorte textura alto más...
	else if(this->controles_sdl.es_tecla_down(SDLK_s)) this->actualizar_representacion_flexible(0, 0, 0, 1, 0, 0, 0);

	//Alineamiento...
	else if(this->controles_sdl.es_tecla_down(SDLK_f)) this->actualizar_representacion_flexible(0, 0, 0, 0, -1, 0, 0);
	else if(this->controles_sdl.es_tecla_down(SDLK_h)) this->actualizar_representacion_flexible(0, 0, 0, 0, 1, 0, 0);
	else if(this->controles_sdl.es_tecla_down(SDLK_t)) this->actualizar_representacion_flexible(0, 0, 0, 0, 0, -1, 0);
	else if(this->controles_sdl.es_tecla_down(SDLK_g)) this->actualizar_representacion_flexible(0, 0, 0, 0, 0, 1, 0);

	else if(this->controles_sdl.es_tecla_down(SDLK_PAGEUP)) this->actualizar_representacion_flexible(0, 0, 0, 0, 0, 0, -1);
	else if(this->controles_sdl.es_tecla_down(SDLK_PAGEDOWN)) this->actualizar_representacion_flexible(0, 0, 0, 0, 0, 0, 1);
	else if(this->controles_sdl.es_tecla_down(SDLK_SPACE)) this->actualizar_representacion_seleccion_actual(0, 0, 32, 32, 0, 0, -1);
}

void Controlador_editor::controles_modo_editar_nivel_tamano()
{
	if(! (this->seleccion_actual && this->seleccion_actual->actor->es_redimensionable()))
	{
		this->tipo_modificar_actor=T_NORMAL;
		return;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
	{
		this->tipo_modificar_actor=T_NORMAL;
		return;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_LEFT)) this->actualizar_tamano(-1, 0);
	else if(this->controles_sdl.es_tecla_down(SDLK_RIGHT)) this->actualizar_tamano(1, 0);
	else if(this->controles_sdl.es_tecla_down(SDLK_UP)) this->actualizar_tamano(0, -1);
	else if(this->controles_sdl.es_tecla_down(SDLK_DOWN)) this->actualizar_tamano(0, 1);
}

void Controlador_editor::controles_modo_editar_nivel_normal()
{
	if(this->controles_sdl.es_tecla_down(SDLK_c)) this->tipo_modificar_actor=T_REPRESENTACION_FLEXIBLE;
	else if(this->controles_sdl.es_tecla_down(SDLK_x)) this->tipo_modificar_actor=T_TAMANO;
	else if(this->controles_sdl.es_tecla_down(SDLK_d)) this->duplicar_actor_actual();
	else if(this->controles_sdl.es_tecla_down(SDLK_ESCAPE)) this->seleccion_actual=NULL;
	else if(this->controles_sdl.es_tecla_down(SDLK_q)) this->actualizar_filtro_profundidad(F_FILTRO_FONDO);
	else if(this->controles_sdl.es_tecla_down(SDLK_w)) this->actualizar_filtro_profundidad(F_FILTRO_NORMAL);
	else if(this->controles_sdl.es_tecla_down(SDLK_e)) this->actualizar_filtro_profundidad(F_FILTRO_FRENTE);
	else if(this->controles_sdl.es_tecla_down(SDLK_a)) mostrar_ocultos=!mostrar_ocultos;
	else if(this->controles_sdl.es_tecla_down(SDLK_m)) this->vaciar_seleccion_multiple();
	else if(this->controles_sdl.es_tecla_down(SDLK_o)) this->eliminar_duplicados();
	else if(this->controles_sdl.es_tecla_down(SDLK_r)) duplicar_seleccion_multiple_obstaculo_flexible_en_decoracion(Actor_mapa::T_DECORACION_NIVEL_FONDO);
	else if(this->controles_sdl.es_tecla_down(SDLK_y)) duplicar_seleccion_multiple_obstaculo_flexible_en_decoracion(Actor_mapa::T_DECORACION_NIVEL_FRENTE);

	else if(this->controles_sdl.es_tecla_down(SDLK_LEFT)) 
	{
		if(this->seleccion_actual || this->seleccion_multiple.size()) this->desplazar_seleccion_actual(-1, 0);
		else this->escoger_tipo(-1);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_RIGHT)) 
	{
		if(this->seleccion_actual || this->seleccion_multiple.size()) this->desplazar_seleccion_actual(1, 0);
		else this->escoger_tipo(1);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_UP)) 
	{
		if(this->seleccion_actual || this->seleccion_multiple.size()) 
		{
			this->desplazar_seleccion_actual(0, -1);
			this->desplazar_camara(-1);
		}
		else this->desplazar_camara(-1);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_DOWN)) 
	{
		if(this->seleccion_actual || this->seleccion_multiple.size()) 
		{
			this->desplazar_seleccion_actual(0, 1);
			this->desplazar_camara(1);
		}
		else this->desplazar_camara(1);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_PAGEUP)) 
	{
		if(!this->seleccion_actual) this->escoger_grupo(-1);
		else this->seleccionar_actual(-1);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_PAGEDOWN)) 
	{
		if(!this->seleccion_actual) this->escoger_grupo(1);
		else this->seleccionar_actual(1);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_p) 
		&& this->seleccion_actual 
		&& this->seleccion_actual->actor->es_parametrizable()) 
	{
		this->indice_parametro=0;
		this->cambiar_estado(E_EDITAR_PARAMETROS);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_v) 
		&& this->seleccion_actual 
		&& this->seleccion_actual->actor->es_ordenable_profundidad()) 
	{	
		if(controles_sdl.es_tecla_pulsada(SDLK_LSHIFT)) modificar_profundidad_seleccion_actual(-20);
		else modificar_profundidad_seleccion_actual(-1);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_b) 
		&& this->seleccion_actual 
		&& this->seleccion_actual->actor->es_ordenable_profundidad()) 
	{
		if(controles_sdl.es_tecla_pulsada(SDLK_LSHIFT)) modificar_profundidad_seleccion_actual(20);
		else modificar_profundidad_seleccion_actual(1);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_t) 
		&& this->seleccion_actual) 
	{
		this->cambiar_estado(E_EDITAR_TAG);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_DELETE)) 
	{
		if(this->seleccion_actual || this->seleccion_multiple.size()) this->eliminar_actor_seleccionado();
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_g)) this->mostrar_rejilla=!this->mostrar_rejilla;
	else if(this->controles_sdl.es_tecla_down(SDLK_h)) this->bloquear_a_rejilla=!this->bloquear_a_rejilla;
	else if(this->controles_sdl.es_tecla_down(SDLK_k)) this->mostrar_info=!this->mostrar_info;

	//Datos...
	else if(this->controles_sdl.es_tecla_down(SDLK_l)) this->cargar();
	else if(this->controles_sdl.es_tecla_down(SDLK_s)) 
	{
		this->guardar(); 		
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_n)) this->reiniciar();

}

void Controlador_editor::controles_modo_editar_nivel()
{
	//Click ratón...
	if(this->controles_sdl.recibe_eventos_boton_raton())
	{
		this->procesar_click_raton();
	}

	switch(this->tipo_modificar_actor)
	{
		case T_REPRESENTACION_FLEXIBLE:
			if(this->seleccion_actual) this->controles_modo_editar_nivel_representacion_flexible(); 
		break;

		case T_TAMANO:
			this->controles_modo_editar_nivel_tamano(); 
		break;

		case T_NORMAL:
			this->controles_modo_editar_nivel_normal();
		break;
	}

	if(this->controles_sdl.es_tecla_down(SDLK_COMMA)) this->modificar_rejilla(-1);
	else if(this->controles_sdl.es_tecla_down(SDLK_PERIOD)) this->modificar_rejilla(1);
}

void Controlador_editor::controles_modo_editar_propiedades()
{
	if(this->controles_sdl.es_tecla_down(SDLK_ESCAPE)) this->cambiar_estado(E_EDITAR_NIVEL);
	else if(this->controles_sdl.es_tecla_down(SDLK_UP)) this->cambiar_propiedad(-1);
	else if(this->controles_sdl.es_tecla_down(SDLK_DOWN)) this->cambiar_propiedad(1);
	else if(this->controles_sdl.es_tecla_down(SDLK_RETURN))
	{
		unsigned int valor=DLibH::Herramientas::cadena_a_entero(this->escritor.acc_cadena());
		this->escritor.limpiar();

		//Confirmar...
		switch(this->editando_propiedad)
		{
			case P_NIVEL_VECTOR_Y: 				this->modificar_vector(valor); break;
			case P_NIVEL_FLAGS_ARTEFACTOS_CANON: 		this->modificar_artefactos(Definiciones::P_CANON, valor); break;
			case P_NIVEL_FLAGS_ARTEFACTOS_TELETRANSPORTE: 	this->modificar_artefactos(Definiciones::P_TELETRANSPORTADOR, valor); break;
			case P_NIVEL_FLAGS_ARTEFACTOS_EDU:		this->modificar_artefactos(Definiciones::P_EMISOR_DENSIDAD_UNIFICADA, valor); break;		
			case P_NIVEL_FLAGS_ARTEFACTOS_ESCUDO:		this->modificar_artefactos(Definiciones::P_ESCUDO, valor); break;		
			case P_NIVEL_MAX_OBSTACULOS:			this->modificar_max_obstaculos(valor); break;			
			case P_NIVEL_MAX_BONUS:				this->modificar_max_bonus(valor); break;
			case P_NIVEL_GEMAS_EN_NIVEL:			this->modificar_gemas_en_nivel(valor); break;
			case P_NIVEL_GEMAS_MAX_NIVEL:			this->modificar_gemas_max_nivel(valor); break;
			case P_PROXIMO_NIVEL:				INFO_NIVEL.indice_proximo_nivel=valor; break;
			case P_PROXIMO_NIVEL_SECRETO:			INFO_NIVEL.indice_proximo_nivel_secreto=valor; break;
			case P_INDICE_MUSICA:				INFO_NIVEL.indice_musica=valor; break;
		}
	}

	this->cadena_parametros=this->escritor.acc_cadena();
}

void Controlador_editor::controles_modo_editar_acciones()
{
	if(this->controles_sdl.es_tecla_down(SDLK_SPACE) && this->indice_parametro_accion!=1)
	{
		this->insertar_nueva_accion();
	}
	if(this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
	{
		this->cambiar_estado(E_EDITAR_NIVEL);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_DELETE))
	{
		this->eliminar_accion_actual();
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_LEFT)) 
	{
		this->cambiar_accion_actual(-1);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_RIGHT)) 
	{
		this->cambiar_accion_actual(1);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_UP))
	{
		if(this->indice_parametro_accion<=0) this->indice_parametro_accion=9;
		else --this->indice_parametro_accion;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_DOWN))
	{
		++this->indice_parametro_accion;
		if(this->indice_parametro_accion >= 10) this->indice_parametro_accion=0;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_RETURN))
	{
		std::string cadena=this->escritor.acc_cadena();
		long int valor=DLibH::Herramientas::cadena_a_entero(cadena); //OJO: TIene signo...
		this->escritor.limpiar();

		switch(this->indice_parametro_accion)
		{
			case 0: this->acciones[this->indice_accion].mut_tipo(valor); break;
			case 1: this->acciones[this->indice_accion].mut_descripcion(cadena.c_str()); break;
			case 2: this->acciones[this->indice_accion].mut_tag(valor); break;
			case 3: this->acciones[this->indice_accion].mut_tag_actores(valor); break;
			case 4: this->acciones[this->indice_accion].mut_tag_actores_secundario(valor); break;
			case 5: this->acciones[this->indice_accion].mut_tag_siguiente(valor); break;
			case 6: this->acciones[this->indice_accion].mut_ms_antes(valor); break;
			case 7: this->acciones[this->indice_accion].mut_ms_despues(valor); break;
			case 8: this->acciones[this->indice_accion].mut_desp_x(valor); break;
			case 9: this->acciones[this->indice_accion].mut_desp_y(valor); break;
		}
	}

	this->cadena_acciones=this->escritor.acc_cadena();
}


void Controlador_editor::procesar_click_raton()
{
	int raton_x=0, raton_y=0;

	if(this->controles_sdl.es_boton_down(SDL_BUTTON_LEFT)) 
	{
		raton_x=this->controles_sdl.acc_raton().acc_x();
		raton_y=this->controles_sdl.acc_raton().acc_y();

		if(this->controles_sdl.es_tecla_pulsada(SDLK_LSHIFT))
		{
			CAJA_SELECCION_MULTIPLE.reiniciar();					
			T_TIPO_ACTOR * sel=this->obtener_actor_en_posicion(raton_x, raton_y);

			if(sel)
			{
				if(!existe_en_seleccion_multiple(sel)) 	this->insertar_en_seleccion_multiple(sel);
				else this->sacar_de_seleccion_multiple(sel);
			}
		}
		else if(this->controles_sdl.es_tecla_pulsada(SDLK_RSHIFT))
		{
			raton_y+=this->posicion_y;

			switch(CAJA_SELECCION_MULTIPLE.estado)
			{
				case Caja_seleccion_multiple::E_INTRODUCIR_PUNTO_1:
					CAJA_SELECCION_MULTIPLE.introducir_punto1(raton_x, raton_y);
				break;

				case Caja_seleccion_multiple::E_INTRODUCIR_PUNTO_2:
					CAJA_SELECCION_MULTIPLE.introducir_punto2(raton_x, raton_y);
					CAJA_SELECCION_MULTIPLE.actualizar_caja();
					seleccionar_actores_en_caja(CAJA_SELECCION_MULTIPLE.caja);
				break;
			}			
		}
		else
		{
			CAJA_SELECCION_MULTIPLE.reiniciar();
			this->seleccion_actual=this->obtener_actor_en_posicion(raton_x, raton_y);

			if(!this->seleccion_actual)
			{
				Parametros_actor dummy_param=Parametros_actor();
				Parametros_actor_flexible dummy_rep=Parametros_actor_flexible();
				Parametros_actor_redimensionable dummy_redim=Parametros_actor_redimensionable();

				unsigned int tipo=this->TIPOS[this->grupo_tipo_actual].TIPOS[this->tipo_actor_actual].tipo;

				Parametros_actor_ordenable_profundidad dummy_profundidad=Parametros_actor_ordenable_profundidad();

				switch(tipo)
				{
					case Actor_mapa::T_DECORACION_NIVEL_FONDO: dummy_profundidad.p=Actor_mapa::P_FONDO; break;
					case Actor_mapa::T_DECORACION_NIVEL_FRENTE: dummy_profundidad.p=Actor_mapa::P_FRENTE; break;
					default: dummy_profundidad.p=Actor_mapa::P_NORMAL; break;
				}

				dummy_rep.indice_recurso=Recursos::G_TILE_JUEGO_IND;
				dummy_redim.w=32;
				dummy_redim.h=32;
	
				this->insertar_actor_en_posicion(raton_x, raton_y, tipo, 0, dummy_param, dummy_rep, dummy_redim, dummy_profundidad, false, true);
			}
		}
	}
}

void Controlador_editor::controles_modo_editar_parametros()
{
	if(this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
	{
		this->cambiar_estado(E_EDITAR_NIVEL);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_UP))
	{
		--this->indice_parametro;
		if(this->indice_parametro < 0) this->indice_parametro=TOTAL_PARAMS-1;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_DOWN))
	{
		++this->indice_parametro;
		if(this->indice_parametro >= TOTAL_PARAMS) this->indice_parametro=0;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_RETURN))
	{
		unsigned int valor=DLibH::Herramientas::cadena_a_entero(this->escritor.acc_cadena());
		this->escritor.limpiar();

		//Confirmar...
		switch(this->indice_parametro)
		{
			case 0: this->seleccion_actual->parametros.param1=valor; break;
			case 1: this->seleccion_actual->parametros.param2=valor; break;
			case 2: this->seleccion_actual->parametros.param3=valor; break;
			case 3: this->seleccion_actual->parametros.param4=valor; break;
			case 4: this->seleccion_actual->parametros.param5=valor; break;
			case 5: this->seleccion_actual->parametros.param6=valor; break;
			case 6: this->seleccion_actual->parametros.param7=valor; break;
			case 7: this->seleccion_actual->parametros.param8=valor; break;
		}
	}

	this->cadena_parametros=this->escritor.acc_cadena();
}

void Controlador_editor::controles_modo_editar_tag()
{
	if(this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
	{
		this->estado_actual=E_EDITAR_NIVEL;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_RETURN))
	{
		unsigned int valor=DLibH::Herramientas::cadena_a_entero(this->escritor.acc_cadena());
		this->escritor.limpiar();
		this->seleccion_actual->actor->mut_tag(valor);

		this->actualizar_maximo_tag();
	}
}

void Controlador_editor::controles_modo_listado()
{
	if(this->controles_sdl.es_tecla_down(SDLK_SPACE))
	{
		this->seleccionar_indice(indice_actual_listado);		
		this->cambiar_estado(E_EDITAR_NIVEL);
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_DELETE))
	{
		this->seleccionar_indice(indice_actual_listado);		
		this->eliminar_actor_seleccionado();
		indice_actual_listado=0;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_p))
	{
		this->guardar_archivo_listado_actores();
	}

	if(this->controles_sdl.es_tecla_down(SDLK_UP))
	{
		if(indice_actual_listado>=1) --indice_actual_listado;
		else indice_actual_listado=actores_mapa.size()-1;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_DOWN))
	{
		if(indice_actual_listado+1 < actores_mapa.size()) ++indice_actual_listado;
		else indice_actual_listado=0;
	}
	if(this->controles_sdl.es_tecla_down(SDLK_PAGEUP))
	{
		if(indice_actual_listado>=20) indice_actual_listado-=20;
		else indice_actual_listado=0;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_PAGEDOWN))
	{
		if(indice_actual_listado+20 < actores_mapa.size()) indice_actual_listado+=20;
		else indice_actual_listado=actores_mapa.size()-1;
	}

}

void Controlador_editor::controles_modo_info()
{
	unsigned int alto_glifo=this->rep_txt_ayuda->acc_alto_glifo();
	int desp=0;

	if(this->controles_sdl.es_tecla_down(SDLK_UP)) 
	{
		desp=alto_glifo;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_DOWN)) 
	{
		desp=-alto_glifo;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_PAGEUP))
	{
		desp=20*alto_glifo;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_PAGEDOWN))
	{
		desp=-20*alto_glifo;
	}

	this->rep_txt_ayuda->desplazar(0, desp);

	//Controlar posicion...

	int pos=abs(this->rep_txt_ayuda->acc_posicion().y);
	int max_pos=this->rep_txt_ayuda->acc_alto_area()-150;

	if(this->rep_txt_ayuda->acc_posicion().y > 120) 
	{
		this->rep_txt_ayuda->establecer_posicion(0, 120);
	}
	else if(pos > max_pos)
	{
		this->rep_txt_ayuda->establecer_posicion(0, -max_pos);
	} 
}

void Controlador_editor::controles_modo_editar_tablas(unsigned int p_tabla)
{
	int l=0;

	switch(p_tabla)
	{
		case E_EDITAR_TABLA_OBSTACULOS: l=this->TABLA_OBSTACULOS.size(); break;
		case E_EDITAR_TABLA_BONUS: l=this->TABLA_BONUS.size(); break;
		case E_EDITAR_TABLA_BONUS_CAJA: l=this->TABLA_DESTRUIR_CAJA.size(); break;
		case E_EDITAR_TABLA_BONUS_OBSTACULO: l=this->TABLA_DESTRUIR_OBSTACULO.size(); break;
	}

	if(this->controles_sdl.es_tecla_down(SDLK_ESCAPE)) this->cambiar_estado(E_EDITAR_NIVEL);
	/*else if(this->controles_sdl.es_tecla_down(SDLK_LEFT) || this->controles_sdl.es_tecla_down(SDLK_RIGHT))
	{
		int factor=this->controles_sdl.es_tecla_down(SDLK_RIGHT) ? 1 : -1;

		switch(p_tabla)
		{
			case E_EDITAR_TABLA_OBSTACULOS: 	this->cambiar_dato_tabla(this->TABLA_OBSTACULOS, this->indice_editando_tabla, factor); break;
			case E_EDITAR_TABLA_BONUS: 		this->cambiar_dato_tabla(this->TABLA_BONUS, this->indice_editando_tabla, factor); break;
			case E_EDITAR_TABLA_BONUS_CAJA: 	this->cambiar_dato_tabla(this->TABLA_DESTRUIR_CAJA, this->indice_editando_tabla, factor); break;
			case E_EDITAR_TABLA_BONUS_OBSTACULO: 	this->cambiar_dato_tabla(this->TABLA_DESTRUIR_OBSTACULO, this->indice_editando_tabla, factor); break;
		}
		
	}*/
	else if(this->controles_sdl.es_tecla_down(SDLK_UP) || this->controles_sdl.es_tecla_down(SDLK_DOWN)) 
	{
		//El índice es sin signo, por eso hacemos eso...

		int factor=this->controles_sdl.es_tecla_down(SDLK_UP) ? -1 : 1;
		int t=this->indice_editando_tabla+factor;

		if(t < 0) t=l-1;
		else if(t >= l) t=0;

		this->indice_editando_tabla=t;
	}
	else if(this->controles_sdl.es_tecla_down(SDLK_RETURN))
	{
		unsigned int valor=DLibH::Herramientas::cadena_a_entero(this->escritor.acc_cadena());
		this->escritor.limpiar();

		switch(p_tabla)
		{
			case E_EDITAR_TABLA_OBSTACULOS: 	this->cambiar_dato_tabla(this->TABLA_OBSTACULOS, this->indice_editando_tabla, valor); break;
			case E_EDITAR_TABLA_BONUS: 		this->cambiar_dato_tabla(this->TABLA_BONUS, this->indice_editando_tabla, valor); break;
			case E_EDITAR_TABLA_BONUS_CAJA: 	this->cambiar_dato_tabla(this->TABLA_DESTRUIR_CAJA, this->indice_editando_tabla, valor); break;
			case E_EDITAR_TABLA_BONUS_OBSTACULO: 	this->cambiar_dato_tabla(this->TABLA_DESTRUIR_OBSTACULO, this->indice_editando_tabla, valor); break;
		}
	}

	this->cadena_parametros=this->escritor.acc_cadena();
}
