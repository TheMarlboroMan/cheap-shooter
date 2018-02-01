/*
Gestión completa del sistema de perfiles.
*/


/*Este método controla la selección de perfil o, en su defecto, la selección
de crear uno nuevo o iniciar la partida sin un perfil. Las tres opciones estarán
dispuestas en vertical y el cambio de perfil se hará en horizontal.

Desde este punto sería posible también borrar el perfil seleccionado.
*/

bool Controlador::procesar_estado_seleccion_perfil()
{
	bool resultado=false;
	bool loop=true;
	bool solicitando_confirmacion_borrar=false;
	float delta=0;
	int opcion_actual=0;
	int max_opciones=3;
	int total_perfiles=0;
	int perfil_actual=0;

	//Preparar los elementos gráficos: son el texto de inicio, la caja
	//de fondo, la flecha de selección, las flechas de scroll y las opciones.
	//Esto va a ser un buen ladrillo de código.

	DLibV::Recurso_grafico * fuente=DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1);

	DLibV::Primitiva_grafica_caja_estatica rep_caja_fondo(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA_FONDO_PERFILES, Y_CAJA_FONDO_PERFILES, W_CAJA_FONDO_PERFILES, H_CAJA_FONDO_PERFILES), 
		DLibV::Gestor_color::color(255, 255,255));
	rep_caja_fondo.establecer_alpha(64);

	int x_flecha_izquierda=X_CAJA_PERFILES-MARGEN_X_FLECHA_PERFILES-13;
	int x_flecha_derecha=X_CAJA_PERFILES+W_CAJA_PERFILES+MARGEN_X_FLECHA_PERFILES;

	DLibV::Representacion_bitmap_estatica rep_flecha_izq(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_MENU));
	rep_flecha_izq.establecer_recorte(13,0,13,25);
	rep_flecha_izq.establecer_posicion(x_flecha_izquierda, Y_FLECHA_PERFILES);

	DLibV::Representacion_bitmap_estatica rep_flecha_der(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_MENU));
	rep_flecha_der.establecer_recorte(0,0,13,25);
	rep_flecha_der.establecer_posicion(x_flecha_derecha, Y_FLECHA_PERFILES);

	DLibV::Primitiva_grafica_caja_estatica rep_cursor(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CURSOR_PERFILES, Y_CURSOR_PERFILES, W_CURSOR_PERFILES, H_CURSOR_PERFILES), 
		DLibV::Gestor_color::color(255, 255,255));
	rep_cursor.establecer_alpha(64);

	DLibV::Representacion_texto_auto_estatica rep_txt_info=DLibV::Representacion_texto_auto_estatica(fuente, localizador.obtener(Localizador::C_PERFILES_INFO_PERFIL));
	rep_txt_info.establecer_posicion(X_TXT_INFO_PERFILES, Y_TXT_INFO_PERFILES);

	DLibV::Representacion_texto_auto_estatica rep_txt_nuevo_perfil=DLibV::Representacion_texto_auto_estatica(fuente, localizador.obtener(Localizador::C_MENU_PERFIL_NUEVO_PERFIL));
	rep_txt_nuevo_perfil.establecer_posicion(obtener_centro_texto_perfiles(&rep_txt_nuevo_perfil), Y_OPCION_NUEVO_PERFIL);

	DLibV::Representacion_texto_auto_estatica rep_txt_iniciar_sin_perfil=DLibV::Representacion_texto_auto_estatica(fuente, localizador.obtener(Localizador::C_MENU_PERFIL_CONTINUAR));
	rep_txt_iniciar_sin_perfil.establecer_posicion(obtener_centro_texto_perfiles(&rep_txt_iniciar_sin_perfil), Y_OPCION_INICIAR_SIN_PERFIL);

	std::vector<DLibV::Representacion *> vector_rep;
	vector_rep.push_back(&rep_caja_fondo);
	vector_rep.push_back(&rep_flecha_der);
	vector_rep.push_back(&rep_flecha_izq);
	vector_rep.push_back(&rep_cursor);
	vector_rep.push_back(&rep_txt_info);
	vector_rep.push_back(&rep_txt_nuevo_perfil);
	vector_rep.push_back(&rep_txt_iniciar_sin_perfil);

	//Obtener ahora las representaciones de los perfiles.
	std::vector<DLibV::Representacion_texto *> vector_per;
	total_perfiles=obtener_representacion_perfiles(vector_per);

	//Posicionar la primera de las representaciones de perfil indicada por representacion: quedará ya en la posición que debe.	
	(*vector_per.begin())->establecer_posicion(obtener_centro_texto_perfiles(*vector_per.begin()), Y_PERFILES);

	Grupo_animacion * animacion=NULL;

	//Inicio del loop de turno.
	while(loop)
	{
		this->controles_sdl.recoger();

		if(animacion && animacion->es_finalizado())
		{
			delete animacion;
			animacion=NULL;
		}

		//TODO: El menú también debe manejarse con joystick.

		//Salir
		if(this->controles_sdl.es_senal_salida() || this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
		{
			if(solicitando_confirmacion_borrar)
			{
				solicitando_confirmacion_borrar=false;
				rep_cursor.mut_pixel_color(DLibV::Gestor_color::color(255, 255,255));
			}
			else
			{
				resultado=true;
				loop=false;
				break;
			}
		}
		else if(this->controles_sdl.es_tecla_down(SDLK_DOWN) || this->controles_sdl.es_tecla_down(SDLK_UP))
		{
			if(solicitando_confirmacion_borrar) continue;

			if(this->controles_sdl.es_tecla_down(SDLK_DOWN)) ++opcion_actual;
			else --opcion_actual;

			if(opcion_actual < 0) opcion_actual=max_opciones-1;
			else if(opcion_actual >= max_opciones) opcion_actual=0;		

			rep_cursor.establecer_posicion(X_CURSOR_PERFILES, Y_CURSOR_PERFILES+(opcion_actual*SALTO_Y_CURSOR_PERFILES));
		}
		else if(this->controles_sdl.es_tecla_down(SDLK_LEFT) || this->controles_sdl.es_tecla_down(SDLK_RIGHT))
		{
			if(solicitando_confirmacion_borrar || total_perfiles < 2 || opcion_actual || animacion) continue;
			else
			{
				int perfil_original=perfil_actual;
				int direccion=this->controles_sdl.es_tecla_down(SDLK_LEFT) ? -1 : 1;

				if(direccion==-1) --perfil_actual;
				else ++perfil_actual;
			
				if(perfil_actual < 0) perfil_actual=total_perfiles-1;
				else if(perfil_actual >= total_perfiles) perfil_actual=0;
				
				//Animación...

				int destino_nuevo=obtener_centro_texto_perfiles(vector_per[perfil_actual]);
				int vector=direccion==1 ? -1*VECTOR_X_PERFIL : VECTOR_X_PERFIL;

				//El original debe llegar justo hasta el borde izquierdo o derecho.
				unsigned int destino_original=direccion==1
					? X_CAJA_PERFILES-vector_per[perfil_original]->acc_ancho_area() 
					: X_CAJA_PERFILES+W_CAJA_PERFILES;

				//El nuevo debe salir justo del borde izquierdo o derecho.
				unsigned int origen_nuevo=direccion==1 
					? X_CAJA_PERFILES+W_CAJA_PERFILES 
					: X_CAJA_PERFILES-vector_per[perfil_actual]->acc_ancho_area();

				vector_per[perfil_actual]->establecer_posicion(origen_nuevo, Y_PERFILES);
				animacion=new Grupo_animacion();
				animacion->asignar_animacion(new Estructura_animacion_movimiento(vector_per[perfil_original], DLibH::Vector_2d(vector, 0), destino_original, Y_PERFILES));
				animacion->asignar_animacion(new Estructura_animacion_movimiento(vector_per[perfil_actual], DLibH::Vector_2d(vector, 0), destino_nuevo, Y_PERFILES));	
			}
		}
		else if(this->controles_sdl.es_tecla_down(SDLK_DELETE))
		{
			//Eliminar un perfil...
			if(!total_perfiles || opcion_actual!=0) continue;

			if(!solicitando_confirmacion_borrar)
			{
				solicitando_confirmacion_borrar=true;
				rep_txt_info.asignar(this->localizador.obtener(Localizador::C_PERFILES_CONFIRMACION_BORRAR));
				rep_cursor.mut_pixel_color(DLibV::Gestor_color::color(255, 128, 0));
			}
			else
			{
				solicitando_confirmacion_borrar=false;
				rep_cursor.mut_pixel_color(DLibV::Gestor_color::color(255, 255, 255));

				if(!this->controlador_perfiles.eliminar_por_indice(perfil_actual))
				{
					rep_txt_info.asignar(this->localizador.obtener(Localizador::C_PERFILES_ERROR_BORRAR));			
				}
				else
				{
					//Reconstruimos las opciones y mostramos de nuevo el texto original.
					total_perfiles=obtener_representacion_perfiles(vector_per);
					perfil_actual=0;
					//Posicionar la primera...
					(*vector_per.begin())->establecer_posicion(obtener_centro_texto_perfiles(*vector_per.begin()), Y_PERFILES);
				}
			}

		}
		else if(this->controles_sdl.es_tecla_down(SDLK_RETURN))
		{
			switch(opcion_actual)
			{
				case 0:
					if(!total_perfiles || animacion) continue;
					else
					{
						//Asignamos el perfil. Si es online comprobamos que existe y también la clave la clave.
						if(this->controlador_perfiles.asignar_perfil_por_indice(perfil_actual))
						{
							loop=false;
							estado=controlador_perfiles.es_perfil_online() ? E_VALIDAR_LOGIN : E_MENU;
						}
						else rep_txt_info.asignar(this->localizador.obtener(Localizador::C_PERFILES_INFO_ERROR_CARGA));
					}
				break;

				case 1:
					this->estado=E_CREACION_PERFIL;
					loop=false;
				break;

				case 2:
					this->controlador_perfiles.asignar_perfil_anonimo();
					this->estado=E_MENU;					
					loop=false;
				break;
			}
		}
		
		delta=this->control_frames.obtener_delta();
		if(animacion) animacion->turno(delta);
		this->campo_estrellas->procesar_turno(delta);
		this->mostrar_estado_perfiles(vector_rep, vector_per);
		this->control_frames.turno();
	}

	//Si la animación sigue en marcha, destruirla para no dejar la memoria por ahí.
	if(animacion)
	{
		animacion->forzar_finalizacion();
		delete animacion;
	}

	return resultado;
}

