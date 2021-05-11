
/*Si un no jugador tiene una actividad se procesa aquí como un
"obstaculo_actividad".* La actuación puede darse sobre el resto de actores
y puede también emitir un sonido.

Carne de visitor pattern...*/

void Controlador_juego::procesar_turno_actividad_no_jugador(Obstaculo_actividad * p_actor)
{
	if(p_actor->es_activo())
	{
		Visitante_actividades v(*this);
		p_actor->aceptar_visitante(v);

		unsigned int sonido=v.acc_r();
		if(sonido)
		{
			this->cola_sonido.insertar(DLibA::Gestor_recursos_audio::obtener_sonido(sonido));
		}
	}
}

/*Esto no llama a ningún método el obstáculo en si: es distinto al resto.
El funcionamiento implica coger todos los actores y proyectiles y acercarlos
a su posición, como un pozo de gravedad.*/

unsigned int Controlador_juego::actividad_no_jugador(Obstaculo_gravedad &p_trasto)
{
	//Este es el único estado en el que permitimos que ocurra algo.
	if(estado_actual != E_JUEGO) return 0;

	std::vector<Actor_mapa*>::iterator 	ini=this->actores_mapa.begin(),
						fin=this->actores_mapa.end();

	std::vector<Proyectil*>::iterator 	ini_pr=this->proyectiles.begin(),
						fin_pr=this->proyectiles.end();

	while(ini < fin)
	{
		if((*ini)->es_afectado_por_edu() && (*ini)!=&p_trasto)
		{
			(*ini)->sumar_vector((*ini)->obtener_vector_unidad_hasta_actor(p_trasto ), 0.5);
		}

		ini++;
	};

	while(ini_pr < fin_pr)
	{
		(*ini_pr)->sumar_vector((*ini_pr)->obtener_vector_unidad_hasta_actor(p_trasto ), 0.5);
		ini_pr++;
	};

	return p_trasto.actuar_sonido();
}

unsigned int Controlador_juego::actividad_no_jugador(Obstaculo_explosivo &p_mina)
{
	//Este es el único estado en el que permitimos que ocurra algo.
	if(estado_actual!=E_JUEGO) return 0;

	Proyectil * pr=NULL;
	pr=p_mina.explotar();

	unsigned int resultado=0;

	//Si no devuelve un proyectil es que aún ha terminado el ciclo de explosión.
	if(!pr)
	{
		this->generar_explosion(&p_mina, false, 3);
		resultado=p_mina.actuar_sonido();
		p_mina.marcar_para_borrar();
	}
	//Si lo devuelve entonces aún podemos disparar un poco más.
	else
	{
		this->proyectiles.push_back(pr);
		resultado=p_mina.actuar_sonido();
	}

	return resultado;
}

/*A base de vueltas integramos en el sistema este tipo de acción... Al activarse
el trigger se lanza a este actor la recepción, que lo activa. Cuando está el
actor activo entra aquí, que lanza el cambio a la pantalla. Cuando la pantalla
ha finalizado el actor se marca como finalizado, que es recogido por el trigger
y que puede dar como finalizada la acción...*/

unsigned int Controlador_juego::actividad_no_jugador(Obstaculo_propiedades_fondo &p_propiedades)
{
	//Por aquí entramos la primera vez: marcamos para no entrar más e iniciamos los cambios.
	if(!p_propiedades.es_iniciado_cambio())
	{
		p_propiedades.iniciar_cambio();
		this->campo_estrellas.transformar_color_fondo(p_propiedades.acc_rojo(), p_propiedades.acc_verde(), p_propiedades.acc_azul(), p_propiedades.acc_velocidad());
		this->campo_estrellas.mut_max_puntos(p_propiedades.acc_estrellas());
	}
	//Por aquí entramos el resto de las veces. Cuando el fondo haya finalizado
	//sus cambios entonces lo marcamos como finalizado y al procesarse la
	//acción que lo provocó, marcará el trigger y la acción como finalizados.
	else
	{
		if(!this->campo_estrellas.es_cambio_color())
		{
			p_propiedades.finalizar_proceso();
		}
	}

	return 0;
}

/*Este sistema es prácticamente idéntico al anterior...*/

