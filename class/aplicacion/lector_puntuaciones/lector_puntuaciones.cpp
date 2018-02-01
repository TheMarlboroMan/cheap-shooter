#include "lector_puntuaciones.h"

const char Lector_puntuaciones::URL_SOLICITAR_POSICION_POR_PUNTUACION[]="http://www.caballorenoir.net/servicios/stupiproject.php?modo=posicion&valor=";
const char Lector_puntuaciones::URL_SOLICITAR_PUNTUACIONES[]="http://www.caballorenoir.net/servicios/stupiproject.php?modo=obtener&indice=";
const char Lector_puntuaciones::URL_INGRESAR_PUNTUACION[]="http://www.caballorenoir.net/servicios/stupiproject.php?modo=ingresar";
const char Lector_puntuaciones::SEPARADOR_CAMPO_SOLICITAR_PUNTUACIONES='|';
const char Lector_puntuaciones::RUTA_PUNTUACION_LOCAL[]="data/puntuaciones.dat";
const char Lector_puntuaciones::CURL_EXPECT[]="Expect:";
const char Lector_puntuaciones::SOLICITUD_CAMPO_ID[]="id";
const char Lector_puntuaciones::SOLICITUD_CAMPO_NOMBRE[]="nombre";
const char Lector_puntuaciones::SOLICITUD_CAMPO_PUNTUACION[]="puntuacion";
const char Lector_puntuaciones::SOLICITUD_CAMPO_NIVEL[]="nivel";
const char Lector_puntuaciones::SOLICITUD_CAMPO_FINALIZADO[]="finalizado";
const char Lector_puntuaciones::NULO='\0';

Lector_puntuaciones::Lector_puntuaciones()
{
	this->conectado=false;
}

Lector_puntuaciones::~Lector_puntuaciones()
{

}

/*Inicializa los vectores de máximas puntuaciones con valores vacíos*/

void Lector_puntuaciones::iniciar()
{
	struct_puntuacion tmp;

	unsigned int i=0;
	for(;i<Lector_puntuaciones::MAX_CANTIDAD_MOSTRAR; i++)
	{
		this->puntuaciones.push_back(tmp);
		this->puntuaciones_online.push_back(tmp);
	}
}

/*Este bloque es para cargar las puntuaciones offline*/

void Lector_puntuaciones::cargar_local()
{
	std::ifstream archivo(Lector_puntuaciones::RUTA_PUNTUACION_LOCAL, std::ios::in | std::ios::binary);

	if(!archivo.is_open())
	{
		this->guardar();
		this->cargar_local();
		return;
	}

	unsigned int i=0, buf=0, j=0;
	U_ENTERO_8 niv=0;

//		char * buffer=new char[Lector_puntuaciones::MAX_LONGITUD_PUNTUACION_LOCAL];

	for(;i<Lector_puntuaciones::MAX_CANTIDAD_MOSTRAR && !archivo.eof(); i++)
	{
		char * buffer=new char[Lector_puntuaciones::MAX_LONGITUD_PUNTUACION_LOCAL];
		memset(buffer, Lector_puntuaciones::NULO, Lector_puntuaciones::MAX_LONGITUD_PUNTUACION_LOCAL);
		U_ENTERO_8 finalizado=false;

		archivo.read((char*) &buf, sizeof(U_ENTERO_8));
		archivo.read((char*) &niv, sizeof(U_ENTERO_8));
		archivo.read((char*) &finalizado, sizeof(U_ENTERO_8));
		for(j=0;j<Lector_puntuaciones::MAX_LONGITUD_PUNTUACION_LOCAL-1; j++) archivo.read((char*) &buffer[j], sizeof(U_ENTERO_8));

		struct_puntuacion tmp;
		tmp.rellenar(buf, niv, buffer, i+1, finalizado);
		this->puntuaciones[i]=tmp;		
		delete[] buffer;
	}

//		delete[] buffer;

	archivo.close();

}