void Controlador::mostrar_estado_perfiles(std::vector<DLibV::Representacion* >&rep, std::vector<DLibV::Representacion_texto* >&per)
{
	this->campo_estrellas->acc_representacion()->volcar(pantalla);
	for(DLibV::Representacion * r : rep) r->volcar(pantalla);
	pantalla.establecer_clip(DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA_PERFILES, Y_CAJA_PERFILES, W_CAJA_PERFILES, H_CAJA_PERFILES));
	for(DLibV::Representacion * r : per) r->volcar(pantalla);

	pantalla.reiniciar_clip_completo();
	pantalla.flipar();
}

int Controlador::obtener_centro_texto_perfiles(DLibV::Representacion_texto * rep)
{
	int ancho=rep->acc_ancho_area();
	int x=(Medidas::ANCHO_VISTA - ancho) / 2;
	return x;
}

int Controlador::obtener_representacion_perfiles(std::vector<DLibV::Representacion_texto *> &vec)
{
	//Vaciar todo y liberar memoria.
	for(DLibV::Representacion * r : vec) delete r;
	vec.clear();

	int resultado=controlador_perfiles.obtener_total_perfiles();

	//Insertar los perfiles o el texto de "no hay perfiles".
	if(!resultado)
	{
		vec.push_back(new DLibV::Representacion_texto_auto_estatica(
			DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_5), 
			localizador.obtener(Localizador::C_MENU_PERFIL_SIN_PERFIL) ) ) ;
	}
	else
	{
		std::vector<std::string> cadenas=controlador_perfiles.obtener_vector_indice();
		for(std::string c : cadenas) vec.push_back(new DLibV::Representacion_texto_auto_estatica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1), c));
	}

	for(DLibV::Representacion * r : vec) r->establecer_posicion(-800, Y_PERFILES);

	//Devolver la cantidad de perfiles disponibles.
	return resultado;
}

