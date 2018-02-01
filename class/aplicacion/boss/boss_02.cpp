#include "boss_02.h"

/*Este tiene varias partes independientes: aparte de los puntos débiles tiene 
cuatro patas de lógica independiente con tres posiciones cada una (arriba, 
centro y abajo) y más historias.*/

const float Boss_02::Cuerpo::MIN_TIEMPO_EN_ANGULO=1.5;
const float Boss_02::Cuerpo::PAUSA_DISPAROS=0.1;
const float Boss_02::Cuerpo::VELOCIDAD_DISPAROS=350.0;
const float Boss_02::Cuerpo::MAX_CUENTA_REP_CANON_FASE_1=0.20;
const float Boss_02::Cuerpo::MAX_CUENTA_REP_CANON_FASE_2=0.10;
const float Boss_02::Cuerpo::MAX_CUENTA_REP_CANON_FASE_3=0.05;
const float Boss_02::Pata::T_POSICION=3.5;
const float Boss_02::Pata::VELOCIDAD=100.0;
const float Boss_02::Cubierta::VELOCIDAD=50.0;
const float Boss_02::Cubierta::VELOCIDAD_DISPAROS=350.0;
const float Boss_02::Cubierta::PAUSA_DISPAROS=2.3;
const float Boss_02::Pincho::VELOCIDAD=350.0;
const float Boss_02::Pincho::VELOCIDAD_DISPAROS=200.0;
const float Boss_02::Pincho::ESPACIADO_DISPAROS=0.333;
const float Boss_02::Laser_trasero::VELOCIDAD_REP=64.0;
const float Boss_02::Laser_trasero::VELOCIDAD=1800.0;

/******************************************************************************/
/*Implementación de Boss_02::Pata*/
/******************************************************************************/

Boss_02::Pata::Pata(unsigned int p, float t):Actor(),
	posicion_actual(p), direccion(1),
	en_movimiento(false), tiempo(t),
	destino(0.0), offset_x(0.0), offset_y(0.0)
{}

/*Establece el mapa de colisión, las dimensiones y el offset...*/

void Boss_02::Pata::preparar(int pw, int ph, DLibV::Mapa_colision_bitmap const * pmapa)
{
	offset_x=0;
	offset_y=posicion_actual * DISTANCIA_POSICIONES;
	w=pw;
	h=ph;
	mapa_colision=pmapa;
}

void Boss_02::Pata::situar(float px, float py)
{
	x=px+offset_x;
	y=py+offset_y;
	this->establecer_posicion();
}

void Boss_02::Pata::ajustar_posicion_destino()
{
	destino=offset_y+(direccion * DISTANCIA_POSICIONES);
	en_movimiento=true;
}

void Boss_02::Pata::procesar_turno(float p_delta)
{
	float t=p_delta / No_jugador::divisor_movimiento;

	if(!en_movimiento)
	{
		tiempo+=t;
		if(tiempo >= T_POSICION)
		{
			posicion_actual+=direccion;
			if(posicion_actual==P_MAX || posicion_actual==P_MIN)  
			{ 
				direccion=posicion_actual==P_MAX ? -1 : 1;
				posicion_actual=P_CENTRO;
			}

			ajustar_posicion_destino();
		}
	}
	else
	{
		//Continuar moviéndose hasta la posición de turno.
		offset_y+=(direccion * VELOCIDAD) * t;
		if( (direccion > 0 && offset_y>=destino) 
		|| (direccion < 0 && offset_y<=destino))
		{	
			//Ha llegado.
			offset_y=destino;
			en_movimiento=false;
			tiempo=0.0;
		}
	}

	this->recargar_posicion();
}

/******************************************************************************/
/*Implementación de Boss_02::Cuerpo*/
/******************************************************************************/

Boss_02::Cuerpo::Cuerpo()
	:Actor(), estado(E_CERRADO), tiempo_herido(0.0), acertado(false),
	resistencia(RESISTENCIA), pausa_disparos(0.0), tiempo_en_angulo(0.0),
	tiempo_rep_canon(0.0), fase_canon(0)
{
	tunel_colision.w=W_TUNEL;
	tunel_colision.h=H_TUNEL;

	caja_debil.w=W_DEBIL;
	caja_debil.h=H_DEBIL;

	zona_libre.w=W_ZONA_LIBRE;
	zona_libre.h=H_ZONA_LIBRE;
}

/*Establece el mapa de colisión, las dimensiones y el offset...*/

void Boss_02::Cuerpo::preparar(int pw, int ph, DLibV::Mapa_colision_bitmap const * pmapa)
{
	w=pw;
	h=ph;
	mapa_colision=pmapa;
}

void Boss_02::Cuerpo::situar(float px, float py)
{
	x=px;
	y=py;
	this->establecer_posicion();

	//También las cajas de colisión, por supuesto...
	tunel_colision.x=x+X_TUNEL;
	tunel_colision.y=y+Y_TUNEL;
	caja_debil.x=x+X_DEBIL;
	caja_debil.y=y+Y_DEBIL;
	zona_libre.x=x+X_ZONA_LIBRE;
	zona_libre.y=y+Y_ZONA_LIBRE;
}

void Boss_02::Cuerpo::acumular_tiempo_en_angulo(float p_delta, const Actor * const actor)
{	
	float ax=actor->acc_x() + (actor->acc_w() / 2);
	float ay=actor->acc_y() + (actor->acc_h() / 2);
	float angulo=DLibH::Herramientas::angulo_entre_puntos(x+OFFSET_X_DISPARO, y+OFFSET_Y_DISPARO, ax, ay);

	if(angulo >= ANGULO_MIN && angulo <= ANGULO_MAX)
	{
		float t=p_delta / No_jugador::divisor_movimiento;
		tiempo_en_angulo+=t;
		tiempo_rep_canon+=t;
	}
	else
	{
		tiempo_en_angulo=0;
		tiempo_rep_canon=0;
	}
}

void Boss_02::Cuerpo::procesar_turno(float p_delta)
{
	float t=p_delta / No_jugador::divisor_movimiento;

	pausa_disparos+=t;
	if(pausa_disparos >= PAUSA_DISPAROS) pausa_disparos=0;

	if(tiempo_herido)
	{
		tiempo_herido-=t;
		if(tiempo_herido < 0.0f) tiempo_herido=0.0f;
	}

	this->recargar_posicion();
	acertado=false;
}

int Boss_02::Cuerpo::obtener_fase_canon()
{
	//Esto lo calculamos de forma estática... Se divide el tiempo en cuatro
	//cuartos. En el primero no se mueve, en el resto va aumentando la 
	//velocidad.

	if(tiempo_en_angulo <= 0.375)
	{
		return 0;
	}
	else
	{	
		float vcomp=0.0;

		if(tiempo_en_angulo <= 0.75) vcomp=MAX_CUENTA_REP_CANON_FASE_1;
		else if(tiempo_en_angulo <= 1.125) vcomp=MAX_CUENTA_REP_CANON_FASE_2;
		else vcomp=MAX_CUENTA_REP_CANON_FASE_3;
	
		if(tiempo_rep_canon >= vcomp)
		{
			++fase_canon;
			tiempo_rep_canon=0;
			if(fase_canon >= MAX_FASE_CANON) fase_canon=0;
		}
		
		return fase_canon;
	}
}

bool Boss_02::Cuerpo::en_colision_con(Actor const& otro) const
{
	SDL_Rect caja=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0);
	SDL_Rect caja_actor=otro.acc_posicion();

	//Comprobamos en orden: primero si entra en la caja, luego si está
	//abierto sólo contra el túnel y la caja débil, si no, contra el mapa
	//de colisión. Para comparar con el mapa tiramos del que está en actor.
	//Hace alguna comprobación redundante, pero no es grave.

	if(!DLibH::Herramientas_SDL::rectangulos_superpuestos(posicion, caja_actor, caja))
	{	
		return false;
	}
	else
	{
		if(estado==E_ABIERTO)
		{
			//Primero vemos si está en la caja débil...
			if(DLibH::Herramientas_SDL::rectangulos_superpuestos(caja_debil, caja_actor, caja))
			{
				acertado=true;
				return true;
			}
			//Si no, miramos si está en el túnel o en la caja libre, donde no chocamos....
			else if(DLibH::Herramientas_SDL::rectangulos_superpuestos(tunel_colision, caja_actor, caja)
				|| DLibH::Herramientas_SDL::rectangulos_superpuestos(zona_libre, caja_actor, caja))
			{
				return false; //En el túnel NO chocamos.
			}		
			//Si no está en el túnel ni caja libre, contra el mapa de colisión.
			else
			{
				return Actor::en_colision_con(otro);
			}
		}
		else
		{
			if(DLibH::Herramientas_SDL::rectangulos_superpuestos(zona_libre, caja_actor, caja))
			{
				return false; //Idem que antes...
			}	
			else 
			{
				return Actor::en_colision_con(otro);
			}
		}
	}
}

bool Boss_02::Cuerpo::herir(int pot)
{
	if(estado==E_CERRADO)
	{
		return false;
	}
	else 
	{
		tiempo_herido=(float)T_HERIDO / 1000;
		resistencia-=pot;
		if(resistencia <= 0) 
		{
			resistencia=0;
			estado=E_DESTRUIDO;
		}

		return estado==E_DESTRUIDO;
	}
}

