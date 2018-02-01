/*Estos son los fuentes relacionados con redefinir los controles. En un 
principio sería un nuevo estado como "config" en el que saldrían dos bloques: 
todos los controles de teclado y todos los de joystick. 

La pantalla se dividiría en dos columnas: la izquierda para los nombres de 
los campos y la derecha para los valores actuales.

Con los cursores iríamos cambiando y al pulsar "enter" entraríamos en el modo
de aprendizaje.

Al terminar se volverían a guardar.
*/

//Genera el texto de la columna derecha con los valores actuales de cada tipo.

std::string Controlador::actualizar_datos_configuracion_controles()
{
	std::string resultado="\n\n"+		
		this->controles_teclado_raton.generar_cadena_configuracion()+"\n\n\n"+
		this->controles_joystick.generar_cadena_configuracion();

	return resultado;
}

//Loop principal de la sección.

void Controlador::procesar_estado_configuracion_controles(std::vector<DLibV::Representacion *> & v_rep)
{
	bool salir=false;
	bool actualizar_posicion=false;
	bool aprendiendo=false;
	float delta=0;

	unsigned int y_cursor=Y_CURSOR_CONTROLES;
	unsigned int opcion_actual=0;
	unsigned int total_opciones=CC_MAX_CONTROLES;

	std::string columna_izq=
		this->localizador.obtener(Localizador::C_CONFIGURACION_CONTROLES_TITULO_TECLADO_RATON)+"\n\n"+
		this->localizador.obtener(Localizador::C_CONFIGURACION_CONTROLES_RISTRA_OPCIONES)+"\n\n"+
		this->localizador.obtener(Localizador::C_CONFIGURACION_CONTROLES_TITULO_JOYSTICK)+"\n\n"+
		this->localizador.obtener(Localizador::C_CONFIGURACION_CONTROLES_RISTRA_OPCIONES);

	std::string columna_der=actualizar_datos_configuracion_controles();
	std::string texto_asignar=this->localizador.obtener(Localizador::C_CONFIGURACION_CONTROLES_ASIGNANDO);

	//Representaciones...
	DLibV::Recurso_grafico * fuente_peq=DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_2);

	//Cursor.
	DLibV::Primitiva_grafica_caja_estatica rep_cursor(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CURSOR_CONTROLES, y_cursor, W_CURSOR_CONTROLES, H_CURSOR_CONTROLES), 
		DLibV::Gestor_color::color(255, 255,255));
	rep_cursor.establecer_alpha(64);

	DLibV::Representacion_texto_auto_estatica rep_columna_izq=DLibV::Representacion_texto_auto_estatica(fuente_peq, columna_izq);
	rep_columna_izq.establecer_posicion(X_COL_IZQ_CONTROLES, Y_COL_IZQ_CONTROLES);
	rep_columna_izq.mut_interlineado(INTERLINEADO_CONTROLES);

	DLibV::Representacion_texto_auto_estatica rep_columna_der=DLibV::Representacion_texto_auto_estatica(fuente_peq, columna_der);
	rep_columna_der.establecer_posicion(X_COL_DER_CONTROLES, Y_COL_DER_CONTROLES);
	rep_columna_der.mut_interlineado(INTERLINEADO_CONTROLES);

	//Caja de fondo.
	DLibV::Primitiva_grafica_caja_estatica rep_caja_fondo(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA_FONDO_CONTROLES, Y_CAJA_FONDO_CONTROLES, W_CAJA_FONDO_CONTROLES, H_CAJA_FONDO_CONTROLES), 
		DLibV::Gestor_color::color(255, 255,255));
	rep_caja_fondo.establecer_alpha(64);

	//Todas las representaciones aquí, incluyendo las que hemos recibido.
	std::vector<DLibV::Representacion *> v_representaciones;
	v_representaciones.push_back(&rep_caja_fondo);
	v_representaciones.push_back(&rep_cursor);
	v_representaciones.push_back(&rep_columna_izq);
	v_representaciones.push_back(&rep_columna_der);
	for(DLibV::Representacion * r : v_rep) v_representaciones.push_back(r);

	while(!salir)
	{
		delta=this->control_frames.obtener_delta();
		this->campo_estrellas->procesar_turno(delta);

		if(aprendiendo)
		{
			//Aquí cedemos el control del teclado y ratón a otro método.
			aprendiendo=this->aprender_controles(opcion_actual);
			rep_cursor.mut_pixel_color(DLibV::Gestor_color::color(0, 255,128));

			if(!aprendiendo)
			{
				columna_der=actualizar_datos_configuracion_controles();
				rep_columna_der.asignar(columna_der);
				rep_cursor.mut_pixel_color(DLibV::Gestor_color::color(255, 255,255));
			}
		}
		else
		{
			this->controles_sdl.recoger();

			//Controlar la salida.
			if(this->controles_sdl.es_senal_salida() || this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
			{
				salir=true;
			}
			//Controlar los cambios de campo...
			else if(
				this->controles_sdl.es_tecla_down(SDLK_DOWN) 
				|| this->controles_sdl.es_tecla_down(SDLK_UP))
			{
				int factor=this->controles_sdl.es_tecla_down(SDLK_DOWN) ? 1 : -1;
			
				if(factor==-1 && opcion_actual==0) opcion_actual=total_opciones-1;
				else
				{
					opcion_actual+=factor;
					if(opcion_actual==total_opciones) opcion_actual=0;
				}

				actualizar_posicion=true;	
			}
			//Controlar la selección de una opción.
			else if(this->controles_sdl.es_tecla_down(SDLK_RETURN))
			{
				aprendiendo=true;			
			}

			//Actualizar la posición del cursor...

			if(actualizar_posicion)
			{
				y_cursor=Y_CURSOR_CONTROLES+(SALTO_Y_CONTROLES * opcion_actual);		

				//Compensamos por los saltos de línea en la presentación...
				if(opcion_actual > (CC_MAX_CONTROLES/2)-1) y_cursor+=SALTO_Y_BLOQUE_CONTROLES; 

				rep_cursor.establecer_posicion(X_CURSOR_CONTROLES, y_cursor);
				actualizar_posicion=false;
			}
		}
	
		this->mostrar_estados_generico(v_representaciones);
		this->control_frames.turno();
		//Aquí tampoco hacemos el control de perfiles: no hay perfil seleccionado.
	}

	this->controles_teclado_raton.guardar();
	this->controles_joystick.guardar();
}