/*Procesa el estado de creación del perfil, que consta de introducción de datos,
comprobación de los mismos y mostrado del resultado en caso de que haya algún
error. Si el proceso de creación de perfil es correcto, intentaremos cargarlo
como perfil en uso.*/

void Controlador::procesar_estado_creacion_perfil()
{
	//Variables de control...

	unsigned int opcion_actual=0, opcion_anterior=0;
	unsigned int total_opciones_disponibles=3;
	bool loop=true;
	bool actualizar_posicion=false;
	bool actualizar_cadenas=true;
	float delta=0.0f;
	const unsigned int origen_y_cursor=268;
	unsigned int y_cursor=origen_y_cursor;

	//Cadenas y escritor...
	DLibI::Escritor_texto e(this->controles_sdl);

	std::string nombre="";
	std::string email="";
	std::string clave="";

	e.mut_cadena(nombre); 
	e.establecer_limite_longitud(Perfil::MAX_CARACTERES_NOMBRE);

	//Representaciones de texto y gráficas.
	DLibV::Recurso_grafico * fuente=DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1);
	DLibV::Recurso_grafico * fuente_error=DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_5);
		
	DLibV::Representacion_texto_auto_estatica txt_info=DLibV::Representacion_texto_auto_estatica(fuente, this->localizador.obtener(Localizador::C_CREAR_PERFIL_INFO));
	txt_info.establecer_posicion(100, 60); 

	DLibV::Representacion_texto_auto_estatica txt_datos=DLibV::Representacion_texto_auto_estatica(fuente, "");
	txt_datos.establecer_posicion(100, 270);
	txt_datos.mut_interlineado(12);

	DLibV::Representacion_texto_auto_estatica txt_error=DLibV::Representacion_texto_auto_estatica(fuente_error, "");
	txt_error.establecer_posicion(100, 460);

	DLibV::Primitiva_grafica_caja_estatica rep_cursor(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0, y_cursor, 800, 20), 
		DLibV::Gestor_color::color(255, 255,255));
	rep_cursor.establecer_alpha(64);

	DLibV::Primitiva_grafica_caja_estatica rep_caja_fondo(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 256, 800, 120), 
		DLibV::Gestor_color::color(255, 255,255));
	rep_caja_fondo.establecer_alpha(64);

	std::vector<DLibV::Representacion *> v_representaciones;
	v_representaciones.push_back(&rep_caja_fondo);
	v_representaciones.push_back(&txt_info);
	v_representaciones.push_back(&txt_datos);
	v_representaciones.push_back(&txt_error);
	v_representaciones.push_back(&rep_cursor);

	//Un hilo para las comprobaciones de crear perfil que se hagan...
	DLibH::Hilo_SDL * hilo_perfil=NULL;

	//La estructura que acompañará al hilo...

	Controlador_perfiles::Estructura_comprobar_perfil estructura_hilo_perfil;

	//Iniciamos el loop.

	while(loop)
	{
		if(hilo_perfil && hilo_perfil->es_finalizado())
		{

			//La carga del hilo ha finalizado y podemos procesar los resultados...	
			//Si el resultado tiene éxito saldremos inmediatamente
			//del loop.

			int resultado_hilo=hilo_perfil->completar();
			DLibH::Hilo_SDL::destruir_hilo(hilo_perfil);

			switch(resultado_hilo)
			{
				case Controlador_perfiles::CPERFIL_DATOS_INCOMPLETOS: txt_error.asignar(this->localizador.obtener(Localizador::C_CREAR_PERFIL_DATOS_INCOMPLETOS)); break;
				case Controlador_perfiles::CPERFIL_OFFLINE_YA_EXISTE: txt_error.asignar(this->localizador.obtener(Localizador::C_CREAR_PERFIL_OFFLINE_EXISTE)); break;
				case Controlador_perfiles::CPERFIL_ONLINE_ERROR:	txt_error.asignar(this->localizador.obtener(Localizador::C_CREAR_PERFIL_ERROR_SERVER)); break;
				case Controlador_perfiles::CPERFIL_ONLINE_DATOS_INCORRECTOS: txt_error.asignar(this->localizador.obtener(Localizador::C_CREAR_PERFIL_ONLINE_DATOS_INCORRECTOS)); break;
				case Controlador_perfiles::CPERFIL_ONLINE_YA_EXISTE_ERROR_PASS: txt_error.asignar(this->localizador.obtener(Localizador::C_IMPORTAR_PERFIL_ERROR_PASS)); break;
				case Controlador_perfiles::CPERFIL_NOMBRE_INCORRECTO:	txt_error.asignar(this->localizador.obtener(Localizador::C_CREAR_PERFIL_NOMBRE_INCORRECTO)); break;
				case Controlador_perfiles::CPERFIL_ONLINE_EMAIL_INCORRECTO: txt_error.asignar(this->localizador.obtener(Localizador::C_CREAR_PERFIL_ONLINE_EMAIL_INCORRECTO)); break;
				case Controlador_perfiles::CPERFIL_ONLINE_CLAVE_INCORRECTA: txt_error.asignar(this->localizador.obtener(Localizador::C_CREAR_PERFIL_ONLINE_CLAVE_INCORRECTA)); break;
				case Controlador_perfiles::CPERFIL_ONLINE_ERROR_CREAR: txt_error.asignar(this->localizador.obtener(Localizador::C_CREAR_PERFIL_ONLINE_ERROR_CREAR)); break;
				case Controlador_perfiles::CPERFIL_ONLINE_SUPERADO_LIMITE_POR_EMAIL: txt_error.asignar(this->localizador.obtener(Localizador::C_CREAR_PERFIL_ONLINE_SUPERADO_LIMITE)); break;
				case Controlador_perfiles::CPERFIL_ONLINE_YA_EXISTE:	
				case Controlador_perfiles::CPERFIL_OFFLINE_OK:
				case Controlador_perfiles::CPERFIL_ONLINE_OK:
					loop=false;
					this->estado=E_MENU;
				break;
			}
		}

		delta=this->control_frames.obtener_delta();
		this->campo_estrellas->procesar_turno(delta);

		e.loop_control(); //Esto bombea los eventos para el escritor y además carga controles_sdl.

		//Controlar la salida.
		if(this->controles_sdl.es_senal_salida() || this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
		{
			this->estado=E_SELECCION_PERFIL;
			break;
		}
		//Controlar los cambios de campo...
		else if(
			this->controles_sdl.es_tecla_down(SDLK_TAB)
			|| this->controles_sdl.es_tecla_down(SDLK_DOWN) 
			|| this->controles_sdl.es_tecla_down(SDLK_UP))
		{
			opcion_anterior=opcion_actual;

			if(this->controles_sdl.es_tecla_down(SDLK_UP)) opcion_actual--;
			else opcion_actual++;

			if(opcion_actual < 0) opcion_actual=total_opciones_disponibles-1;
			else if(opcion_actual >= total_opciones_disponibles) opcion_actual=0;

			actualizar_posicion=true;
		}
		//Controlar la selección de una opción.
		else if(this->controles_sdl.es_tecla_down(SDLK_RETURN))
		{
			if(!hilo_perfil)
			{
				hilo_perfil=new DLibH::Hilo_SDL();

				estructura_hilo_perfil.preparar(&this->controlador_perfiles, nombre, email, clave);
				if(!hilo_perfil->iniciar(Controlador_perfiles::hilo__comprobar_perfil, &estructura_hilo_perfil))
				{
					DLibH::Hilo_SDL::destruir_hilo(hilo_perfil);
				}
			}
		}

		if(e.es_modificada_cadena()) actualizar_cadenas=true;

		//Actualizar la posición del cursor implica no sólo moverlo,
		//sino asignar las cadenas de turno correctamente.

		if(actualizar_posicion)
		{
			actualizar_cadenas=true;
			procesar_estado_creacion_perfil_aux_actualizar_posicion(e, nombre, email, clave, opcion_anterior, opcion_actual);
			y_cursor=origen_y_cursor+(32 * opcion_actual);		
			rep_cursor.establecer_posicion(0, y_cursor);
			actualizar_posicion=false;
		}

		//Sólo llamaremos a este método cuando la cadena haya cambiado,
		//para ahorrarnsos composidciones de la clase de Representación_texto.

		if(actualizar_cadenas)
		{
			std::string cadena_datos=procesar_estado_creacion_perfil_aux_cadena_datos(e, nombre, email, clave, opcion_actual);
			txt_datos.asignar(cadena_datos);
			actualizar_cadenas=false;
		}

		this->mostrar_estados_generico(v_representaciones);
		this->control_frames.turno();
		//Aquí tampoco hacemos el control de perfiles: no hay perfil seleccionado.
	}

	//Nunca dejamos el hilo abierto...
	if(hilo_perfil) DLibH::Hilo_SDL::finalizar_y_destruir_hilo(hilo_perfil);
}