unsigned int Controlador_juego::actividad_no_jugador(Obstaculo_propiedades &p_obstaculo)
{
	if(!p_obstaculo.es_usado())
	{
		LOG<<"INICIANDO OBSTACULO ACTIVIDAD NIVEL"<<std::endl;

		p_obstaculo.usar();

		if(p_obstaculo.es_marca_fin_nivel())
		{
			LOG<<"HABILITANDO FIN DE NIVEL"<<std::endl;
			this->estructura_info_nivel.habilitar_comprobacion_finalizacion_nivel_preparado();
		}

		this->estructura_info_nivel.modificar_artefactos(p_obstaculo.acc_propiedades_artefactos());
		this->estructura_info_nivel.modificar_vector_y(p_obstaculo.acc_nueva_velocidad(), p_obstaculo.acc_velocidad_cambio());

		LOG<<"FLAGS ARTEFACTOS: "<<p_obstaculo.acc_propiedades_artefactos()<<std::endl;
		LOG<<"NUEVA VELOCIDAD: "<<p_obstaculo.acc_nueva_velocidad()<<" A PASO "<<p_obstaculo.acc_velocidad_cambio()<<std::endl;

		establecer_artefacto_jugador_auto();


	/*Es posible que ahora estemos usando o cargando un artefacto que ya
	no podríamos usar: por ejemplo, el fantasma o el cañón. Si ya no se
	puede usar y lo estamos usando vamos a anularlos.*/

		//El fantasma podría quedarse pillado en alguna posición...
		if(!estructura_info_nivel.es_artefacto_permitido(Definiciones::A_TELETRANSPORTADOR))
		{
			this->forzar_fin_fantasma();
		}

		//La carga del cañón podría quedarse pillada.
		if(!estructura_info_nivel.es_artefacto_permitido(Definiciones::A_CANON))
		{
			J->forzar_fin_carga_canon();
		}
	}

	return 0;
}

unsigned int Controlador_juego::actividad_no_jugador(Obstaculo_canon_programable &p_canon)
{
	Proyectil * pr=NULL;
	pr=p_canon.disparar();

	if(pr)
	{
		this->proyectiles.push_back(pr);
		return p_canon.actuar_sonido();
	}
	else
	{
		return 0;
	}
}

unsigned int Controlador_juego::actividad_no_jugador(Obstaculo_canon_apuntado &p_canon)
{
	//Este es el único estado en el que permitimos que ocurra algo.
	if(estado_actual!=E_JUEGO) return 0;

	Proyectil * pr=NULL;
	pr=p_canon.disparar(*this->J);

	if(pr)
	{
		this->proyectiles.push_back(pr);
		return p_canon.actuar_sonido();
	}
	else
	{
		return 0;
	}
}

unsigned int Controlador_juego::actividad_no_jugador(Obstaculo_canon_giratorio &p_canon)
{
	Proyectil * pr=NULL;

	unsigned int cantidad_canones=p_canon.acc_cantidad_canones();
	unsigned int canon_actual=0;
	unsigned int resultado=0;

	while(canon_actual < cantidad_canones)
	{
		pr=p_canon.disparar(canon_actual);

		if(pr)
		{
			this->proyectiles.push_back(pr);
			resultado=p_canon.actuar_sonido();
		}
		else
		{
			return 0;
		}

		canon_actual++;
	}

	return resultado;
}

unsigned int Controlador_juego::actividad_no_jugador(Obstaculo_disparador &p_disparador)
{
	//Este es el único estado en el que permitimos que ocurra algo.
	if(estado_actual!=E_JUEGO) return 0;

	Proyectil * pr=NULL;
	pr=p_disparador.disparar(*this->J);

	if(pr)
	{
		this->proyectiles.push_back(pr);
		return p_disparador.actuar_sonido();
	}
	else
	{
		return 0;
	}
}

unsigned int Controlador_juego::actividad_no_jugador(Obstaculo_generador_decoracion &p_generador)
{
	if(p_generador.acc_explosion()) this->generar_explosion(&p_generador, false, p_generador.acc_humo_explosion());
	if(p_generador.acc_chispas()) this->generar_chispas(&p_generador, p_generador.acc_chispas());
	if(p_generador.acc_chatarra()) this->generar_chatarra(&p_generador, p_generador.acc_chatarra());

	p_generador.desactivar();

	return 0;
}