bool Controlador::aprender_controles(unsigned int opcion_actual)
{
	short int resultado=0;

	switch(opcion_actual)
	{
		case CC_0_PAUSA: 	resultado=this->controles_teclado_raton.aprender_pausa(this->controles_sdl); break;
		case CC_0_ARTEFACTO: 	resultado=this->controles_teclado_raton.aprender_usar_artefacto(this->controles_sdl); break;
		case CC_0_TIEMPO:	resultado=this->controles_teclado_raton.aprender_parar_tiempo(this->controles_sdl); break;
		case CC_0_SIGUIENTE:	resultado=this->controles_teclado_raton.aprender_ciclar_artefacto_siguiente(this->controles_sdl); break;
		case CC_0_ANTERIOR: 	resultado=this->controles_teclado_raton.aprender_ciclar_artefacto_anterior(this->controles_sdl); break;
		case CC_0_ARRIBA:	resultado=this->controles_teclado_raton.aprender_movimiento_arriba(this->controles_sdl); break;
		case CC_0_DERECHA:	resultado=this->controles_teclado_raton.aprender_movimiento_derecha(this->controles_sdl); break;
		case CC_0_ABAJO:	resultado=this->controles_teclado_raton.aprender_movimiento_abajo(this->controles_sdl); break;
		case CC_0_IZQUIERDA:	resultado=this->controles_teclado_raton.aprender_movimiento_izquierda(this->controles_sdl); break;
		case CC_1_PAUSA: 	resultado=this->controles_joystick.aprender_pausa(this->controles_sdl); break;
		case CC_1_ARTEFACTO: 	resultado=this->controles_joystick.aprender_usar_artefacto(this->controles_sdl); break;
		case CC_1_TIEMPO:	resultado=this->controles_joystick.aprender_parar_tiempo(this->controles_sdl); break;
		case CC_1_SIGUIENTE:	resultado=this->controles_joystick.aprender_ciclar_artefacto_siguiente(this->controles_sdl); break;
		case CC_1_ANTERIOR: 	resultado=this->controles_joystick.aprender_ciclar_artefacto_anterior(this->controles_sdl); break;
		case CC_1_ARRIBA:	resultado=this->controles_joystick.aprender_movimiento_arriba(this->controles_sdl); break;
		case CC_1_DERECHA:	resultado=this->controles_joystick.aprender_movimiento_derecha(this->controles_sdl); break;
		case CC_1_ABAJO:	resultado=this->controles_joystick.aprender_movimiento_abajo(this->controles_sdl); break;
		case CC_1_IZQUIERDA:	resultado=this->controles_joystick.aprender_movimiento_izquierda(this->controles_sdl); break;
	}

	//La devolución se usará para volver a llamar a este método o no.
	//Si cancelamos o hay ok, devolveremos "false". En caso contrario, 
	//"true"

	return resultado==Configuracion_controles::C_NADA;
}
