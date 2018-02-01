#include "estructura_info_nivel.h"

//Antes de tratar con la estructura info nivel vamos a definir los métodos
//no inline de algunas clases ayudantes.

void Info_base_nivel::serializar(std::ofstream& fichero)
{
	//Guardar...	
	fichero.write( (char *) &vector_y_nivel, sizeof(U_ENTERO_16));
	fichero.write( (char *) &max_obstaculos, sizeof(U_ENTERO_16));
	fichero.write( (char *) &max_bonus, sizeof(U_ENTERO_16));
	fichero.write( (char *) &gemas_en_nivel, sizeof(U_ENTERO_16));
	fichero.write( (char *) &gemas_max_nivel, sizeof(U_ENTERO_16));
	fichero.write( (char *) &flags_artefactos_nivel, sizeof(U_ENTERO_8));
	fichero.write( (char *) &indice_proximo_nivel, sizeof(U_ENTERO_8));
	fichero.write( (char *) &indice_proximo_nivel_secreto, sizeof(U_ENTERO_8));
	fichero.write( (char *) &indice_musica, sizeof(U_ENTERO_8));
}	

void Info_base_nivel::deserializar(std::ifstream& fichero)
{
	//Cargar...
	fichero.read( (char *) &vector_y_nivel, sizeof(U_ENTERO_16));
	fichero.read( (char *) &max_obstaculos, sizeof(U_ENTERO_16));
	fichero.read( (char *) &max_bonus, sizeof(U_ENTERO_16));
	fichero.read( (char *) &gemas_en_nivel, sizeof(U_ENTERO_16));
	fichero.read( (char *) &gemas_max_nivel, sizeof(U_ENTERO_16));
	fichero.read( (char *) &flags_artefactos_nivel, sizeof(U_ENTERO_8));
	fichero.read( (char *) &indice_proximo_nivel, sizeof(U_ENTERO_8));
	fichero.read( (char *) &indice_proximo_nivel_secreto, sizeof(U_ENTERO_8));
	fichero.read( (char *) &indice_musica, sizeof(U_ENTERO_8));

	gemas_max_nivel_original=gemas_max_nivel;

}

void Info_marcha_nivel::reiniciar()
{
	distancia=0.0f;
	vector_y=0.0f,
	nuevo_vector_y=0;
	velocidad_cambio_vector_y=0;
	divisor_movimiento=1.0f;
	acumulador_camara=0.0f;
	avance_este_turno=0;
	destruido_asteroide_secreto=false;
	comprobar_finalizacion_nivel_preparado=false;
	cambiando_velocidad=false;
	cantidad_actores_cargados=0; 
	flags_artefactos=Definiciones::P_CANON | Definiciones::P_TELETRANSPORTADOR | Definiciones::P_EMISOR_DENSIDAD_UNIFICADA;
}

void Info_marcha_nivel::copiar_de_datos_nivel(const Info_base_nivel& base)
{
	vector_y=base.vector_y_nivel;
	flags_artefactos=base.flags_artefactos_nivel;
}

void Info_fichero::reiniciar()
{
	nombre_fichero="";
	cantidad_actores=0;
	cantidad_acciones=0;
	archivo_cargado=false;
	finalizada_lectura=false;
	posicion_inicio_datos=0;
}

//Métodos de Estructura_info_nivel.

Estructura_info_nivel::Estructura_info_nivel()
		:
		modo_juego(M_ALEATORIO), 	
		INFO_NIVEL(),
		INFO_MARCHA(),
		INFO_FICHERO()
{

}

Estructura_info_nivel::~Estructura_info_nivel()
{
	if(this->fichero.is_open())
	{
		this->fichero.close();
	}
}

void Estructura_info_nivel::reiniciar()
{	
	INFO_MARCHA.reiniciar();
	INFO_FICHERO.reiniciar();

	if(this->fichero.is_open())
	{
		this->fichero.close();
	}
}

bool Estructura_info_nivel::abrir_fichero(std::string const &p_nombre)
{		
	INFO_FICHERO.nombre_fichero=p_nombre;	
	this->fichero.open(INFO_FICHERO.nombre_fichero.c_str(), std::ios::in | std::ios::binary);

	if(!this->fichero.is_open())
	{
		INFO_FICHERO.archivo_cargado=false;
	}
	else
	{
		INFO_FICHERO.archivo_cargado=true;
	}

	return INFO_FICHERO.archivo_cargado;
}

