void Controlador_editor::dibujar_rejilla()
{
	int x=0; int y=-(this->posicion_y % REJILLA_Y);
	Uint32 color=DLibV::Gestor_color::color(REJILLA_R, REJILLA_G, REJILLA_B);

	//Primero para abajo...
	for(; x< Medidas::ANCHO_VISTA; x+=REJILLA_X)
	{
		DLibV::Utilidades_graficas_SDL::linea(x, 0, x, Medidas::ALTO_VISTA , this->pantalla.acc_superficie(), color);
	}

	//Ahora para la derecha...
	for(; y< Medidas::ALTO_VISTA ; y+=REJILLA_Y)
	{
		DLibV::Utilidades_graficas_SDL::linea(0, y, Medidas::ANCHO_VISTA, y, this->pantalla.acc_superficie(), color);
	}
}

void Controlador_editor::representar()
{
	this->pantalla.rellenar(64, 64, 64, this->camara.acc_caja_pos());

	switch(this->estado_actual)
	{
		case E_EDITAR_NIVEL: this->representar_modo_editar_nivel(); break;
		case E_EDITAR_PROPIEDADES: this->representar_modo_editar_propiedades(); break;
		case E_EDITAR_TABLA_OBSTACULOS:
		case E_EDITAR_TABLA_BONUS: 
		case E_EDITAR_TABLA_BONUS_CAJA: 
		case E_EDITAR_TABLA_BONUS_OBSTACULO: this->representar_modo_editar_tablas(this->estado_actual); break;
		case E_EDITAR_PARAMETROS: this->representar_modo_editar_parametros(); break;
		case E_EDITAR_TAG: this->representar_modo_editar_tag(); break;
		case E_EDITAR_ACCIONES: this->representar_modo_editar_acciones(); break;
		case E_LISTADO: this->representar_modo_listado(); break;
		case E_INFO: this->representar_modo_info(); break;
	}

	//Info...

	if(this->mostrar_info)
	{
		this->generar_texto_info();
		this->pantalla.rellenar(128, 64, 64, this->rep_txt_info->acc_posicion());
		this->rep_txt_info->volcar(this->pantalla);	
	}

	this->pantalla.flipar();
}

void Controlador_editor::representar_modo_editar_parametros()
{
	std::vector<std::string> ver_activo;
	for(int i=0; i<TOTAL_PARAMS; i++) ver_activo.push_back(this->indice_parametro==i ? "*" : " ");

	std::vector<std::string> ver_params;
	for(int i=0; i<TOTAL_PARAMS; i++) ver_params.push_back("N/A");

	std::vector<unsigned int> ver_valores;
	ver_valores.push_back(this->seleccion_actual->parametros.param1);
	ver_valores.push_back(this->seleccion_actual->parametros.param2);
	ver_valores.push_back(this->seleccion_actual->parametros.param3);
	ver_valores.push_back(this->seleccion_actual->parametros.param4);
	ver_valores.push_back(this->seleccion_actual->parametros.param5);
	ver_valores.push_back(this->seleccion_actual->parametros.param6);
	ver_valores.push_back(this->seleccion_actual->parametros.param7);
	ver_valores.push_back(this->seleccion_actual->parametros.param8);

	unsigned int indice=this->seleccion_actual->actor->tipo_actor_mapa();

	if(this->mapa_parametros.find(indice) != this->mapa_parametros.end())
	{
		ver_params[0]=this->mapa_parametros[indice].param1;
		ver_params[1]=this->mapa_parametros[indice].param2;
		ver_params[2]=this->mapa_parametros[indice].param3;
		ver_params[3]=this->mapa_parametros[indice].param4;
		ver_params[4]=this->mapa_parametros[indice].param5;
		ver_params[5]=this->mapa_parametros[indice].param6;
		ver_params[6]=this->mapa_parametros[indice].param7;
		ver_params[7]=this->mapa_parametros[indice].param8;
	}
	

	this->cadena_parametros="";

	for(int i=0; i<TOTAL_PARAMS; i++) 
	{
		this->cadena_parametros+=ver_activo[i]+" "+ver_params[i]+" : "+DLibH::Herramientas::entero_a_cadena(ver_valores[i])+"\n";
	}
	this->cadena_parametros+=">>>"+this->escritor.acc_cadena();

	this->rep_txt_parametros->asignar(this->cadena_parametros);
	this->rep_txt_parametros->volcar(this->pantalla);
}

