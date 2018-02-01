#ifndef GESTOR_MAPAS_COLISION
#define GESTOR_MAPAS_COLISION

#include <vector>
#include <iostream>
#include "../recursos/recursos.h"
#include "../../motor_sdl/video/mapa_colision_bitmap/mapa_colision_bitmap.h"
#include "../../motor_sdl/video/gestor_recursos_graficos/gestor_recursos_graficos.h"

/*El gestor de mapas de colisión no es más que una clase con un vector de 
mapas de colisión que el resto de clases usarán para sus colisiones. Tiene
además un enum con los nombres divertidos y un cierto rollo singleton.*/

class Gestor_mapas_colision
{
	private:

	Gestor_mapas_colision(){};
	~Gestor_mapas_colision()
	{	
		finalizar();
	}

	static std::vector<DLibV::Mapa_colision_bitmap *> colisiones;
	static bool preparado;
	static void insertar(unsigned int, unsigned int, unsigned int, unsigned int, SDL_Rect);

	public:

	enum mapas{
	M_JUGADOR=0,
	M_DIAGONAL_32_1,
	M_DIAGONAL_32_2,
	M_DIAGONAL_32_3,
	M_DIAGONAL_32_4,
	M_DIAGONAL_64_1,
	M_DIAGONAL_64_2,
	M_DIAGONAL_64_3,
	M_DIAGONAL_64_4,
	M_DIAGONAL_64_32_1,
	M_DIAGONAL_64_32_2,
	M_DIAGONAL_64_32_3,
	M_DIAGONAL_64_32_4,
	M_DIAGONAL_32_64_1,
	M_DIAGONAL_32_64_2,
	M_DIAGONAL_32_64_3,
	M_DIAGONAL_32_64_4,
	M_BOSS_01,
	M_BOSS_02_CUERPO,
//	M_BOSS_02_CUBIERTA_IZQ,
//	M_BOSS_02_CUBIERTA_DER,
	M_BOSS_02_PATA_INTERIOR_IZQ,
	M_BOSS_02_PATA_INTERIOR_DER,
	M_BOSS_02_PATA_EXTERIOR_IZQ,
	M_BOSS_02_PATA_EXTERIOR_DER,
		M_MAX};

	static DLibV::Mapa_colision_bitmap const * obtener(unsigned int);
	static void preparar();
	static void finalizar();
};

#endif