void Estructura_info_nivel::turno(float p_delta)
{
	switch(this->modo_juego)
	{
		case M_ALEATORIO: this->turno_modo_aleatorio(p_delta); break;
		case M_PREPARADO: this->turno_modo_preparado(p_delta); break;
	}
}

void Estructura_info_nivel::turno_modo_aleatorio(float p_delta)
{

}

void Estructura_info_nivel::turno_modo_preparado(float p_delta)
{
	float factor=INFO_MARCHA.vector_y*(p_delta/INFO_MARCHA.divisor_movimiento);

	INFO_MARCHA.distancia-=factor;
	INFO_MARCHA.acumulador_camara+=factor;

	if(INFO_MARCHA.cambiando_velocidad)
	{
		if(INFO_MARCHA.vector_y < INFO_MARCHA.nuevo_vector_y)
		{
			INFO_MARCHA.vector_y+=INFO_MARCHA.velocidad_cambio_vector_y * p_delta;
			if(INFO_MARCHA.vector_y >= INFO_MARCHA.nuevo_vector_y) INFO_MARCHA.vector_y=INFO_MARCHA.nuevo_vector_y;
		}
		else if(INFO_MARCHA.vector_y > INFO_MARCHA.nuevo_vector_y)
		{
			INFO_MARCHA.vector_y-=INFO_MARCHA.velocidad_cambio_vector_y * p_delta;
			if(INFO_MARCHA.vector_y <= INFO_MARCHA.nuevo_vector_y) INFO_MARCHA.vector_y=INFO_MARCHA.nuevo_vector_y;
		}
		else
		{
			INFO_MARCHA.cambiando_velocidad=false;
		}
	}	

	if(INFO_MARCHA.acumulador_camara > 1.0f)
	{
		double temp;

		//Sacar la parte entera...
		modf(INFO_MARCHA.acumulador_camara, &temp);
		INFO_MARCHA.avance_este_turno=temp;
		INFO_MARCHA.acumulador_camara=INFO_MARCHA.acumulador_camara-((float) INFO_MARCHA.avance_este_turno);
	}
	else
	{
		INFO_MARCHA.avance_este_turno=0;
	}
}

/*El streaming del nivel se hace en bloques de "DISTANCIA_STREAMING" unidades.
Primero se lee esa distancia, luego, cuando llegamos a "DISTANCIA_SEGURIDAD"
se vuelve a leer. Todo esto se hace manejando valores internos, sin molestar
a la clase cliente*

Aunque... ya no se hace así. Estamos cargando el nivel en un bloque completo.
Y poco más. Por esto se entiende un "debe cargar actores".
*/

bool Estructura_info_nivel::debe_cargar()
{
	if(INFO_FICHERO.finalizada_lectura) 
	{
		return false;
	}
	else
	{
		if(INFO_MARCHA.cantidad_actores_cargados >= INFO_FICHERO.cantidad_actores)
		{
			return false;
		}
		else
		{
			INFO_MARCHA.cantidad_actores_cargados++;
			return true;
		}
	}
}

Actor_serializado Estructura_info_nivel::obtener()
{
	Actor_serializado act=Actor_serializado::deserializar(this->fichero);

	//El EOF sólo ocurre al intentar leer DESPUÉS del fin de fichero. Por
	//tanto la orden anterior PUEDE haber ocurrido, pero debe ser 
	//invalidada aquí.

	if(this->fichero.eof())
	{
		//Invalidamos el actor con "nada".
		act.invalidar();		
		INFO_FICHERO.finalizada_lectura=true;

		//Este es el único punto donde se comprueba el EOF del fichero,
		//de modo que esto sería seguro... Al pasar por EOF se levanta
		//el bit de "fail". Lo vamos a limpiar para que al rebobinar
		//podamos hacer seekg (si hay fail peta).

		this->fichero.clear();
	}

	return act;
}

void Estructura_info_nivel::rebobinar()
{
	if(!this->fichero.is_open())
	{
		throw Excepcion_salida(Excepcion_salida::E_FICHERO_CERRADO_AL_REBOBINAR);
	}
	else
	{
		this->fichero.seekg(INFO_FICHERO.posicion_inicio_datos, this->fichero.beg);

		INFO_NIVEL.reiniciar_gemas();
		INFO_MARCHA.reiniciar();
		INFO_MARCHA.copiar_de_datos_nivel(INFO_NIVEL);
	}
}