void Controlador_editor::representar_modo_editar_tag()
{
	this->cadena_tag="TAG: "+this->escritor.acc_cadena();
	this->rep_txt_tag->asignar(this->cadena_tag);
	this->rep_txt_tag->volcar(this->pantalla);
}

void Controlador_editor::representar_modo_editar_nivel()
{
	if(this->mostrar_rejilla) this->dibujar_rejilla();

	//Sacamos los que están en pantalla...

	std::vector<T_TIPO_ACTOR *> en_pantalla;

	for(T_TIPO_ACTOR * ini : actores_mapa)
	{	
		//No dibujar la selección actual...
		if(this->seleccion_actual && this->seleccion_actual->actor==ini->actor)
		{
			continue;
		}
		else if(existe_en_seleccion_multiple(ini))
		{
			continue;
		}

		if(DLibH::Herramientas_SDL::rectangulos_superpuestos(ini->actor->acc_posicion(), camara.acc_caja()))
		{
			en_pantalla.push_back(ini);
		}
	}

	//Los ordenamos y mostramos.

	Ordenador_actores_profundidad_editor ord;
	std::sort(en_pantalla.begin(), en_pantalla.end(), ord);

	for(T_TIPO_ACTOR * ini : en_pantalla)
	{
		//Como este no es representable haremos ahí algo raro.
		if(!ini->actor->es_representable())
		{
			if(this->mostrar_ocultos) this->pantalla.rellenar(0, 30, 200, ini->actor->acc_posicion(), this->camara);
		}
		else
		{
			if(actor_supera_filtro_profundidad(ini))
			{
				ini->actor->acc_representacion()->volcar(this->pantalla, this->camara);
			}	
		}
	}

	//Selecciones... Se muestran siempre. Estén en pantalla o no. Da igual, en serio.

	Uint32 color=0;

	auto representar_seleccion=[this, &color](T_TIPO_ACTOR * a)
	{
		SDL_Rect caja=a->actor->acc_posicion();
		caja.y-=this->posicion_y+2;
		caja.x-=2;
		caja.w+=4;
		caja.h+=4;
		this->pantalla.rellenar(color, caja);

		//Como este no es representable haremos ahí algo raro.
		if(!a->actor->es_representable())
		{
			if(this->mostrar_ocultos) this->pantalla.rellenar(0, 30, 200, a->actor->acc_posicion(), this->camara);
		}
		else
		{
			a->actor->acc_representacion()->volcar(this->pantalla, this->camara);
		}
	};

	if(this->seleccion_multiple.size())
	{
		color=DLibV::Gestor_color::color(0, 0, 124);
		for_each(seleccion_multiple.begin(), seleccion_multiple.end(), representar_seleccion);
	}
	if(this->seleccion_actual)
	{
		color=DLibV::Gestor_color::color(124, 0, 0);
		representar_seleccion(this->seleccion_actual);
	}
}

void Controlador_editor::representar_modo_editar_tablas(unsigned int p_tabla)
{
	std::string cadena="";

	switch(p_tabla)
	{
		case E_EDITAR_TABLA_OBSTACULOS: 
			cadena="TABLA OBSTACULOS ALEATORIOS\n"; 
			this->aux_representar_modo_editar_tablas(this->TABLA_OBSTACULOS, cadena, this->indice_editando_tabla); break;
		break;
		case E_EDITAR_TABLA_BONUS: 
			cadena="TABLA BONUS ALEATORIOS\n";
			this->aux_representar_modo_editar_tablas(this->TABLA_BONUS, cadena, this->indice_editando_tabla); break;
		break;
		case E_EDITAR_TABLA_BONUS_CAJA: 
			cadena="TABLA BONUS AL DESTRUIR CAJA\n"; 
			this->aux_representar_modo_editar_tablas(this->TABLA_DESTRUIR_CAJA, cadena, this->indice_editando_tabla); break;
		break;
		case E_EDITAR_TABLA_BONUS_OBSTACULO: 
			cadena="TABLA BONUS AL DESTRUIR OBSTACULO\n"; 
			this->aux_representar_modo_editar_tablas(this->TABLA_DESTRUIR_OBSTACULO, cadena, this->indice_editando_tabla); break;
		break;
	}

	cadena+="\n\n>>>"+this->escritor.acc_cadena();

	this->rep_txt_editar_tablas->asignar(cadena);
	this->rep_txt_editar_tablas->volcar(this->pantalla);	
}