/******************************************************************************/
/*Implementación de Boss_02::Cubierta....*/
/******************************************************************************/

Boss_02::Cubierta::Cubierta(short int pd, int pox)
	:Actor(), direccion(pd), estado(E_CERRADO), offset_x(0.0), 
	offset_y(0.0), destino(0.0), angulo_min(0.0), angulo_max(0.0),
	punto_disparo_x(0.0), punto_disparo_y(0.0), offset_canon_x(pox), 
	acertado(false), resistencia(RESISTENCIA), tiempo_herido(0.0),
	pausa_entre_humos(0.0)
{
	caja_colision.w=W_CAJA_COLISION;
	caja_colision.h=H_CAJA_COLISION;
	caja_debil.w=W_CAJA_DEBIL;
	caja_debil.h=H_CAJA_DEBIL;

	if(direccion > 0) 
	{
		angulo_min=90-MITAD_ANGULO_CANON_CUBIERTA;
		angulo_max=90+MITAD_ANGULO_CANON_CUBIERTA;
	}
	else
	{
		angulo_min=270-MITAD_ANGULO_CANON_CUBIERTA;
		angulo_max=270+MITAD_ANGULO_CANON_CUBIERTA;
	}
}


void Boss_02::Cubierta::procesar_turno(float p_delta)
{
	float t=p_delta / No_jugador::divisor_movimiento;

	pausa_disparos+=t;
	if(pausa_disparos >= PAUSA_DISPAROS) pausa_disparos=0;
	if(estado!=E_CERRADO) 
	{
		pausa_entre_humos+=t;
		if(pausa_entre_humos >= 0.1) pausa_entre_humos=0.0;
	}

	if(tiempo_herido)
	{
		tiempo_herido-=p_delta / No_jugador::divisor_movimiento;
		if(tiempo_herido < 0.0f) tiempo_herido=0.0f;
	}

	if(estado==E_MOVIMIENTO)
	{
		offset_x+=(direccion * VELOCIDAD) * t;
		if( (direccion > 0 && offset_x>=destino) || (direccion < 0 && offset_x<=destino))
		{	
			offset_x=destino;
			estado=E_ABIERTO;
		}
	}

	acertado=false;
	recargar_posicion();
}

/*Sólo puede disparar cuando el estado sea correcto y la pausa lo permita...
Además, el actor tiene que estar en un cierto ángulo con la horizontal del origen
de los disparos...


Then calculate the angle.

angleInDegrees = arctan(deltaY / deltaX) * 180 / PI

If your language includes an atan2 function it becomes the following instead:

angleInDegrees = atan2(deltaY, deltaX) * 180 / PI

*/

bool Boss_02::Cubierta::puede_disparar(const Actor * const actor) const
{
	float ax=actor->acc_x() + (actor->acc_w() / 2);
	float ay=actor->acc_y() + (actor->acc_h() / 2);
	float angulo=DLibH::Herramientas::angulo_entre_puntos(punto_disparo_x, punto_disparo_y, ax, ay);

	return estado==E_CERRADO && !pausa_disparos && angulo >= angulo_min && angulo <= angulo_max;
}

void Boss_02::Cubierta::preparar(int pxc, int pyc, int pxd, int pyd)
{
	w=W_ACTOR;
	h=H_ACTOR;
	
	offset_caja_colision_x=pxc;
	offset_caja_colision_y=pyc;
	offset_caja_debil_x=pxd;
	offset_caja_debil_y=pyd;
}

void Boss_02::Cubierta::situar(float px, float py)
{
	x=px+offset_x;
	y=py+offset_y;
	punto_disparo_x=x+offset_canon_x;
	punto_disparo_y=y+OFFSET_Y_CANONES_CUBIERTA;

	destino=DISTANCIA_POSICIONES*direccion;

	this->establecer_posicion();

	//También las cajas de colisión, por supuesto...
	caja_colision.x=x+offset_caja_colision_x;
	caja_colision.y=y+offset_caja_colision_y;
	caja_debil.x=x+offset_caja_debil_x;
	caja_debil.y=y+offset_caja_debil_y;
}

bool Boss_02::Cubierta::en_colision_con(Actor const& otro) const
{
	SDL_Rect caja=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0);
	SDL_Rect caja_actor=otro.acc_posicion();

	//Comprobamos en orden: primero si entra en la caja, luego con la de 
	//punto débil y luego con la de colisión en si.

	if(!DLibH::Herramientas_SDL::rectangulos_superpuestos(posicion, caja_actor, caja))
	{
		return false;
	}
	else if(DLibH::Herramientas_SDL::rectangulos_superpuestos(caja_debil, caja_actor, caja)) 
	{
		acertado=true;
		return true;
	}
	else if(DLibH::Herramientas_SDL::rectangulos_superpuestos(caja_colision, caja_actor, caja)) 
	{
		return true;
	}
	else return false;
}

bool Boss_02::Cubierta::herir(int pot)
{
	if(estado==E_CERRADO)
	{
		tiempo_herido=(float)T_HERIDO / 1000;
		resistencia-=pot;
		if(resistencia <= 0) 
		{
			resistencia=0;
			estado=E_MOVIMIENTO;
		}

		return estado==E_MOVIMIENTO;
	}
	else 
	{
		return false;
	}
}

/******************************************************************************/
/*Implementación de Pincho....*/
/******************************************************************************/

Boss_02::Pincho::Pincho(short int pd, float pt):direccion(pd), estado(E_INICIO_CUENTA),
	pausa_disparos(0.0), tiempo(pt), aux_h(0.0), desactivado(false)
{
	w=W;
	h=MIN_H;
	x=0;
	y=0;
	this->establecer_posicion();
}

void Boss_02::Pincho::situar(float px, float py)
{
	x=px;
	y=py;
	this->establecer_posicion();
}

//El pincho dispara cuando pasa la pausa y está en estado E_DISPARANDO.
bool Boss_02::Pincho::puede_disparar() const
{
	return estado==E_DISPARANDO && !pausa_disparos;
}

DLibH::Vector_2d Boss_02::Pincho::obtener_vector_disparos() const
{
	return DLibH::Vector_2d(VELOCIDAD_DISPAROS * direccion, 0.0);
}

void Boss_02::Pincho::procesar_turno(float p_delta)
{
	float t=p_delta / No_jugador::divisor_movimiento;

	if(estado==E_DESACTIVADO)
	{
		//No hace nada...
	}
	//En el estado inicial restamos el tiempo antes de activarse...
	else if(estado==E_INICIO_CUENTA)
	{
		tiempo-=t;
		if(tiempo <=0 ) 
		{
			estado=E_SALIENDO;	//Salta a "saliendo...".
			tiempo=0.0;
		}
	}
	else
	//Contamos el tiempo...
	{
		tiempo+=t;
		float max_t=0;

		switch(estado)
		{
			case E_RETRAIDO: max_t=T_RETRAIDO; break;
			case E_PAUSA_SALIDA: max_t=T_PAUSA_SALIDA; break;
			case E_PAUSA_ENTRADA: max_t=T_PAUSA_ENTRADA; break;
			case E_DISPARANDO: max_t=T_DISPARANDO; break;
		}

		if(max_t && tiempo >= max_t)
		{
			++estado;
			tiempo=0.0;
		}

		//Y ahora controlamos lo que haría algo.
		switch(estado)
		{		
			case E_SALIENDO:
			case E_ENTRANDO:
				//En este estado se hace crecer el tamaño. La representación
				//la reajusta el boss.
			{
				float direccion=estado==E_SALIENDO ? 1.0 : -1.0;
				float factor=(direccion *VELOCIDAD) * t;
				aux_h+=factor;

				//Limitar el máximo.
				if(estado==E_SALIENDO && aux_h >= MAX_H) aux_h=MAX_H;
				else if(estado==E_ENTRANDO && aux_h <= MIN_H) aux_h=MIN_H;

				h=aux_h;
				recargar_posicion();

				if(h==MAX_H || h==MIN_H) 
				{
					++estado;
					//Si hemos llegado al fin del ciclo damos la vuelta pero si estamos desactivados ya no vuelve a hacerlo.
					if(desactivado) estado=E_DESACTIVADO;
					else if(estado==E_MAX) estado=E_RETRAIDO;
					
					tiempo=0.0;
				}
			}
			break;

			case E_DISPARANDO:
				//Aquí simplemente controlamos la pausa.
				pausa_disparos+=t;
				if(pausa_disparos >= ESPACIADO_DISPAROS) pausa_disparos=0;
			break;
		}
	}
}

SDL_Rect Boss_02::Pincho::obtener_rect_representacion()
{
	int x=direccion > 0 ? 760 : 777;
	int y=MAX_H - h;
	return DLibH::Herramientas_SDL::nuevo_sdl_rect(x, y, W, h);
}

/******************************************************************************/
/*Implementación de Laser_trasero....*/
/******************************************************************************/

Boss_02::Laser_trasero::Laser_trasero(float pt):Actor(), 
	estado(E_INICIO_CUENTA), tiempo(pt), aux_h(0.0), 
	aux_y(0.0), dest_y(0.0), aux_rep(MIN_H_REP), canal()
{
	w=W;
	h=0.0;
	x=0;
	y=0;
	this->establecer_posicion();
}

