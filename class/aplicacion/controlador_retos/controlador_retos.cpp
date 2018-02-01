#include "controlador_retos.h"

Controlador_retos::Controlador_retos()
{

}

Controlador_retos::~Controlador_retos()
{
	t_vector_retos::iterator 	ini=this->vector_retos.begin(),
					fin=this->vector_retos.end();

	while(ini < fin)
	{
		delete *ini;
		++ini;
	}

	this->vector_retos.clear();
}

void Controlador_retos::insertar_reto(unsigned int p_id, std::string p_titulo, std::string p_texto)
{
	Reto * temp=new Reto();

	temp->asignar_datos(p_id, p_titulo, p_texto);
	this->vector_retos.push_back(temp);
}

void Controlador_retos::inicializar(Localizador & loc)
{
	insertar_reto(Definicion_retos::R_N_PARTIDAS,		loc.obtener(Localizador::C_RETO_01_TITULO), loc.obtener(Localizador::C_RETO_01_TEXTO)); 
	insertar_reto(Definicion_retos::R_N_DISPAROS, 		loc.obtener(Localizador::C_RETO_02_TITULO), loc.obtener(Localizador::C_RETO_02_TEXTO)); 
	insertar_reto(Definicion_retos::R_N_OBSTACULOS_DESTRUIDOS, 	loc.obtener(Localizador::C_RETO_03_TITULO), loc.obtener(Localizador::C_RETO_03_TEXTO)); 
	insertar_reto(Definicion_retos::R_N_GEMAS, 			loc.obtener(Localizador::C_RETO_04_TITULO), loc.obtener(Localizador::C_RETO_04_TEXTO)); 
	insertar_reto(Definicion_retos::R_N_COMBO, 			loc.obtener(Localizador::C_RETO_05_TITULO), loc.obtener(Localizador::C_RETO_05_TEXTO));  
	insertar_reto(Definicion_retos::R_N_VIDAS_PERDIDAS, 	loc.obtener(Localizador::C_RETO_06_TITULO), loc.obtener(Localizador::C_RETO_06_TEXTO)); 
	insertar_reto(Definicion_retos::R_NIVEL_PACIFISTA, 		loc.obtener(Localizador::C_RETO_07_TITULO), loc.obtener(Localizador::C_RETO_07_TEXTO)); 
	insertar_reto(Definicion_retos::R_N_PARAR_TIEMPO, 		loc.obtener(Localizador::C_RETO_08_TITULO), loc.obtener(Localizador::C_RETO_08_TEXTO)); 
	insertar_reto(Definicion_retos::R_N_TELETRANSPORTE, 	loc.obtener(Localizador::C_RETO_09_TITULO), loc.obtener(Localizador::C_RETO_09_TEXTO)); 
	insertar_reto(Definicion_retos::R_N_EDU, 			loc.obtener(Localizador::C_RETO_10_TITULO), loc.obtener(Localizador::C_RETO_10_TEXTO)); 
	insertar_reto(Definicion_retos::R_N_MINAS_DESTRUIDAS, 	loc.obtener(Localizador::C_RETO_11_TITULO), loc.obtener(Localizador::C_RETO_11_TEXTO)); 
	insertar_reto(Definicion_retos::R_ASTEROIDE_SECRETO, 	loc.obtener(Localizador::C_RETO_12_TITULO), loc.obtener(Localizador::C_RETO_12_TEXTO)); 
	insertar_reto(Definicion_retos::R_NIVELES_SECRETOS, 	loc.obtener(Localizador::C_RETO_13_TITULO), loc.obtener(Localizador::C_RETO_13_TEXTO)); 
	insertar_reto(Definicion_retos::R_FINALIZAR_JUEGO, 		loc.obtener(Localizador::C_RETO_14_TITULO), loc.obtener(Localizador::C_RETO_14_TEXTO)); 
	insertar_reto(Definicion_retos::R_MAXIMIZAR_CANON, 		loc.obtener(Localizador::C_RETO_15_TITULO), loc.obtener(Localizador::C_RETO_15_TEXTO)); 
	insertar_reto(Definicion_retos::R_NIVEL_CON_TODAS_GEMAS, 	loc.obtener(Localizador::C_RETO_16_TITULO), loc.obtener(Localizador::C_RETO_16_TEXTO)); 
	insertar_reto(Definicion_retos::R_N_PUNTUACION, 		loc.obtener(Localizador::C_RETO_17_TITULO), loc.obtener(Localizador::C_RETO_17_TEXTO)); 
}

void Controlador_retos::conseguir_reto(Perfil_datos & p_datos, unsigned int p_identificador_reto, unsigned int p_param_a, unsigned int p_param_b)
{
	p_datos.conseguir_reto(p_identificador_reto);
	this->retos_recien_cumplidos.push_back(this->vector_retos[p_identificador_reto]);
}