void Controlador_editor::aux_representar_modo_editar_tablas(std::vector<Item_tabla_probabilidad> const &vector, std::string & cadena, unsigned int p_actual)
{
	std::vector<Item_tabla_probabilidad>::const_iterator		ini=vector.begin(),
									fin=vector.end();

	unsigned int i=0;
	unsigned int suma=0;
	unsigned int cantidad=0;


	while(ini < fin)
	{
		if(i==p_actual) cadena+=">> ";
		else cadena+="   ";

		i++;

		cantidad=(*ini).probabilidad;
		suma+=cantidad,

		cadena+=(*ini).nombre;
		cadena+=" -> ";
		cadena+=DLibH::Herramientas::entero_a_cadena(cantidad);
		cadena+="\n";		

		ini++;
	}

	cadena+="\n\nTOTAL "+DLibH::Herramientas::entero_a_cadena(suma);		
}

void Controlador_editor::representar_modo_editar_propiedades()
{
	std::string cadena("");

	auto componer=[&cadena, this](int indice, int p2)
	{
		std::string parte=this->editando_propiedad==indice ? ">>" : "  ";
			cadena+=
			parte+
			cadenas_propiedades[indice]+
			": "+
			DLibH::Herramientas::entero_a_cadena(p2)+
			" \n";
			
	};

	componer(P_NIVEL_VECTOR_Y, INFO_NIVEL.vector_y_nivel);
	componer(P_NIVEL_FLAGS_ARTEFACTOS_CANON , INFO_NIVEL.flags_artefactos_nivel & Definiciones::P_CANON ? 1 : 0);
	componer(P_NIVEL_FLAGS_ARTEFACTOS_TELETRANSPORTE, INFO_NIVEL.flags_artefactos_nivel & Definiciones::P_TELETRANSPORTADOR ? 1 : 0);
	componer(P_NIVEL_FLAGS_ARTEFACTOS_EDU, INFO_NIVEL.flags_artefactos_nivel & Definiciones::P_EMISOR_DENSIDAD_UNIFICADA ? 1 : 0);
	componer(P_NIVEL_FLAGS_ARTEFACTOS_ESCUDO, INFO_NIVEL.flags_artefactos_nivel & Definiciones::P_ESCUDO ? 1 : 0);
	componer(P_NIVEL_MAX_OBSTACULOS, INFO_NIVEL.max_obstaculos);
	componer(P_NIVEL_MAX_BONUS, INFO_NIVEL.max_bonus);
	componer(P_NIVEL_GEMAS_EN_NIVEL, INFO_NIVEL.gemas_en_nivel);
	componer(P_NIVEL_GEMAS_MAX_NIVEL, INFO_NIVEL.gemas_max_nivel);
	componer(P_PROXIMO_NIVEL, (int)INFO_NIVEL.indice_proximo_nivel);
	componer(P_PROXIMO_NIVEL_SECRETO, (int)INFO_NIVEL.indice_proximo_nivel_secreto);
	componer(P_INDICE_MUSICA, (int)INFO_NIVEL.indice_musica);

	cadena+="\n\n>>"+this->escritor.acc_cadena();

	this->rep_txt_editar_propiedades->asignar(cadena);
	this->rep_txt_editar_propiedades->volcar(this->pantalla);	
}


