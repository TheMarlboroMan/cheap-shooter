#include "boss_01.h"

const float Boss_01::DURACION_MS_INICIO=1.800;
const float Boss_01::DURACION_MS_CANONES_VERTICALES=1.500;
const float Boss_01::DURACION_MS_PAUSA_1=1.800;
const float Boss_01::DURACION_MS_CANONES_DIAGONALES=1.500;
const float Boss_01::DURACION_MS_PAUSA_2=1.800;
const float Boss_01::DURACION_MS_CANON_GIRATORIO=5.000;
const float Boss_01::DURACION_MS_PAUSA_3=2.000;
const float Boss_01::MS_PAUSA_CANONES_VERTICALES=0.075;
const float Boss_01::MS_PAUSA_CANONES_DIAGONALES=0.075;
const float Boss_01::MS_PAUSA_CANON_CIRCULAR=0.045;

Boss_01::Boss_01():Boss(),
	estado(E_INICIO), 
	tiempo_ms_estado(0.0),
	angulo_disparo_canon_circular(0),
	salto_angulo_disparo_canon_circular(5),
	pausa_entre_disparos(0.0), 
	direccion(1),
	punto_debil_izq(RESISTENCIA_PUNTOS_DEBILES_LATERALES, true),
	punto_debil_der(RESISTENCIA_PUNTOS_DEBILES_LATERALES, true),
	punto_debil_central(RESISTENCIA_PUNTO_DEBIL_CENTRAL, false)
{
	this->preparar_actor(0);

	//Actualizamos las medidas de las cajas de colisión. La posición permanecerá
	//indefinida hasta que intentemos colocar el impacto.

	punto_debil_izq.caja.w=W_PUNTO_DEBIL;
	punto_debil_izq.caja.h=H_PUNTO_DEBIL;

	punto_debil_der.caja.w=W_PUNTO_DEBIL;
	punto_debil_der.caja.h=H_PUNTO_DEBIL;

	punto_debil_central.caja.w=W_PUNTO_DEBIL;
	punto_debil_central.caja.h=H_PUNTO_DEBIL;
}

Boss_01::~Boss_01()
{
	delete rep_general;
	delete rep_punto_debil_izq;
	delete rep_punto_debil_der;
	delete rep_punto_debil_central;
}

/*Se mueve de izquierda a derecha y se mantiene parado mientras dispara...
No hay mucho más. */

void Boss_01::controlar_direccion()
{
	switch(estado)
	{
		case E_CANONES_VERTICALES:
		case E_CANONES_DIAGONALES:
		case E_CANON_GIRATORIO:
			vector_mov.x=0;
		break;

		case E_CAYENDO:
			vector_mov.x=VELOCIDAD_X_DESTRUIDO;
			vector_mov.y=VELOCIDAD_Y_DESTRUIDO;
		break;

		case E_EXPLOSION:
		case E_TRIGGER:
		case E_DESTRUIDO:
			vector_mov.x=0;
		break;

		default:
			if(x <= 0) direccion=1;
			else if(x+w >= Medidas::ANCHO_VISTA) direccion=-1;
			vector_mov.x=VELOCIDAD_X * direccion;
		break;
	}
}

/*Tiene varios estados secuenciales y pasa cierto tiempo en cada uno de ellos.
Según el estado disparará de una u otra forma, o no hará nada.*/

void Boss_01::controlar_estado(float p_delta)
{
	tiempo_ms_estado+=p_delta;
	pausa_entre_disparos+=p_delta;
	float t_max=0;

	switch(estado)
	{
		case E_INICIO: t_max=DURACION_MS_INICIO; break;
		case E_CANONES_VERTICALES: t_max=DURACION_MS_CANONES_VERTICALES; break;
		case E_PAUSA_1: t_max=DURACION_MS_PAUSA_1; break;
		case E_CANONES_DIAGONALES: t_max=DURACION_MS_CANONES_DIAGONALES; break;
		case E_PAUSA_2: t_max=DURACION_MS_PAUSA_2; break;
		case E_CANON_GIRATORIO: t_max=DURACION_MS_CANON_GIRATORIO; break;
		case E_PAUSA_3: t_max=DURACION_MS_PAUSA_2; break;
		case E_CAYENDO:
			if(!es_en_pantalla()) 
			{
				//Si no va a haber acción ninguna podemos deshacernos ya de él.
				if(!Trigger::acc_tag_accion()) 
				{
					marcar_para_borrar();
				}
				//Si hay acción entonces lo pondremos en su estado... Ojo, como
				//sobreescribe "es_fuera_de_juego" no será eliminado al salir
				//de la pantalla por debajo.
				else
				{
					estado=E_TRIGGER;
				}
			}
			return;
		break;
		case E_EXPLOSION:
			//Nada...
			return;
		break;
		case E_TRIGGER:
			//En este estado no hace nada pero el controlador lanzaría la acción por él
			//y pasaría a estar destruido. Cuando reciba el resultado de la acción
			//pasará a ser marcado para borrar.
			return;
		break;

		case E_DESTRUIDO:
			//Aquí no hace nada.
			return;
		break;
	}

	if(tiempo_ms_estado >= t_max)
	{
		pausa_entre_disparos=0.0;
		tiempo_ms_estado=0.0;
		++estado;
		if(estado==E_MAX_ESTADO) estado=E_INICIO;
	}
}

