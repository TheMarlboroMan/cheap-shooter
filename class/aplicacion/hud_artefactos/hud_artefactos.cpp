#include "hud_artefactos.h"

Hud_artefactos::Hud_artefactos():rep(false)
{
	mapa.insert(t_parte(Definiciones::A_CANON, Parte_hud()));
	mapa.insert(t_parte(Definiciones::A_TELETRANSPORTADOR, Parte_hud()));
	mapa.insert(t_parte(Definiciones::A_EMISOR_DENSIDAD_UNIFICADA, Parte_hud()));
	mapa.insert(t_parte(Definiciones::A_ESCUDO, Parte_hud()));

	mapa[Definiciones::A_CANON].posicionar(0);
	mapa[Definiciones::A_TELETRANSPORTADOR].posicionar(8);
	mapa[Definiciones::A_EMISOR_DENSIDAD_UNIFICADA].posicionar(16);
	mapa[Definiciones::A_ESCUDO].posicionar(24);

	rep.insertar_representacion(&mapa[Definiciones::A_CANON].rep);
	rep.insertar_representacion(&mapa[Definiciones::A_TELETRANSPORTADOR].rep);
	rep.insertar_representacion(&mapa[Definiciones::A_EMISOR_DENSIDAD_UNIFICADA].rep);
	rep.insertar_representacion(&mapa[Definiciones::A_ESCUDO].rep);
}

Hud_artefactos::~Hud_artefactos()
{

}

bool Hud_artefactos::comprobar_artefacto(unsigned int a)
{
	return a==Definiciones::A_CANON || 
		a==Definiciones::A_TELETRANSPORTADOR || 
		a==Definiciones::A_EMISOR_DENSIDAD_UNIFICADA || 
		a==Definiciones::A_ESCUDO;	
}

void Hud_artefactos::activar_artefacto(unsigned int a)
{
	if(!comprobar_artefacto(a)) return;
	else mapa[a].activar();
}

void Hud_artefactos::desactivar_artefacto(unsigned int a)
{
	if(!comprobar_artefacto(a)) return;
	else mapa[a].desactivar();
}

void Hud_artefactos::anular_artefacto(unsigned int a)
{
	if(!comprobar_artefacto(a)) return;
	else mapa[a].anular();
}
