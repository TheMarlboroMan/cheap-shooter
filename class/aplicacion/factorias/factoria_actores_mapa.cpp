#include "factoria_actores_mapa.h"

Actor_mapa * Factoria_actores_mapa::fabricar(unsigned int p_tipo, unsigned int aux)
{
	Actor_mapa * temp=NULL;

	switch(p_tipo)
	{
		case Actor_mapa::T_BONUS_DISPARO: 			temp=new Bonus_disparo(aux); break;
		case Actor_mapa::T_BONUS_TIEMPO:			temp=new Bonus_tiempo(aux); break;
		case Actor_mapa::T_BONUS_VIDA:				temp=new Bonus_vida(aux); break;
		case Actor_mapa::T_BONUS_PIMIENTO:			temp=new Bonus_pimiento(aux); break;
		case Actor_mapa::T_BONUS_NORMAL:			temp=new Bonus_normal(aux); break;
		case Actor_mapa::T_BONUS_COMPONENTE_ESCUDO:		temp=new Bonus_componente_escudo(aux); break;
		case Actor_mapa::T_BONUS_CARGA_ESCUDO:			temp=new Bonus_carga_escudo(aux); break;

		case Actor_mapa::T_OBSTACULO_PROPIEDADES:		temp=new Obstaculo_propiedades(); break;
		case Actor_mapa::T_OBSTACULO_VECTOR:			temp=new Obstaculo_vector(); break;
		case Actor_mapa::T_OBSTACULO_PROPIEDADES_FONDO:		temp=new Obstaculo_propiedades_fondo(); break;
		case Actor_mapa::T_OBSTACULO_TRIGGER_CONTACTO:	temp=new Obstaculo_trigger_contacto(); break;
		case Actor_mapa::T_OBSTACULO_TRIGGER_INTERRUPTOR:	temp=new Obstaculo_trigger_interruptor(); break;
		case Actor_mapa::T_PUNTO_RUTA:			 	temp=new Punto_ruta(); break;
		case Actor_mapa::T_OBSTACULO_ACTIVIDAD_GENERADOR_DECORACION: temp=new Obstaculo_generador_decoracion(); break;
		case Actor_mapa::T_OBSTACULO_ACTIVIDAD_GENERADOR_OBSTACULOS: temp=new Obstaculo_generador_obstaculos(); break;
		case Actor_mapa::T_OBSTACULO_PROPIEDADES_MOVIMIENTO_RELATIVO:	temp=new Obstaculo_propiedades_movimiento_relativo(); break;

		case Actor_mapa::T_OBSTACULO_800_32:				temp=new Obstaculo_800_32(aux); break;
		case Actor_mapa::T_OBSTACULO_BONUS:				temp=new Obstaculo_bonus(aux); break;
		case Actor_mapa::T_OBSTACULO_DESTRUIBLE:			temp=new Obstaculo_destruible(aux); break;
		case Actor_mapa::T_OBSTACULO_DESTRUIBLE_SECRETO:		temp=new Obstaculo_destruible_secreto(aux); break;
		case Actor_mapa::T_OBSTACULO_ACTIVIDAD_GRAVEDAD:		temp=new Obstaculo_gravedad(aux); break;
		case Actor_mapa::T_OBSTACULO_ACTIVIDAD_DISPARADOR:		temp=new Obstaculo_disparador(aux); break;
		case Actor_mapa::T_OBSTACULO_ACTIVIDAD_EXPLOSIVO:		temp=new Obstaculo_explosivo(aux); break;
		case Actor_mapa::T_OBSTACULO_ACTIVIDAD_CANON_PROGRAMABLE: 	temp=new Obstaculo_canon_programable(aux); break;
		case Actor_mapa::T_OBSTACULO_ACTIVIDAD_CANON_GIRATORIO: 	temp=new Obstaculo_canon_giratorio(aux); break;
		case Actor_mapa::T_OBSTACULO_ACTIVIDAD_CANON_APUNTADO: 		temp=new Obstaculo_canon_apuntado(); break;
		case Actor_mapa::T_OBSTACULO_ACTIVIDAD_PUERTA_DISPARAR: 	temp=new Obstaculo_puerta_disparar(aux); break;
		case Actor_mapa::T_OBSTACULO_ACTIVIDAD_PUERTA_AUTOMATICA: 	temp=new Obstaculo_puerta_automatica(aux); break;
		case Actor_mapa::T_OBSTACULO_ACTIVIDAD_PUERTA_REMOTA: 		temp=new Obstaculo_puerta_remota(aux); break;
		case Actor_mapa::T_OBSTACULO_ACTIVIDAD_BOSS_01: 		temp=new Boss_01(); break;
		case Actor_mapa::T_OBSTACULO_ACTIVIDAD_BOSS_02: 		temp=new Boss_02(); break;
		case Actor_mapa::T_OBSTACULO_TRIGGER_AUTO:	 		temp=new Obstaculo_trigger_auto(); break;
		case Actor_mapa::T_OBSTACULO_TUTORIAL:		 		temp=new Obstaculo_tutorial(); break;
		case Actor_mapa::T_OBSTACULO_COLOCADOR_JUGADOR:	 		temp=new Obstaculo_colocador_jugador(); break;

		case Actor_mapa::T_TRAZADOR_FANTASMA:	 			temp=new Trazador_fantasma(); break;

		//Estos son obstáculos aleatorios...
		case Actor_mapa::T_OBSTACULO_26_26:			temp=new Obstaculo_aleatorio(aux, Actor_mapa::T_OBSTACULO_26_26, false, true); break;
		case Actor_mapa::T_OBSTACULO_32_64:			temp=new Obstaculo_aleatorio(aux, Actor_mapa::T_OBSTACULO_32_64, false, true); break;
		case Actor_mapa::T_OBSTACULO_64_26:			temp=new Obstaculo_aleatorio(aux, Actor_mapa::T_OBSTACULO_64_26, false, true); break;
		case Actor_mapa::T_OBSTACULO_128_32:			temp=new Obstaculo_aleatorio(aux, Actor_mapa::T_OBSTACULO_128_32, false, true); break;

		//Estos son flexibles del todo - sin edu.
		case Actor_mapa::T_OBSTACULO_FLEX:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_1:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_2:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_3:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_4:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_1:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_2:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_3:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_4:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_64_1:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_64_2:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_64_3:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_32_64_4:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_32_1:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_32_2:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_32_3:
		case Actor_mapa::T_OBSTACULO_FLEX_DIAGONAL_64_32_4:	
			temp=new Obstaculo_flexible(aux, p_tipo, false, false); 
		break;

		case Actor_mapa::T_DECORACION_NIVEL_FONDO: 	temp=new Decoracion_nivel_fondo(); break;
		case Actor_mapa::T_DECORACION_NIVEL_FRENTE:	temp=new Decoracion_nivel_frente(); break;

		default: break;
	}

	return temp;
}
