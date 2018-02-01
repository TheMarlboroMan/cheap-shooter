void Controlador_juego::procesar_turno_marcadores_combo(float p_delta)
{
	std::vector<Marcador_combo>::iterator 	ini=this->cola_combo.begin(),
						fin=this->cola_combo.end();

	while(ini<fin)
	{
		if(!(*ini).procesar_turno(p_delta))
		{
			ini=this->cola_combo.erase(ini);
			fin=this->cola_combo.end();
		}
		ini++;
	}
}

/*Se activa cada vez que el jugador recoge una gema para aumentar el contador
y comprobar si debe pasar o no de nivel.*/

void Controlador_juego::actualizar_marcador_combo()
{
	actualizar_combo=false;

	//Miramos el combo que hay ahora... 
	//Si el combo es distinto ponemos el primero a finalizar e insertamos uno nuevo.
	unsigned int tipo_actual=this->CONTROLADOR_PUNTUACION.acc_tipo_combo();
	unsigned int tipo_visible=this->cola_combo[0].acc_tipo_combo();

	LOG<<"ACTUALIZANDO MARCADOR COMBO DE "<<tipo_actual<<" VS "<<tipo_visible<<std::endl;

	if(tipo_actual != tipo_visible)
	{
		//Inician la salida TODAS ellas y luego se inserta la nueva.
		std::vector<Marcador_combo>::iterator 	ini=this->cola_combo.begin(),
							fin=this->cola_combo.end();

		while(ini<fin)
		{
			(*ini).iniciar_salida();
			ini++;
		}

		this->cola_combo.push_back(Marcador_combo(tipo_actual, DLibV::Gestor_recursos_graficos::obtener(Recursos::G_COMBO)));
	}

	actualizar_barra_combo();

	//Aprovechamos ahora para actualizar el controlador de perfiles que guarda el máximo tipo de combo.
	this->datos_actualizables.actualizar_combo(tipo_actual);
}

void Controlador_juego::actualizar_barra_combo()
{
	if(!CONTROLADOR_PUNTUACION.es_al_maximo())
	{
		barra_combo.establecer_color_barra(Barra_combo::COLOR_BARRA);
		unsigned int cc=CONTROLADOR_PUNTUACION.acc_contador_combo();
		unsigned int crc=CONTROLADOR_PUNTUACION.acc_cantidad_restante_combo();

		barra_combo.establecer_valor_actual(cc);
		barra_combo.establecer_valor_maximo(cc+crc);
		barra_combo.actualizar();
	}
	else //Cuando está al máximo simplemente lo ponemos a 1 de 1, que es la barra llena.
	{
		barra_combo.establecer_valor_actual(1);
		barra_combo.establecer_valor_maximo(1);
		barra_combo.actualizar();
	}
}

void Controlador_juego::controlar_destruccion_gema(Bonus_normal * gema)
{
	if(!gema->es_recogido())
	{
		//Se resta una gema de las máximas que podemos perder.
		this->estructura_info_nivel.decrementar_gemas_max_nivel();
		
		//La perdemos en el controlador para trofeos...
		this->datos_actualizables.perder_gema();
		this->actualizar_datos_hud_gemas();
	}
}

void Controlador_juego::recoger_gema(Bonus_normal * gema, unsigned int p_cantidad)
{
	this->cont_gemas+=p_cantidad;
	gema->recoger();

/*
	El hack ya no sería necesario al implementar el método de recogida.

	//Esto es un hack, cuando el objeto gema se borre quitará de este 
	//contador, añadimos al contador para representar que coger una gema
	//no resta de la cantidad máxima que puedes perder.


	this->estructura_info_nivel.incrementar_gemas_max_nivel();
*/
	this->actualizar_datos_hud_gemas();

	//Actualizamos los datos de perfil..
	this->datos_actualizables.sumar_gema();
}

void Controlador_juego::disparar_reto(unsigned int p_indice)
{
	this->controlador_perfiles.disparar_reto(p_indice , this->controlador_retos);
}


/* Esto calcula un vector de dirección para el jugador, se lo aplica y luego 
cambia el estado a uno pasivo. Para facilitar la animación cambiamos antes todo
al modo "pantalla" como en juego aleatorio, para calcular la posición hasta
un punto en la pantalla.
*/

void Controlador_juego::iniciar_animacion_fin_juego()
{
	this->estructura_info_nivel.preparar_para_final_juego();
	this->restaurar_posiciones_post_nivel_preparado(this->estructura_info_nivel.acc_distancia());
	this->camara.enfocar_a(0,0);

	//Como vamos a perder el input durante un rato vamos a indicarle que no 
	//retraiga los vectores. Además, lo liberamos del control de posiciónes 
	//para que pueda salir disparado fuera de la pantalla.
	this->J->mut_retraer_vectores(false);
	this->J->mut_debe_controlar_posiciones(false);

	//Nos ponemos en el primer paso de la animación y cambiamos el estado del motor.
	this->cambiar_estado_animacion_fin_juego(PAF_MOVERSE_A_INICIO);
	this->cambiar_estado(E_ANIMACION_FIN_JUEGO);
}

