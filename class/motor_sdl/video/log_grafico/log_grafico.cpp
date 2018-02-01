#include "log_grafico.h"

using namespace DLibV;

Log_grafico::Log_grafico(const Recurso_grafico * p_rec, const SDL_Rect& p_caja):
	proximo_id(0), indice_actual(0), lineas_vertical(0), max_caracteres_linea(0),
	rep(p_rec, "", p_caja.w, p_caja.h)
{
	rep.ir_a(p_caja.x, p_caja.y);
	realizar_calculos_iniciales();
}

Log_grafico::~Log_grafico()
{
	//Nothing.
}

/*Se encarga de hacer los cálculos previos necesarios, como el máximo de líneas
que caben en el espacio o la máxima cantidad de caractéres que caben en una
línea...*/

void Log_grafico::realizar_calculos_iniciales()
{
	lineas_vertical=ceil((float)rep.acc_alto_area() / (float)rep.acc_alto_glifo());
	max_caracteres_linea=ceil((float)rep.acc_ancho_area() / (float)rep.acc_ancho_glifo());
}

/*Este método debe preparar la cadena que pasamos para la longitud de línea
máxima según las dimensiones que tenemos. La iremos partiendo y añadiendo 
nuevas líneas...
*/

Log_grafico::Entrada_log Log_grafico::preparar_entrada(const t_log_grafico_tipo& p_cadena)
{
	t_log_grafico_tipo cadena="["+DLibH::Herramientas::entero_a_cadena(++proximo_id)+"] "+p_cadena;
	unsigned int cantidad_lineas=ceil((float)cadena.length() / (float)max_caracteres_linea);
	cadena+=DLibH::Herramientas::cadena_a_maximo_ancho(p_cadena, max_caracteres_linea);
	return Log_grafico::Entrada_log(cadena, cantidad_lineas);
}

/*Inserta una nueva cadena después de prepararla. Por defecto intenta avanzar
en uno el índice actual... A continuación mira a ver si con la nueva entrada
hay que avanzar para mostrarla y lo hace*/

void Log_grafico::insertar(const t_log_grafico_tipo& p_cadena)
{
	Entrada_log temp=preparar_entrada(p_cadena);
	contenedor.push_back(temp);

	unsigned int lineas_consumidas=temp.lineas;
	t_log_grafico_contenedor::iterator 	ini=contenedor.begin()+indice_actual,
						fin=contenedor.end();

	while(ini < fin)
	{
		lineas_consumidas+=(*ini).lineas;

		if(lineas_consumidas >/*=*/ lineas_vertical) 
		{
			//Avanzará sólo una... Igual no cabe la línea nueva entera,
			//pero es lo que hay.

			for(int i=lineas_consumidas -lineas_vertical; i>0; --i) avanzar();
			break;
		}
		++ini;
	}

	redibujar();
}

/*Vacía el vector de líneas y poco más...*/

void Log_grafico::limpiar()
{
	contenedor.clear();
	indice_actual=0;
	redibujar();
}

void Log_grafico::redibujar()
{
	std::string compuesta="";
	unsigned int lineas_consumidas=0;
	t_log_grafico_contenedor::iterator 	ini=contenedor.begin()+indice_actual,
						fin=contenedor.end();

	while(ini < fin)
	{
		compuesta+=(*ini).cadena+"\n";
		lineas_consumidas+=(*ini).lineas;

		if(lineas_consumidas >= lineas_vertical) break;
		++ini;
	}

	rep.asignar(compuesta);
}

bool Log_grafico::avanzar()
{
	unsigned int max=contenedor.size();

	if(!max) 
	{
		return false;
	}
	else
	{
		if(indice_actual < max) 
		{
			++indice_actual;
			redibujar();
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Log_grafico::retroceder()
{
	if(!contenedor.size())
	{
		return false;
	}
	else
	{
		if(indice_actual <= 0)
		{
			return false;
		}
		else
		{
			--indice_actual;
			redibujar();
			return true;

		}
	}
}

bool Log_grafico::seleccionar(unsigned int p_indice)
{
	unsigned int max=contenedor.size();

	if(!max) 
	{
		return false;
	}
	else
	{
		if(p_indice >= max) return false;
		else
		{
			indice_actual=p_indice;
			redibujar();
			return true;
		}
	}
}
