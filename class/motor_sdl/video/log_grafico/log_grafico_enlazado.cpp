#include "log_grafico_enlazado.h"

using namespace DLibV;

Log_grafico_enlazado::Log_grafico_enlazado(const Recurso_grafico * p_r, const SDL_Rect& p_c, DLibH::Log_base& p_l)
	:Log_grafico(p_r, p_c), log(p_l)
{

}

Log_grafico_enlazado::~Log_grafico_enlazado()
{
	
}

void Log_grafico_enlazado::procesar()
{
	if(log.es_cadena_actualizada())
	{
		Log_grafico::insertar(log.cadena());
		log.limpiar_cadena();
	}
}