/*Cuando se cambia el estado de la animación del fin de juego hay que hacer
varias cosas, como cambiar vectores, reiniciar un contador de tiempo o cambiar
el estado general del controlador. Todo eso se hace aquí.*/

void Controlador_juego::cambiar_estado_animacion_fin_juego(unsigned int p_estado)
{
	this->paso_animacion_final=p_estado;

	switch(this->paso_animacion_final)
	{
		case PAF_MOVERSE_A_INICIO:
			//Le indicamos que vaya a la posición final...
			this->J->establecer_vector(this->J->obtener_vector_unidad_hasta_posicion(400.0f, 500.0f), 200.0f);
		break;

		case PAF_PAUSA_INTERMEDIA:
			//Esto no hace falta porque ya está inicializado pero...
			this->contador_animacion_final=0;
		break;

		case PAF_SALIR_DISPARADO:
			//Le indicamos que salga lanzado hacia adelante y sacamos 200 chispas!!!.
			this->generar_chispas(this->J, 200);
			this->J->establecer_vector(DLibH::Vector_2d(0.0f, -1000.0f));
		break;

		case PAF_PAUSA_FINAL:
			this->contador_animacion_final=0;
		break;

		case PAF_FIN:
			this->iniciar_fin_juego(GO_FIN_JUEGO);
		break;
	}	
}	

/*A cada tic de juego que pase en la animación del final le corresponde un paso
por este método. Se usa para comprobar si el paso concreto de la animación ha
finalizado y para iniciar el paso siguiente.*/

void Controlador_juego::procesar_animacion_fin_juego(float p_delta)
{
	switch(this->paso_animacion_final)
	{
		//En este punto se trata de determinar si el jugador ha llegado ya
		//a la posición final. En función de la posición desde la que venga
		//tendremos varias posibilidades. El punto de fin es 400, 500, el
		//mismo que el punto de inicio.

		case PAF_MOVERSE_A_INICIO:
			
			//Comprobamos por la x. Si se pasa lo paramos.
			if((this->J->acc_vector_mov_x() > 0 && this->J->acc_x() > 400) ||
				(this->J->acc_vector_mov_x() < 0 && this->J->acc_x() < 400))
			{
				this->J->mut_vector_x(0);						 
			}

			//Idem para la y.
			if((this->J->acc_vector_mov_y() > 0 && this->J->acc_y() > 500) ||
				(this->J->acc_vector_mov_y() < 0 && this->J->acc_y() < 500))
			{
				this->J->mut_vector_y(0);						 
			}
	
			//Cuando hayamos llegado al punto final, lo sabremos porque hemos parado.
			if(!this->J->acc_vector_mov_x() && !this->J->acc_vector_mov_y())
			{
				this->cambiar_estado_animacion_fin_juego(PAF_PAUSA_INTERMEDIA);
			}
		break;
		
		case PAF_PAUSA_INTERMEDIA:

			//Estará parado 2.5 segundos.
			this->contador_animacion_final+=p_delta;
			if(this->contador_animacion_final >= 2.5)
			{
				this->cambiar_estado_animacion_fin_juego(PAF_SALIR_DISPARADO);
			}
		break;
		
		case PAF_SALIR_DISPARADO:
			if(this->J->acc_y() < -64)
			{
				this->cambiar_estado_animacion_fin_juego(PAF_PAUSA_FINAL);
			}
		break;

		case PAF_PAUSA_FINAL:
			//Estará parado dos segundos antes de sacar el game over..
			this->contador_animacion_final+=p_delta;
			if(this->contador_animacion_final >= 1.0)
			{
				this->cambiar_estado_animacion_fin_juego(PAF_FIN);
			}
		break;
	}
}


unsigned short int Controlador_juego::preguntar_nueva_maxima_puntuacion_local()
{
	return this->lector_puntuaciones.preguntar_nueva_puntuacion_local(this->CONTROLADOR_PUNTUACION.acc_puntuacion());
}

unsigned long int Controlador_juego::preguntar_nueva_maxima_puntuacion_online()
{
	return this->lector_puntuaciones.solicitud_servidor_posicion_para_puntuacion(this->CONTROLADOR_PUNTUACION.acc_puntuacion());
}