void Controlador_editor::generar_texto_info()
{
	unsigned int cantidad_actores=this->actores_mapa.size();
	unsigned int vect=(unsigned int)INFO_NIVEL.vector_y_nivel;
	int raton_x=this->controles_sdl.acc_raton().acc_x();
	int raton_y=this->controles_sdl.acc_raton().acc_y()+this->posicion_y;
	bool p_canon=INFO_NIVEL.flags_artefactos_nivel & Definiciones::P_CANON;
	bool p_teletransportador=INFO_NIVEL.flags_artefactos_nivel & Definiciones::P_TELETRANSPORTADOR;
	bool p_edu=INFO_NIVEL.flags_artefactos_nivel & Definiciones::P_EMISOR_DENSIDAD_UNIFICADA;
	bool p_escudo=INFO_NIVEL.flags_artefactos_nivel & Definiciones::P_ESCUDO;

	std::string c1("Y=");
	c1+=DLibH::Herramientas::entero_a_cadena(this->posicion_y);
	c1+=" / tot=";
	c1+=DLibH::Herramientas::entero_a_cadena(cantidad_actores);
	c1+=" | pos=";
	c1+=DLibH::Herramientas::entero_a_cadena(raton_x);
	c1+=",";
	c1+=DLibH::Herramientas::entero_a_cadena(raton_y);
	c1+=" | V=";
	c1+=DLibH::Herramientas::entero_a_cadena(vect);
	c1+=" | st=";
	c1+=DLibH::Herramientas::entero_a_cadena(maximo_tag);
	c1+="\nF=";
	c1+=DLibH::Herramientas::entero_a_cadena(p_canon);
	c1+=DLibH::Herramientas::entero_a_cadena(p_teletransportador);
	c1+=DLibH::Herramientas::entero_a_cadena(p_edu);
	c1+=DLibH::Herramientas::entero_a_cadena(p_escudo);
	c1+=" | VA=";
	c1+=this->filtro_profundidad_actores & F_FILTRO_FONDO ? "3" : "0";
	c1+=this->filtro_profundidad_actores & F_FILTRO_NORMAL ? "2" : "0";
	c1+=this->filtro_profundidad_actores & F_FILTRO_FRENTE ? "1" : "0";
	c1+=this->mostrar_ocultos ? "I" : "0";
	c1+=" | MO=";
	c1+=DLibH::Herramientas::entero_a_cadena(INFO_NIVEL.max_obstaculos);
	c1+=" | MB=";
	c1+=DLibH::Herramientas::entero_a_cadena(INFO_NIVEL.max_bonus);
	c1+=" | G=";
	c1+=DLibH::Herramientas::entero_a_cadena(INFO_NIVEL.gemas_en_nivel);
	c1+=" | GM=";
	c1+=DLibH::Herramientas::entero_a_cadena(INFO_NIVEL.gemas_max_nivel);
	c1+=" | ";
	c1+=this->bloquear_a_rejilla ? "BR" : "LR";
	c1+=" -> ";
	c1+=DLibH::Herramientas::entero_a_cadena(REJILLA_X);
	c1+="\n";

	if(this->seleccion_actual)
	{
		c1+=
		DLibH::Herramientas::entero_a_cadena(this->seleccion_actual->actor->acc_x())+
		","+
		DLibH::Herramientas::entero_a_cadena(this->seleccion_actual->actor->acc_y())+
		" T="+
		DLibH::Herramientas::entero_a_cadena(this->seleccion_actual->actor->acc_tag())+
		" ["+
		DLibH::Herramientas::entero_a_cadena(this->seleccion_actual->parametros.param1)+
		","+
		DLibH::Herramientas::entero_a_cadena(this->seleccion_actual->parametros.param2)+
		"] "+
		DLibH::Herramientas::entero_a_cadena(this->seleccion_actual->actor->acc_profundidad_representacion())+
		","+
		this->traducir_tipo_actor(this->seleccion_actual->actor->tipo_actor_mapa() );
	}
	else
	{
		c1+=this->TIPOS[this->grupo_tipo_actual].nombre;
		c1+=" -> ";
		c1+=this->TIPOS[this->grupo_tipo_actual].TIPOS[this->tipo_actor_actual].nombre;

	}

	if(this->mensaje.length())
	{
		c1+="\n";
		c1+=this->mensaje;
	}

	this->rep_txt_info->asignar(c1);
}