unsigned int Controlador_juego::actividad_no_jugador(Obstaculo_generador_obstaculos &p_obs)
{
	if(p_obs.es_activo())
	{
		Actor_mapa * obstaculo=NULL;

		//Generar un nuevo obstáculo del tipo.
		switch(p_obs.acc_tipo_obstaculo())
		{
			case Obstaculo_generador_obstaculos::TO_26_26:
				obstaculo=Factoria_actores_mapa::fabricar(Actor_mapa::T_OBSTACULO_26_26, 0);
			break;

			case Obstaculo_generador_obstaculos::TO_32_64:
				obstaculo=Factoria_actores_mapa::fabricar(Actor_mapa::T_OBSTACULO_32_64, 0);
			break;

			case Obstaculo_generador_obstaculos::TO_64_26:
				obstaculo=Factoria_actores_mapa::fabricar(Actor_mapa::T_OBSTACULO_64_26, 0);
			break;

			default:
			case Obstaculo_generador_obstaculos::TO_128_32:
				obstaculo=Factoria_actores_mapa::fabricar(Actor_mapa::T_OBSTACULO_128_32, 0);
			break;
		}

		if(obstaculo)
	 	{
			//Aplicarle el vector generado desde los valores.
			DLibH::Vector_2d vec=p_obs.obtener_vector();
			obstaculo->establecer_vector(vec);

			//Colocarlo en posición.
			float x=0, y=0;
			p_obs.obtener_posicion(x, y);
			obstaculo->ir_a(x, y);

			//Colocarlo con el resto de obstáculos.
			this->actores_mapa.push_back(obstaculo);
		}
	}

	return 0;
}

/*Activa un trigger automático. El método que llama a esto no llegará a este
punto si el obstáculo no está "activo."*/

unsigned int Controlador_juego::actividad_no_jugador(Obstaculo_trigger_auto &p_trigger)
{
	LOG<<"ACTIVANDO TRIGGER NO JUGADOR "<<p_trigger.acc_tag_accion()<<std::endl;
	this->activar_accion_trigger(p_trigger);
	return 0;
}

/*Muestra un tutorial. Hay un número limitado y estático de cadenas que pueden
mostrarse en el tutorial y desde aquí las estamos mostrando todas. Aunque los
valores del enum son correlativos tampoco es esto tan crítico como para
calcularlo con offset y está bien claro así.*/

unsigned int Controlador_juego::actividad_no_jugador(Obstaculo_tutorial &p_obstaculo)
{
	//Este es el único estado en el que permitimos que ocurra algo.
	if(estado_actual!=E_JUEGO) return 0;

	p_obstaculo.usar();

	if(controlador_perfiles.es_mostrar_tutoriales())
	{
		std::string cadena;
		switch(p_obstaculo.acc_codigo_mensaje())
		{
			case 0: cadena=""; break;
			case 1: cadena=localizador.obtener(Localizador::C_TUTORIAL_01); break;
			case 2: cadena=localizador.obtener(Localizador::C_TUTORIAL_02); break;
			case 3: cadena=localizador.obtener(Localizador::C_TUTORIAL_03); break;
			case 4: cadena=localizador.obtener(Localizador::C_TUTORIAL_04); break;
			case 5: cadena=localizador.obtener(Localizador::C_TUTORIAL_05); break;
			case 6: cadena=localizador.obtener(Localizador::C_TUTORIAL_06); break;
			case 7: cadena=localizador.obtener(Localizador::C_TUTORIAL_07); break;
			case 8: cadena=localizador.obtener(Localizador::C_TUTORIAL_08); break;
			case 9: cadena=localizador.obtener(Localizador::C_TUTORIAL_09); break;
			case 10: cadena=localizador.obtener(Localizador::C_TUTORIAL_10); break;
			case 11: cadena=localizador.obtener(Localizador::C_TUTORIAL_11); break;
			case 12: cadena=localizador.obtener(Localizador::C_TUTORIAL_12); break;
			case 13: cadena=localizador.obtener(Localizador::C_TUTORIAL_13); break;
			case 14: cadena=localizador.obtener(Localizador::C_TUTORIAL_14); break;
			case 15: cadena=localizador.obtener(Localizador::C_TUTORIAL_15); break;
			case 16: cadena=localizador.obtener(Localizador::C_TUTORIAL_16); break;
			case 17: cadena=localizador.obtener(Localizador::C_TUTORIAL_17); break;
			case 18: cadena=localizador.obtener(Localizador::C_TUTORIAL_18); break;
			case 19: cadena=localizador.obtener(Localizador::C_TUTORIAL_19); break;
			case 20: cadena=localizador.obtener(Localizador::C_TUTORIAL_20); break;
			case 21: cadena=localizador.obtener(Localizador::C_TUTORIAL_21); break;
		}

		tutorial=Tutorial(cadena, localizador.obtener(Localizador::C_TUTORIAL_ESPERANDO_INPUT), p_obstaculo.acc_ms_tiempo(), 255.0f);
//		tutorial=Tutorial(cadena, localizador.obtener(Localizador::C_TUTORIAL_ESPERANDO_INPUT), 100.0f, 255.0f);
		tutorial.activar();

		if(p_obstaculo.es_finaliza_tutoriales())
		{
			LOG<<"DESACTIVANDO TUTORIALES EN PERFIL..."<<std::endl;
			controlador_perfiles.desactivar_tutoriales_en_perfil();
		}
	}

	return 0;
}