void Controlador_juego::enviar_nueva_puntuacion()
{
	//Si el perfil activo es online lo validaremos antes de proceder. Si
	//no valida no se guardará offline ni online.

	unsigned long int id=this->controlador_perfiles.obtener_id_perfil();

	if(this->controlador_perfiles.es_perfil_online())
	{
		try
		{
			std::string login=this->controlador_perfiles.obtener_nombre_perfil();
			std::string email=this->controlador_perfiles.obtener_email_perfil();
			std::string pass=this->controlador_perfiles.obtener_clave_perfil();

			if(!Solicitud_servidor_perfil::validar_perfil(login, email, pass, id))
			{
				return;
			}
		}
		catch(Excepcion_solicitud_perfil E)
		{
			return;
		}
	}

	this->lector_puntuaciones.enviar_nueva_puntuacion(this->CONTROLADOR_PUNTUACION.acc_puntuacion(), this->nivel_actual, this->escritor.acc_cadena(), this->juego_finalizado, this->posicion_local!=0, id);
}

/*Se resta una vida al jugador y se hacen los cálculos pertinentes para ver si
la partida ha terminado, si se reinicia el nivel o no... En función del tipo 
de nivel que estemos jugando ocurrirán unas cosas u otras.*/

void Controlador_juego::restar_vida_jugador()
{
	bool modo_preparado=this->estructura_info_nivel.acc_modo_juego()==Estructura_info_nivel::M_PREPARADO;	

//	this->generar_explosion(this->J, true, 8); Esto lo pondremos en otro sitio: cuando se llega aquí estamos iterando por el
//	vector de actores, y este método lo toca!!!-

	this->estructura_info_nivel.perder_vida(); 	//Se suma una a las vidas que se han perdido en el nivel, para luego almacenarlo en el perfil.
	this->J->quitar_vida(!modo_preparado);
	this->E->reiniciar();

	//Reiniciamos los valores máximos para las barras de HUD.
	this->barra_canon.establecer_valor_maximo(this->J->acc_carga_maxima_canon());
	this->barra_tiempo.establecer_valor_maximo(this->J->acc_carga_maxima_parar_tiempo());
	this->barra_escudo.establecer_valor_maximo(this->E->acc_max_carga_escudo());

	//Esto lo hacemos para "reiniciar" todo lo que esté haciendo el jugador,
	//que no se queden colgados los artefactos entre vida y vida.
	this->procesar_input_jugador(0, 0, 0, true, false, false);
	this->J->recibir_senal_tiempo(2);
	this->forzar_fin_fantasma();

	this->CONTROLADOR_PUNTUACION.reiniciar_combo();
	this->actualizar_marcador_combo();

	//La sumamos a todos los que hay.
	this->datos_actualizables.perder_vida();

	//Esto quizás quedaría más bonito al principio del loop, pero aquí se comprobaría sólo cuando tocase y no cada vez.
	if(!this->J->acc_vidas())
	{
		this->iniciar_fin_juego(GO_NO_VIDAS);
	}
/*Si estamos en modo preparado, cambiamos el estado. Cuando haya
finalizado la iteración actual del loop entraremos en el nuevo estado,
donde se limpiarán los actores y se rebobinará el nivel. No borramos nada
aquí porque el loop continuará cuando salgamos de este caso y podemos
invalidar todas las referencias.*/
	else if(modo_preparado)
	{
		this->preparar_reinicio_nivel();
	}
}

/*Estos métodos sólo son llamados al montar y al desmontar de modo que se
libran de pasar por la puerta de "eliminar_actor_mapa()". No van a afectar al
funcionamiento del juego.*/

void Controlador_juego::vaciar_actores_mapa()
{
	std::vector<Actor_mapa *>::iterator 	ini=this->actores_mapa.begin(),
						fin=this->actores_mapa.end();

	for(;ini < fin; ini++) delete *ini;
	this->actores_mapa.clear();
}

void Controlador_juego::vaciar_mapa_rutas()
{
	this->puntos_ruta.clear();
}

void Controlador_juego::vaciar_proyectiles()
{
	std::vector<Proyectil *>::iterator 	ini=this->proyectiles.begin(),
						fin=this->proyectiles.end();

	for(;ini < fin; ini++) delete *ini;
	this->proyectiles.clear();
}

void Controlador_juego::vaciar_decoraciones()
{
	for(Decoracion * d : decoraciones) delete d;
	for(Decoracion * d : decoraciones_frente) delete d;

	this->decoraciones.clear();
	this->decoraciones_frente.clear();
}