void Controlador_editor::representar_modo_editar_acciones()
{
	std::string cadena="";

	if(!this->acciones.size())
	{
		cadena="Sin acciones creadas. ESPACIO para crear nueva(no en campo descripción)";
	}
	else
	{
		//He aquí un ejemplo de lambda :D.

		auto linea= [](const std::string& pt1, const std::string& pt2, bool es) -> std::string
		{ 
			std::string ini=es ? "*" : " ";
			return "\n"+ini+pt1+": "+pt2;
		};

		cadena="Accion "+
		DLibH::Herramientas::entero_a_cadena(this->indice_accion+1)+
		" / "+		
		DLibH::Herramientas::entero_a_cadena(this->acciones.size())+
		"\n"+
		"ESPACIO PARA CREAR NUEVA, DEL PARA BORRAR ESTA\n"+
		"0-activar, 1-desactivar, 2-intercambiar, 3-ruta, 4-fin ruta, 5-ir a, \n"+
		"6-fondo, 7- propiedades, 8 - Visible, 9 - Invisible, 10 - Intercambiar visible\n"+
		"11 - Vector, 12 - Decoración, 13 - Desplazamiento, 14 - Fin desplazamiento\n"+
		linea("TIPO ", DLibH::Herramientas::entero_a_cadena(this->acciones[this->indice_accion].acc_tipo()), this->indice_parametro_accion==0)+
		linea("DESCRIPCION", this->acciones[this->indice_accion].acc_descripcion(), this->indice_parametro_accion==1)+
		linea("TAG", DLibH::Herramientas::entero_a_cadena(this->acciones[this->indice_accion].acc_tag()), this->indice_parametro_accion==2)+
		linea("TAG ACTORES", DLibH::Herramientas::entero_a_cadena(this->acciones[this->indice_accion].acc_tag_actores()), this->indice_parametro_accion==3)+
		linea("TAG ACTORES SECUNDARIO", DLibH::Herramientas::entero_a_cadena(this->acciones[this->indice_accion].acc_tag_actores_secundario()), this->indice_parametro_accion==4)+
		linea("TAG SIGUIENTE", DLibH::Herramientas::entero_a_cadena(this->acciones[this->indice_accion].acc_tag_siguiente()), this->indice_parametro_accion==5)+
		linea("MS ANTES", DLibH::Herramientas::entero_a_cadena(this->acciones[this->indice_accion].acc_ms_antes()), this->indice_parametro_accion==6)+
		linea("MS DESPUÉS", DLibH::Herramientas::entero_a_cadena(this->acciones[this->indice_accion].acc_ms_despues()), this->indice_parametro_accion==7)+
		linea("DESPLAZAMIENTO X", DLibH::Herramientas::entero_a_cadena(this->acciones[this->indice_accion].acc_desp_x()), this->indice_parametro_accion==8)+
		linea("DESPLAZAMIENTO Y", DLibH::Herramientas::entero_a_cadena(this->acciones[this->indice_accion].acc_desp_y()), this->indice_parametro_accion==9)+
		"\n\n>>>>"+this->cadena_acciones;
	}

	this->rep_txt_acciones->asignar(cadena);
	this->rep_txt_acciones->volcar(this->pantalla);
}

void Controlador_editor::representar_modo_listado()
{
	unsigned int MAX_LISTADO=22, i=0;
	std::string cadena("");
	
	auto funcion_mostrado=[this, &cadena](T_TIPO_ACTOR * actor, unsigned int i)
	{
		unsigned int tipo_actor=actor->actor->tipo_actor_mapa();
		
		std::string indice=DLibH::Herramientas::entero_a_cadena(this->indice_actual_listado+i);
		std::string x=DLibH::Herramientas::entero_a_cadena(actor->actor->acc_x());
		std::string y=DLibH::Herramientas::entero_a_cadena(actor->actor->acc_y());
		std::string w=DLibH::Herramientas::entero_a_cadena(actor->actor->acc_w());
		std::string h=DLibH::Herramientas::entero_a_cadena(actor->actor->acc_h());
		std::string tag=DLibH::Herramientas::entero_a_cadena(actor->actor->acc_tag());
		std::string prof=DLibH::Herramientas::entero_a_cadena(actor->params_profundidad.p);

		cadena+=" "+indice+" : TAG "+tag+" :: "+
			traducir_tipo_actor(tipo_actor)+
			" ["+x+","+y+"] - ("+w+", "+h+") p:"+prof+
			" \n";
	};

	cadena=DLibH::Herramientas::entero_a_cadena(indice_actual_listado+1)+
				" / "+
				DLibH::Herramientas::entero_a_cadena(actores_mapa.size())+
				"\n\n>>";


	std::vector<T_TIPO_ACTOR *>::iterator 	ini=actores_mapa.begin()+indice_actual_listado,
							fin=actores_mapa.end();


	while(ini<fin && i < MAX_LISTADO)
	{
		funcion_mostrado((*ini), i);
		++i;
		++ini;
	}

	this->rep_txt_listado->asignar(cadena);
	this->rep_txt_listado->volcar(this->pantalla);
}

void Controlador_editor::representar_modo_info()
{
	this->rep_txt_ayuda->volcar(this->pantalla);	
}