void Boss_01::procesar_turno(float p_delta)
{
	//Usamos el normal...
	Obstaculo_actividad::procesar_turno(p_delta);
	p_delta/=divisor_movimiento;

	punto_debil_izq.turno(p_delta);
	punto_debil_der.turno(p_delta);
	punto_debil_central.turno(p_delta);

	controlar_posicion_puntos_debiles();
	controlar_direccion();
	controlar_estado(p_delta);
	controlar_representacion();

	//Si es posible disparar o generar decoraciones periódicas, será aquí.
	evaluar_y_disparar();
	evaluar_y_generar_decoraciones();
}

void Boss_01::controlar_posicion_puntos_debiles()
{
	//Actualizamos las cajas con respecto a la posición.
	punto_debil_izq.caja.x=OFFSET_X_PUNTO_DEBIL_1+this->x;
	punto_debil_izq.caja.y=OFFSET_Y_PUNTO_DEBIL_1+this->y;
	punto_debil_der.caja.x=OFFSET_X_PUNTO_DEBIL_2+this->x;
	punto_debil_der.caja.y=OFFSET_Y_PUNTO_DEBIL_2+this->y;
	punto_debil_central.caja.x=OFFSET_X_PUNTO_DEBIL_3+this->x;
	punto_debil_central.caja.y=OFFSET_Y_PUNTO_DEBIL_3+this->y;
}

/*Hay que destruir en primer lugar los dos puntos débiles de la izquierda y 
derecha. Luego es posible destruir el central. Cuando ha terminado pasa al 
estado destruido, que activaría otra animación...*/

void Boss_01::comprobar_destruccion()
{
	if(punto_debil_izq.destruido && punto_debil_der.destruido)
	{
		punto_debil_central.vulnerable=true;
		if(punto_debil_central.destruido)
		{
			estado=E_EXPLOSION;
		}
	}
}

/*Cuando se produce una colisión con un proyectil puede no chocar con ningún
punto débil o puede chocar con uno de ellos, en cuyo caso es el punto el que 
procesa la colisión...*/

bool Boss_01::procesar_colision_proyectil(Proyectil * proyectil)
{
	//En función de la posición del proyectil habrá dado en un sitio u otro.
	unsigned short int pos=obtener_posicion_impacto(*proyectil);

	switch(pos)
	{
		case POS_NADA: break;

		case POS_DEBIL_IZQ: 
			if(punto_debil_izq.vulnerable && !punto_debil_izq.destruido)
			{
				if(punto_debil_izq.alcanzar(proyectil->acc_potencia())) 
				{
					insertar_decoracion(Estructura_decoracion_boss(
								OFFSET_X_PUNTO_DEBIL_1+this->x,
								OFFSET_Y_PUNTO_DEBIL_1+this->y,
								Estructura_decoracion_boss::ED_EXPLOSION,
								10, 0, 0,
								(W_PUNTO_DEBIL / 2), (H_PUNTO_DEBIL / 2)));
					comprobar_destruccion();
				}
			}
		break;

		case POS_DEBIL_DER: 
			if(punto_debil_der.vulnerable && !punto_debil_der.destruido)
			{
				if(punto_debil_der.alcanzar(proyectil->acc_potencia())) 
				{
					insertar_decoracion(Estructura_decoracion_boss(
								OFFSET_X_PUNTO_DEBIL_2+this->x,
								OFFSET_Y_PUNTO_DEBIL_2+this->y,
								Estructura_decoracion_boss::ED_EXPLOSION,
								10, 0, 0,
								(W_PUNTO_DEBIL / 2), (H_PUNTO_DEBIL / 2)));
					comprobar_destruccion();
				}
			}
		break;

		case POS_DEBIL_CENTRAL: 
			if(punto_debil_central.vulnerable && !punto_debil_central.destruido)
			{
				if(punto_debil_central.alcanzar(proyectil->acc_potencia()))
				{
					insertar_decoracion(Estructura_decoracion_boss(
								OFFSET_X_PUNTO_DEBIL_3+this->x,
								OFFSET_Y_PUNTO_DEBIL_3+this->y,
								Estructura_decoracion_boss::ED_EXPLOSION,
								10, 0, 0,
								(W_PUNTO_DEBIL / 2), (H_PUNTO_DEBIL / 2)));
					comprobar_destruccion();
				}
			}
		break;
	}

	return false;
}

