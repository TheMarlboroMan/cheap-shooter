#include "representacion_agrupada.h"

using namespace DLibV;

Representacion_agrupada::Representacion_agrupada(bool p_poseer):
	posee_las_representaciones(p_poseer)
{
	this->reiniciar_posicion();
	this->reiniciar_recorte();
}

Representacion_agrupada::Representacion_agrupada(const Representacion_agrupada& p_otra):
	posee_las_representaciones(p_otra.posee_las_representaciones)
{
	if(!this->posee_las_representaciones)
	{
		this->grupo=p_otra.grupo;
	}

	this->reiniciar_posicion();
	this->reiniciar_recorte();
}

Representacion_agrupada::~Representacion_agrupada()
{
	if(this->posee_las_representaciones)
	{
		std::vector<Representacion *>::iterator ini=grupo.begin(),
						fin=grupo.end();
		while(ini < fin)
		{
			delete (*ini);
			ini++;
		};

		grupo.clear();
	}
}

//Ojo con poseer las representaciones y hacer una copia!!!!.
/*
Representacion_agrupada& Representacion_agrupada::operator=(const Representacion_agrupada & p_otra)
{
	Representacion::operator=(p_otra);

	if(!posee_las_representaciones)
	{
		this->grupo=p_otra.grupo;
	}
	
	return *this;
}
*/
/*La posición que demos a la representación que se pasa se sumará luego a 
la posición del grupo. Para poner algo en la esquina superior izquierda del 
grupo estableceríamos la posición de rep en 0,0.
El alpha del grupo se aplica directamente a cada componente siempre que tenga
un valor distinto de "opaco".
Sería posible hacerlo "acumulativo" sin muchos problemas de modo que los valores
se sumen y resten dentro del rango 0-255.
*/

bool Representacion_agrupada::volcado(SDL_Surface * p_superficie)
{
	if(!this->es_visible())
	{
		return false;
	}

	std::vector<Representacion *>::iterator 	inicio=this->grupo.begin(),
							fin=this->grupo.end();

	SDL_Rect pos=acc_posicion();
	Uint8 alpha_original=0;
	int px=pos.x, py=pos.y;
	Uint8 alpha_propio=this->acc_alpha();

	while(inicio < fin)
	{
		(*inicio)->desplazar(px, py);
		if(alpha_propio!=SDL_ALPHA_OPAQUE)
		{
			alpha_original=(*inicio)->acc_alpha();
			(*inicio)->establecer_alpha(alpha_propio);
		}

		(*inicio)->volcar(p_superficie);

		(*inicio)->desplazar(-px, -py);
		if(alpha_propio!=SDL_ALPHA_OPAQUE)
		{
			(*inicio)->establecer_alpha(alpha_original);
		}

		++inicio;
	}

	return true;
}

bool Representacion_agrupada::volcado(SDL_Surface * p_superficie, const SDL_Rect& p_vista, const SDL_Rect& p_posicion, int desp_x, int desp_y)
{
	if(!this->es_visible())
	{
		return false;
	}

	std::vector<Representacion *>::iterator 	inicio=this->grupo.begin(),
							fin=this->grupo.end();

	SDL_Rect pos=acc_posicion();
	Uint8 alpha_original=0;
	int px=pos.x, py=pos.y;
	Uint8 alpha_propio=this->acc_alpha();

	while(inicio < fin)
	{
		(*inicio)->desplazar(px, py);
		if(alpha_propio!=SDL_ALPHA_OPAQUE)
		{
			alpha_original=(*inicio)->acc_alpha();
			(*inicio)->establecer_alpha(alpha_propio);
		}

		(*inicio)->volcar(p_superficie, p_vista, p_posicion, desp_x, desp_y);

		(*inicio)->desplazar(-px, -py);
		if(alpha_propio!=SDL_ALPHA_OPAQUE)
		{
			(*inicio)->establecer_alpha(alpha_original);
		}

		++inicio;
	}

	return true;
}

bool Representacion_agrupada::insertar_representacion(Representacion * p_rep)
{
	this->grupo.push_back(p_rep);
	return true;

}