Boss_02::Laser_trasero::~Laser_trasero() {}

void Boss_02::Laser_trasero::situar(float px, float py)
{
	x=px;
	y=py+aux_y;
	this->establecer_posicion();
}

void Boss_02::Laser_trasero::procesar_turno(float p_delta)
{
	float t=p_delta / No_jugador::divisor_movimiento;

	if(estado==E_DESACTIVADO)
	{
		//No hace nada...
	}
	//En el estado inicial restamos el tiempo antes de activarse...
	else if(estado==E_INICIO_CUENTA)
	{
		tiempo-=t;

		if(tiempo <=0 ) 
		{
			estado=E_CARGANDO;	//Salta a "cargando...".
			tiempo=0.0;
		}
	}
	else
	//Contamos el tiempo y cambiamos el estado. No hay mucho más.
	{
 		tiempo+=t;
		float max_t=0;

		switch(estado)
		{
			case E_APAGADO: max_t=T_APAGADO; break;
			case E_CARGANDO: max_t=T_CARGANDO; break;
			case E_DESCARGANDO: max_t=T_DESCARGANDO; break;
			case E_DISPARANDO: max_t=T_DISPARANDO; break;
			case E_INICIAR_SONIDO:
			case E_FINALIZAR_SONIDO: 
				max_t=-1; 	//Ojo, -1 para que entre en el siguiente :D.
			break;
		}

		if(estado==E_FINALIZAR_SONIDO)
		{
			finalizar_sonido();
		}

		if(max_t && tiempo >= max_t)
		{
			++estado;
			tiempo=0.0;
		}

		//Controlamos ahora los cambios de tamaño...

		switch(estado)
		{
			case E_CRECIENDO:
			{
				aux_h+=VELOCIDAD * t;
				h=aux_h;
				recargar_posicion();

				if(h>=MAX_H) 
				{
					++estado;
					tiempo=0.0;
					aux_h=0;
					dest_y=y+h; //Establecemos el destino para decrecer luego...	
				}
			}
			break;

			case E_DECRECIENDO:
				aux_y+=VELOCIDAD * t;

				if(aux_y>=dest_y) 
				{
					++estado;
					tiempo=0.0;

					aux_y=0;
					aux_h=0;
					h=0;
					recargar_posicion();				
				}
			break;
		}

		if(estado==E_MAX) estado=E_APAGADO;

		//Ajustamos el parámetro de representación que hará que el
		//grafico "ondule".
		aux_rep+=VELOCIDAD_REP * t;
		if(aux_rep >= MAX_H_REP) aux_rep=MIN_H_REP;
	}
}

SDL_Rect Boss_02::Laser_trasero::obtener_rect_representacion()
{
	return DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 320, W+4, aux_rep);
}

Uint32 Boss_02::Laser_trasero::obtener_color_canon() const
{
	switch(estado)
	{
		case E_CARGANDO:
		{
			int g=(tiempo * MAX_COLOR_G) / T_CARGANDO;
			int b=(tiempo * MAX_COLOR_B) / T_CARGANDO;
			return DLibV::Gestor_color::color(0, g, b);
		}
		break;

		case E_DESCARGANDO:
		{
			int g=MAX_COLOR_G-(tiempo * MAX_COLOR_G) / T_CARGANDO;
			int b=MAX_COLOR_B-(tiempo * MAX_COLOR_B) / T_CARGANDO;
			return DLibV::Gestor_color::color(0, g, b);
		}
		break;

		case E_CRECIENDO:
		case E_DISPARANDO:
		case E_DECRECIENDO:
			return DLibV::Gestor_color::color(0, MAX_COLOR_G, MAX_COLOR_B);
		break;

		case E_INICIO_CUENTA:
		case E_APAGADO:
		case E_DESACTIVADO:
		case E_INICIAR_SONIDO:
		case E_FINALIZAR_SONIDO:
		default:
			return 0;
		break;
	}
}

bool Boss_02::Laser_trasero::debe_solicitar_canal() const
{
	return estado==E_INICIAR_SONIDO && !canal.es_vinculado();
}

void Boss_02::Laser_trasero::finalizar_sonido()
{
	if(canal.es_vinculado())
	{
		canal.iniciar_fade_out(300);
		canal.desvincular();
	}
}

void Boss_02::Laser_trasero::asignar_canal(DLibA::Canal_audio p_canal)
{
	canal=p_canal;


	canal.iniciar_reproduccion(
		DLibA::Estructura_sonido(
			DLibA::Gestor_recursos_audio::obtener_sonido(Recursos::S_GRAN_LASER), 
			-1, -1)
		);
}

/******************************************************************************/
/*Implementación de Boss_02....*/
/******************************************************************************/

Boss_02::Boss_02():Boss(),
	estado(E_OCULTO), posicion_y_final(0.0), batalla_iniciada(false),
	senal_sonido(SND_NADA), blanco(NULL), 
	pata_exterior_izq(Boss_02::Pata::P_ARRIBA, 0.0),
	pata_interior_izq(Boss_02::Pata::P_CENTRO, 1.5),
	pata_exterior_der(Boss_02::Pata::P_ABAJO, 2.3),
	pata_interior_der(Boss_02::Pata::P_CENTRO, 0.8),
	cuerpo(), 
	cubierta_izq(-1, OFFSET_X_CANON_CUBIERTA_IZQ), 
	cubierta_der(1, OFFSET_X_CANON_CUBIERTA_DER),
	punto_debil_canon_izq(RESISTENCIA_CANON),
	punto_debil_canon_der(RESISTENCIA_CANON),
	punto_debil_pata_exterior_izq(RESISTENCIA_PATA),
	punto_debil_pata_interior_izq(RESISTENCIA_PATA),
	punto_debil_pata_exterior_der(RESISTENCIA_PATA),
	punto_debil_pata_interior_der(RESISTENCIA_PATA),
	pincho_pata_exterior_izq(1, 4.0),
	pincho_pata_interior_izq(1, 1.0),
	pincho_pata_exterior_der(-1, 2.5),
	pincho_pata_interior_der(-1, 5.5),
	laser_trasero_izq(2.6),
	laser_trasero_der(7.6)
{
	this->preparar_actor(0);

	//Realmente no se conoce la posición final. Cuando se conozca, situaremos
	//cada parte.

	pata_exterior_izq.preparar(W_PATAS_EXTERIORES, H_PATAS_EXTERIORES, 
		Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_BOSS_02_PATA_EXTERIOR_IZQ));

	pata_interior_izq.preparar(W_PATAS_INTERIORES, H_PATAS_INTERIORES, 
		Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_BOSS_02_PATA_INTERIOR_IZQ));

	pata_exterior_der.preparar(W_PATAS_EXTERIORES, H_PATAS_EXTERIORES, 
		Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_BOSS_02_PATA_EXTERIOR_DER));

	pata_interior_der.preparar(W_PATAS_INTERIORES, H_PATAS_INTERIORES, 
		Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_BOSS_02_PATA_INTERIOR_DER));

	cuerpo.preparar(W_CUERPO, H_CUERPO, 
		Gestor_mapas_colision::obtener(Gestor_mapas_colision::M_BOSS_02_CUERPO));

	cubierta_izq.preparar(X_COLISION_CUBIERTA_IZQ, Y_COLISION_CUBIERTA_IZQ, 
			X_DEBIL_CUBIERTA_IZQ, Y_DEBIL_CUBIERTA_IZQ);

	cubierta_der.preparar(X_COLISION_CUBIERTA_DER, Y_COLISION_CUBIERTA_DER, 
			X_DEBIL_CUBIERTA_DER, Y_DEBIL_CUBIERTA_DER);

	caja_canon_izq.w=W_PUNTO_DEBIL_CANON;
	caja_canon_izq.h=H_PUNTO_DEBIL_CANON;
	caja_canon_der.w=W_PUNTO_DEBIL_CANON;
	caja_canon_der.h=H_PUNTO_DEBIL_CANON;
	caja_punto_debil_pata_interior_izq.w=W_PUNTO_DEBIL_PATA;
	caja_punto_debil_pata_interior_izq.h=H_PUNTO_DEBIL_PATA;
	caja_punto_debil_pata_interior_der.w=W_PUNTO_DEBIL_PATA;
	caja_punto_debil_pata_interior_der.h=H_PUNTO_DEBIL_PATA;
	caja_punto_debil_pata_exterior_izq.w=W_PUNTO_DEBIL_PATA;
	caja_punto_debil_pata_exterior_izq.h=H_PUNTO_DEBIL_PATA;
	caja_punto_debil_pata_exterior_der.w=W_PUNTO_DEBIL_PATA;
	caja_punto_debil_pata_exterior_der.h=H_PUNTO_DEBIL_PATA;
}