/*A cada paso comprobamos cómo debemos dibujar cada uno de los puntos débiles...*/

void Boss_01::controlar_representacion()
{
	rep_punto_debil_izq->establecer_recorte(punto_debil_izq.obtener_x_representacion(), Y_REP_PUNTOS_DEBILES, W_REP_PUNTOS_DEBILES, H_REP_PUNTOS_DEBILES);
	rep_punto_debil_der->establecer_recorte(punto_debil_der.obtener_x_representacion(), Y_REP_PUNTOS_DEBILES, W_REP_PUNTOS_DEBILES, H_REP_PUNTOS_DEBILES);
	rep_punto_debil_central->establecer_recorte(punto_debil_central.obtener_x_representacion(), Y_REP_PUNTOS_DEBILES, W_REP_PUNTOS_DEBILES, H_REP_PUNTOS_DEBILES);
}

/*Tiene una representación agrupada en cuatro partes: la central y los puntos
débiles...*/

void Boss_01::establecer_representacion()
{
	//El fondo...
	rep_general=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_01));
	rep_general->establecer_recorte(0, 0, w, h);
	rep_general->establecer_posicion(0, 0);

	//Los puntos débiles...
	rep_punto_debil_izq=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_01));
	rep_punto_debil_izq->establecer_recorte(0, 0, W_REP_PUNTOS_DEBILES, H_REP_PUNTOS_DEBILES);
	rep_punto_debil_izq->establecer_posicion(OFFSET_X_PUNTO_DEBIL_1, OFFSET_Y_PUNTO_DEBIL_1);

	rep_punto_debil_der=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_01));
	rep_punto_debil_der->establecer_recorte(0, 0, W_REP_PUNTOS_DEBILES, H_REP_PUNTOS_DEBILES);
	rep_punto_debil_der->establecer_posicion(OFFSET_X_PUNTO_DEBIL_2, OFFSET_Y_PUNTO_DEBIL_2);

	rep_punto_debil_central=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_01));
	rep_punto_debil_central->establecer_recorte(0, 0, W_REP_PUNTOS_DEBILES, H_REP_PUNTOS_DEBILES);
	rep_punto_debil_central->establecer_posicion(OFFSET_X_PUNTO_DEBIL_3, OFFSET_Y_PUNTO_DEBIL_3);

	//Importante: La representación se hace cargo de liberar las representaciones
	//que se hayan agrupado. Aquí la liberamos de eso con el false.

	this->establecer_representacion_agrupada_dinamica(false);
	this->establecer_recorte(0, 0, w, h);

	this->insertar_en_representacion_agrupada(rep_general); 
	this->insertar_en_representacion_agrupada(rep_punto_debil_izq); 
	this->insertar_en_representacion_agrupada(rep_punto_debil_der); 
	this->insertar_en_representacion_agrupada(rep_punto_debil_central); 

	controlar_representacion();

	this->mapa_colision=Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_BOSS_01);
}

void Boss_01::establecer_caracteristicas()
{
	this->w=352;
	this->h=256;
}

/*La resistencia es 1, pero no se resta nunca*/

void Boss_01::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	resistencia=1;
	this->vector_mov.x=VELOCIDAD_X;
	this->vector_mov.y=0;

	//Preparando el trigger...
	Trigger::mut_tag_accion(p.param1);
	Trigger::mut_repetible(false);
}

/*Métodos auxiliares para generar proyectiles en función de varios parámetros.*/