void Controlador_juego::comprobar_proyectiles_contra_escudo()
{
	//Sólo cuando el escudo está activo.
	if(!this->E->es_activo()) return;

	std::vector<Proyectil *>::iterator 	pr_ini=this->proyectiles.begin(),
						pr_fin=this->proyectiles.end();

	while(pr_ini < pr_fin)
	{
		if( (*pr_ini)->es_disparado_por(this->J))
		{
			pr_ini++;
		}
		else
		{
			if(this->E->comprobar_colision_con(**pr_ini))
			{	
				generar_chispas(*pr_ini, 20, Decoracion_chispa::NARANJAS);

				delete *pr_ini;

				pr_ini=this->proyectiles.erase(pr_ini);
				pr_fin=this->proyectiles.end();
			}
			else
			{
				pr_ini++;
			}
		}
	}
}

void Controlador_juego::evaluar_anulacion_combo_por_proyectil(const Proyectil * const p)
{
	if(p->es_de_faccion(Definiciones::F_JUGADOR))
	{
		//this->CONTROLADOR_PUNTUACION.reiniciar_combo();
		this->CONTROLADOR_PUNTUACION.bajar_combo();
		actualizar_combo=true;
	}
}

void Controlador_juego::comprobar_proyectiles_fuera_juego()
{
	std::vector<Proyectil *>::iterator 	pr_ini=this->proyectiles.begin(),
						pr_fin=this->proyectiles.end();


	while(pr_ini < pr_fin)
	{
		if((*pr_ini)->es_fuera_de_juego())
		{	
			evaluar_anulacion_combo_por_proyectil(*pr_ini);				

			delete *pr_ini;
			pr_ini=this->proyectiles.erase(pr_ini);
			pr_fin=this->proyectiles.end();
		}
		else
		{
			pr_ini++;
		}
	}
}

void Controlador_juego::activar_escudo()
{
	if(this->E->activar()) 	this->cola_sonido.insertar(
		DLibA::Estructura_sonido(DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_ESCUDO_ON)));
}

void Controlador_juego::desactivar_escudo()
{
	if(this->E->desactivar()) this->cola_sonido.insertar(
		DLibA::Estructura_sonido(DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_ESCUDO_OFF)));
}		

/*La pausa sólo hace efecto en los estados de juego. Los estados de puntuación
máxima o game over no se ven afectados.*/

void Controlador_juego::procesar_pausa()
{	
	switch(this->estado_actual)
	{
//		case E_PREPARANDO_JUGADOR_INICIAR_NIVEL:
		case E_JUEGO:
		case E_FINALIZANDO_NIVEL:
			if(!this->pausado) this->tiempo_pausa=0;
			this->pausado=!this->pausado;

			if(pausado) controlador_audio.pausar_sonido();
			else controlador_audio.reanudar_sonido();
		break;

		default: break;
	}	
}

void Controlador_juego::iniciar_fantasma()
{
	this->F->hacer_visible(this->J->acc_x(), this->J->acc_y());
	this->F->establecer_vector_desde_actor(*this->J);
}

void Controlador_juego::finalizar_fantasma()
{
	unsigned int distancia=abs(DLibH::Linea::distancia_entre_puntos(this->J->acc_x(), this->J->acc_y(), this->F->acc_x(), this->J->acc_y()));

	Trazador_fantasma * tr=new Trazador_fantasma();
	tr->configurar(J, F);
	this->actores_mapa.push_back(tr);

	this->cola_sonido.insertar(
		DLibA::Estructura_sonido(DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_PHASER)));

	this->F->hacer_invisible();

	//Generamos esto antes de movernos.
	this->generar_chispas_teletransporte();

	this->J->ir_a(this->F->acc_x(), this->F->acc_y());
	this->J->establecer_vector_desde_actor(*this->F, 2);
	this->datos_actualizables.sumar_pixeles_teletransportados(distancia);
}

void Controlador_juego::forzar_fin_fantasma()
{
	F->hacer_invisible();
	F->ir_a(J->acc_x(), J->acc_y());
}

/*Genera unas chispas en la posición del actor que se pasa.*/

void Controlador_juego::generar_chispas(Actor * p_actor, unsigned int p_cantidad, unsigned int gama)
{
	unsigned int cantidad_chispas=p_cantidad, i=0;

	Decoracion_chispa * ch=NULL;

	float x=p_actor->acc_x_centro();
	float y=p_actor->acc_y_centro();

	for(;i<cantidad_chispas; i++)
	{
		ch=new Decoracion_chispa(x, y, gama);
		this->decoraciones.push_back(ch);
		ch=NULL;
	}
}

/*Genera unas chispas hacia abajo en el jugador cuando acelera... Es pura 
decoración, pero da un efecto simpático.*/

