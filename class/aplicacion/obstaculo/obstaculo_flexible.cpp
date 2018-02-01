#include "obstaculo_flexible.h"

Obstaculo_flexible::Obstaculo_flexible(unsigned int mod_contador, unsigned int p_tipo, bool p_desaparece, bool p_edu)
	:Obstaculo(), 
	tipo_actor_flexible(p_tipo), desaparece_colision(p_desaparece), 
	afectado_edu(p_edu)
{
	this->preparar_actor(mod_contador);
}

Obstaculo_flexible::~Obstaculo_flexible()
{

}

/*Auí se establecen las dimensiones y al final el mapa de colisión...*/

void Obstaculo_flexible::establecer_caracteristicas()
{
	int w=0, h=0, desp_rep_x=0, desp_rep_y=0;

	switch(this->tipo_actor_flexible)
	{
		case T_OBSTACULO_FLEX:
			w=32;
			h=32;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_1:
		case T_OBSTACULO_FLEX_DIAGONAL_32_2:
		case T_OBSTACULO_FLEX_DIAGONAL_32_3:
		case T_OBSTACULO_FLEX_DIAGONAL_32_4:
			w=32;
			h=32;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_32_1:
		case T_OBSTACULO_FLEX_DIAGONAL_64_32_2:
		case T_OBSTACULO_FLEX_DIAGONAL_64_32_3:
		case T_OBSTACULO_FLEX_DIAGONAL_64_32_4:
			w=64;
			h=32;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_64_1:
		case T_OBSTACULO_FLEX_DIAGONAL_32_64_2:
		case T_OBSTACULO_FLEX_DIAGONAL_32_64_3:
		case T_OBSTACULO_FLEX_DIAGONAL_32_64_4:
			w=32;
			h=64;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_1:
		case T_OBSTACULO_FLEX_DIAGONAL_64_2:
		case T_OBSTACULO_FLEX_DIAGONAL_64_3:
		case T_OBSTACULO_FLEX_DIAGONAL_64_4:
			w=64;
			h=64;
		break;
	}

	this->w=w;
	this->h=h;
	this->posicion.w=w;
	this->posicion.h=h;
	this->establecer_desp_rep_x(desp_rep_x);
	this->establecer_desp_rep_y(desp_rep_y);
	this->establecer_mapa_colision();

}

/*Aquí se establecen las representaciones por defecto para cada tipo.
Lo que se hace es establecer los valores por defecto y luego llamar al 
método que hace uso de ellos.
*/

void Obstaculo_flexible::establecer_representacion()
{
	unsigned int indice_recurso=0;
	SDL_Rect caja_recorte=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0);

	caja_recorte.w=this->w;
	caja_recorte.h=this->h;

	switch(this->tipo_actor_flexible)
	{
		case T_OBSTACULO_FLEX:
			caja_recorte.x=0;
			caja_recorte.y=0;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_1:
			caja_recorte.x=32;
			caja_recorte.y=0;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_2:
			caja_recorte.x=64;
			caja_recorte.y=0;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_3:
			caja_recorte.x=32;
			caja_recorte.y=32;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_4:
			caja_recorte.x=64;
			caja_recorte.y=32;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_1:
			caja_recorte.x=128;
			caja_recorte.y=0;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_2:
			caja_recorte.x=192;
			caja_recorte.y=0;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_3:
			caja_recorte.x=128;
			caja_recorte.y=64;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_4:
			caja_recorte.x=192;
			caja_recorte.y=64;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_64_1:
			caja_recorte.x=256;
			caja_recorte.y=0;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_64_2:
			caja_recorte.x=288;
			caja_recorte.y=0;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_64_3:
			caja_recorte.x=256;
			caja_recorte.y=64;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_64_4:
			caja_recorte.x=288;
			caja_recorte.y=64;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_32_1:
			caja_recorte.x=0;
			caja_recorte.y=64;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_32_2:
			caja_recorte.x=64;
			caja_recorte.y=64;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_32_3:
			caja_recorte.x=0;
			caja_recorte.y=96;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_32_4:
			caja_recorte.x=64;
			caja_recorte.y=96;
			indice_recurso=Recursos::G_TILE_JUEGO_IND;
		break;
	}

	Representable::mut_indice_recurso(indice_recurso);
	this->crear_representacion(caja_recorte);
}

void Obstaculo_flexible::establecer_mapa_colision()
{
	switch(this->tipo_actor_flexible)
	{
		case T_OBSTACULO_FLEX_DIAGONAL_32_1:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_32_1);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_2:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_32_2);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_3:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_32_3);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_4:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_32_4);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_1:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_64_1);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_2:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_64_2);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_3:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_64_3);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_4:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_64_4);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_64_1:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_32_64_1);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_64_2:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_32_64_2);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_64_3:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_32_64_3);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_32_64_4:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_32_64_4);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_32_1:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_64_32_1);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_32_2:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_64_32_2);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_32_3:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_64_32_3);
		break;

		case T_OBSTACULO_FLEX_DIAGONAL_64_32_4:
			this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_DIAGONAL_64_32_4);
		break;
	}
}

/*Recibe el recorte porque se ha calculado antes pero no puede aplicarse
hasta que la representación haya sido creada...*/

void Obstaculo_flexible::crear_representacion(const SDL_Rect& caja_recorte)
{
//std::cout<<"HOLA"<<std::endl;

	if(this->con_representacion()) return;
	else
	{
//std::cout<<"EST REP"<<std::endl;

		switch(this->tipo_actor_flexible)
		{
			case T_OBSTACULO_FLEX:
			{	
				SDL_Rect patron=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, 32, 32);
				this->establecer_representacion_patron_dinamica(this->posicion, patron, DLibV::Gestor_color::color(0, 255, 255), Recursos::G_TILE_JUEGO_IND);
			}
			break;

			default:
				this->establecer_representacion_bitmap_dinamica();
				Representable::establecer_posicion(this->posicion.x, this->posicion.y);
			break;
		}
		
		Representable::establecer_recorte(caja_recorte);	
		this->actualizar_representacion();
	}
}	

void Obstaculo_flexible::actualizar_representacion()
{
	Representable::establecer_posicion(this->posicion);
}

void Obstaculo_flexible::refrescar_propiedades()
{
	this->actualizar_representacion();
	this->recargar_posicion();	
}

void Obstaculo_flexible::procesar_cambio_dimensiones()
{
	this->actualizar_representacion();
	this->recargar_posicion();	
}

void Obstaculo_flexible::recibir_valores_actor_redimensionable(const Parametros_actor_redimensionable& red)
{
	Actor_mapa::recibir_valores_actor_redimensionable(red);
	this->refrescar_propiedades();
}
