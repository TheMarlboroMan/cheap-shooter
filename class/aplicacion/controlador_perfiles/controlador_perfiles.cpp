#include "controlador_perfiles.h"

const std::string Controlador_perfiles::ARCHIVO_PERFILES="indice.dat";

Controlador_perfiles::Controlador_perfiles()
	:indice_ok(false)
{
	this->comprobar_y_crear_archivo_indice();
	this->leer_indice();
}

Controlador_perfiles::~Controlador_perfiles()
{
	this->vaciar_indice();
}

/*Comprueba la existencia del archivo de índice. Si el archivo no existe se
creará. Si al final del método existe el archivo, devolverá true. Si devuelve
false es que el archivo de índice no estará presente y no podremos usar este
controlador.*/

bool Controlador_perfiles::comprobar_y_crear_archivo_indice()
{
	//Comprobar si existe...

	std::string ruta=Perfil::RUTA_PERFILES+ARCHIVO_PERFILES;
	std::ifstream archivo(ruta.c_str(), std::ios::in);

	if(archivo)
	{
		this->indice_ok=true;
		archivo.close();
	}

	//Si no existe lo creamos.
	else
	{
		std::ofstream nuevo_archivo(ruta.c_str(), std::ios::out);

		if(nuevo_archivo)
		{
			this->indice_ok=true;
			nuevo_archivo.close();
		}
		else
		{
			this->indice_ok=false;
		}
	}

	return this->indice_ok;
}

void Controlador_perfiles::vaciar_indice()
{
	this->indice.clear();	
}

/*Guarda el nombre del archivo de perfil en el archivo de índice. Este archivo
es el que se usa para controlar los perfiles que están presentes en el juego.
En caso de error no hace realmente nada. Tengo claro que esto no debería ser
así pero tampoco voy a ponerme a controlarlo ahora.*/

void Controlador_perfiles::insertar_perfil(Perfil & p_perfil)
{
	if(!this->indice_ok) 
	{
		return;
	}
	else
	{
		std::string ruta=Perfil::RUTA_PERFILES+ARCHIVO_PERFILES;
		std::ofstream fichero(ruta.c_str(), std::ios::out | std::ios::app);

		if(fichero)
		{
			try
			{	
				p_perfil.guardar();
				fichero<<p_perfil.acc_nombre_archivo()<<std::endl;
			}
			catch(Perfil_excepcion p)
			{}

			fichero.close();
		}
	}
}


/*Se lee cada línea del fichero que, a su vez, contiene el nombre
de un fichero con un perfil. Se abre ese fichero, se lee la información
contenida y se crea una entrada de resumen que se añade al vector.
Va guardando en un vector los que se han leido correctamente y si en algún
momento falla alguna lectura, se reconstruye el índice con los que han 
funcionado.*/

void Controlador_perfiles::leer_indice()
{
	if(!this->indice_ok) 
	{
		return;
	}
	else
	{
		std::string ruta=Perfil::RUTA_PERFILES+ARCHIVO_PERFILES;
		std::ifstream fichero(ruta.c_str(), std::ios::in);
		std::string nombre_fichero="";
		char * buffer=new char[1024];
		memset(buffer, '\0', 1024);

		bool sanear_al_finalizar=false;
		std::vector<std::string> leidos_ok;


		while(!fichero.eof())
		{
			fichero.getline(buffer, 1024);

			if(!fichero.eof()) //El EOF se marca sólo al pasar por el final.
			{
				nombre_fichero=buffer;

				//Se lee el perfil. Si la lectura falla levantamos una vez la bandera de sanear al finalizar.
				if(this->leer_perfil_en_indice(nombre_fichero))
				{
					leidos_ok.push_back(nombre_fichero);
				}
				else
				{
					sanear_al_finalizar=true;
				}
			}
		}					

		delete [] buffer;
		fichero.close();

		if(sanear_al_finalizar)
		{
			this->sanear_indice(leidos_ok);
		}
	}
}

/*A partir de un nombre de fichero, abre el perfil relacionado, lo crea y 
genera una estructura para t_vector_datos_perfil. Si la carga falla se lanza
una excepción y no se carga nada en el vector de turno.*/