/*Esto es un método auxiliar de procesar_estado_creacion_perfil para quitar
bulto. Se encarga de realizar las acciones que se suceden al cambiar una 
opción al crear un perfil.*/

void Controlador::procesar_estado_creacion_perfil_aux_actualizar_posicion(DLibI::Escritor_texto &e, std::string &nombre, std::string &email, std::string &clave, int opcion_anterior, int opcion_actual)
{
	switch(opcion_anterior)
	{
		case 0: nombre=e.acc_cadena(); break;
		case 1: email=e.acc_cadena(); break;
		case 2: clave=e.acc_cadena(); break;
	}

	switch(opcion_actual)
	{
		case 0: 
			e.mut_cadena(nombre); 
			e.establecer_limite_longitud(Perfil::MAX_CARACTERES_NOMBRE);
		break;
		case 1: 
			e.mut_cadena(email); 
			e.establecer_limite_longitud(Perfil::MAX_CARACTERES_EMAIL);
		break;
		case 2: 
			e.mut_cadena(clave); 
			e.establecer_limite_longitud(Perfil::MAX_CARACTERES_CLAVE);
		break;
	}
}

/*Esto es un método auxiliar de procesar_estado_creacion_perfil para quitar 
bulto. Se encarga de recomponer la cadena que representaría al formulario de
datos al crear un perfil.*/

