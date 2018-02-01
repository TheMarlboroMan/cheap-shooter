#include "perfil_datos.h"
#include "../definiciones/definicion_retos.h"

Perfil_datos::Perfil_datos(unsigned int p_version)
	:version(p_version), 
	sesiones_iniciadas(0), 
	puntuacion_acumulada(0),
	max_tipo_combo(0), 
	disparos_efectuados(0),
	partidas_iniciadas(0), 
	partidas_abortadas(0),
	partidas_ganadas(0),
	partidas_perdidas(0),
	total_gemas_recogidas(0),
	total_segundos_tiempo_detenido(0),
	total_obstaculos_destruidos(0),
	total_vidas_perdidas(0),
	total_pixeles_teletransportados(0),
	total_potencia_edu_usada(0),
	total_minas_destruidas(0),
	mostrar_tutoriales(1),
	suma_retos_obtenidos(0),
	cuenta_niveles_finalizados(0)
{
	this->retos_obtenidos.insert(this->retos_obtenidos.begin(), Definicion_retos::TOTAL_RETOS, false);

	//Inicializar la info de los niveles.
	int i=0;
	while(i < Definicion_retos::TOTAL_NIVELES + Definicion_retos::TOTAL_NIVELES_SECRETOS) info_niveles.push_back(Info_nivel(i++));
}

Perfil_datos::Perfil_datos(const Perfil_datos& otro)
{
	sesiones_iniciadas=otro.sesiones_iniciadas;
	puntuacion_acumulada=otro.puntuacion_acumulada;
	max_tipo_combo=otro.max_tipo_combo;
	disparos_efectuados=otro.disparos_efectuados;
	partidas_iniciadas=otro.partidas_iniciadas;
	partidas_abortadas=otro.partidas_abortadas;
	partidas_ganadas=otro.partidas_ganadas;
	partidas_perdidas=otro.partidas_perdidas;
	total_gemas_recogidas=otro.total_gemas_recogidas;
	total_segundos_tiempo_detenido=otro.total_segundos_tiempo_detenido;
	total_obstaculos_destruidos=otro.total_obstaculos_destruidos;
	total_vidas_perdidas=otro.total_vidas_perdidas;
	total_pixeles_teletransportados=otro.total_pixeles_teletransportados;
	total_potencia_edu_usada=otro.total_potencia_edu_usada;
	total_minas_destruidas=otro.total_minas_destruidas;
	mostrar_tutoriales=otro.mostrar_tutoriales;

	retos_obtenidos=otro.retos_obtenidos;
	info_niveles=otro.info_niveles;

	suma_retos_obtenidos=otro.suma_retos_obtenidos;
	total_niveles_secretos_finalizados=otro.total_niveles_secretos_finalizados;
	cuenta_niveles_finalizados=otro.cuenta_niveles_finalizados;
}

Perfil_datos& Perfil_datos::operator=(const Perfil_datos& otro)
{
	sesiones_iniciadas=otro.sesiones_iniciadas;
	puntuacion_acumulada=otro.puntuacion_acumulada;
	max_tipo_combo=otro.max_tipo_combo;
	disparos_efectuados=otro.disparos_efectuados;
	partidas_iniciadas=otro.partidas_iniciadas;
	partidas_abortadas=otro.partidas_abortadas;
	partidas_ganadas=otro.partidas_ganadas;
	partidas_perdidas=otro.partidas_perdidas;
	total_gemas_recogidas=otro.total_gemas_recogidas;
	total_segundos_tiempo_detenido=otro.total_segundos_tiempo_detenido;
	total_obstaculos_destruidos=otro.total_obstaculos_destruidos;
	total_vidas_perdidas=otro.total_vidas_perdidas;
	total_pixeles_teletransportados=otro.total_pixeles_teletransportados;
	total_potencia_edu_usada=otro.total_potencia_edu_usada;
	total_minas_destruidas=otro.total_minas_destruidas;
	mostrar_tutoriales=otro.mostrar_tutoriales;

	retos_obtenidos=otro.retos_obtenidos;
	info_niveles=otro.info_niveles;

	suma_retos_obtenidos=otro.suma_retos_obtenidos;
	total_niveles_secretos_finalizados=otro.total_niveles_secretos_finalizados;

	cuenta_niveles_finalizados=otro.cuenta_niveles_finalizados;

	return *this;
}