void Controlador_juego::generar_chispas_propulsor()
{
	DLibH::Vector_2d v(0, 0);
	
	bool preparado=this->estructura_info_nivel.acc_modo_juego()==Estructura_info_nivel::M_PREPARADO;

//	float my=this->estructura_info_nivel.acc_modo_juego()==Estructura_info_nivel::M_PREPARADO
//		? 10.f : this->estructura_info_nivel.acc_vector_y();

	DLibH::Punto_2d<int> 	p1=J->obtener_punto_propulsor(0),
				p2=J->obtener_punto_propulsor(1);

	unsigned int gama=J->es_parando_tiempo() ? Decoracion_chispa::VERDES : Decoracion_chispa::AZULES;

	Decoracion_chispa * ch=NULL;
	float d=(1000+(rand()%2000)) / 1000;
		
	ch=new Decoracion_chispa(p1.x, p1.y, gama);
	v*=100.0f+rand()%100;
	v.x=( (rand()%10)-5 ) * 5.0f;
	v.y=preparado ? (rand()%3 ) * 10.0f : this->estructura_info_nivel.acc_vector_y() * 3.0f;
	ch->establecer_vector(v);

	ch->establecer_duracion(d);
	this->decoraciones.push_back(ch);

	ch=new Decoracion_chispa(p2.x, p2.y, gama);
	v*=100.0f+rand()%100;
	v.x=( (rand()%10)-5 ) * 5.0f;
	v.y=preparado ? (rand()%3 ) * 10.0f : this->estructura_info_nivel.acc_vector_y() * 3.0f;
	ch->establecer_vector(v);
	d=(1000+(rand()%2000)) / 1000;
	ch->establecer_duracion(d);
	this->decoraciones.push_back(ch);

	ch=NULL;
}

/*Genera unas chispas con la forma de la nave al teletransportarse...*/

void Controlador_juego::generar_chispas_teletransporte()
{
	DLibH::Vector_2d v(0, 0);
	Decoracion_chispa * ch=NULL;
	
	int x=0, y=0, dx=J->acc_x(), dy=J->acc_y(), w=J->acc_w(), h=J->acc_h();
	float d=0.0f;
	
	for(x=0; x<w; ++x)
	{
		for(y=0; y<h; ++y)
		{
			if(J->comprobar_mapa_colision(x, y))
			{
				d=(1000+(rand()%2000)) / 1000;

				ch=new Decoracion_chispa(x+dx, y+dy, Decoracion_chispa::AZULES);
				v.x=( (rand()%10)-5 ) * 5.0f;
				v.y=( (rand()%10)-5 ) * 5.0f;

				v=v*2;

				ch->establecer_vector(v);
				ch->establecer_duracion(d);
				this->decoraciones.push_back(ch);
			}
		}
	}

	ch=NULL;
}



/*Genera una explosión en la pantalla en la posición indicada por el actor que
se pasa*/

void Controlador_juego::generar_explosion(Actor * p_actor, bool con_chatarra, unsigned short int secciones_humo)
{
	float px=p_actor->acc_x(),
		py=p_actor->acc_y(),
		vx=p_actor->acc_vector().x,
		vy=p_actor->acc_vector().y,
		desp_x=p_actor->acc_w() / 2,
		desp_y=p_actor->acc_h() / 2;

	generar_explosion(px, py, vx, vy, con_chatarra, secciones_humo, desp_x, desp_y);
}

void Controlador_juego::generar_explosion(float px, float py, float vx, float vy, bool con_chatarra, unsigned short int secciones_humo, float desp_x, float desp_y)
{
	if(con_chatarra)
	{
		unsigned int cantidad_chatarra=(rand()%10)+30;
		this->generar_chatarra(px+desp_x, py+desp_y, cantidad_chatarra);
	}

	if(secciones_humo)
	{
		float arcos=360 / secciones_humo;

		for(int i=0; i<secciones_humo; ++i)
		{
			Trazador_explosion * te=new Trazador_explosion();
			te->configurar(px+desp_x, py+desp_y, (i*arcos), (i*arcos)+arcos);
			this->actores_mapa.push_back(te);
			te=NULL;
		}
	}

	this->decoraciones.push_back(new Decoracion_explosion(px, py, DLibH::Vector_2d(vx, vy) ) );
}

void Controlador_juego::generar_humo(Actor * p_actor, float vx, float vy, float dur)
{
	float 	x=p_actor->acc_x()+(p_actor->acc_w()/2),	
		y=p_actor->acc_y()+(p_actor->acc_h()/2);

	generar_humo(x, y, vx, vy, dur);
}

void Controlador_juego::generar_humo(float px, float py, float vx, float vy, float dur)
{
	Decoracion_humo * h=new Decoracion_humo(px, py, 0);
	h->establecer_vector(DLibH::Vector_2d(vx, vy));
	h->establecer_duracion(dur);	
	h->activar_auto_parte();

	decoraciones.push_back(h);
}

void Controlador_juego::generar_chatarra(Actor * p_actor, unsigned int p_cantidad)
{
	float 	x=p_actor->acc_x()+(p_actor->acc_w()/2),	
		y=p_actor->acc_y()+(p_actor->acc_h()/2);

	generar_chatarra(x, y, p_cantidad);
}