Boss_02::~Boss_02()
{
	delete rep_cuerpo;
	delete rep_canon_cuerpo;
	delete rep_cuerpo_punto_debil;
	delete rep_cubierta_izq;
	delete rep_cubierta_izq_punto_debil;
	delete rep_cubierta_der;
	delete rep_cubierta_der_punto_debil;
	delete rep_pata_interior_izq;
	delete rep_pata_interior_der;
	delete rep_pata_exterior_izq;
	delete rep_pata_exterior_der;
	delete rep_pata_interior_izq_punto_debil;
	delete rep_pata_interior_der_punto_debil;
	delete rep_pata_exterior_izq_punto_debil;
	delete rep_pata_exterior_der_punto_debil;
	delete rep_canon_izq_punto_debil;
	delete rep_canon_der_punto_debil;
	delete rep_pincho_pata_interior_izq;
	delete rep_pincho_pata_interior_der;
	delete rep_pincho_pata_exterior_izq;
	delete rep_pincho_pata_exterior_der;
	delete rep_laser_trasero_izq;
	delete rep_aviso_laser_trasero_izq;
	delete rep_aviso_laser_trasero_der;

	blanco=NULL;
}

/*En este momento se conoce la posición exacta del boss, de modo que podemos
colocar el resto de partes en las posiciones relativas que sean mediante el método
"controlar_posicion_partes".*/

void Boss_02::preparar_actor_mapa_manual(float p_x, float p_y, float v_x, float v_y)
{
	Actor_mapa::preparar_actor_mapa_manual(p_x, p_y, v_x, v_y);
	posicion_y_final=p_y;
	controlar_posicion_partes();
}

void Boss_02::controlar_posicion_partes()
{
	//Colocar las partes en posición.
	cuerpo.situar(x+X_CUERPO, y+Y_CUERPO);

	cubierta_izq.situar(x+X_CUBIERTA_IZQ, y+Y_CUBIERTAS);
	cubierta_der.situar(x+X_CUBIERTA_DER, y+Y_CUBIERTAS);

	pata_exterior_izq.situar(x+X_PATAS_EXTERIORES_IZQ, y+Y_PATAS_EXTERIORES);
	pata_interior_izq.situar(x+X_PATAS_INTERIORES_IZQ, y+Y_PATAS_INTERIORES);

	pata_exterior_der.situar(x+X_PATAS_EXTERIORES_DER, y+Y_PATAS_EXTERIORES);
	pata_interior_der.situar(x+X_PATAS_INTERIORES_DER, y+Y_PATAS_INTERIORES);

//	pincho_pata_exterior_izq.situar(x+X_PATAS_EXTERIORES_IZQ+X_PINCHO_PATA_EXTERIOR_IZQ, y+Y_PATAS_EXTERIORES+Y_PINCHO_PATA_EXTERIOR_IZQ);
//	pincho_pata_interior_izq.situar(x+X_PATAS_INTERIORES_IZQ+X_PINCHO_PATA_INTERIOR_IZQ, y+Y_PATAS_INTERIORES+Y_PINCHO_PATA_INTERIOR_IZQ);
	pincho_pata_exterior_izq.situar(x+X_PATAS_EXTERIORES_IZQ+X_PINCHO_PATA_EXTERIOR_IZQ, pata_exterior_izq.acc_y()+Y_PINCHO_PATA_EXTERIOR_IZQ);
	pincho_pata_interior_izq.situar(x+X_PATAS_INTERIORES_IZQ+X_PINCHO_PATA_INTERIOR_IZQ, pata_interior_izq.acc_y()+Y_PINCHO_PATA_INTERIOR_IZQ);

//	pincho_pata_exterior_der.situar(x+X_PATAS_EXTERIORES_DER+X_PINCHO_PATA_EXTERIOR_DER, y+Y_PATAS_EXTERIORES+Y_PINCHO_PATA_EXTERIOR_DER);
//	pincho_pata_interior_der.situar(x+X_PATAS_INTERIORES_DER+X_PINCHO_PATA_INTERIOR_DER, y+Y_PATAS_INTERIORES+Y_PINCHO_PATA_INTERIOR_DER);
	pincho_pata_exterior_der.situar(x+X_PATAS_EXTERIORES_DER+X_PINCHO_PATA_EXTERIOR_DER, pata_exterior_der.acc_y()+Y_PINCHO_PATA_EXTERIOR_DER);
	pincho_pata_interior_der.situar(x+X_PATAS_INTERIORES_DER+X_PINCHO_PATA_INTERIOR_DER, pata_exterior_der.acc_y()+Y_PINCHO_PATA_INTERIOR_DER);

	caja_canon_izq.x=x+X_CUERPO+X_PUNTO_DEBIL_CANON_IZQ;
	caja_canon_izq.y=y+Y_CUERPO+Y_PUNTO_DEBIL_CANON_IZQ;
	caja_canon_der.x=x+X_CUERPO+X_PUNTO_DEBIL_CANON_DER;
	caja_canon_der.y=y+Y_CUERPO+Y_PUNTO_DEBIL_CANON_DER;

	caja_punto_debil_pata_interior_izq.x=pata_interior_izq.acc_x()+X_PUNTO_DEBIL_PATA_INTERIOR_IZQ;

	caja_punto_debil_pata_interior_der.x=pata_interior_der.acc_x()+X_PUNTO_DEBIL_PATA_INTERIOR_DER;
	caja_punto_debil_pata_exterior_izq.x=pata_exterior_izq.acc_x()+X_PUNTO_DEBIL_PATA_EXTERIOR_IZQ;
	caja_punto_debil_pata_exterior_der.x=pata_exterior_der.acc_x()+X_PUNTO_DEBIL_PATA_EXTERIOR_DER;

	caja_punto_debil_pata_interior_izq.y=pata_interior_izq.acc_y()+Y_PUNTO_DEBIL_PATA_INTERIOR_IZQ;
	caja_punto_debil_pata_interior_der.y=pata_interior_der.acc_y()+Y_PUNTO_DEBIL_PATA_INTERIOR_DER;
	caja_punto_debil_pata_exterior_izq.y=pata_exterior_izq.acc_y()+Y_PUNTO_DEBIL_PATA_EXTERIOR_IZQ;
	caja_punto_debil_pata_exterior_der.y=pata_exterior_der.acc_y()+Y_PUNTO_DEBIL_PATA_EXTERIOR_DER;

	laser_trasero_izq.situar(x+X_LASER_TRASERO_IZQ, y+Y_LASER_TRASERO);
	laser_trasero_der.situar(x+X_LASER_TRASERO_DER, y+Y_LASER_TRASERO);
}

void Boss_02::controlar_estado(float p_delta)
{
	switch(estado)
	{
		case E_OCULTO: 
			this->acc_representacion()->hacer_invisible();
		break;

		case E_VISIBLE:
		{
			float py=y+=DESPLAZAMIENTO_Y_APARICION;
			ir_a(x, py); //Colocar fuera de la pantalla...
			mut_vector_y(VELOCIDAD_APARICION);
			this->acc_representacion()->hacer_visible(); //Hacer visible...
			
			estado=E_APARECIENDO; //Cambiar estado...
		}
		break;

		case E_APARECIENDO:
		{
			aplicar_vector(p_delta);

			if(y <= posicion_y_final)
			{
				ir_a(x, posicion_y_final);
				mut_vector_y(0.0);
				estado=E_LOOP;
			}
		}

		break;

		case E_LOOP:
		case E_EXPLOSION:
		case E_DESTRUIDO:
		break;

		case E_CAYENDO:
			mut_vector_y(VELOCIDAD_DESTRUCCION_Y);
			mut_vector_x(VELOCIDAD_DESTRUCCION_X);
		break;
	}
}

void Boss_02::procesar_turno(float p_delta)
{
	//Usamos el normal...

	senal_sonido=SND_NADA;
	Obstaculo_actividad::procesar_turno(p_delta);

	if(estado==E_LOOP || estado==E_CAYENDO)
	{
		//Si los puntos débiles están destruidos no se moverán las patas.
		if(!punto_debil_pata_exterior_izq.destruido) pata_exterior_izq.procesar_turno(p_delta);
		if(!punto_debil_pata_exterior_der.destruido) pata_exterior_der.procesar_turno(p_delta);
		if(!punto_debil_pata_interior_izq.destruido) pata_interior_izq.procesar_turno(p_delta);
		if(!punto_debil_pata_interior_der.destruido) pata_interior_der.procesar_turno(p_delta);

		pincho_pata_exterior_izq.procesar_turno(p_delta);
		pincho_pata_interior_izq.procesar_turno(p_delta);
		pincho_pata_exterior_der.procesar_turno(p_delta);
		pincho_pata_interior_der.procesar_turno(p_delta);

		cuerpo.procesar_turno(p_delta);
		cuerpo.acumular_tiempo_en_angulo(p_delta, blanco);
		cubierta_izq.procesar_turno(p_delta);
		cubierta_der.procesar_turno(p_delta);

		laser_trasero_izq.procesar_turno(p_delta);
		laser_trasero_der.procesar_turno(p_delta);

		punto_debil_canon_izq.turno(p_delta);
		punto_debil_canon_der.turno(p_delta);
		punto_debil_pata_exterior_izq.turno(p_delta);
		punto_debil_pata_interior_izq.turno(p_delta);
		punto_debil_pata_exterior_der.turno(p_delta);
		punto_debil_pata_interior_der.turno(p_delta);
	}

	controlar_estado(p_delta);
	controlar_posicion_partes();
	controlar_representacion();

	//Si es posible disparar o generar decoraciones periódicas, será aquí.
	evaluar_y_disparar();
	evaluar_y_generar_decoraciones();
}