bool Controlador_perfiles::leer_perfil_en_indice(std::string nombre_fichero)
{
	try
	{
		Perfil p=Perfil::cargar_desde_archivo(nombre_fichero);

		//Lo convertimos a un perfil de datos. Sólo cuando se abra realmente
		//el perfil se usará la clase completa.

		Entrada_indice_perfil_datos datos(nombre_fichero, p.acc_nombre(), p.es_online(), this->indice.size());
		this->indice.push_back(datos);

		return true;

	}
	catch(Perfil_excepcion e)
	{
		return false;
	}
}

std::vector<std::string> Controlador_perfiles::obtener_vector_indice()
{
	std::vector<std::string> resultado;
	for(Entrada_indice_perfil_datos p : indice) resultado.push_back(p.online ? p.nombre+" [*]" : p.nombre);
	return resultado;
}

/*Obtiene el perfil que hay en el índice según el número que se le
pase. Si se busca un perfil erróneo tira excepción. Si la carga falla
tira excepción también.*/

Perfil Controlador_perfiles::obtener_por_indice(unsigned int p_indice) 
	throw (Perfil_excepcion) //const
{
	unsigned int cantidad=this->indice.size();

	if(!cantidad)
	{
		throw Perfil_excepcion(Perfil_excepcion::C_CONTROLADOR_VACIO, "NO HAY PERFILES EN EL CONTROLADOR PARA SELECCIONAR UNO");
	}
	else if(p_indice >= cantidad)
	{
		throw Perfil_excepcion(Perfil_excepcion::C_CONTROLADOR_INDICE_INVALIDO, "EL INDICE DEL CONTROLADOR NO ES CORRECTO PARA SELECCIONAR PERFIL");
	}
	else
	{
		//Esto también lanza una excepción Perfil_excepcion si algo falla.
		Perfil resultado=Perfil::cargar_desde_archivo(this->indice[p_indice].fichero);
		return resultado;
	}
}

/*Comprueba la existencia de un perfil concreto en el índice a partir del nombre
de usuario que correspondería con el mismo. Se usará para asegurarnos de que
no sobreescribimos un nuevo perfil. El proceso sería:

existe_perfil(perfil.nombre_archivo);
insertar_perfil(perfil);
*/

bool Controlador_perfiles::existe_perfil(std::string p_param) const
{
	t_vector_datos_perfil::const_iterator	ini=this->indice.begin(),
						fin=this->indice.end();

	std::string nombre_fichero=Perfil::generar_nombre_fichero_para_nombre(p_param);
	
	bool resultado=false;

	while(ini < fin)
	{
		if( (*ini).fichero==nombre_fichero)
		{
			resultado=true;
			break;
		}

		++ini;
	}

	return resultado;
}

/*Recorre el índice y va eliminando del mismo aquellos perfiles que ya no 
están presentes. Se usa sólo cuando se lee el índice y se detecta algún fallo.
Se asume que el indice está ok.*/

void Controlador_perfiles::sanear_indice(std::vector<std::string> perfiles_ok)
{
	std::string ruta=Perfil::RUTA_PERFILES+ARCHIVO_PERFILES;
	std::ofstream fichero(ruta.c_str(), std::ios::out | std::ios::trunc);
	
	std::vector<std::string>::iterator	ini=perfiles_ok.begin(),
						fin=perfiles_ok.end();
	
	if(fichero)
	{
		while(ini < fin)
		{
			fichero<<(*ini)<<std::endl;			
			++ini;	
		}

		fichero.close();
	}
}

/*Se elimina un archivo de perfil del disco. Se elimina la entrada del vector
pero no se toca el archivo del índice: se recargará sólo la próxima vez que
se lea.*/

bool Controlador_perfiles::eliminar_por_indice(unsigned int p_indice)
{
	try
	{
		Perfil p=obtener_por_indice(p_indice);
		
		if(!p.eliminar())
		{
			return false;
		}
		else
		{
			this->indice.erase(this->indice.begin()+p_indice);
			return true;
		}		
	}
	catch(Perfil_excepcion e)
	{
		return false;
	}
}

/*Este es el hilo de comprobación de perfiles. Se usa en un hilo ya que puede
realizar alguna que otra petición al servidor...*/