unsigned int Controlador_juego::actividad_no_jugador(Obstaculo_colocador_jugador &p_obs)
{
	p_obs.usar();

	int 	x=p_obs.acc_x(),
		y=p_obs.acc_y();


	this->estructura_info_nivel.debug_forzar_distancia(y);
	J->ir_a(x, y);
	F->ir_a(x, y);
	M->ir_a(0, y);
	camara.enfocar_a(0, y);

	LOG<<"MOVIENDO JUGADOR A: "<<x<<","<<y<<std::endl;

	return 0;
}

unsigned int Controlador_juego::actividad_no_jugador(Trazador_fantasma &t)
{
	//Este es el único estado en el que permitimos que ocurra algo.
	if(estado_actual!=E_JUEGO) return 0;

	Decoracion_chispa * ch=NULL;
	DLibH::Vector_2d v(0, 0);
	float d=(1000+(rand()%2000)) / 1000;

	ch=new Decoracion_chispa(t.acc_x_centro(), t.acc_y_centro(), Decoracion_chispa::ROJOS);
	v*=100.0f+rand()%100;
	v.x=( (rand()%10)-5 ) * 5.0f;
	ch->establecer_vector(v);
	ch->establecer_duracion(d);
	this->decoraciones.push_back(ch);

	ch=NULL;

	if(t.debe_dejar_reflejo())
	{
		Decoracion_fantasma * f=NULL;
		f=new Decoracion_fantasma(t.acc_x(), t.acc_y());
		this->decoraciones.push_back(f);
		f=NULL;
	}

	return 0;
}

unsigned int Controlador_juego::actividad_no_jugador(Trazador_explosion &t)
{
	if(t.puede_soltar_humo())
	{
		Decoracion_humo * h=NULL;
		h=new Decoracion_humo(t.acc_x(), t.acc_y(), t.obtener_parte_humo());
		this->decoraciones.push_back(h);
		h=NULL;
	}

	return 0;
}

unsigned int Controlador_juego::actividad_no_jugador(Boss_01 &b)
{
	int resultado=0;

	if(b.puede_disparar())
	{
		std::vector<Proyectil *> generados=b.obtener_disparos();
		for(Proyectil * pr : generados)
		{
			this->proyectiles.push_back(pr);
			resultado=b.actuar_sonido();
		}

		//Limpiamos los proyectiles del turno anterior: están en posesión del controlador ahora...
		b.vaciar_proyectiles();
	}

	//Echar humo si procede.
	if(b.puede_generar_decoraciones())
	{
		std::vector<Boss::Estructura_decoracion_boss> generados=b.obtener_decoraciones();
		for(Boss::Estructura_decoracion_boss & d : generados)
			generar_decoracion_desde_estructura_boss(d);

		b.vaciar_decoraciones();
	}

	//Lanzar el trigger.
	if(b.es_preparado_para_lanzar_trigger())
	{
		this->activar_accion_trigger(b);
		b.finalizar_estado_trigger();
	}

	if(b.es_preparado_para_hacer_explosion())
	{
		pantalla.adjuntar_proceso(DLibV::Proceso_superficie_fundido::generar(this->pantalla.acc_superficie(), 200,80,30, 255, 0, 1));
		b.finalizar_estado_explosion();
	}

	return resultado;
}