void Controlador_juego::generar_chatarra(float x, float y, unsigned int p_cantidad)
{
	unsigned int i=0;
	while(i++<p_cantidad)
		this->decoraciones.push_back(new Decoracion_chatarra(x, y));
}

void Controlador_juego::generar_decoracion_desde_estructura_boss(Boss::Estructura_decoracion_boss &datos)
{
	switch(datos.tipo)
	{
		case Boss::Estructura_decoracion_boss::ED_CHATARRA:
			generar_chatarra(datos.x, datos.y, datos.aux);
		break;

		case Boss::Estructura_decoracion_boss::ED_EXPLOSION:
			generar_explosion(datos.x, datos.y, datos.vector.x, datos.vector.y, datos.aux, datos.aux_desp_x, datos.aux_desp_y);
		break;

		case Boss::Estructura_decoracion_boss::ED_HUMO:
			generar_humo(datos.x, datos.y, datos.vector.x, datos.vector.y, datos.aux);
		break;
	}
}

void Controlador_juego::sumar_puntuacion(int p_puntuacion, const Actor_mapa * const p_actor)
{
	this->estructura_info_nivel.sumar_puntuacion(p_puntuacion);
	this->CONTROLADOR_PUNTUACION.sumar_puntuacion(p_puntuacion);

	//Sumamos la puntuación a los datos actualizables del perfil...
	this->datos_actualizables.sumar_puntuacion(p_puntuacion);

	if(this->CONTROLADOR_PUNTUACION.acc_puntuacion() > Controlador_retos::N_PUNTUACION)
	{
		this->disparar_reto(Definicion_retos::R_N_PUNTUACION);
	}
	
	//Si hemos un actor estos parámetros de posición entonces 
	//podemos insertar una representación.

	if(p_actor)
	{
		float x=p_actor->acc_x();

		//En caso de que el nivel se mueva tenemos que calcular esto así...
		float y=p_actor->acc_y()-this->estructura_info_nivel.acc_distancia();
		Decoracion_puntuacion * nueva=this->CONTROLADOR_PUNTUACION.obtener_decoracion_puntuacion(p_puntuacion, x, y);
		this->decoraciones_frente.push_back(nueva);	
	}
}

/*Esto se llama para una decoración de puntuación que está a punto de ser
eliminada. Básicamente sirve para aumentar la puntuación representada en el
marcador una vez que la representación desaparece. La puntuación real se ha
sumado ya.*/

void Controlador_juego::sumar_puntuacion_representada(Decoracion_puntuacion * p_decoracion)
{
	this->CONTROLADOR_PUNTUACION.sumar_puntuacion_representada(p_decoracion->acc_puntuacion());
}

/*Genera un proyectil del jugador en la posición del jugador. No tiene
mucha más historia.*/

void Controlador_juego::generar_proyectil_jugador()
{
	Proyectil * pr=Proyectil::generar(*this->J, *this->M, Proyectil::D_NORMAL);

	unsigned int potencia=J->acc_potencia_disparo();
	unsigned short int color=0;

	if(potencia <= 25) color=Proyectil::C_AZUL;
	else if(potencia <= 50) color=Proyectil::C_VERDE;
	else if(potencia <= 75) color=Proyectil::C_VIOLETA;
	else color=Proyectil::C_ROJO;

	pr->establecer_potencia(potencia);
	pr->establecer_color(color);
	pr->establecer_velocidad(400.0f);
	pr->establecer_faccion(Definiciones::F_JUGADOR | Definiciones::F_TRIGGER | Definiciones::F_BONUS);
	this->proyectiles.push_back(pr);								

	//Añadimos un nuevo proyectil a la cuenta total.
	this->datos_actualizables.sumar_disparo_efectuado();
}

/*El proceso para activar el Edu es obtener una cantidad de potencia a partir
del jugador. Recorreremos TODOS los actores de la pantalla y los alejaremos
con un sólo "golpe".
*/

void Controlador_juego::activar_edu()
{
	std::vector<Actor_mapa *>::iterator 	ini=this->actores_mapa.begin(),
						fin=this->actores_mapa.end();

	float potencia=J->usar_edu();

	//Sumamos la potencia al acumulador general del juego.
	this->datos_actualizables.sumar_potencia_edu(potencia);

	while(ini < fin)
	{
		/*Para que haga efecto tiene que ser que no se vaya a borrar,
		que sea "afectable" y que esté dentro de la pantalla. Esto 
		último es muy importante: en niveles preparados podemos echar a
		volar todo el mapa de una sóla activación!!!!*/

		if(!(*ini)->es_marcado_para_borrar() && (*ini)->es_afectado_por_edu() && (*ini)->es_en_pantalla())
		{		
			DLibH::Vector_2d v=(*ini)->obtener_vector_unidad_hasta_actor(*this->J);
			v=v*(potencia*3);	

			(*ini)->restar_vector(v);
		}

		ini++;
	}

	//Finalmente, la representacion...

	float x=this->J->acc_x_centro();
	float y=this->J->acc_y_centro();
	
	for(int i=0; i<4; ++i) this->decoraciones.push_back(new Decoracion_edu(x, y, potencia, i));
	this->generar_chispas(this->J, potencia, Decoracion_chispa::AZULES);
}