void Boss_02::comprobar_destruccion()
{
	senal_sonido=senal_sonido | SND_EXPLOSION;

	if(punto_debil_canon_izq.destruido) 
	{
		laser_trasero_izq.forzar_destruccion();
		rep_canon_izq_punto_debil->hacer_invisible();
		rep_aviso_laser_trasero_izq->hacer_invisible();
	}

	if(punto_debil_canon_der.destruido) 
	{
		laser_trasero_der.forzar_destruccion();
		rep_canon_der_punto_debil->hacer_invisible();
		rep_aviso_laser_trasero_der->hacer_invisible();
	}

	if(punto_debil_pata_exterior_izq.destruido) 
	{
		rep_pata_exterior_izq_punto_debil->hacer_invisible();
		pincho_pata_exterior_izq.forzar_retraccion();
	}

	if(punto_debil_pata_exterior_der.destruido) 
	{
		rep_pata_exterior_der_punto_debil->hacer_invisible();
		pincho_pata_exterior_der.forzar_retraccion();
	}

	if(punto_debil_pata_interior_izq.destruido) 
	{
		rep_pata_interior_izq_punto_debil->hacer_invisible();
		pincho_pata_interior_izq.forzar_retraccion();
	}

	if(punto_debil_pata_interior_der.destruido) 
	{
		rep_pata_interior_der_punto_debil->hacer_invisible();
		pincho_pata_interior_der.forzar_retraccion();
	}

	if(!cubierta_izq.es_cerrada()) rep_cubierta_izq_punto_debil->hacer_invisible();
	if(!cubierta_der.es_cerrada()) rep_cubierta_der_punto_debil->hacer_invisible();

	if(!cubierta_izq.es_cerrada() && !cubierta_der.es_cerrada() && cuerpo.es_cerrado())
	{
		cuerpo.abrir();
	}

	if(cuerpo.es_destruido())
	{
		rep_cuerpo_punto_debil->hacer_invisible();
		laser_trasero_izq.forzar_destruccion();
		laser_trasero_der.forzar_destruccion();

		estado=E_DESTRUIDO;
	}
}

bool Boss_02::procesar_colision_proyectil(Proyectil * proyectil)
{
	//En función de la posición del proyectil habrá dado en un sitio u otro.
	unsigned short int pos=obtener_posicion_impacto(*proyectil);

	switch(pos)
	{
		case POS_NADA:
		case POS_PATA_EXTERIOR_IZQ:
		case POS_PATA_INTERIOR_IZQ:
		case POS_PATA_EXTERIOR_DER:
		case POS_PATA_INTERIOR_DER:
		case POS_PINCHO_PATA_EXTERIOR_IZQ:
		case POS_PINCHO_PATA_INTERIOR_IZQ:
		case POS_PINCHO_PATA_EXTERIOR_DER:
		case POS_PINCHO_PATA_INTERIOR_DER:
		case POS_LASER_TRASERO_IZQ:
		case POS_LASER_TRASERO_DER:
			//Chocando con esto no se consigue nada.
		break;

		case POS_PATA_EXTERIOR_IZQ_PUNTO_DEBIL:
			if(punto_debil_pata_exterior_izq.vulnerable && !punto_debil_pata_exterior_izq.destruido)
			{
				if(punto_debil_pata_exterior_izq.alcanzar(proyectil->acc_potencia()))
				{
					generar_explosion_parte(caja_punto_debil_pata_exterior_izq);
					comprobar_destruccion();
				}
			}
		break;
		case POS_PATA_INTERIOR_IZQ_PUNTO_DEBIL:
			if(punto_debil_pata_interior_izq.vulnerable && !punto_debil_pata_interior_izq.destruido)
			{
				if(punto_debil_pata_interior_izq.alcanzar(proyectil->acc_potencia()))
				{
					generar_explosion_parte(caja_punto_debil_pata_interior_izq);
					comprobar_destruccion();
				}
			}
		break;
		case POS_PATA_EXTERIOR_DER_PUNTO_DEBIL:
			if(punto_debil_pata_exterior_der.vulnerable && !punto_debil_pata_exterior_der.destruido)
			{
				if(punto_debil_pata_exterior_der.alcanzar(proyectil->acc_potencia()))
				{
					generar_explosion_parte(caja_punto_debil_pata_exterior_der);
					comprobar_destruccion();
				}
			}
		break;
		case POS_PATA_INTERIOR_DER_PUNTO_DEBIL:
			if(punto_debil_pata_interior_der.vulnerable && !punto_debil_pata_interior_der.destruido)
			{
				if(punto_debil_pata_interior_der.alcanzar(proyectil->acc_potencia()))
				{
					generar_explosion_parte(caja_punto_debil_pata_interior_der);
					comprobar_destruccion();
				}
			}
		break;
		case POS_CANON_TRASERO_IZQ:
			if(punto_debil_canon_izq.vulnerable && !punto_debil_canon_izq.destruido)
			{
				if(punto_debil_canon_izq.alcanzar(proyectil->acc_potencia()))
				{
					generar_explosion_parte(caja_canon_izq);
					comprobar_destruccion();
				}
			}
		break;
		case POS_CANON_TRASERO_DER:
			if(punto_debil_canon_der.vulnerable && !punto_debil_canon_der.destruido)
			{
				if(punto_debil_canon_der.alcanzar(proyectil->acc_potencia()))
				{
					generar_explosion_parte(caja_canon_der);
					comprobar_destruccion();
				}
			}
		break;
		case POS_CUBIERTA_IZQ:
			if(cubierta_izq.es_acertada() && cubierta_izq.herir(proyectil->acc_potencia()))
			{
				generar_explosion_parte(cubierta_izq.acc_caja_debil());
				comprobar_destruccion();
			}
		break;

		case POS_CUBIERTA_DER:
			if(cubierta_der.es_acertada() && cubierta_der.herir(proyectil->acc_potencia()))
			{
				generar_explosion_parte(cubierta_der.acc_caja_debil());
				comprobar_destruccion();
			}
		break;

		case POS_CUERPO:
			if(cuerpo.es_acertado() && cuerpo.herir(proyectil->acc_potencia()))
			{
				comprobar_destruccion();
			}
		break;
	}

	return false;
}

void Boss_02::generar_explosion_parte(const SDL_Rect& caja)
{
	int x=caja.x+(caja.w / 2)-16;
	int y=caja.y+(caja.h / 2)-16;

	insertar_decoracion(Estructura_decoracion_boss(x, y, 
				Estructura_decoracion_boss::ED_EXPLOSION,
				10, 0, 0,
				(W_PUNTO_DEBIL / 2), (H_PUNTO_DEBIL / 2)));
}

void Boss_02::generar_humo_desde_caja(const SDL_Rect& caja)
{
	int x=caja.x+(caja.w / 2)-8; //este -8 es por el ancho del humo.
	int y=caja.y+(caja.h / 2)-8;
	insertar_decoracion(Estructura_decoracion_boss(x, y, Estructura_decoracion_boss::ED_HUMO, 0.7, 0, -200));
}
/*Aquí se controla la representación de las partes desde el punto 0.0 del propio
actor... Cada una de las partes tiene una posición real en el mundo que es
un distinta de modo que la equilibramos con la posición del boss en si. En este
punto ya no hay nada que hacer con las posiciones como constantes.
*/