int Controlador_perfiles::hilo__comprobar_perfil(void * datos_recibidos)
{
	//Desempaquetamos los datos... primero desempaquetamos el hilo que
	//necesitaremos para "finalizarlo".
	DLibH::Hilo_SDL::Hilo_estructura_datos * estructura_datos=static_cast<DLibH::Hilo_SDL::Hilo_estructura_datos*>(datos_recibidos);
	DLibH::Hilo_SDL * h=estructura_datos->ref_hilo;

	//Ahora podemos desempaquetar la estructura de datos que se ha
	//enviado con el hilo. En este caso es una estructura con tres cadenas,
	//una referencia al controlador de perfiles y un entero.
	Estructura_comprobar_perfil * est=static_cast<Estructura_comprobar_perfil*>(estructura_datos->datos);

	int resultado;
	std::string nombre=est->nombre;
	std::string email=est->email;
	std::string clave=est->clave;

	bool es_online=email.size() || clave.size();
	bool datos_online_completos=email.size() && clave.size();

	//Comprobar que los datos están completos... Es obligatorio el nombre
	//y si lo vamos a hacer online, todo es obligatorio.

	if(!nombre.size() || (es_online && !datos_online_completos))
	{
		resultado=CPERFIL_DATOS_INCOMPLETOS;
	}
	else if(nombre.size() < Perfil::MIN_CARACTERES_NOMBRE)
	{
		resultado=CPERFIL_NOMBRE_INCORRECTO;
	}
	//Comprobar que ya existe el perfil offline.
	else if(est->cp->existe_perfil(nombre))
	{
		resultado=CPERFIL_OFFLINE_YA_EXISTE;
	}
	else
	{
		//Intentar crear perfil offline.
		if(!es_online)
		{
			est->id=0;
			resultado=CPERFIL_OFFLINE_OK;
		}
		//Intentar crear perfil online.
		else
		{
			if(!DLibH::Herramientas::validar_email(email))
			{
				resultado=CPERFIL_ONLINE_EMAIL_INCORRECTO;
			}
			else if(clave.size() < Perfil::MIN_CARACTERES_CLAVE)
			{
				resultado=CPERFIL_ONLINE_CLAVE_INCORRECTA;
			}			
			else
			{
				try
				{
					if(Solicitud_servidor_perfil::existe_perfil(nombre, email))
					{
						unsigned int id=Solicitud_servidor_perfil::obtener_identificador_perfil(nombre, email, clave);

						if(!id)
						{
							resultado=CPERFIL_ONLINE_YA_EXISTE_ERROR_PASS;
						}
						else
						{
							resultado=CPERFIL_ONLINE_YA_EXISTE;				
							est->id=id;
						}
					}
					else
					{
						long int resultado_solicitud=Solicitud_servidor_perfil::crear_perfil(nombre, email, clave);
				
						if(resultado_solicitud > 0) 
						{
							est->id=resultado_solicitud;
							resultado=CPERFIL_ONLINE_OK;
						}
						else
						{
							switch(resultado_solicitud)
							{
								case 0: resultado=CPERFIL_ONLINE_ERROR_CREAR; break;
								case -1: resultado=CPERFIL_ONLINE_SUPERADO_LIMITE_POR_EMAIL; break;
							}			
						}
					}
				}
				catch(Excepcion_solicitud_perfil E)
				{
					h->finalizar();
					resultado=CPERFIL_ONLINE_ERROR;
				}
			}
		}
	}
	
	h->finalizar();

	return est->cp->procesar_resultado_comprobacion_perfil(resultado, *est);
}


/*Este es el método de proceso que se llama cuando finaliza el hilo de crear
un perfil... Se supone que debe crear el perfil y cambiar el estado o bien
sacar el mensaje de error. Si el perfil es online ya se habría creado antes
en el servidor de modo que no habría mucho más que hacer. La devolución se
usa para salir o no del loop de control.

Cuando el control llega aquí todavía estamos en un hilo de modo que es posible
hacer las operaciones online que queramos sin que se resienta el refrescado de 
pantalla.
*/