/*Este método va a decidir los artefactos de HUD que aparecen como "usables" y
cuál aparece como activo. Lo llamaremos siempre que haya un cambio en la 
estructura_info_nivel que controla esto o cuando el jugador cicle los 
artefactos.*/

void Controlador_juego::actualizar_datos_hud_artefactos()
{
	auto proceso=[this](unsigned int a)
	{
		if(estructura_info_nivel.es_artefacto_permitido(a)) hud_artefactos.desactivar_artefacto(a);
		else hud_artefactos.anular_artefacto(a);
	};

	proceso(Definiciones::A_CANON);
	proceso(Definiciones::A_TELETRANSPORTADOR);
	proceso(Definiciones::A_EMISOR_DENSIDAD_UNIFICADA);
	proceso(Definiciones::A_ESCUDO);

	unsigned int actual=J->acc_artefacto_actual();
	if(actual) hud_artefactos.activar_artefacto(actual);
}

void Controlador_juego::establecer_artefacto_jugador_auto()
{
	if(!estructura_info_nivel.es_artefacto_permitido(J->acc_artefacto_actual()))
	{
		unsigned short int artefacto_siguiente=estructura_info_nivel.ciclar_artefacto(1, J->acc_artefacto_actual());
		J->establecer_artefacto_actual(artefacto_siguiente);
	}

	actualizar_datos_hud_artefactos();
}

/*Este método decide el color del hud de gemas y el contenido del mismo. Sólo
se actualizará cuando alguno de los valores cambie.*/

void Controlador_juego::actualizar_datos_hud_gemas()
{
	if(this->estructura_info_nivel.es_estado_gemas_peligro())
	{
		this->rep_txt_gemas_nivel->modificar_fuente(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_4));
	}
	else
	{
		this->rep_txt_gemas_nivel->modificar_fuente(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_3));
	}

	this->rep_txt_gemas_nivel->mut_interletra(1);

	int gemas_por_coger=this->estructura_info_nivel.acc_gemas_en_nivel()-this->cont_gemas;
	if(gemas_por_coger < 0) gemas_por_coger=0;	//Esto puede pasar en un nivesel preparado con muuuuchas más gemas de las que se exigen.


	std::string c1=DLibH::Herramientas::entero_a_cadena(gemas_por_coger)
		+"/"
		+DLibH::Herramientas::entero_a_cadena(this->estructura_info_nivel.acc_gemas_max_nivel());

	this->rep_txt_gemas_nivel->asignar(c1);
}


/*Sólo para las minas. Esto ya es un desastre. Un poco más
no importa :P.*/

void Controlador_juego::generar_destruccion_mina(Obstaculo_explosivo * p_mina)
{
	

	Proyectil * pr=NULL;	

	//Como el jugador la ha destruido salen menos proyectiles...
	p_mina->dividir_cantidad(2);

	do
	{
		pr=p_mina->explotar();
		if(pr) this->proyectiles.push_back(pr);
	}while(pr);

	//Además, añadimos una mina destruida al controlador de trofeos.
	this->datos_actualizables.sumar_mina_destruida();
	
}

/*Esto se llama después de un cambio de artefacto y sirve para que el cañón
se ponga al frente del jugador nada más iniciar, por ejemplo.*/

void Controlador_juego::procesar_post_seleccion_artefacto()
{
	switch(J->acc_artefacto_actual())
	{
		case Definiciones::A_CANON:
		{
			DLibH::Punto_2d<float> p=coordenadas_juego_a_pantalla(J->acc_x_centro(), J->acc_y_centro());
			M->ir_a(p.x, p.y);
			this->controles_sdl.mover_raton(p.x, p.y);
		}
		break;

		default:

		break;
	}
}

/*Dadas unas coordenadas de juego según el desplazamiento actual y demás,
devuelve las coordenadas correspondientes en pantalla.*/ 

DLibH::Punto_2d<float> Controlador_juego::coordenadas_juego_a_pantalla(float x, float y)
{
	return DLibH::Punto_2d<float>(x, y-estructura_info_nivel.acc_distancia());
}

