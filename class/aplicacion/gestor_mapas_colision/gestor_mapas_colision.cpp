#include "gestor_mapas_colision.h"

bool Gestor_mapas_colision::preparado=false;
std::vector<DLibV::Mapa_colision_bitmap *> Gestor_mapas_colision::colisiones; //=std::vector<DLibV::Mapa_colision_bitmap *>();

void Gestor_mapas_colision::insertar(unsigned int p_indice, unsigned int p_r, unsigned int p_g, unsigned int p_b, SDL_Rect p_caja)
{
	DLibV::Mapa_colision_bitmap * temp=new DLibV::Mapa_colision_bitmap();
	temp->inicializar(DLibV::Gestor_recursos_graficos::obtener(p_indice)->acc_superficie(), DLibV::Gestor_color::color(p_r, p_g, p_b), p_caja);
	colisiones.push_back(temp);
}

void Gestor_mapas_colision::preparar()
{
	if(preparado) return;

	//La correspondencia aquí es completamente manual...

//	M_JUGADOR=0,
	insertar(Recursos::G_TILE_JUGADOR, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,32,32));

//	M_DIAGONAL_32_1,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,32,32));

//	M_DIAGONAL_32_2,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(32,0,32,32));

//	M_DIAGONAL_32_3,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(0,32,32,32));

//	M_DIAGONAL_32_4,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(32,32,32,32));

//	M_DIAGONAL_64_1,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(64,0,64,64));

//	M_DIAGONAL_64_2,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(128,0,64,64));

//	M_DIAGONAL_64_3,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(64,64,64,64));

//	M_DIAGONAL_64_4,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(128,64,64,64));

//	M_DIAGONAL_64H_1,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(128, 128, 64, 32));

//	M_DIAGONAL_64H_2,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(192, 128, 64, 32));

//	M_DIAGONAL_64H_3,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(128, 160, 64, 32));

//	M_DIAGONAL_64H_4,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(192, 160, 64, 32));

//	M_DIAGONAL_64V_1,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(192, 0, 32, 64));

//	M_DIAGONAL_64V_2,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(224, 0, 32, 64));

//	M_DIAGONAL_64V_3,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(192, 64, 32, 64));

//	M_DIAGONAL_64V_4,
	insertar(Recursos::G_TILE_COLISIONES, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(224, 64, 32, 64));

//	M_BOSS_01, Se usa sólo parte del mapa.
	insertar(Recursos::G_BOSS_01, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, 352, 256));

//	M_BOSS_02_CUERPO... Es multiparte.
	insertar(Recursos::G_BOSS_02, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, 256, 256));
//	M_BOSS_02_CUBIERTA_IZQ
//	insertar(Recursos::G_BOSS_02, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(256, 0, 64, 198));
//	M_BOSS_02_CUBIERTA_DER,
//	insertar(Recursos::G_BOSS_02, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(320, 0, 64, 198));
//	M_BOSS_02_PATA_INTERIOR_IZQ,
	insertar(Recursos::G_BOSS_02, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(384, 0, 120, 96));
//	M_BOSS_02_PATA_INTERIOR_DER,
	insertar(Recursos::G_BOSS_02, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(384, 96, 120, 96));
//	M_BOSS_02_PATA_EXTERIOR_IZQ,
	insertar(Recursos::G_BOSS_02, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(512, 0, 248, 128));
//	M_BOSS_02_PATA_EXTERIOR_DER,
	insertar(Recursos::G_BOSS_02, 0, 255, 255, DLibH::Herramientas_SDL::nuevo_sdl_rect(512, 128, 248, 128));

	preparado=true;
}

DLibV::Mapa_colision_bitmap const * Gestor_mapas_colision::obtener(unsigned int p_indice)
{
	if(!preparado) preparar();

	if(p_indice >= M_MAX) return NULL;
	else
	{
		return colisiones[p_indice];
	}
}

void Gestor_mapas_colision::finalizar()
{
	if(!preparado) return;

	preparado=false;

	std::vector<DLibV::Mapa_colision_bitmap *>::iterator ini=colisiones.begin(),
							fin=colisiones.end();

	while(ini < fin)
	{
		delete(*ini);
		ini++;
	}

	colisiones.clear();

}