void Boss_02::controlar_representacion()
{
	//Cálculos de recortes...

	int x_rep_punto_debil_izq=Boss_02::Cubierta::X_REP_PUNTO_DEBIL_IZQ;
	int x_rep_punto_debil_der=Boss_02::Cubierta::X_REP_PUNTO_DEBIL_DER;
	int x_rep_punto_debil_cuerpo=X_REP_PUNTO_DEBIL_CUERPO;
	int x_rep_canon_cuerpo=X_REP_CANON_CUERPO + (cuerpo.obtener_fase_canon() * W_REP_CANON_CUERPO);

	if(cubierta_izq.es_herida()) x_rep_punto_debil_izq=Boss_02::Cubierta::X_REP_PUNTO_DEBIL_IZQ_ROJO;
	if(cubierta_der.es_herida()) x_rep_punto_debil_der=Boss_02::Cubierta::X_REP_PUNTO_DEBIL_DER_ROJO;
	if(cuerpo.es_herido()) x_rep_punto_debil_cuerpo=X_REP_PUNTO_DEBIL_CUERPO_ROJO;
	rep_canon_cuerpo->establecer_recorte(x_rep_canon_cuerpo, Y_REP_CANON_CUERPO, W_REP_CANON_CUERPO, H_REP_CANON_CUERPO);

	rep_cubierta_izq_punto_debil->establecer_recorte(x_rep_punto_debil_izq, Boss_02::Cubierta::Y_REP_PUNTO_DEBIL, Boss_02::Cubierta::W_PUNTO_DEBIL, Boss_02::Cubierta::H_PUNTO_DEBIL);
	rep_cubierta_der_punto_debil->establecer_recorte(x_rep_punto_debil_der, Boss_02::Cubierta::Y_REP_PUNTO_DEBIL, Boss_02::Cubierta::W_PUNTO_DEBIL, Boss_02::Cubierta::H_PUNTO_DEBIL);
	rep_cuerpo_punto_debil->establecer_recorte(x_rep_punto_debil_cuerpo, Y_REP_PUNTO_DEBIL_CUERPO, W_PUNTO_DEBIL, H_PUNTO_DEBIL);

	rep_canon_izq_punto_debil->establecer_recorte(punto_debil_canon_izq.obtener_x_representacion(), punto_debil_canon_izq.obtener_y_representacion(), W_PUNTO_DEBIL_CANON, H_PUNTO_DEBIL_CANON);
	rep_canon_der_punto_debil->establecer_recorte(punto_debil_canon_der.obtener_x_representacion(), punto_debil_canon_der.obtener_y_representacion(), W_PUNTO_DEBIL_CANON, H_PUNTO_DEBIL_CANON);
	rep_pata_exterior_izq_punto_debil->establecer_recorte(punto_debil_pata_exterior_izq.obtener_x_representacion(), punto_debil_pata_exterior_izq.obtener_y_representacion(), W_PUNTO_DEBIL_PATA, H_PUNTO_DEBIL_PATA);
	rep_pata_interior_izq_punto_debil->establecer_recorte(punto_debil_pata_interior_izq.obtener_x_representacion(), punto_debil_pata_interior_izq.obtener_y_representacion(), W_PUNTO_DEBIL_PATA, H_PUNTO_DEBIL_PATA);
	rep_pata_exterior_der_punto_debil->establecer_recorte(punto_debil_pata_exterior_der.obtener_x_representacion(), punto_debil_pata_exterior_der.obtener_y_representacion(), W_PUNTO_DEBIL_PATA, H_PUNTO_DEBIL_PATA);
	rep_pata_interior_der_punto_debil->establecer_recorte(punto_debil_pata_interior_der.obtener_x_representacion(), punto_debil_pata_interior_der.obtener_y_representacion(), W_PUNTO_DEBIL_PATA, H_PUNTO_DEBIL_PATA);

	if(pincho_pata_interior_izq.debe_reajustar_representacion()) rep_pincho_pata_interior_izq->establecer_recorte(pincho_pata_interior_izq.obtener_rect_representacion());
	if(pincho_pata_interior_der.debe_reajustar_representacion()) rep_pincho_pata_interior_der->establecer_recorte(pincho_pata_interior_der.obtener_rect_representacion());
	if(pincho_pata_exterior_izq.debe_reajustar_representacion()) rep_pincho_pata_exterior_izq->establecer_recorte(pincho_pata_exterior_izq.obtener_rect_representacion());
	if(pincho_pata_exterior_der.debe_reajustar_representacion()) rep_pincho_pata_exterior_der->establecer_recorte(pincho_pata_exterior_der.obtener_rect_representacion());

	//Cálculos de posiciones...
	float y_final_pei=-(y-pata_exterior_izq.acc_y()); //Patas
	float y_final_pii=-(y-pata_interior_izq.acc_y());
	float y_final_ped=-(y-pata_exterior_der.acc_y());
	float y_final_pid=-(y-pata_interior_der.acc_y());

	float y_final_pdpei=y_final_pei+Y_PUNTO_DEBIL_PATA_EXTERIOR_IZQ; //Puntos débiles patas
	float y_final_pdpii=y_final_pii+Y_PUNTO_DEBIL_PATA_INTERIOR_IZQ;
	float y_final_pdped=y_final_ped+Y_PUNTO_DEBIL_PATA_EXTERIOR_DER;
	float y_final_pdpid=y_final_pid+Y_PUNTO_DEBIL_PATA_INTERIOR_DER;

	float y_final_ppei=y_final_pei+Y_PINCHO_PATA_EXTERIOR_IZQ; //Pinchos patas
	float y_final_ppii=y_final_pii+Y_PINCHO_PATA_INTERIOR_IZQ;
	float y_final_pped=y_final_ped+Y_PINCHO_PATA_EXTERIOR_DER;
	float y_final_ppid=y_final_pid+Y_PINCHO_PATA_INTERIOR_DER;

	float y_final_cuerpo=-(y-cuerpo.acc_y());
	float x_final_cubierta_izq=cubierta_izq.acc_x()-x;
	float x_final_cubierta_der=cubierta_der.acc_x()-x;
	float x_final_cubierta_izq_punto_debil=cubierta_izq.acc_x()+X_DEBIL_CUBIERTA_IZQ-x;
	float x_final_cubierta_der_punto_debil=cubierta_der.acc_x()+X_DEBIL_CUBIERTA_DER-x;

	//Establecer posiciones.
	rep_pata_exterior_izq->establecer_posicion(X_PATAS_EXTERIORES_IZQ, y_final_pei);
	rep_pata_interior_izq->establecer_posicion(X_PATAS_INTERIORES_IZQ, y_final_pii);
	rep_pata_exterior_der->establecer_posicion(X_PATAS_EXTERIORES_DER, y_final_ped);
	rep_pata_interior_der->establecer_posicion(X_PATAS_INTERIORES_DER, y_final_pid);

	rep_pata_exterior_izq_punto_debil->establecer_posicion(X_PATAS_EXTERIORES_IZQ+X_PUNTO_DEBIL_PATA_EXTERIOR_IZQ, y_final_pdpei);
	rep_pata_interior_izq_punto_debil->establecer_posicion(X_PATAS_INTERIORES_IZQ+X_PUNTO_DEBIL_PATA_INTERIOR_IZQ, y_final_pdpii);
	rep_pata_exterior_der_punto_debil->establecer_posicion(X_PATAS_EXTERIORES_DER+X_PUNTO_DEBIL_PATA_EXTERIOR_DER, y_final_pdped);
	rep_pata_interior_der_punto_debil->establecer_posicion(X_PATAS_INTERIORES_DER+X_PUNTO_DEBIL_PATA_INTERIOR_DER, y_final_pdpid);

	rep_pincho_pata_exterior_izq->establecer_posicion(X_PATAS_EXTERIORES_IZQ+X_PINCHO_PATA_EXTERIOR_IZQ, y_final_ppei);
	rep_pincho_pata_interior_izq->establecer_posicion(X_PATAS_INTERIORES_IZQ+X_PINCHO_PATA_INTERIOR_IZQ, y_final_ppii);
	rep_pincho_pata_exterior_der->establecer_posicion(X_PATAS_EXTERIORES_DER+X_PINCHO_PATA_EXTERIOR_DER, y_final_pped);
	rep_pincho_pata_interior_der->establecer_posicion(X_PATAS_INTERIORES_DER+X_PINCHO_PATA_INTERIOR_DER, y_final_ppid);

	rep_cuerpo->establecer_posicion(X_CUERPO, y_final_cuerpo);
	rep_canon_cuerpo->establecer_posicion(X_CUERPO+OFFSET_X_REP_CANON, y_final_cuerpo+OFFSET_Y_REP_CANON);
	rep_cuerpo_punto_debil->establecer_posicion(X_CUERPO+X_PUNTO_DEBIL, y_final_cuerpo+Y_PUNTO_DEBIL);
	rep_canon_izq_punto_debil->establecer_posicion(X_CUERPO+X_PUNTO_DEBIL_CANON_IZQ, y_final_cuerpo+Y_PUNTO_DEBIL_CANON_IZQ);
	rep_canon_der_punto_debil->establecer_posicion(X_CUERPO+X_PUNTO_DEBIL_CANON_DER, y_final_cuerpo+Y_PUNTO_DEBIL_CANON_DER);
	rep_aviso_laser_trasero_izq->establecer_posicion(X_CUERPO+X_CAJA_AVISO_CANON_IZQ, y_final_cuerpo+Y_CAJA_AVISO_CANON);
	rep_aviso_laser_trasero_der->establecer_posicion(X_CUERPO+X_CAJA_AVISO_CANON_DER, y_final_cuerpo+Y_CAJA_AVISO_CANON);

	rep_cubierta_izq->establecer_posicion(x_final_cubierta_izq, Y_CUBIERTAS);
	rep_cubierta_der->establecer_posicion(x_final_cubierta_der, Y_CUBIERTAS);
	rep_cubierta_izq_punto_debil->establecer_posicion(x_final_cubierta_izq_punto_debil, Y_DEBIL_CUBIERTA_IZQ);
	rep_cubierta_der_punto_debil->establecer_posicion(x_final_cubierta_der_punto_debil, Y_DEBIL_CUBIERTA_DER);

	//Los lásers sólo e muestran cuando son colisionables...

	if(laser_trasero_izq.es_colisionable())
	{
		float x_final_laser_trasero_izq=laser_trasero_izq.acc_x()-x+OFFSET_LASER_X;
		float y_final_laser_trasero_izq=-(y-laser_trasero_izq.acc_y());;

		rep_laser_trasero_izq->hacer_visible();
		rep_laser_trasero_izq->actualizar_caja_patron(laser_trasero_izq.obtener_rect_representacion());		
		rep_laser_trasero_izq->establecer_posicion(x_final_laser_trasero_izq, y_final_laser_trasero_izq, W_REP_LASER, laser_trasero_izq.acc_h());
	}
	else
	{
		rep_laser_trasero_izq->hacer_invisible();
	}

	if(laser_trasero_der.es_colisionable())
	{
		float x_final_laser_trasero_der=laser_trasero_der.acc_x()-x+OFFSET_LASER_X;
		float y_final_laser_trasero_der=-(y-laser_trasero_der.acc_y());;

		rep_laser_trasero_der->hacer_visible();
		rep_laser_trasero_der->actualizar_caja_patron(laser_trasero_der.obtener_rect_representacion());		
		rep_laser_trasero_der->establecer_posicion(x_final_laser_trasero_der, y_final_laser_trasero_der, W_REP_LASER, laser_trasero_der.acc_h());
	}
	else
	{
		rep_laser_trasero_der->hacer_invisible();
	}

	//Colores de aviso de láser...
	rep_aviso_laser_trasero_izq->mut_pixel_color(laser_trasero_izq.obtener_color_canon());
	rep_aviso_laser_trasero_der->mut_pixel_color(laser_trasero_der.obtener_color_canon());

}