void Controlador_juego::componer_representacion_estadisticas_fin_nivel(unsigned int p_nivel, bool es_nivel_preparado, bool encontrado_secreto)
{
	rep_fin_nivel->rellenar(0, 255, 255);

	const std::string tabulacion="    ";
	const std::string salto="\n";
	const std::string doble_salto="\n\n";
	const std::string triple_salto="\n\n\n";

	std::string texto_titulo=
		obtener_nombre_nivel(localizador, p_nivel)+
		localizador.obtener(Localizador::C_ESTADISTICAS_NIVEL_FINALIZADO);

	if(encontrado_secreto)
		texto_titulo+=salto+localizador.obtener(Localizador::C_ESTADISTICAS_NIVEL_SECRETO_ENCONTRADO);

	std::string texto_estadisticas=
localizador.obtener(Localizador::C_ESTADISTICAS_NIVEL_PUNTUACION_OBTENIDA)+
doble_salto+tabulacion+
DLibH::Herramientas::entero_a_cadena(estructura_info_nivel.acc_puntuacion_acumulada())+
triple_salto+
localizador.obtener(Localizador::C_ESTADISTICAS_NIVEL_VIDAS_PERDIDAS)+
doble_salto+tabulacion+
DLibH::Herramientas::entero_a_cadena(estructura_info_nivel.acc_vidas_perdidas())+
triple_salto;

	if(es_nivel_preparado)
	{
		int items=(estructura_info_nivel.cuenta_recogible() * 100) / estructura_info_nivel.total_recogible();
		int destruidos=(estructura_info_nivel.cuenta_destruible() * 100) / estructura_info_nivel.total_destruible();

		texto_estadisticas+=
localizador.obtener(Localizador::C_ESTADISTICAS_NIVEL_ITEMS)+
doble_salto+tabulacion+
DLibH::Herramientas::entero_a_cadena(items)+"%"+
triple_salto+
localizador.obtener(Localizador::C_ESTADISTICAS_NIVEL_DESTRUIDOS)+
doble_salto+tabulacion+
DLibH::Herramientas::entero_a_cadena(destruidos)+"%";
	}

	//Componemos ahora las representaciones...
	DLibV::Representacion_texto_auto_estatica titulo(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), texto_titulo);
	titulo.establecer_posicion(0, 0);

	DLibV::Representacion_texto_auto_estatica estadisticas(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2), texto_estadisticas);
	estadisticas.establecer_posicion(0, 0);

	//Cada uno de estos elementos va en una posición...
	SDL_Rect caja_texto_titulo=DLibH::Herramientas_SDL::nuevo_sdl_rect(20, 20, W_CAJA_FONDO_FIN_NIVEL, H_CAJA_FONDO_FIN_NIVEL);
	SDL_Rect caja_texto_estadisticas=DLibH::Herramientas_SDL::nuevo_sdl_rect(20, 80, W_CAJA_FONDO_FIN_NIVEL, H_CAJA_FONDO_FIN_NIVEL);

	//Unirlo todo...
	rep_fin_nivel->volcar_representacion(titulo, caja_texto_titulo);
	rep_fin_nivel->volcar_representacion(estadisticas, caja_texto_estadisticas);
}

std::string Controlador_juego::obtener_nombre_nivel(Localizador &localizador, unsigned int p_nivel)
{
	std::string nombre_nivel("");

	switch(p_nivel)
	{
		case 0: nombre_nivel=localizador.obtener(Localizador::C_TITULO_NIVEL_01); break;
		case 1: nombre_nivel=localizador.obtener(Localizador::C_TITULO_NIVEL_02); break;
		case 2: nombre_nivel=localizador.obtener(Localizador::C_TITULO_NIVEL_03); break;
		case 3: nombre_nivel=localizador.obtener(Localizador::C_TITULO_NIVEL_04); break;
		case 4: nombre_nivel=localizador.obtener(Localizador::C_TITULO_NIVEL_05); break;
		case 5: nombre_nivel=localizador.obtener(Localizador::C_TITULO_NIVEL_06); break;
		case 6: nombre_nivel=localizador.obtener(Localizador::C_TITULO_NIVEL_07); break;
		case 7: nombre_nivel=localizador.obtener(Localizador::C_TITULO_NIVEL_08); break;
		case 8: nombre_nivel=localizador.obtener(Localizador::C_TITULO_NIVEL_09); break;
		case 9: nombre_nivel=localizador.obtener(Localizador::C_TITULO_NIVEL_SECRETO_01); break;
		case 10: nombre_nivel=localizador.obtener(Localizador::C_TITULO_NIVEL_SECRETO_02); break;
		case 11: nombre_nivel=localizador.obtener(Localizador::C_TITULO_NIVEL_SECRETO_03); break;

	}

	return nombre_nivel;
}
