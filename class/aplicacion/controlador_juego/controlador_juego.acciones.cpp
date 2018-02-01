//En este archivo está metido todo lo que esté relacionado con las acciones
//y triggers.

/*Activa el trigger recibido. Busca las acciones asignadas y las pone en marcha.
El trigger que se recibe no es constante ya que se pasa también como referencia
a las acciones para poder notificar que la acción ha finalizado.*/

void Controlador_juego::activar_accion_trigger(Trigger& p_trigger)
{
	if(p_trigger.puede_ser_activado())
	{
		if(!p_trigger.acc_tag_accion()) return;
		p_trigger.activar();	
		this->instanciar_acciones_para_tag(p_trigger.acc_tag_accion(), &p_trigger);
		LOG<<"INSTANCIANDO ACCION "<<p_trigger.acc_tag_accion()<<" COMPLETA"<<std::endl;
	}
}

void Controlador_juego::instanciar_acciones_para_tag(unsigned int p_tag, Trigger * p_trigger)
{
	std::vector<Accion_trigger_compuesta> acciones=this->estructura_info_nivel.obtener_composicion_accion_trigger_por_tag(p_tag);

LOG<<"INSTANCIADAS "<<acciones.size()<<" ACCIONES PARA TAG "<<p_tag<<std::endl;

	if(acciones.size())
	{
		//Esto de las lambas está empezando a salir por todas partes!.
		auto asignar_actores_tag=[this](Accion_trigger_compuesta& p_accion)
		{
			p_accion.actores=this->obtener_actores_por_tag(p_accion.accion->acc_tag_actores());
		};

		auto asignar_trigger=[&p_trigger](Accion_trigger_compuesta& p_accion)
		{
			p_accion.trigger=p_trigger;
		};

		auto insertar_en_vector=[this](Accion_trigger_compuesta& p_accion)
		{
			this->vector_acciones_corrientes.push_back(p_accion);
		};

		auto preparar=[this](Accion_trigger_compuesta& p_accion)
		{
			this->preparar_accion(p_accion);
		};

		//A estas acciones se les asignan referencias a todos los actores con ese tag...
		for_each(acciones.begin(), acciones.end(), asignar_actores_tag);

		//A cada acción se le pasa una referencia del trigger 	
		//para que puedan marcarse como "corriendo" y finalizada 
		//para el mismo.

		if(p_trigger) 
		{
			for_each(acciones.begin(), acciones.end(), asignar_trigger);
		}

		//Un último paso es preparar las acciones si es necesario. Por
		//ejemplo, las acciones de ir_a o ruta deben ser preparadas antes
		//de ser lanzadas. El método es uno propio pero como no tengo
		//ni puta idea he metido la llamada en un lambda.

		for_each(acciones.begin(), acciones.end(), preparar);
		
		//Y ahora las insertamos en el vector de acciones que están en marcha.
		//Todas esas acciones deben pervivir en un vector dedicado. Cuando vayamos
		//a eliminar un actor miraremos en ese vector dedicado.

		for_each(acciones.begin(), acciones.end(), insertar_en_vector);
	}
}

/*Prepara una acción con toda la información que debe tener. Por ejemplo, si
es una acción de ir_a obtendrá las coordenadas necesarias.*/