Perfil_datos::~Perfil_datos()
{

}

bool Perfil_datos::es_finalizado_todos_niveles_secretos() const
{
	return this->total_niveles_secretos_finalizados>=Definicion_retos::TOTAL_NIVELES_SECRETOS; //-1;
}

bool Perfil_datos::cargar(std::ifstream & p_archivo)
{
	suma_retos_obtenidos=0;
	total_niveles_secretos_finalizados=0;
	cuenta_niveles_finalizados=0;

	p_archivo.read( (char *) &this->version, sizeof(U_ENTERO_16));
	p_archivo.read( (char *) &this->sesiones_iniciadas, sizeof(U_ENTERO_16));
	p_archivo.read( (char *) &this->puntuacion_acumulada, sizeof(U_ENTERO_32));
	p_archivo.read( (char *) &this->max_tipo_combo, sizeof(U_ENTERO_8));
	p_archivo.read( (char *) &this->disparos_efectuados, sizeof(U_ENTERO_32));
	p_archivo.read( (char *) &this->partidas_iniciadas, sizeof(U_ENTERO_16));
	p_archivo.read( (char *) &this->total_gemas_recogidas, sizeof(U_ENTERO_32));
	p_archivo.read( (char *) &this->total_segundos_tiempo_detenido, sizeof(U_ENTERO_16));
	p_archivo.read( (char *) &this->total_obstaculos_destruidos, sizeof(U_ENTERO_32));
	p_archivo.read( (char *) &this->total_vidas_perdidas, sizeof(U_ENTERO_16));
	p_archivo.read( (char *) &this->total_pixeles_teletransportados, sizeof(U_ENTERO_16));
	p_archivo.read( (char *) &this->total_potencia_edu_usada, sizeof(U_ENTERO_16));
	p_archivo.read( (char *) &this->partidas_abortadas, sizeof(U_ENTERO_16));
	p_archivo.read( (char *) &this->partidas_ganadas, sizeof(U_ENTERO_16));
	p_archivo.read( (char *) &this->partidas_perdidas, sizeof(U_ENTERO_16));
	p_archivo.read( (char *) &this->total_minas_destruidas, sizeof(U_ENTERO_16));
	p_archivo.read( (char *) &this->mostrar_tutoriales, sizeof(U_ENTERO_8));

	unsigned int i=0;
	bool temp=false;
	while(i < Definicion_retos::TOTAL_RETOS)
	{
		p_archivo.read( (char *) &temp, sizeof(U_ENTERO_8));	
		this->retos_obtenidos[i]=temp;
		++i;

		if(temp) ++this->suma_retos_obtenidos;
	}

	for(auto& dummy : info_niveles) dummy.cargar(p_archivo);
	refrescar_cuenta_niveles_finalizados();

	return !p_archivo.eof(); //Por ejemplo, devolvemos esto mismo :P.
}

void Perfil_datos::refrescar_cuenta_niveles_finalizados()
{
	unsigned int i=0;

	for(auto& dummy : info_niveles) 
	{
		if(dummy.es_finalizado()) 
		{
			if(i<Definicion_retos::TOTAL_NIVELES) ++cuenta_niveles_finalizados;
			else ++total_niveles_secretos_finalizados;
		}

		++i;
	}
}

