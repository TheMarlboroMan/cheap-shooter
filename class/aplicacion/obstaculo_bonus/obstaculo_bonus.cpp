#include "obstaculo_bonus.h"

Obstaculo_bonus::Obstaculo_bonus(unsigned int mod_contador)
	:Actor_mapa(), tipo_bonus(B_ALEATORIO)

{
	this->preparar_actor(mod_contador);
}

Obstaculo_bonus::~Obstaculo_bonus()
{

}

void Obstaculo_bonus::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
	this->resistencia=30;
}

void Obstaculo_bonus::establecer_representacion()
{
	this->mut_indice_recurso(Recursos::G_TILE_JUEGO);
	this->establecer_representacion_bitmap_dinamica();
	Representable::establecer_posicion(this->posicion.x, this->posicion.y);
	this->establecer_recorte(0,0,this->w,this->h);
}

bool Obstaculo_bonus::procesar_colision_proyectil(Proyectil * proyectil)
{
	herir();
	this->resistencia-=proyectil->acc_potencia();
	if(this->resistencia < 0) this->resistencia=0;
	return this->resistencia==0;
}

void Obstaculo_bonus::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p) 
{
	tipo_bonus=p.param1;
}

short int Obstaculo_bonus::obtener_tirada() const
{
	switch(tipo_bonus)
	{
		case B_DISPARO: 		return Actor_mapa::T_BONUS_DISPARO; break;
		case B_TIEMPO:			return Actor_mapa::T_BONUS_TIEMPO; break;
		case B_VIDA:			return Actor_mapa::T_BONUS_VIDA; break;
		case B_PIMIENTO:		return Actor_mapa::T_BONUS_PIMIENTO; break;
		case B_NORMAL:			return Actor_mapa::T_BONUS_NORMAL; break;
		case B_CARGA_ESCUDO:		return Actor_mapa::T_BONUS_CARGA_ESCUDO; break;
		case B_COMPONENTE_ESCUDO:	return Actor_mapa::T_BONUS_COMPONENTE_ESCUDO; break;
		case B_ALEATORIO:  		
		default:
						return -1; break;
	}
}