void Controlador_juego::preparar_accion(Accion_trigger_compuesta& p_accion)
{
	switch(p_accion.accion->acc_tipo())
	{
		//Localizar al primer actor con la tag deseada y extraer sus
		//coordenadas.

		case Accion_trigger::T_IR_A:
		{
			float x=0, y=0;
			std::vector<Actor_mapa *> vactores=this->obtener_actores_por_tag(p_accion.accion->acc_tag_actores_secundario());

			if(vactores.size())
			{
				x=vactores[0]->acc_x();
				y=vactores[0]->acc_y();
			}

			LOG<<"TAG "<<p_accion.accion->acc_tag_actores()<<" IR A PUNTO RUTA "<<p_accion.accion->acc_tag_actores_secundario()<<" "<<x<<","<<y<<std::endl;

			p_accion.datos.asignar_datos_posicion(x,y);
			p_accion.datos.asignar_desplazamientos(p_accion.accion->acc_desp_x(), p_accion.accion->acc_desp_y()); 
		}
		break;

		//Localizar al primer actor con la tag deseada y obtener del 
		//mismo los valores...

		case Accion_trigger::T_CAMBIAR_VECTOR:
		{
			float x=0, y=0;
			std::vector<Actor_mapa *> vactores=this->obtener_actores_por_tag(p_accion.accion->acc_tag_actores_secundario());

			if(vactores.size())
			{
				//Castear el actor al tipo de turno...
				Obstaculo_vector * pr=static_cast<Obstaculo_vector *> (vactores[0]);
		
				x=pr->acc_nuevo_vector_x();
				y=pr->acc_nuevo_vector_y();
			}			

			LOG<<"CAMBIO DE VECTOR A "<<x<<","<<y<<" PARA TAG "<<p_accion.accion->acc_tag_actores()<<std::endl;

			p_accion.datos.asignar_datos_posicion(x,y);
		}
		break;

		//Localizar al primer actor con la tag deseada y luego a todos
		//los puntos de ruta que compartan el mismo grupo.

		case Accion_trigger::T_RUTA:
		{
			//Localizar el actor, castearlo a punto ruta.

			LOG<<"RUTA: TAG "<<p_accion.accion->acc_tag_actores()<<" VIAJANDO A PUNTO RUTA "<<p_accion.accion->acc_tag_actores_secundario()<<std::endl;

			std::vector<Actor_mapa *> vactores=this->obtener_actores_por_tag(p_accion.accion->acc_tag_actores_secundario());

			if(vactores.size())
			{

				LOG<<"RUTA: HAY "<<vactores.size()<<" ACTORES PARA LA TAG "<<p_accion.accion->acc_tag_actores_secundario()<<std::endl;
				Punto_ruta * pr=static_cast<Punto_ruta *> (vactores[0]);
				
	/*Enviar el mapa de puntos con el mismo grupo de ruta a donde sea. Los 
	datos del trigger tienen su propio tipo de datos, de modo que haremos 
	conversiones... alguna fumadilla: si la agrupación es cero no la usaremos,
	luego, al buscar en los puntos de ruta lo que se nos devuelve es el tipo
	t_par_punto_ruta que es std::pair<unsigned int, std::map<Punto_ruta *>,
	que hay que saber manejarlo.*/

				if(pr->acc_agrupacion()) //OJO: La agrupación cero no vale.
				{
					t_mapa_puntos_ruta::iterator buscar;
					buscar=this->puntos_ruta.find(pr->acc_agrupacion());

					if(buscar != this->puntos_ruta.end()) //Comprobar que la agrupación existe.
					{
						auto insertar_informacion=[&p_accion](t_par_punto_ruta_simple p_par)
						{
							Punto_ruta * pr=p_par.second;
							p_accion.datos.insertar_datos_ruta(pr->acc_x(), pr->acc_y(), pr->acc_velocidad(), pr->es_instantaneo(), pr->acc_identificador(), pr->acc_siguiente());
						};

						//Rellenar la información.						
						for_each(buscar->second.begin(), buscar->second.end(), insertar_informacion);

						//Enviar el actual para saber dónde hay que empezar.
						p_accion.datos.escoger_punto_ruta(pr->acc_identificador());

						//Los desplazamientos...
						p_accion.datos.asignar_desplazamientos(p_accion.accion->acc_desp_x(), p_accion.accion->acc_desp_y()); 
					}
				}
			}
		}
		break;

		/*Localizar con la tag secundaria al actor de tipo "propiedades_movimiento_relativo".		*/

		case Accion_trigger::T_MOVIMIENTO_RELATIVO:
		{
			//Localizar el actor, castearlo...

			LOG<<"MOVIMIENTO RELATIVO: TAG "<<p_accion.accion->acc_tag_actores()<<" CON PROPIEDADES DE "<<p_accion.accion->acc_tag_actores_secundario()<<std::endl;
			std::vector<Actor_mapa *> vactores=this->obtener_actores_por_tag(p_accion.accion->acc_tag_actores_secundario());

			//Aunque haya varios sólo vamos a coger uno.

			if(vactores.size())
			{
				//Obtenemos las propiedades. Si no es un realmente de ese tipo vamos a tener un problema.
				Obstaculo_propiedades_movimiento_relativo * pr=static_cast<Obstaculo_propiedades_movimiento_relativo *> (vactores[0]);
				
				float 	mov_x=pr->acc_desplazamiento_x(), 
					mov_y=pr->acc_desplazamiento_y(), 
					vel=pr->acc_velocidad();
			
				//Cargamos las propiedades en p_accion.datos, que es del tipo Accion_trigger_datos y tiene
				//un campo para guardar esta información.

				p_accion.datos.asignar_datos_posicion(mov_x, mov_y, vel);

				//Lo siguiente ya ocurrirá en actor_mapa, cuando llegue a recibir_senal_trigger: leerá estos
				//valores y contará su propio movimiento para establecer datos de ruta.
			}
		}
		break;

		/*Para las propiedades de fondo y de nivel buscamos en los
		actores primarios los datos. El actor debe ser del tipo
		Obstaculo_propiedades_nivel u obstaculo_propiedades_fondo y 
		debe haber tan sólo uno.*/

		case Accion_trigger::T_PROPIEDADES_FONDO:
		{
			if(!(p_accion.actores.size()==1 && p_accion.actores[0]->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_PROPIEDADES_FONDO))
			{
				p_accion.invalidar();
			}
		}
		break;

		case Accion_trigger::T_PROPIEDADES_NIVEL:
		{
			if(!	(p_accion.actores.size()==1 && p_accion.actores[0]->tipo_actor_mapa()==Actor_mapa::T_OBSTACULO_PROPIEDADES))
			{
				p_accion.invalidar();
			}
		}
		break;
	}
}

