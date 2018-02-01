/*
	Reto * reto_apuntado=this->controlador_retos.obtener_por_indice(p_indice);
	rep.rellenar(0,0,0);

	if(this->controlador_perfiles.es_reto_conseguido(p_indice)) rep.volcar_representacion(*reto_apuntado->acc_representacion());
	else rep.volcar_representacion(*reto_apuntado->acc_representacion_off());
*/

void Controlador::mostrar_retos(std::vector<DLibV::Representacion *> & v_rep, std::vector<DLibV::Representacion *> & v_retos)
{
	this->campo_estrellas->acc_representacion()->volcar(this->pantalla);

	for(DLibV::Representacion * r : v_rep) r->volcar(pantalla);
	pantalla.establecer_clip(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CLIP_RETOS, Y_CLIP_RETOS, W_CLIP_RETOS, H_CLIP_RETOS));
	for(DLibV::Representacion * r : v_retos) r->volcar(pantalla);
	pantalla.reiniciar_clip_completo();

	this->pantalla.flipar();
}

void Controlador::procesar_estado_retos(std::vector<DLibV::Representacion *> & v_rep)
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

	//Este otro contiene sólo los retos...
	std::vector<DLibV::Representacion *> vector_ret;
	unsigned int i=0;
	while(i < Definicion_retos::TOTAL_RETOS)
	{
		Reto * reto_apuntado=this->controlador_retos.obtener_por_indice(i);
		if(this->controlador_perfiles.es_reto_conseguido(i)) vector_ret.push_back(reto_apuntado->acc_representacion());
		else vector_ret.push_back(reto_apuntado->acc_representacion_off());

		++i;
	}

	//Los posicionamos...
	i=0;
	int x=X_RETOS_INICIO;
	int y=Y_RETOS_INICIO;

	for(DLibV::Representacion * r : vector_ret)
	{
		r->establecer_posicion(x, y);

		if((i+1)%4) y+=SALTO_Y_RETOS;
		else
		{
			x+=SALTO_X_RETOS;
			y=Y_RETOS_INICIO;
		}

		++i;
	}
	
	Grupo_animacion * animacion=NULL;
	bool salir=false;

	int pag=0;
	int max_pag=4;

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
					int vector=direccion==1 ? -VECTOR_X_RETOS : VECTOR_X_RETOS;

					animacion=new Grupo_animacion();

					for(DLibV::Representacion * r : vector_ret)
					{
						int x=r->acc_posicion().x+(-direccion * SALTO_X_RETOS);
						int y=r->acc_posicion().y;
						animacion->asignar_animacion(new Estructura_animacion_movimiento(r, DLibH::Vector_2d(vector, 0), x, y));
					}
				}
			}	
		}

		if(!salir)
		{
			this->mostrar_retos(vector_rep, vector_ret);
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
}