int Controlador_perfiles::procesar_resultado_comprobacion_perfil(int resultado_hilo, Estructura_comprobar_perfil const& est)
{
	int id=est.id;
	std::string nombre=est.nombre;
	std::string email=est.email;

	switch(resultado_hilo)
	{	
		case CPERFIL_ONLINE_YA_EXISTE:	
		case CPERFIL_OFFLINE_OK:
		case CPERFIL_ONLINE_OK:
		{
			Perfil perfil_resultante=resultado_hilo==CPERFIL_OFFLINE_OK ? 
					Perfil::crear_perfil_desde_datos(nombre, "", id):
					Perfil::crear_perfil_desde_datos(nombre, email, id);

			this->insertar_perfil(perfil_resultante);
			this->asignar_perfil(perfil_resultante);
		}
		break;
	}

	//Si el perfil es online y ya existe vamos a importar los retos que ya
	//tuviera en el servidor.

	if(resultado_hilo==CPERFIL_ONLINE_YA_EXISTE)
	{
		this->cargar_datos_perfil_de_servidor(est.nombre, est.email, est.clave, id);
	}	

	return resultado_hilo;
}

bool Controlador_perfiles::asignar_perfil_por_indice(unsigned int p_indice)
	throw (Perfil_excepcion)
{
	try
	{
		this->asignar_perfil(this->obtener_por_indice(p_indice));
		return true;
	}
	catch(Perfil_excepcion e)
	{
		return false;
	}
}

/*Seleccionar y activar uno de los perfiles disponibles.*/

void Controlador_perfiles::asignar_perfil(Perfil const& p_perfil)
{
	this->perfil=p_perfil;
	this->datos_perfil_juego=this->perfil.obtener_datos_juego();
	this->datos_perfil_juego->agregar_inicio_sesion();

	try
	{
		//Guardamos los datos del perfil para forzar el guardado del nuevo inicio de sesión.
		//No pasamos por "actualizar_datos_juego" porque nos pediría la estructura
		//de datos Estructura_datos_perfil que no tenemos aquí.

		this->perfil.guardar();
		this->activar_crono_perfiles();
	}
	catch(Perfil_excepcion)
	{

	}
}

void Controlador_perfiles::actualizar_perfil_jugador(Estructura_datos_perfil &datos_actualizables)
	throw(Perfil_excepcion)
{
	if(!this->perfil.es_anonimo())
	{
		this->perfil.actualizar_datos_juego(datos_actualizables);
		datos_actualizables.reiniciar();
	}
}

/* Esto es para actualizar aunque no haya nuevos datos. Una estructura sin
datos no afectará al perfil.*/

void Controlador_perfiles::actualizar_perfil_jugador()
	throw(Perfil_excepcion)
{
	Estructura_datos_perfil t;
	this->actualizar_perfil_jugador(t);
}

/*Guardar la información de un sólo nivel concreto.*/

void Controlador_perfiles::actualizar_informacion_nivel(Estructura_datos_perfil_nivel& p_datos, unsigned int p_nivel, bool p_finalizado)
{
	try
	{
		perfil.obtener_datos_juego()->obtener_datos_nivel(p_nivel).procesar_datos_fin_nivel(p_datos, p_finalizado);
		perfil.guardar();
	}
	catch(Perfil_datos::Info_nivel_excepcion e){}
	catch(Perfil_excepcion e){}
}


void Controlador_perfiles::preparar_estructura_para_login(Controlador_perfiles * cp, Estructura_comprobar_perfil &estructura_hilo_perfil, std::string const& clave)
{
	estructura_hilo_perfil.preparar(cp, cp->perfil.acc_nombre(), cp->perfil.acc_email(), clave, cp->perfil.acc_id_bd());
}

/*Este método realiza una petición curl y guarda los datos del perfil en el
servidor. Antes se encargará de realizar la comprobación del login.*/

int Controlador_perfiles::guardar_datos_perfil_en_servidor()
{
	//Validación.

	if(!this->es_perfil_online()) 
	{
		return 0;
	}

	unsigned long int id=this->obtener_id_perfil();
	std::string login("");
	std::string email("");
	std::string pass("");

	try
	{
		login=this->obtener_nombre_perfil();
		email=this->obtener_email_perfil();
		pass=this->obtener_clave_perfil();

		if(!Solicitud_servidor_perfil::validar_perfil(login, email, pass, id))
		{
			return 0;
		}
	}
	catch(Excepcion_solicitud_perfil E)
	{
		return 0;
	}

	try
	{
		//Si hemos llegado hasta aquí es que hemos validado los datos y podemos
		//guardar. Lo que guardaremos realmente será una estructura muy sencilla
		//indicando los trofeos que hemos conseguido y los que no. Esto implica
		//que los datos de perfil deben tener conocimiento de esta estructura.

		Estructura_intercambio_retos est=this->datos_perfil_juego->obtener_estructura_retos_rellena();
		return Solicitud_servidor_perfil::guardar_datos_retos(login, email, pass, id, est);
	}
	catch(Excepcion_solicitud_perfil E)
	{
		return 0;
	}
}