/*Tiene una representación agrupada en varias partes... cuatro patas, dos 
cubiertas de cabeza y un cuerpo. Tiene además representaciones para puntos 
débils, un par de láser y yo que sé que más.*/

void Boss_02::establecer_representacion()
{
	//El cuerpo y cubiertas...
	rep_cuerpo=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_cuerpo->establecer_recorte(0, 0, W_CUERPO, H_CUERPO);

	rep_canon_cuerpo=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_canon_cuerpo->establecer_recorte(X_REP_CANON_CUERPO, Y_REP_CANON_CUERPO, W_REP_CANON_CUERPO, H_REP_CANON_CUERPO);

	rep_cuerpo_punto_debil=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));

	rep_cubierta_izq=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_cubierta_izq->establecer_recorte(256, 0, W_CUBIERTAS, H_CUBIERTAS);

	rep_cubierta_izq_punto_debil=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));

	rep_cubierta_der=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_cubierta_der->establecer_recorte(320, 0, W_CUBIERTAS, H_CUBIERTAS);

	rep_cubierta_der_punto_debil=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));

	//Cañones...
	rep_canon_izq_punto_debil=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_canon_der_punto_debil=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	
	//Patas izquierda...
	rep_pata_interior_izq=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_pata_interior_izq->establecer_recorte(384, 0, W_PATAS_INTERIORES, H_PATAS_INTERIORES);

	rep_pata_exterior_izq=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_pata_exterior_izq->establecer_recorte(512, 0, W_PATAS_EXTERIORES, H_PATAS_EXTERIORES);

	//Patas derecha...
	rep_pata_interior_der=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_pata_interior_der->establecer_recorte(384, 96, W_PATAS_INTERIORES, H_PATAS_INTERIORES);

	rep_pata_exterior_der=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_pata_exterior_der->establecer_recorte(512, 128, W_PATAS_EXTERIORES, H_PATAS_EXTERIORES);

	//Puntos débiles de patas...
	rep_pata_interior_izq_punto_debil=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_pata_interior_der_punto_debil=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_pata_exterior_izq_punto_debil=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_pata_exterior_der_punto_debil=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));

	//Los pinchos...
	rep_pincho_pata_interior_izq=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_pincho_pata_interior_der=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_pincho_pata_exterior_izq=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));
	rep_pincho_pata_exterior_der=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_BOSS_02));

	rep_pincho_pata_interior_izq->establecer_recorte(pincho_pata_interior_izq.obtener_rect_representacion());
	rep_pincho_pata_interior_der->establecer_recorte(pincho_pata_interior_der.obtener_rect_representacion());
	rep_pincho_pata_exterior_izq->establecer_recorte(pincho_pata_exterior_izq.obtener_rect_representacion());
	rep_pincho_pata_exterior_der->establecer_recorte(pincho_pata_exterior_der.obtener_rect_representacion());

	rep_aviso_laser_trasero_izq=new DLibV::Primitiva_grafica_caja_dinamica(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, W_CAJA_AVISO_CANON, H_CAJA_AVISO_CANON), 
		6453632);

	rep_aviso_laser_trasero_der=new DLibV::Primitiva_grafica_caja_dinamica(
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, W_CAJA_AVISO_CANON, H_CAJA_AVISO_CANON), 
		6453632);

	rep_laser_trasero_izq=new DLibV::Representacion_patron_dinamica(
		laser_trasero_izq.acc_posicion(), 
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0),
		DLibV::Gestor_color::color(0, 255, 255), 
		DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUEGO));

	rep_laser_trasero_der=new DLibV::Representacion_patron_dinamica(
		laser_trasero_der.acc_posicion(), 
		DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0),
		DLibV::Gestor_color::color(0, 255, 255), 
		DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUEGO));


	//Importante: La representación se hace cargo de liberar las representaciones
	//que se hayan agrupado. Aquí la liberamos de eso con el false.

	this->establecer_representacion_agrupada_dinamica(false);
	this->establecer_recorte(0, 0, w, h);

	//El orden es importante: al fondo van las patas, enmedio el cuerpo y arriba las cubiertas...

	this->insertar_en_representacion_agrupada(rep_pata_exterior_izq);
	this->insertar_en_representacion_agrupada(rep_pincho_pata_exterior_izq);
 	this->insertar_en_representacion_agrupada(rep_pata_exterior_izq_punto_debil);

	this->insertar_en_representacion_agrupada(rep_pata_interior_izq);
	this->insertar_en_representacion_agrupada(rep_pincho_pata_interior_izq);
	this->insertar_en_representacion_agrupada(rep_pata_interior_izq_punto_debil); 

	this->insertar_en_representacion_agrupada(rep_pata_exterior_der); 
	this->insertar_en_representacion_agrupada(rep_pincho_pata_exterior_der); 
	this->insertar_en_representacion_agrupada(rep_pata_exterior_der_punto_debil); 

	this->insertar_en_representacion_agrupada(rep_pata_interior_der);
	this->insertar_en_representacion_agrupada(rep_pincho_pata_interior_der);
	this->insertar_en_representacion_agrupada(rep_pata_interior_der_punto_debil);

	this->insertar_en_representacion_agrupada(rep_cuerpo);
	this->insertar_en_representacion_agrupada(rep_canon_cuerpo);
	this->insertar_en_representacion_agrupada(rep_cuerpo_punto_debil);
	this->insertar_en_representacion_agrupada(rep_canon_izq_punto_debil);
	this->insertar_en_representacion_agrupada(rep_aviso_laser_trasero_izq);
	this->insertar_en_representacion_agrupada(rep_laser_trasero_izq);
	this->insertar_en_representacion_agrupada(rep_canon_der_punto_debil);
	this->insertar_en_representacion_agrupada(rep_aviso_laser_trasero_der);
	this->insertar_en_representacion_agrupada(rep_laser_trasero_der);
	this->insertar_en_representacion_agrupada(rep_cubierta_izq); 
	this->insertar_en_representacion_agrupada(rep_cubierta_izq_punto_debil); 
	this->insertar_en_representacion_agrupada(rep_cubierta_der);
	this->insertar_en_representacion_agrupada(rep_cubierta_der_punto_debil); 

	controlar_representacion();
}

void Boss_02::establecer_caracteristicas()
{
	this->w=704;
	this->h=286;
}

/*La resistencia es 1, pero no se resta nunca*/

void Boss_02::parametrizar_actor_en_nivel_preparado(const Parametros_actor& p)
{
	resistencia=1;
	this->vector_mov.x=0;
	this->vector_mov.y=0;

	//Preparando el trigger...
	Trigger::mut_tag_accion(p.param1);
	Trigger::mut_repetible(false);
}

void Boss_02::generar_proyectil(int px, int py, DLibH::Vector_2d vector, unsigned int col, float mult)
{
	Proyectil * resultado=Proyectil::generar(px, py, *this, vector);
	resultado->establecer_potencia(1);
	resultado->establecer_color(col);
	resultado->establecer_velocidad(mult);
	resultado->establecer_faccion(Definiciones::F_NO_PREPARADO 
					| Definiciones::F_BONUS
					| Definiciones::F_TRIGGER);

	insertar_proyectil(resultado);
}

/*Si hay puntos débiles rotos echarán humo. Si hay puntos débiles que se acaban
de destruir, explotarán... etc.*/

void Boss_02::evaluar_y_generar_decoraciones()
{	
	if(punto_debil_canon_izq.puede_echar_humo()) generar_humo_desde_caja(caja_canon_izq);
	if(punto_debil_canon_der.puede_echar_humo()) generar_humo_desde_caja(caja_canon_der);
	if(punto_debil_pata_exterior_izq.puede_echar_humo()) generar_humo_desde_caja(caja_punto_debil_pata_exterior_izq);
	if(punto_debil_pata_exterior_der.puede_echar_humo()) generar_humo_desde_caja(caja_punto_debil_pata_exterior_der);
	if(punto_debil_pata_interior_izq.puede_echar_humo()) generar_humo_desde_caja(caja_punto_debil_pata_interior_izq);
	if(punto_debil_pata_interior_der.puede_echar_humo()) generar_humo_desde_caja(caja_punto_debil_pata_interior_der);
	if(cubierta_izq.puede_echar_humo()) generar_humo_desde_caja(cubierta_izq.acc_caja_debil());
	if(cubierta_der.puede_echar_humo()) generar_humo_desde_caja(cubierta_der.acc_caja_debil());
}