void Lector_puntuaciones::guardar()
{
	unsigned int i=0, j;
	char * buffer=new char[Lector_puntuaciones::MAX_LONGITUD_PUNTUACION_LOCAL];

	std::ofstream archivo(Lector_puntuaciones::Lector_puntuaciones::RUTA_PUNTUACION_LOCAL, std::ios::out | std::ios::binary);
	
	for(;i<Lector_puntuaciones::MAX_CANTIDAD_MOSTRAR; i++)
	{
		memset(buffer, Lector_puntuaciones::NULO, Lector_puntuaciones::MAX_LONGITUD_PUNTUACION_LOCAL);

		archivo.write((char*) &this->puntuaciones[i].puntuacion, sizeof(U_ENTERO_8));
		archivo.write((char*) &this->puntuaciones[i].nivel, sizeof(U_ENTERO_8));
		archivo.write((char*) &this->puntuaciones[i].finalizado, sizeof(U_ENTERO_8));
		for(j=0; j<Lector_puntuaciones::MAX_LONGITUD_PUNTUACION_LOCAL-1; j++) archivo.write((char*) &this->puntuaciones[i].nombre[j], sizeof(U_ENTERO_8));
	}

	archivo.close();
	delete[] buffer;
}

/*Este comprueba si tenemos una nueva puntuación local comparando la puntuación
obtenida con las puntuaciones guardadas en el archivo de datos. Devuelve un 
valor numérico. Cero significa que NO hemos obtenido nada*/

short unsigned int Lector_puntuaciones::preguntar_nueva_puntuacion_local(unsigned int p)
{
	bool hay=false;
	unsigned int resultado=1;

	std::vector<struct_puntuacion>::iterator	ini=this->puntuaciones.begin(),
							fin=this->puntuaciones.end();

	for(;ini<fin;ini++)
	{
		if((*ini).puntuacion < p) 
		{
			hay=true;
			break;
		}
		else resultado++;
	}

	if(!hay) return 0;
	else return resultado;
}

/*
Este envía una nueva puntuación a la tabla propia y también al online.
máxima local. La puntuación online se envía siempre. La puntuación local
dependerá del parámetro p_nueva_puntuacion_local, que indica si se ha conseguido
o no una nueva puntuación máxima.
*/

void Lector_puntuaciones::enviar_nueva_puntuacion(unsigned int p, unsigned short int niv, std::string const& n, bool p_finalizado, bool p_nueva_puntuacion_local, unsigned int p_id)
{
	//Localizar posición.
	struct_puntuacion tmp;
	tmp.rellenar(p, niv, n.c_str(), 0, p_finalizado, p_id);
	this->guardar_servidor(tmp);
	int i=1;

	if(p_nueva_puntuacion_local)
	{
		std::vector<struct_puntuacion>::iterator ini=this->puntuaciones.begin(),
						fin=this->puntuaciones.end();

		for(;ini<fin;ini++, i++)
		{
			if(p > (*ini).puntuacion)
			{
				tmp.posicion=i;

				this->puntuaciones.insert(ini, tmp);
				ini=fin;
				this->puntuaciones.erase(ini);
				break;
			}
		}
	
		this->guardar();
	}
}

/** Estas son para pedirlas al server... **/

/*Este método hace una petición por CURL para obtener las máximas puntuaciones.
Devuelve un texto formateado que procesaremos para mostrarlas luego en 
pantalla. El formato del texto es:

12			-> Total de entradas, incluyendo aquellas que se muestran aquí.
10			-> Entradas que se muestran.
1|1900|0|Mr. Pani	-> N bloques de puntuacion: posicion|puntos|nivel|nombre
[...]

El p_indice no es más que la "paginación". 0 es la primera página, 1 la segunda y así sucesivamente.
El segúndo parámetro es para devolver por referencia el total de entradas, incluyendo las listadas.
**/


bool Lector_puntuaciones::cargar_online(unsigned int p_indice, unsigned int &p_total_entradas)
{
	bool resultado=false;
	Solicitud_CURL C;

	std::string indice=DLibH::Herramientas::entero_a_cadena(p_indice);
	std::string cadena_solicitud=Lector_puntuaciones::URL_SOLICITAR_PUNTUACIONES;
	cadena_solicitud.append(indice);

	C.inicializar(cadena_solicitud);

	if(!C.lanzar())
	{
		resultado=false;
	}
	else
	{
		this->procesar_respuesta_cargar_online(C.acc_respuesta(), p_total_entradas);
		resultado=true;		
	}

	this->conectado=resultado;
	return resultado;
}