/*El orden de los factores lo es todo... 

*/

void Controlador_juego::procesar_turno_acciones(float p_delta)
{
	if(!this->vector_acciones_corrientes.size()) 
	{
		return;
	}
	//Primero se hace el turno...

	auto turno_accion=[p_delta, this](Accion_trigger_compuesta& accion)
	{
		accion.turno(p_delta);
	};
	for_each(this->vector_acciones_corrientes.begin(), this->vector_acciones_corrientes.end(), turno_accion);

	//Ahora se comprueban si ha finalizado y se borran. Usamos stable partition
	//que reordena según el predicado y conserva el orden. Antes usábamos 
	//"remove if" que al parecer estaba dejando todos los elementos partir 
	//de nfin en un estado no usable. Eso si, para stable_partition primero
	//se ordenan los elementos que devuelven "TRUE" y luego los que 
	//devuelven "FALSE" de modo que queremos devolver "!es_finalizada".

	unsigned int acciones_finalizadas=0;

	auto ha_finalizado=[&acciones_finalizadas, p_delta](Accion_trigger_compuesta& accion) -> bool 
	{
		if(accion.es_finalizada() || accion.es_invalidada()) 
		{
			acciones_finalizadas++;
		}
		//Leer comentario superior.
		return !accion.es_finalizada();
	};

	//std::vector<Accion_trigger_compuesta>::iterator nfin=std::remove_if(this->vector_acciones_corrientes.begin(), this->vector_acciones_corrientes.end(), ha_finalizado);
	std::vector<Accion_trigger_compuesta>::iterator nfin=std::stable_partition(this->vector_acciones_corrientes.begin(), this->vector_acciones_corrientes.end(), ha_finalizado);
	
	if(acciones_finalizadas)
	{
		//Antes de eliminarlas definitivamente vamos a ver si tenemos que invocar
		//a otra acción al finalizar esta. Luego las borramos. Como no podemos 
		//instanciarlas directamente porque se nos joden los iteradores vamos
		//a meterlas en un vector...
	
		std::vector<unsigned int> tags;

		auto tag_a_seguir=[&tags](Accion_trigger_compuesta& accion)
		{
			if(accion.accion->acc_tag_siguiente())
				tags.push_back(accion.accion->acc_tag_siguiente());	
		};

		for_each(nfin, this->vector_acciones_corrientes.end(), tag_a_seguir);

		this->vector_acciones_corrientes.erase(nfin, this->vector_acciones_corrientes.end());

		//Ahora podemos instanciar las nuevas...
		if(tags.size())
		{
			auto instanciar=[this](unsigned int tag) 
			{
				this->instanciar_acciones_para_tag(tag, NULL);
			};
			for_each(tags.begin(), tags.end(), instanciar);
		}
	}
}

void Controlador_juego::eliminar_actor_de_acciones(Actor_mapa * p_actor)
{
	auto proceso=[p_actor](Accion_trigger_compuesta& accion)
	{
		accion.eliminar_actor(p_actor);
	};

	for_each(this->vector_acciones_corrientes.begin(), this->vector_acciones_corrientes.end(), proceso);
}

std::vector<Actor_mapa *> Controlador_juego::obtener_actores_por_tag(unsigned int p_tag)
{
	std::vector<Actor_mapa *> resultado;

	//Otra lambda más, para seguir probando...
	auto obtener_actores_tag=[p_tag, &resultado](Actor_mapa * p_actor)
	{
		if(p_actor->acc_tag()==p_tag) resultado.push_back(p_actor);
	};

	for_each(this->actores_mapa.begin(), this->actores_mapa.end(), obtener_actores_tag);

LOG<<resultado.size()<<" ACTORES PARA TAG "<<p_tag<<std::endl;

	return resultado;
}

void Controlador_juego::vaciar_vector_de_acciones()
{
	this->vector_acciones_corrientes.clear();
}