/*Hay distintos orígenes de los disparos...
Pinchos: en su dirección, disparan desde cinco puntos como una muralla. Sólo
	disparan cuando el punto débil de turno no está destruido y el pincho
	en si lo autoriza.
Cubiertas: cada una dispara en dirección al jugador en ráfagas cuando entra 
	dentro de cierto ángulo.
Cuerpo: Cuando el jugador entra en un ángulo se empieza a calentar y dispara a
ráfagas contínuas en dirección al mismo.
*/

void Boss_02::evaluar_y_disparar()
{
	if(estado==E_LOOP)
	{		
		if(!punto_debil_pata_exterior_izq.destruido && pincho_pata_exterior_izq.puede_disparar()) generar_disparos(pincho_pata_exterior_izq);
		if(!punto_debil_pata_exterior_der.destruido && pincho_pata_exterior_der.puede_disparar()) generar_disparos(pincho_pata_exterior_der);
		if(!punto_debil_pata_interior_izq.destruido && pincho_pata_interior_izq.puede_disparar()) generar_disparos(pincho_pata_interior_izq);
		if(!punto_debil_pata_interior_der.destruido && pincho_pata_interior_der.puede_disparar()) generar_disparos(pincho_pata_interior_der);
		if(cubierta_izq.es_cerrada() && cubierta_izq.puede_disparar(blanco)) generar_disparos(cubierta_izq, blanco);
		if(cubierta_der.es_cerrada() && cubierta_der.puede_disparar(blanco)) generar_disparos(cubierta_der, blanco);
		if(cuerpo.puede_disparar()) generar_disparos(cuerpo, blanco);
	}
}

//Genera los cinco disparos del pincho de turno...

void Boss_02::generar_disparos(const Pincho& pincho)
{
	DLibH::Vector_2d v=pincho.obtener_vector_disparos();

	int i=0;
	float x=pincho.acc_x()+(pincho.acc_w() / 2);
	float y=pincho.acc_y()+OFFSET_Y_PRIMER_CANON_PINCHO;

	while(i < CANONES_POR_PINCHO)
	{
		generar_proyectil(x, y, v, Proyectil::C_VERDE);
		y+=DISTANCIA_ENTRE_CANONES_PINCHO;
		++i;
	}

	senal_sonido=senal_sonido | SND_CANON;
}

//Generar disparo de la cubierta...

void Boss_02::generar_disparos(const Cubierta& cubierta, const Actor * const actor)
{
	float x=cubierta.acc_punto_disparo_x();
	float y=cubierta.acc_punto_disparo_y();
	float ax=actor->acc_x() + (actor->acc_w() / 2);
	float ay=actor->acc_y() + (actor->acc_h() / 2);

	DLibH::Vector_2d v=DLibH::Vector_2d::obtener_para_puntos(ax, ay, x, y);
	generar_proyectil(x, y, v, Proyectil::C_VERDE, Cubierta::VELOCIDAD_DISPAROS);

	senal_sonido=senal_sonido | SND_CANON;
}

//Generar disparo del cuerpo...

void Boss_02::generar_disparos(const Cuerpo& cuerpo, const Actor * const actor)
{
	float x=cuerpo.acc_x()+Cuerpo::OFFSET_X_DISPARO;
	float y=cuerpo.acc_y()+Cuerpo::OFFSET_Y_DISPARO;
	float ax=actor->acc_x() + (actor->acc_w() / 2);
	float ay=actor->acc_y() + (actor->acc_h() / 2);

	DLibH::Vector_2d v=DLibH::Vector_2d::obtener_para_puntos(ax, ay, x, y);
	generar_proyectil(x, y, v, Proyectil::C_ROJO, Cuerpo::VELOCIDAD_DISPAROS);

	senal_sonido=senal_sonido | SND_GATLING;
}

unsigned short int Boss_02::obtener_posicion_impacto(Actor const& act) const
{
	if(laser_trasero_izq.es_colisionable() && act.en_colision_con(laser_trasero_izq)) return POS_LASER_TRASERO_IZQ;
	else if(laser_trasero_der.es_colisionable() && act.en_colision_con(laser_trasero_der)) return POS_LASER_TRASERO_DER;
	else if(act.en_colision_con(caja_canon_izq)) return POS_CANON_TRASERO_IZQ;
	else if(act.en_colision_con(caja_canon_der)) return POS_CANON_TRASERO_DER;
	else if(act.en_colision_con(caja_punto_debil_pata_interior_izq)) return POS_PATA_INTERIOR_IZQ_PUNTO_DEBIL;
	else if(act.en_colision_con(caja_punto_debil_pata_interior_der)) return POS_PATA_INTERIOR_DER_PUNTO_DEBIL;
	else if(act.en_colision_con(caja_punto_debil_pata_exterior_izq)) return POS_PATA_EXTERIOR_IZQ_PUNTO_DEBIL;
	else if(act.en_colision_con(caja_punto_debil_pata_exterior_der)) return POS_PATA_EXTERIOR_DER_PUNTO_DEBIL;
	else if(act.en_colision_con(pata_exterior_izq)) return POS_PATA_EXTERIOR_IZQ;
	else if(act.en_colision_con(pata_interior_der)) return POS_PATA_INTERIOR_IZQ;
	else if(act.en_colision_con(pata_exterior_der)) return POS_PATA_EXTERIOR_DER;
	else if(act.en_colision_con(pata_interior_der)) return POS_PATA_INTERIOR_DER;
	else if(cubierta_izq.en_colision_con(act)) return POS_CUBIERTA_IZQ;
	else if(cubierta_der.en_colision_con(act)) return POS_CUBIERTA_DER;
	else if(cuerpo.en_colision_con(act)) return POS_CUERPO;
	else if(pincho_pata_exterior_izq.es_colisionable() && act.en_colision_con(pincho_pata_exterior_izq)) return POS_PINCHO_PATA_EXTERIOR_IZQ;
	else if(pincho_pata_interior_izq.es_colisionable() && act.en_colision_con(pincho_pata_interior_izq)) return POS_PINCHO_PATA_INTERIOR_IZQ;
	else if(pincho_pata_exterior_der.es_colisionable() && act.en_colision_con(pincho_pata_exterior_der)) return POS_PINCHO_PATA_EXTERIOR_DER;
	else if(pincho_pata_interior_der.es_colisionable() && act.en_colision_con(pincho_pata_interior_der)) return POS_PINCHO_PATA_INTERIOR_DER;
	else return POS_NADA;
}

bool Boss_02::en_colision_con(Actor const& act) const
{
	if(estado!=E_LOOP)
	{
		return false;
	}
	else
	{
		unsigned int colision=obtener_posicion_impacto(act);

		if(colision==POS_NADA) return false;
		else switch(act.obtener_tipo_colision())
		{
			//El actor choca con todos los mapas de colisión.
			case Actor::T_TIPO_COLISION_ESTANDAR:
				return true;
			break;

			//El proyectil sólo choca con las cajas de puntos débiles,
			//pinchos y las cubiertas.
	
			case Actor::T_TIPO_COLISION_PROYECTIL:						
				return colision==POS_CUBIERTA_IZQ ||
					colision==POS_CUBIERTA_DER ||
					colision==POS_PATA_EXTERIOR_IZQ_PUNTO_DEBIL ||
					colision==POS_PATA_INTERIOR_IZQ_PUNTO_DEBIL ||
					colision==POS_PATA_EXTERIOR_DER_PUNTO_DEBIL ||
					colision==POS_PATA_INTERIOR_DER_PUNTO_DEBIL ||
					colision==POS_PINCHO_PATA_EXTERIOR_IZQ ||
					colision==POS_PINCHO_PATA_INTERIOR_IZQ ||
					colision==POS_PINCHO_PATA_EXTERIOR_DER ||
					colision==POS_PINCHO_PATA_INTERIOR_DER ||
					colision==POS_CANON_TRASERO_IZQ ||
					colision==POS_CANON_TRASERO_DER ||
					colision==POS_CUERPO;
			break;
		}

		return true;
	}
}

bool Boss_02::realiza_actividad() const 
{
	if(this->es_en_pantalla()) activar();
	return es_activado();
}

bool Boss_02::es_activo() const 
{
	return es_activado();
}

unsigned short int Boss_02::acc_profundidad_representacion() const
{
	switch(estado)
	{
		case E_OCULTO:
		case E_VISIBLE:
		case E_APARECIENDO:
		case E_DESTRUIDO:
		case E_EXPLOSION:
			return 0;
		break;
		case E_LOOP: 
		case E_CAYENDO:
		default: 
			return P_NORMAL; 
		break;
	}
}

void Boss_02::obtener_canal(DLibA::Canal_audio p_canal)
{
	if(laser_trasero_izq.debe_solicitar_canal())
	{
		laser_trasero_izq.asignar_canal(p_canal);
	}
	else if(laser_trasero_der.debe_solicitar_canal())
	{
		laser_trasero_der.asignar_canal(p_canal);
	}
}