void Lector_puntuaciones::procesar_respuesta_cargar_online(std::string const& respuesta, unsigned int &p_total_entradas)
{
	//Creamos un stream con la respuesta y una cadena para contener el resultado...
	std::istringstream stream(respuesta);
	std::string linea;    
	unsigned int total_entradas=0, mostrando_entradas=0, i=0;
	struct_puntuacion tmp;
	tmp.puntuacion=0;
	tmp.posicion=0;
	tmp.nivel=0;
	tmp.nombre="";

	//Antes de nada, ponemos a cero, por si vamos a leer menos de las que caben.
	for(i=0;i<Lector_puntuaciones::MAX_CANTIDAD_MOSTRAR; i++) this->puntuaciones_online[i]=tmp;

	//Leer la primera: total de entradas.
	std::getline(stream, linea);
	total_entradas=DLibH::Herramientas::cadena_a_entero(linea.c_str());

	float temp=(float)total_entradas / (float)MAX_CANTIDAD_MOSTRAR;
	p_total_entradas=ceil(temp);

	//Leer la segunda: cantidad de entradas devueltas.
	std::getline(stream, linea);
	mostrando_entradas=DLibH::Herramientas::cadena_a_entero(linea.c_str());

	//Iterar por el resto...

	for(i=0; i < mostrando_entradas; i++)
	{
		std::getline(stream, linea);
		tmp.rellenar_desde_servidor(linea);	
		this->puntuaciones_online[i]=tmp;
	}
}

/*Este método envía al servidor la puntuación que hayamos obtenido. No es más
que un form que se envía por CURL. Devolverá verdadero si la petición tiene 
éxito y falso en cualquier otro caso. En el futuro podremos hacer que el 
servidor cribe las entradas que se manden.*/

bool Lector_puntuaciones::guardar_servidor(struct_puntuacion const& param)
{
	bool resultado=false;
	Solicitud_CURL C;

	C.inicializar(Lector_puntuaciones::URL_INGRESAR_PUNTUACION);

	//Datos de POST...

	C.post(Lector_puntuaciones::SOLICITUD_CAMPO_ID, param.id);
	C.post(Lector_puntuaciones::SOLICITUD_CAMPO_NOMBRE, param.nombre);
	C.post(Lector_puntuaciones::SOLICITUD_CAMPO_PUNTUACION, DLibH::Herramientas::entero_a_cadena(param.puntuacion));
	C.post(Lector_puntuaciones::SOLICITUD_CAMPO_NIVEL, DLibH::Herramientas::entero_a_cadena(param.nivel));
	C.post(Lector_puntuaciones::SOLICITUD_CAMPO_FINALIZADO, DLibH::Herramientas::entero_a_cadena(param.finalizado));

	if(!C.lanzar())
	{
		resultado=false;
	}
	else
	{
		resultado=true;
	}

	this->conectado=resultado;
	return resultado;
}

/*
Solicita al servidor la posición en la que estaríamos en las puntuaciones 
online para la puntuación enviada. Devolverá cero cuando NO sea posible 
conectar y un número cuando haya conectado y todo vaya bien. 
*/

unsigned long int Lector_puntuaciones::solicitud_servidor_posicion_para_puntuacion(unsigned int p_puntuacion)
{
	Solicitud_CURL C;
	unsigned long int resultado=0;

	std::string puntuacion=DLibH::Herramientas::entero_a_cadena(p_puntuacion);
	std::string cadena_solicitud=Lector_puntuaciones::URL_SOLICITAR_POSICION_POR_PUNTUACION;
	cadena_solicitud.append(puntuacion);

	C.inicializar(cadena_solicitud);

	if(!C.lanzar())
	{
		resultado=0;		
	}
	else
	{
		//Creamos un stream con la respuesta y una cadena para contener el resultado...
		std::istringstream stream(C.acc_respuesta());
  		std::string linea;    

		//Leer la primera: el resultado. Con el c_str no perdemos memoria: apunta realmente a una parte de std::string.
		std::getline(stream, linea);
		resultado=DLibH::Herramientas::cadena_a_entero(linea.c_str());

	}

	this->conectado=(bool)resultado;
	return resultado;
}