void Boss_01::generar_proyectil(int px, int py, float vx, float vy)
{
	DLibH::Vector_2d vector(vx, vy);
	generar_proyectil(px, py, vector);
}

void Boss_01::generar_proyectil(int px, int py, DLibH::Vector_2d vector)
{
	int posx=this->x+px;
	int posy=this->y+py;

	Proyectil * resultado=Proyectil::generar(posx, posy, *this, vector);
	resultado->establecer_potencia(1);
	resultado->establecer_color(Proyectil::C_VERDE);
	resultado->establecer_velocidad(450.0f);
	resultado->establecer_faccion(Definiciones::F_NO_PREPARADO 
					| Definiciones::F_BONUS
					| Definiciones::F_TRIGGER);

	insertar_proyectil(resultado);
}

/*Si hay puntos débiles rotos echarán humo. Si hay puntos débiles que se acaban
de destruir, explotarán... etc.*/

void Boss_01::evaluar_y_generar_decoraciones()
{
	if(punto_debil_izq.puede_echar_humo())
	{
		insertar_decoracion(Estructura_decoracion_boss(
						OFFSET_X_PUNTO_DEBIL_1+this->x+(W_PUNTO_DEBIL / 4),
						OFFSET_Y_PUNTO_DEBIL_1+this->y+(H_PUNTO_DEBIL / 4),
						Estructura_decoracion_boss::ED_HUMO,
						0.7, 0, -200));
	}

	if(punto_debil_der.puede_echar_humo())
	{
		insertar_decoracion(Estructura_decoracion_boss(
						OFFSET_X_PUNTO_DEBIL_2+this->x+(W_PUNTO_DEBIL / 4),
						OFFSET_Y_PUNTO_DEBIL_2+this->y+(H_PUNTO_DEBIL / 4),
						Estructura_decoracion_boss::ED_HUMO,
						0.7, 0, -200));
	}

	if(punto_debil_central.puede_echar_humo())
	{
		insertar_decoracion(Estructura_decoracion_boss(
						OFFSET_X_PUNTO_DEBIL_3+this->x+(W_PUNTO_DEBIL / 4),
						OFFSET_Y_PUNTO_DEBIL_3+this->y+(H_PUNTO_DEBIL / 4),
						Estructura_decoracion_boss::ED_HUMO,
						0.7, 0, -200));
	}

}

/*Cuando dispara lo que hace es rellenar un vector de disparos que el controlador
recogerá luego. Según el estado que tiene genera diferentes cantidades de 
proyectiles en diferentes posiciones.*/