unsigned int Controlador_juego::actividad_no_jugador(Boss_02 &b)
{
	int resultado=0;

	//Mientras la batalla no se haya iniciado no hace nada. Cuando esté
	//activado y el scroll se pare se iniciará la batalla.
	if(!b.es_batalla_iniciada())
	{
		if(!estructura_info_nivel.acc_vector_y())
		{
			b.iniciar_batalla();
		}
	}
	//Cuando la batalla se haya iniciado entonces pueden ocurrir varias cosas...
	else
	{
		unsigned short int senal_sonido=b.obtener_senales_sonido();

		if(senal_sonido & Boss_02::SND_EXPLOSION) this->cola_sonido.insertar(DLibA::Estructura_sonido(DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_EXPLOSION), 127));
		if(senal_sonido & Boss_02::SND_CANON) this->cola_sonido.insertar(DLibA::Estructura_sonido(DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_PHASER), 60));
		if(senal_sonido & Boss_02::SND_GATLING) this->cola_sonido.insertar(DLibA::Estructura_sonido(DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_DISPARO), 60));

		//Establecemos el blanco al jugador, que se necesitará para apuntar los disparos.
		b.establecer_blanco(this->J);

		//Lanzar el trigger.
		if(b.es_preparado_para_lanzar_trigger())
		{
			this->activar_accion_trigger(b);
			b.finalizar_estado_trigger();
		}

		//Realizar explosión...
		if(b.es_destruido())
		{
			pantalla.adjuntar_proceso(DLibV::Proceso_superficie_fundido::generar(this->pantalla.acc_superficie(), 200,80,30, 255, 0, 1));
			b.iniciar_caida();

			//TODO: Si está reproduciendo algo, detenerlo.

//			if(b.obtener_canal_sonido_canon_izq())
//			{
//				controlador_audio.detener_sonido(b.obtener_canal_sonido_canon_izq());
//				b.liberar_canal_sonido_canon_izq();
//			}
		}

		if(b.puede_disparar())
		{
			std::vector<Proyectil *> generados=b.obtener_disparos();
			for(Proyectil * pr : generados)
			{
				this->proyectiles.push_back(pr);
				resultado=b.actuar_sonido();
			}

			//Limpiamos los proyectiles del turno anterior: están en posesión del controlador ahora...
			b.vaciar_proyectiles();
		}

		//Echar humo si procede.
		if(b.puede_generar_decoraciones())
		{
			std::vector<Boss::Estructura_decoracion_boss> generados=b.obtener_decoraciones();
			for(Boss::Estructura_decoracion_boss & d : generados)
				generar_decoracion_desde_estructura_boss(d);

			b.vaciar_decoraciones();
		}

		try
		{
			while(b.debe_solicitar_canal())
			{
				b.obtener_canal(controlador_audio.obtener_canal_libre());
			}
		}
		catch(const DLibA::Excepcion_controlador_audio& e)
		{}

		//Para el cañón izquierdo...
/*		if(b.debe_iniciar_sonido_canon_izq()) //Debe reproducir sonido? Insertar sonido y asignar canal.
		{
			int canal=controlador_audio.reproducir_sonido(*DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_RECARGA), 1, -1);
			b.asignar_canal_sonido_canon_izq(canal);
		}
		else if(b.debe_detener_sonido_canon_izq()) //Debe detener sonido?. Pararlo y reiniciar el canal.
		{
			controlador_audio.detener_sonido(b.obtener_canal_sonido_canon_izq());
			b.liberar_canal_sonido_canon_izq();
		}

		//TODO: Controlar destrucción de canón y detener sonido!.
*/
	}

	return resultado;
}