void Estructura_info_nivel::cargar_valores_para_nivel(unsigned short int p_nivel)
{
	//Cargar info nivel...

	INFO_NIVEL.deserializar(this->fichero);

	this->TABLA_OBSTACULOS.cargar_tabla(this->fichero);
	this->TABLA_BONUS.cargar_tabla(this->fichero);
	this->TABLA_DESTRUIR_CAJA.cargar_tabla(this->fichero);
	this->TABLA_DESTRUIR_OBSTACULO.cargar_tabla(this->fichero);

	//Vamos ahora a leer todas las acciones.

	this->fichero.read((char *) &INFO_FICHERO.cantidad_acciones, sizeof(U_ENTERO_16));
	for(int i=0; i<INFO_FICHERO.cantidad_acciones; i++) this->acciones_trigger.push_back(Accion_trigger::deserializar(this->fichero));

	//Y ahora los actores... Leemos sólo los que hay. El resto se hará más
	//adelante por parte del controlador.

	this->fichero.read((char *) &INFO_FICHERO.cantidad_actores, sizeof(U_ENTERO_16));

	INFO_FICHERO.posicion_inicio_datos=this->fichero.tellg();
	INFO_MARCHA.copiar_de_datos_nivel(INFO_NIVEL);

	this->modo_juego=INFO_FICHERO.cantidad_actores ? M_PREPARADO : M_ALEATORIO;
}

int Estructura_info_nivel::generar_tirada_tabla(unsigned int p_tabla)
{
	int resultado=Tabla_probabilidad::TIRADA_INVALIDA;

	switch(p_tabla)
	{
		case T_OBSTACULOS:
			resultado=this->TABLA_OBSTACULOS.generar_tirada();
		break;

		case T_BONUS:
			resultado=this->TABLA_BONUS.generar_tirada();
		break;

		case T_DESTRUIR_CAJA:
			resultado=this->TABLA_DESTRUIR_CAJA.generar_tirada();
		break;

		case T_DESTRUIR_OBSTACULO:
			resultado=this->TABLA_DESTRUIR_OBSTACULO.generar_tirada();
		break;
	}		

	return resultado;
}

void Estructura_info_nivel::preparar_para_final_juego()
{
	this->modo_juego=M_ALEATORIO;
}

void Estructura_info_nivel::modificar_artefactos(unsigned short int p_flags)
{
	INFO_MARCHA.flags_artefactos=p_flags;
}

std::vector<Accion_trigger_compuesta> Estructura_info_nivel::obtener_composicion_accion_trigger_por_tag(unsigned int p_tag)
{
	std::vector<Accion_trigger_compuesta> resultado;
	std::vector<Accion_trigger>::iterator 	ini=this->acciones_trigger.begin(),
						fin=this->acciones_trigger.end();

	while(ini < fin)
	{
		if( (*ini).acc_tag()==p_tag) 
		{
			Accion_trigger_compuesta temp( &(*ini) );
			resultado.push_back(temp);
		}
		++ini;
	}	
	
	return resultado;
}

bool Estructura_info_nivel::es_artefacto_permitido(unsigned short int a)
{
	unsigned short int pot=0;
	
	//Vamos a convertir el artefacto actual a una "flag" adecuada para ser 
	//comparada con Definiciones::ARTEFACTOS_PERMITIDOS.
	pot=(unsigned short int) pow(2, (a-1));

	return pot & INFO_MARCHA.flags_artefactos;
}

unsigned short int Estructura_info_nivel::ciclar_artefacto(int direccion, unsigned short int actual)
{
	if(!INFO_MARCHA.flags_artefactos)
	{
		return Definiciones::A_MIN_ARTEFACTO;
	}

	unsigned short int copia_actual=actual;	

	//Iteramos hasta dar una vuelta completa o encontrar un artefacto permitido.
	do
	{
		actual+=direccion;

		if(actual>=Definiciones::A_MAX_ARTEFACTO) actual=Definiciones::A_MIN_ARTEFACTO+1;
		else if(actual <= Definiciones::A_MIN_ARTEFACTO) actual=Definiciones::A_MAX_ARTEFACTO-1;

		//Si está permitido salimos del loop... con un return.
		if(es_artefacto_permitido(actual))
		{
			return actual;
		}
	}
	while(actual != copia_actual);

	//Si hemos llegado hasta aquí es porque no hay ningún artefacto válido, ergo...
	return Definiciones::A_MIN_ARTEFACTO;
}
