#ifndef RECURSOS_H
#define RECURSOS_H

#include <string>

/*
Esto es un índice de todos los recursos :D.
*/

class Recursos
{
	public:

	enum GRAFICOS{	
	G_FUENTE_1,
	G_FUENTE_2,
	G_FUENTE_3,
	G_TILE_COLISIONES,
	G_TILE_ITEMS,
	G_TILE_JUGADOR,
	G_TILE_JUEGO,
	G_TILE_JUEGO_IND,
	G_TITULO_1,
	G_TITULO_2,
	G_MENU,
	G_COMBO,
	G_FONDO_RETOS,
	G_ICONO_RETOS,
	G_RESUMEN_NIVELES,
	G_BOSS_01,
	G_BOSS_02,
		G_FIN_COMUNES,

		//Aquí irán todas las de los niveles.
		G_INI_NIVELES=32,
	G_TILE_NIV_01_A,	//Roca
	G_TILE_NIV_01_B,	//Roca trans.
	G_TILE_NIV_02_A,	//Metal
	G_TILE_NIV_02_B,	//Metal trans
	G_TILE_NIV_02_C,	//Metal detalles
	G_TILE_NIV_04_A,	//Hielo
	G_TILE_NIV_04_B,	//Hielo trans
	G_TILE_NIV_04_C,	//Hielo trans
	//Nuevas aquí... Ojo con mover las que ya están!!!. Deben quedarse donde están.	

		G_FIN_NIVELES,
	
		//Estas son especiales... Salen de transformaciones de otras...
		G_INI_TRANSFORMADAS=128, //128 por poner algo. Se guardará en un uint8 y lo usamos para que sea un nuevo comienzo fijo de modo que podamos añadir por delante y detrás.
	G_FUENTE_4,
	G_TILE_NIV_01_A_ALT_01,	//Roca marrón oscurecida.
	G_TILE_NIV_01_B_ALT_01, //Roca marrón transparente oscurecida.
	G_TILE_NIV_02_A_ALT_01,	//Metal oscurecida.
	G_TILE_NIV_02_B_ALT_01, //Metal transparente oscurecida.
	G_TILE_NIV_03_A,	//Roca verdosa.
	G_TILE_NIV_03_B,	//Roca verdosa transparente.
	G_TILE_NIV_03_A_ALT_01,	//Roca verdosa oscurecida.
	G_TILE_NIV_03_B_ALT_01,	//Roca verdosa oscurecida transparente.
	G_TILE_NIV_02_C_ALT_01, //Metal detalles oscurecida.
	G_FUENTE_5,
	G_TILE_NIV_04_A_ALT_01,	//Hielo oscurecido
	G_TILE_NIV_04_B_ALT_01, //Hielo oscurecido trans.
	G_TILE_NIV_04_C_ALT_01, //Hielo oscurecido trans.
	//Nuevas aquí... Ojo con mover las que ya están!!!. Deben quedarse donde están.

		G_FIN_TRANSFORMADAS
	};
	
	enum SONIDOS{
	S_DISPARO,
	S_I_PHASER,
	S_I_PHASER_02,
	S_ITEM,
	S_PHASER,
	S_RECARGA,
	S_EXPLOSION,
	S_GRAN_LASER,
	S_GRAN_LASER_OFF,
	S_WHOOSH_01,
	S_WHOOSH_02,
	S_ESCUDO_ON,
	S_ESCUDO_OFF,
		S_MAX
	};

	enum MUSICAS{
	M_MUSICA_01,
		M_MAX
	};

	std::string static musica(int p_valor)
	{
		std::string resultado="";
	
		switch(p_valor)
		{
			default:
			case M_MUSICA_01:
				resultado="musica.ogg"; 
			break;
		}

		return resultado;
	}

	std::string static sonido(int p_valor)
	{
		std::string resultado="";
		
		switch(p_valor)
		{
			case S_DISPARO:		resultado="disparo.wav"; 	break;
			case S_I_PHASER:	resultado="i_phaser.wav"; 	break;
			case S_I_PHASER_02:	resultado="i_phaser_02.wav"; 	break;
			case S_ITEM:		resultado="item.wav"; 		break;
			case S_PHASER:		resultado="phaser.wav"; 	break;
			case S_EXPLOSION:	resultado="explosion.wav"; 	break;
			case S_GRAN_LASER:	resultado="gran_laser.wav"; 	break;
			case S_GRAN_LASER_OFF:	resultado="gran_laser_off.wav"; break;
			case S_WHOOSH_01:	resultado="whoosh_01.wav"; 	break;
			case S_WHOOSH_02:	resultado="whoosh_02.wav"; 	break;
			case S_ESCUDO_ON:	resultado="escudo_on.wav"; 	break;
			case S_ESCUDO_OFF:	resultado="escudo_off.wav"; 	break;
			default:	
			case S_RECARGA:		resultado="recarga.wav"; 	break;
		}

		return resultado;
	}

	std::string static grafico(int p_valor)
	{
		std::string resultado="";

		switch(p_valor)
		{
			case G_FUENTE_1: 	resultado="fuente.png"; 	break;
			case G_FUENTE_2: 	resultado="fuente_02.png"; 	break;
			case G_FUENTE_3: 	resultado="fuente_03.png"; 	break;
			case G_TILE_COLISIONES: resultado="mapa_colision.png"; 	break;
			case G_TILE_ITEMS:	resultado="items.png";		break;
			case G_TILE_JUGADOR:	resultado="tile_1.png";		break;
			case G_TILE_JUEGO:	resultado="tile_2.png"; 	break;
			case G_TILE_JUEGO_IND:	resultado="tile_3.png"; 	break;
			case G_TITULO_1:	resultado="titulo_1.png"; 	break;						
			case G_TITULO_2:	resultado="titulo_2.png";	break;
			case G_COMBO:		resultado="combos.png";		break;
			case G_FONDO_RETOS:	resultado="fondo_reto.png"; 	break;
			case G_ICONO_RETOS:	resultado="ico_reto.png"; 	break;
			case G_TILE_NIV_01_A:	resultado="tile_n01_a.png"; 	break;
			case G_TILE_NIV_01_B:	resultado="tile_n01_b.png"; 	break;
			case G_TILE_NIV_02_A:	resultado="tile_n02_a.png"; 	break;
			case G_TILE_NIV_02_B:	resultado="tile_n02_b.png"; 	break;
			case G_TILE_NIV_02_C:	resultado="tile_n02_c.png"; 	break;
			case G_TILE_NIV_04_A:	resultado="tile_n04_a.png"; 	break;
			case G_TILE_NIV_04_B:	resultado="tile_n04_b.png"; 	break;
			case G_TILE_NIV_04_C:	resultado="tile_n04_c.png"; 	break;
			case G_RESUMEN_NIVELES: resultado="resumen_niveles.png"; break;
			case G_BOSS_01: resultado="tile_boss_1.png"; break;
			case G_BOSS_02: resultado="tile_boss_2.png"; break;

			default:
			case G_MENU:		resultado="menu.png";		break;

		}

		return resultado;
	}
};

#endif