bool Perfil_datos::guardar(std::fstream & p_archivo)
{
	p_archivo.write( (char *) &this->version, sizeof(U_ENTERO_16));
	p_archivo.write( (char *) &this->sesiones_iniciadas, sizeof(U_ENTERO_16));
	p_archivo.write( (char *) &this->puntuacion_acumulada, sizeof(U_ENTERO_32));
	p_archivo.write( (char *) &this->max_tipo_combo, sizeof(U_ENTERO_8));
	p_archivo.write( (char *) &this->disparos_efectuados, sizeof(U_ENTERO_32));
	p_archivo.write( (char *) &this->partidas_iniciadas, sizeof(U_ENTERO_16));
	p_archivo.write( (char *) &this->total_gemas_recogidas, sizeof(U_ENTERO_32));
	p_archivo.write( (char *) &this->total_segundos_tiempo_detenido, sizeof(U_ENTERO_16));
	p_archivo.write( (char *) &this->total_obstaculos_destruidos, sizeof(U_ENTERO_32));
	p_archivo.write( (char *) &this->total_vidas_perdidas, sizeof(U_ENTERO_16));
	p_archivo.write( (char *) &this->total_pixeles_teletransportados, sizeof(U_ENTERO_16));
	p_archivo.write( (char *) &this->total_potencia_edu_usada, sizeof(U_ENTERO_16));
	p_archivo.write( (char *) &this->partidas_abortadas, sizeof(U_ENTERO_16));
	p_archivo.write( (char *) &this->partidas_ganadas, sizeof(U_ENTERO_16));
	p_archivo.write( (char *) &this->partidas_perdidas, sizeof(U_ENTERO_16));
	p_archivo.write( (char *) &this->total_minas_destruidas, sizeof(U_ENTERO_16));
	p_archivo.write( (char *) &this->mostrar_tutoriales, sizeof(U_ENTERO_8));

	unsigned int i=0;
	bool temp;
	while(i < Definicion_retos::TOTAL_RETOS)
	{
		temp=this->retos_obtenidos[i];
		p_archivo.write( (char *) &temp, sizeof(U_ENTERO_8));	
		++i;
	}

	for(auto& dummy : info_niveles) dummy.guardar(p_archivo);

	return true;
}

void Perfil_datos::actualizar_por_estructura(Estructura_datos_perfil &datos_nuevos)
{
	this->puntuacion_acumulada+=datos_nuevos.puntuacion;
	this->total_gemas_recogidas+=datos_nuevos.gemas_recogidas;
	this->disparos_efectuados+=datos_nuevos.disparos_efectuados;
	this->total_obstaculos_destruidos+=datos_nuevos.obstaculos_destruidos;
	this->total_vidas_perdidas+=datos_nuevos.vidas_perdidas;
	this->total_pixeles_teletransportados+=datos_nuevos.pixeles_teletransportados;
	this->total_potencia_edu_usada+=datos_nuevos.potencia_edu_usada;
	this->total_minas_destruidas+=datos_nuevos.minas_destruidas;

	if(datos_nuevos.segundos_tiempo_detenido >= 1)
	{
		this->total_segundos_tiempo_detenido+=floor(datos_nuevos.segundos_tiempo_detenido);
	}

	if(datos_nuevos.max_tipo_combo > this->max_tipo_combo) this->max_tipo_combo=datos_nuevos.max_tipo_combo;
}

bool Perfil_datos::es_finalizado_nivel_secreto(unsigned int p_valor) const
{
	if(p_valor < Definicion_retos::TOTAL_NIVELES_SECRETOS)
	{
		unsigned int nivel=Definicion_retos::TOTAL_NIVELES+p_valor; //Si preguntamos por el nivel secreto 0 es el índice 9.
		return info_niveles[nivel].es_finalizado();
	}
	else 
	{
		return false;
	}
}

void Perfil_datos::conseguir_reto(unsigned int p_valor)
{
	if(p_valor < Definicion_retos::TOTAL_RETOS) 
	{
		this->retos_obtenidos[p_valor]=true;
		++this->suma_retos_obtenidos;
	}
}

bool Perfil_datos::es_reto_conseguido(unsigned int p_indice)
{
	if(p_indice < Definicion_retos::TOTAL_RETOS) return this->retos_obtenidos[p_indice];
	else return false;
}

Estructura_intercambio_retos Perfil_datos::obtener_estructura_retos_rellena()
{
	Estructura_intercambio_retos resultado;
	resultado.datos=this->retos_obtenidos;
	return resultado;
}

void Perfil_datos::rellenar_datos_retos_desde_servidor(const Estructura_intercambio_retos& est)
{
	this->retos_obtenidos=est.datos;
}

Perfil_datos::Info_nivel& Perfil_datos::obtener_datos_nivel(unsigned int p_valor) throw ()
{
	if(p_valor >= info_niveles.size())
	{
		throw Info_nivel_excepcion(Info_nivel_excepcion::C_FUERA_DE_RANGO);
	}
	else
	{
		return info_niveles[p_valor];
	}
}