std::string Controlador::procesar_estado_creacion_perfil_aux_cadena_datos(DLibI::Escritor_texto &e, std::string &nombre, std::string &email, std::string &clave, int opcion_actual)
{
	std::string cadena_datos;

	switch(opcion_actual)
	{
		case 0: nombre=e.acc_cadena(); break;
		case 1: email=e.acc_cadena(); break;
		case 2: clave=e.acc_cadena(); break;
	}

	cadena_datos=this->localizador.obtener(Localizador::C_CREAR_PERFIL_CAMPO_USUARIO);
	cadena_datos+=nombre;
	cadena_datos+="\n";

	cadena_datos+=this->localizador.obtener(Localizador::C_CREAR_PERFIL_CAMPO_EMAIL);
	cadena_datos+=email;
	cadena_datos+="\n";

	cadena_datos+=this->localizador.obtener(Localizador::C_CREAR_PERFIL_CAMPO_CLAVE);
	cadena_datos+=clave;

	return cadena_datos;
}

/*Este estado se da sólo tras seleccionar un perfil online. Te muestra en
pantalla los datos de tu cuenta y te pide la contraseña. Si no consigues 
dar con la contraseña vuelve al estado de selección. En caso contrario
pasa al juego-*/

void Controlador::procesar_estado_login_perfil()
{
	//Variables de control...

	bool loop=true;
	float delta=0.0f;

	//Cadenas y escritor...
	DLibI::Escritor_texto e(this->controles_sdl);

	std::string clave="";

	e.mut_cadena(clave); 
	e.establecer_limite_longitud(Perfil::MAX_CARACTERES_CLAVE);

	//Representaciones de texto y gráficas.
	DLibV::Recurso_grafico * fuente=DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_1);
	DLibV::Recurso_grafico * fuente_error=DLibV::Gestor_recursos_graficos::obtener(Recursos::G_FUENTE_5);
		
	DLibV::Representacion_texto_auto_estatica txt_info=DLibV::Representacion_texto_auto_estatica(fuente, this->localizador.obtener(Localizador::C_LOGIN_INFO));
	txt_info.establecer_posicion(X_TXT_INFO_PERFILES, Y_TXT_INFO_PERFILES);

	DLibV::Representacion_texto_auto_estatica txt_datos=DLibV::Representacion_texto_auto_estatica(fuente, "");
	txt_datos.establecer_posicion(X_TXT_PASS_PERFILES, Y_TXT_PASS_PERFILES);

	DLibV::Representacion_texto_auto_estatica txt_error=DLibV::Representacion_texto_auto_estatica(fuente_error, "");
	txt_error.establecer_posicion(100, 460);

	DLibV::Primitiva_grafica_caja_estatica rep_caja_fondo(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(X_CAJA_FONDO_PERFILES, Y_CAJA_FONDO_PERFILES, W_CAJA_FONDO_PERFILES, H_CAJA_FONDO_PERFILES), 
		DLibV::Gestor_color::color(255, 255,255));
	rep_caja_fondo.establecer_alpha(64);

	std::vector<DLibV::Representacion *> v_representaciones;
	v_representaciones.push_back(&rep_caja_fondo);
	v_representaciones.push_back(&txt_info);
	v_representaciones.push_back(&txt_datos);
	v_representaciones.push_back(&txt_error);

	//Un hilo para las comprobaciones de crear perfil que se hagan...
	DLibH::Hilo_SDL * hilo_perfil=NULL;

	//La estructura que acompañará al hilo...

	Controlador_perfiles::Estructura_comprobar_perfil estructura_hilo_perfil;

	//Iniciamos el loop.

	while(loop)
	{
		if(hilo_perfil && hilo_perfil->es_finalizado())
		{
			int resultado_hilo=hilo_perfil->completar();
			DLibH::Hilo_SDL::destruir_hilo(hilo_perfil);

			//La carga del hilo ha finalizado y podemos procesar los resultados...	
			//Si el resultado tiene éxito saldremos inmediatamente
			//del loop.

			switch(resultado_hilo)
			{
				case Controlador_perfiles::CLOGIN_ERROR: txt_error.asignar(this->localizador.obtener(Localizador::C_LOGIN_ERROR)); break;
				case Controlador_perfiles::CLOGIN_NO_VALIDO: txt_error.asignar(this->localizador.obtener(Localizador::C_LOGIN_NO_VALIDO)); break;
				case Controlador_perfiles::CLOGIN_NO_EXISTE: txt_error.asignar(this->localizador.obtener(Localizador::C_LOGIN_NO_EXISTE)); break;

				default:
					this->controlador_perfiles.asignar_clave_perfil(clave);

					this->estado=E_MENU;
					loop=false;
				break;
			}		
		}

		//Esto lo ponemos para ignorar el resto del cuerpo si el login ha tenido éxito.
		if(!loop) break;

		delta=this->control_frames.obtener_delta();
		this->campo_estrellas->procesar_turno(delta);

		//Mientras estemos solicitando al servidor no podemos volver a tocar.
		if(!hilo_perfil)
		{
			e.loop_control(); //Esto bombea los eventos para el escritor y además carga controles_sdl.
		

			//Controlar la salida.
			if(this->controles_sdl.es_senal_salida() || this->controles_sdl.es_tecla_down(SDLK_ESCAPE))
			{
				this->estado=E_SELECCION_PERFIL;
				break;
			}
			//Controlar la introducción de la clave.
			else if(this->controles_sdl.es_tecla_down(SDLK_RETURN))
			{
				if(!hilo_perfil)
				{
					clave=e.acc_cadena();
					hilo_perfil=new DLibH::Hilo_SDL();

					Controlador_perfiles::preparar_estructura_para_login(&this->controlador_perfiles, estructura_hilo_perfil, clave);

					if(!hilo_perfil->iniciar(Controlador_perfiles::hilo__login_perfil_online, &estructura_hilo_perfil))
					{
						DLibH::Hilo_SDL::destruir_hilo(hilo_perfil);
					}
				}
			}
		}

		if(e.es_modificada_cadena()) txt_datos.asignar(e.acc_cadena());
		this->mostrar_estados_generico(v_representaciones);
		this->control_frames.turno();
		//Y aquí, una vez más, no hacemos turno de controlador de perfiles: no hay perfil que valga.
	}

	//Nunca dejamos el hilo abierto...
	if(hilo_perfil) DLibH::Hilo_SDL::finalizar_y_destruir_hilo(hilo_perfil);
}