bool Controlador_retos::implementacion_reto_numerico(Perfil_datos & p_datos, unsigned int p_reto, unsigned int p_param_a, unsigned int p_param_b)
{
	if(!p_datos.es_reto_conseguido(p_reto))
	{
		if(p_param_a >= p_param_b)
		{
//			Definicion_retos::debug_reto(p_reto, p_param_a, p_param_b);
			this->conseguir_reto(p_datos,p_reto);
			return true;
		}
	}

	return false;
}

bool Controlador_retos::comprobar_retos(Perfil_datos & p_datos)
{
	bool resultado=false;

	resultado=resultado || implementacion_reto_numerico(p_datos, Definicion_retos::R_N_PARTIDAS, p_datos.acc_partidas_ganadas() + p_datos.acc_partidas_perdidas(), N_PARTIDAS);
	resultado=resultado || implementacion_reto_numerico(p_datos, Definicion_retos::R_N_DISPAROS, p_datos.acc_disparos_efectuados(), N_DISPAROS);
	resultado=resultado || implementacion_reto_numerico(p_datos, Definicion_retos::R_N_OBSTACULOS_DESTRUIDOS, p_datos.acc_total_obstaculos_destruidos(), N_OBSTACULOS_DESTRUIDOS);
	resultado=resultado || implementacion_reto_numerico(p_datos, Definicion_retos::R_N_GEMAS, p_datos.acc_total_gemas_recogidas(), N_GEMAS);
	resultado=resultado || implementacion_reto_numerico(p_datos, Definicion_retos::R_N_COMBO, p_datos.acc_max_tipo_combo(), N_COMBO);
	resultado=resultado || implementacion_reto_numerico(p_datos, Definicion_retos::R_N_VIDAS_PERDIDAS, p_datos.acc_total_vidas_perdidas(), N_VIDAS_PERDIDAS);
	resultado=resultado || implementacion_reto_numerico(p_datos, Definicion_retos::R_N_PARAR_TIEMPO, p_datos.acc_total_segundos_tiempo_detenido(), N_PARAR_TIEMPO);
	resultado=resultado || implementacion_reto_numerico(p_datos, Definicion_retos::R_N_TELETRANSPORTE, p_datos.acc_total_pixeles_teletransportados(), N_TELETRANSPORTE);
	resultado=resultado || implementacion_reto_numerico(p_datos, Definicion_retos::R_N_EDU, p_datos.acc_total_potencia_edu_usada(), N_EDU);
	resultado=resultado || implementacion_reto_numerico(p_datos, Definicion_retos::R_N_MINAS_DESTRUIDAS, p_datos.acc_total_minas_destruidas(), N_MINAS_DESTRUIDAS);

	return resultado;
}

/*Dispara manualmente el tema de conseguir un reto.*/

bool Controlador_retos::disparar_reto(unsigned int p_identificador_reto, Perfil_datos &p_datos)
{
	if(!p_datos.es_reto_conseguido(p_identificador_reto))
	{
		this->conseguir_reto(p_datos, p_identificador_reto);
		return true;
	}

	return false;
}

//void Controlador_retos::eliminar_primero_de_recien_cumplidos()
//{
//	if(this->retos_recien_cumplidos.size())
//	{
//		this->retos_recien_cumplidos.erase(this->retos_recien_cumplidos.begin());
//	}
//}

Reto * Controlador_retos::turno_controlador(float p_delta)
{
	if(this->retos_recien_cumplidos.size())
	{
		Reto * r=*(this->retos_recien_cumplidos.begin());

		if(!r->es_finalizado())
		{
			r->turno(p_delta);
		}
		else
		{
			this->retos_recien_cumplidos.erase(this->retos_recien_cumplidos.begin());			
			
			if(this->retos_recien_cumplidos.size())
			{
				r=*(this->retos_recien_cumplidos.begin());
			}
		}

		return r;
	}
	else
	{
		return NULL;
	}
}

Reto * Controlador_retos::obtener_por_indice(unsigned int p_indice)
{
	Reto * resultado=NULL;

	if(p_indice < this->vector_retos.size())
	{
		resultado=this->vector_retos[p_indice];
	}

	return resultado;
}

/*Este elimina un reto del vector de retos recién cumplidos (por ejemplo, porque
el reto ya se ha avisado y ha salido de la pantalla). Elimina el elemento del
vector pero no hace delete del Reto subyacente.
*/

//void Controlador_retos::eliminar_de_recien_cumplidos(Reto * p_reto)
//{
//	vt_vector_retos::iterator it=find(this->retos_recien_cumplidos.begin(), this->retos_recien_cumplidos.end(), p_reto);
//	if(it!=this->retos_recien_cumplidos.end()) this->retos_recien_cumplidos.erase(it);
//}