void Boss_01::evaluar_y_disparar()
{
	//Evaluar si puede disparar...
	bool puede_disparar=false;

	switch(estado)
	{
		case E_CANONES_VERTICALES: puede_disparar=pausa_entre_disparos >= MS_PAUSA_CANONES_VERTICALES; break;
		case E_CANONES_DIAGONALES: puede_disparar=pausa_entre_disparos >= MS_PAUSA_CANONES_DIAGONALES; break;
		case E_CANON_GIRATORIO: puede_disparar=pausa_entre_disparos >= MS_PAUSA_CANON_CIRCULAR; break;
	}

	if(puede_disparar) 
	{
		pausa_entre_disparos=0.0;

		switch(estado)
		{
			case E_CANONES_VERTICALES:
				generar_proyectil(ORIGEN_X_CANON_VERTICAL_SUPERIOR_IZQ, ORIGEN_Y_CANON_VERTICAL_SUPERIOR_IZQ, VECTOR_X_CANON_VERTICAL, -VECTOR_Y_CANON_VERTICAL);
				generar_proyectil(ORIGEN_X_CANON_VERTICAL_SUPERIOR_DER, ORIGEN_Y_CANON_VERTICAL_SUPERIOR_DER, VECTOR_X_CANON_VERTICAL, -VECTOR_Y_CANON_VERTICAL);
				generar_proyectil(ORIGEN_X_CANON_VERTICAL_INFERIOR_IZQ, ORIGEN_Y_CANON_VERTICAL_INFERIOR_IZQ, VECTOR_X_CANON_VERTICAL, VECTOR_Y_CANON_VERTICAL);
				generar_proyectil(ORIGEN_X_CANON_VERTICAL_INFERIOR_DER, ORIGEN_Y_CANON_VERTICAL_INFERIOR_DER, VECTOR_X_CANON_VERTICAL, VECTOR_Y_CANON_VERTICAL);
			break;

			case E_CANONES_DIAGONALES:
				generar_proyectil(ORIGEN_X_CANON_DIAGONAL_SUPERIOR_IZQ, ORIGEN_Y_CANON_DIAGONAL_SUPERIOR_IZQ, VECTOR_X_CANON_DIAGONAL, -VECTOR_Y_CANON_DIAGONAL);
				generar_proyectil(ORIGEN_X_CANON_DIAGONAL_SUPERIOR_DER, ORIGEN_Y_CANON_DIAGONAL_SUPERIOR_DER, -VECTOR_X_CANON_DIAGONAL, -VECTOR_Y_CANON_DIAGONAL);
				generar_proyectil(ORIGEN_X_CANON_DIAGONAL_INFERIOR_IZQ, ORIGEN_Y_CANON_DIAGONAL_INFERIOR_IZQ, VECTOR_X_CANON_DIAGONAL,  VECTOR_Y_CANON_DIAGONAL);
				generar_proyectil(ORIGEN_X_CANON_DIAGONAL_INFERIOR_DER, ORIGEN_Y_CANON_DIAGONAL_INFERIOR_DER, -VECTOR_X_CANON_DIAGONAL, VECTOR_Y_CANON_DIAGONAL);
		
			break;
			case E_CANON_GIRATORIO:
				generar_proyectil(ORIGEN_X_CANON_CIRCULAR, ORIGEN_Y_CANON_CIRCULAR, DLibH::Vector_2d::vector_unidad_para_angulo(angulo_disparo_canon_circular));
				generar_proyectil(ORIGEN_X_CANON_CIRCULAR, ORIGEN_Y_CANON_CIRCULAR, DLibH::Vector_2d::vector_unidad_para_angulo(angulo_disparo_canon_circular+180));

				angulo_disparo_canon_circular+=salto_angulo_disparo_canon_circular ;
				if(angulo_disparo_canon_circular > 360) angulo_disparo_canon_circular=angulo_disparo_canon_circular % 360;
			break;
		}
	}
}

unsigned short int Boss_01::obtener_posicion_impacto(Actor const& act) const
{
	if(act.en_colision_con(punto_debil_izq.caja))
	{
		return POS_DEBIL_IZQ;
	}
	else if(act.en_colision_con(punto_debil_der.caja))
	{
		return POS_DEBIL_DER;
	}
	else if(act.en_colision_con(punto_debil_central.caja))
	{
		return POS_DEBIL_CENTRAL;
	}
	else return POS_NADA;
}

/*La colisión se hace por varias cajas... En realidad debería depender un poco
también del actor en si: el jugador chocaría con todo, los proyectiles no, por
ejemplo...
Podríamos tener un overload "colisión con proyectil" pero hay varias cosas que van
mal: no podemos definirlo aquí porque se llama desde un "Actor_mapa" y llamaría
siempre a este. No podemos definirlo en la base porque proyectil deriva de esa
base... En fin, que si lo hubieramos pensado desde el principio estaría bien.
La solución cutre es la que hemos adoptado: el actor tendrá un método para ver
si es proyectil o algo así.
*/

bool Boss_01::en_colision_con(Actor const& act) const
{
	//La segunda condición comprueba si hay choque con el mapa de colisión.
	if(estado==E_CAYENDO || !Actor::en_colision_con(act))
	{
		return false;
	}
	else
	{
		switch(act.obtener_tipo_colision())
		{
			//El actor choca con todo el mapa de colisión.
			case Actor::T_TIPO_COLISION_ESTANDAR:
				return true;
			break;

			//El proyectil sólo choca con las cajas.
			case Actor::T_TIPO_COLISION_PROYECTIL:
				return obtener_posicion_impacto(act)!=POS_NADA;
			break;
		}

		return true;
	}
}

//Sobreescribe también este método: se inicia desactivado pero cuando
//entra en la pantalla se activa. Cuando sale de la pantalla puede seguir
//realizando acciones. No activamos la flag "actúa fuera de pantalla" 
//para que no dispare desde el final del nivel.

bool Boss_01::realiza_actividad() const 
{
	if(this->es_en_pantalla()) activar();
	return es_activado();
}

//Una vez activado siempre debe estar activo para poder echar humo y otras cosas.
bool Boss_01::es_activo() const {return es_activado();}