/*Obtiene los datos de los retos del servidor y los carga en el perfil. Al
igual que en el método anterior, pasamos antes por un tema de validación. Todo
este proceso se produce en un hilo así que podemos hacer peticiones al 
exterior.*/

int Controlador_perfiles::cargar_datos_perfil_de_servidor(
	const std::string & login, const std::string & email, 
	const std::string & pass, unsigned long int id)
{
	//Validación.

	if(!this->es_perfil_online()) 
	{
		return 0;
	}

	try
	{
		if(!Solicitud_servidor_perfil::validar_perfil(login, email, pass, id))
		{
			return 0;
		}
	}
	catch(Excepcion_solicitud_perfil E)
	{
		return 0;
	}

	try
	{
		//Si hemos llegado hasta aquí es que hemos validado los datos y podemos
		//cargar. Mandaremos la estructura de intercambio y luego se la
		//pasaremos a nuestros datos_perfil_juego.

		Estructura_intercambio_retos est=Solicitud_servidor_perfil::obtener_datos_retos(login, email, pass, id);

		if(!est.carga_correcta)
		{
			return 0;
		}
		else
		{
			//Aplicar la estructura al perfil de juego...
			this->datos_perfil_juego->rellenar_datos_retos_desde_servidor(est);

			//Guardamos los datos.
			this->perfil.guardar();

			return 1;
		}
	}
	catch(Excepcion_solicitud_perfil E)
	{
		return 0;
	}
}

/*Método para el hilo de comprobar el perfil...*/

int Controlador_perfiles::hilo__login_perfil_online(void * datos_recibidos)
{
	//Desempaquetamos los datos... primero desempaquetamos el hilo que necesitaremos para "finalizarlo".
	DLibH::Hilo_SDL::Hilo_estructura_datos * estructura_datos=static_cast<DLibH::Hilo_SDL::Hilo_estructura_datos*>(datos_recibidos);
	DLibH::Hilo_SDL * h=estructura_datos->ref_hilo;

	//Ahora podemos desempaquetar la estructura de datos que se ha enviado con el hilo.
	Estructura_comprobar_perfil * est=static_cast<Estructura_comprobar_perfil*>(estructura_datos->datos);

	int resultado=0;

	unsigned long int id=(unsigned long int)est->id;
	std::string nombre=est->nombre;
	std::string email=est->email;
	std::string clave=est->clave;

	try
	{
		if(!Solicitud_servidor_perfil::existe_perfil(nombre, email))
		{
			resultado=CLOGIN_NO_EXISTE;
		}
		else if(!Solicitud_servidor_perfil::login_perfil(nombre, email, clave, id))
		{
			resultado=CLOGIN_NO_VALIDO;
		}
		else
		{
			resultado=id;
		}
	}
	catch(Excepcion_solicitud_perfil E)
	{
		resultado=CLOGIN_ERROR;
		h->finalizar();
	}	

	h->finalizar();
	return resultado;	
}

void Controlador_perfiles::procesar_crono_perfiles(Estructura_datos_perfil &datos, Controlador_retos &p_c_retos)
{
	if(this->crono_perfil.es_pasado_limite())
	{
		if(this->perfil.es_anonimo()) return;
		else try
		{
			this->crono_perfil.reiniciar();

			//Actualizamos el perfil con los datos recibidos.
			this->actualizar_perfil_jugador(datos);

			//Ahora vamos a comprobar los retos con los nuevos datos.
			this->comprobar_retos(p_c_retos);		
		}
		catch (Perfil_excepcion e)
		{

		}
	}
}

void Controlador_perfiles::comprobar_retos(Controlador_retos & p_c_retos)
{
	if(p_c_retos.comprobar_retos(*this->datos_perfil_juego))
	{
		this->actualizar_perfil_jugador();
	}
}

void Controlador_perfiles::disparar_reto(unsigned int p_indice, Controlador_retos &c_retos)
{
	if(c_retos.disparar_reto(p_indice, *this->datos_perfil_juego))
	{
		this->actualizar_perfil_jugador();
	}
}
