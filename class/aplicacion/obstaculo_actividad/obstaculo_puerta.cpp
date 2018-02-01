#include "obstaculo_puerta.h"

Obstaculo_puerta::Obstaculo_puerta(unsigned int mod_contador)
	:Obstaculo_actividad(),
	rep_hoja_izquierda(NULL), rep_hoja_derecha(NULL),
	estado_actual(E_CERRADO), velocidad_hojas(80.0f), 
	x_hoja_derecha_actual(0),
	x_hoja_derecha_cerrada(0), 
	x_hoja_derecha_abierta(0)
{
	this->preparar_actor(mod_contador);
	this->hoja_izquierda=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,W_MAPA_BITS_HOJAS, H_MAPA_BITS_HOJAS);
	this->hoja_derecha=DLibH::Herramientas_SDL::nuevo_sdl_rect(W_MAPA_BITS_HOJAS,0, W_MAPA_BITS_HOJAS, H_MAPA_BITS_HOJAS);
}

Obstaculo_puerta::~Obstaculo_puerta()
{
	delete this->rep_hoja_izquierda;
	delete this->rep_hoja_derecha;
}

/*Las hojas están como el rectángulo que realmente ocupan en la pantalla. 
Por eso tenemos que calcular la posición restando la posición x del propio 
obstáculo: se espera que las representaciones agrupadas tengan el 0,0 en la
esquina superior izquierda de la representación agrupada en si.
*/

void Obstaculo_puerta::actualizar_representacion_hojas()
{
	//Para la hoja izquierda la posición se queda tal cual. Lo que cambiamos 
	//es el recorte para ir sacando cada vez menos. 
//	int diferencia_izquierda=this->posicion.x-this->hoja_derecha.w;
//	int posicion_izquierda=X_MAPA_BITS_HOJA_IZQUIERDA+diferencia_izquierda;
//	int ancho_izquierda=W_MAPA_BITS_HOJAS-diferencia_izquierda;

	int posicion_izquierda=X_MAPA_BITS_HOJA_IZQUIERDA+(W_MAPA_BITS_HOJAS-this->hoja_derecha.w)+1;
	int ancho_izquierda=this->hoja_derecha.w;
	this->rep_hoja_izquierda->establecer_recorte(posicion_izquierda, Y_MAPA_BITS_HOJA_IZQUIERDA, ancho_izquierda, H_MAPA_BITS_HOJAS);

	//La hoja derecha: la movemos un poco pero además recortamos en la misma
	//proporción.

	this->rep_hoja_derecha->establecer_recorte(X_MAPA_BITS_HOJA_DERECHA, Y_MAPA_BITS_HOJA_DERECHA, this->hoja_derecha.w, H_MAPA_BITS_HOJAS);
	this->rep_hoja_derecha->establecer_posicion(this->hoja_derecha.x-this->posicion.x, 0);

	//Antes las hojas se movían.
//	this->rep_hoja_izquierda->establecer_posicion(this->hoja_izquierda.x-this->posicion.x, 0);
//	this->rep_hoja_derecha->establecer_posicion(this->hoja_derecha.x-this->posicion.x, 0);
}

/*Algún método para cuando se posicione en el mapa. Debe calcular las x
de ambas hojas para estados abiertos y cerrados. Debe establecer la x e y
de las hojas. Llama al método general en actor_mapa.*/

void Obstaculo_puerta::preparar_actor_mapa_manual(float p_x, float p_y, float v_x, float v_y)
{
	Actor_mapa::preparar_actor_mapa_manual(p_x, p_y, v_x, v_y);

	//Cálculo de posiciones...
	this->x_hoja_derecha_cerrada=p_x+W_MAPA_BITS_HOJAS;
	this->x_hoja_derecha_abierta=p_x+(W_MAPA_BITS_HOJAS*2);
	this->x_hoja_derecha_actual=this->x_hoja_derecha_cerrada;

	//Colocar las cajas.
	this->hoja_izquierda.x=this->posicion.x;
	this->hoja_izquierda.y=this->y;

	this->hoja_derecha.x=this->x_hoja_derecha_actual;
	this->hoja_derecha.y=this->y;

	//Actualizar forzosamente las representaciones.
	this->actualizar_representacion_hojas();
}

void Obstaculo_puerta::establecer_representacion()
{
	this->rep_hoja_izquierda=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUEGO));
	this->rep_hoja_izquierda->establecer_recorte(X_MAPA_BITS_HOJA_IZQUIERDA, Y_MAPA_BITS_HOJA_IZQUIERDA, W_MAPA_BITS_HOJAS, H_MAPA_BITS_HOJAS);
	this->rep_hoja_izquierda->establecer_posicion(0, 0);

	this->rep_hoja_derecha=new DLibV::Representacion_bitmap_dinamica(DLibV::Gestor_recursos_graficos::obtener(Recursos::G_TILE_JUEGO));
	this->rep_hoja_derecha->ir_a(80, 0);
	this->rep_hoja_derecha->establecer_recorte(X_MAPA_BITS_HOJA_DERECHA, Y_MAPA_BITS_HOJA_DERECHA, W_MAPA_BITS_HOJAS, H_MAPA_BITS_HOJAS);
	this->rep_hoja_derecha->establecer_posicion(0, 0, W_MAPA_BITS_HOJAS, H_MAPA_BITS_HOJAS);

//Importante: La representación se hace cargo de liberar las representaciones
//que se hayan agrupado. Aquí la liberamos de eso con el false.

	this->establecer_representacion_agrupada_dinamica(false);
	this->establecer_recorte(0,0,2 * W_MAPA_BITS_HOJAS, H_MAPA_BITS_HOJAS);

//ERROR: Tú no estableces la posición: el juego la hará por ti. En este momento aún no hay posición.
//rep->establecer_posicion(this->posicion.x,this->posicion.y,64,32);

	this->insertar_en_representacion_agrupada(this->rep_hoja_izquierda); 
	this->insertar_en_representacion_agrupada(this->rep_hoja_derecha); 
}

void Obstaculo_puerta::establecer_caracteristicas()
{
	this->w=92;
	this->h=16;
	this->resistencia=100;
}

void Obstaculo_puerta::cambiar_estado(unsigned int p_estado)
{
	this->estado_actual=p_estado;
}
	
/*Los estados intermedios (inicio y fin de abriendo y cerrando) son para
permitir sonidos en esos casos. Sólo están implementados los estados para 
iniciar y finalizar apertura y cierre. La implementación de los estados
abierto y cerrrado dependerá ya de cada clase.*/

void Obstaculo_puerta::procesar_turno(float p_delta)
{
	switch(this->estado_actual)
	{
		case E_CERRADO:
			//Nada...
		break;

		case E_INICIO_ABRIR:
			this->estado_actual=E_ABRIENDO;
		break;
	
		case E_ABRIENDO:
			if(this->abrir_hojas(p_delta)) this->estado_actual=E_FIN_ABRIR;
		break;

		case E_FIN_ABRIR:
			this->estado_actual=E_ABIERTO;
		break;

		case E_ABIERTO:
			//Nada...
		break;

		case E_INICIO_CERRAR:
			this->estado_actual=E_CERRANDO;
		break;

		case E_CERRANDO:
			if(this->cerrar_hojas(p_delta)) this->estado_actual=E_FIN_CERRAR;
		break;

		case E_FIN_CERRAR:
			this->estado_actual=E_CERRADO;
		break;
	}

	this->actualizar_representacion_hojas();
	this->implementacion_procesar_turno(p_delta);
}

/*Las hojas no sólo se mueven sino que también se pliegan y despliegan de forma
que cuando están completamente abiertas tienen un ancho de cero. Estos 
métodos se encargan de todo. 

El primer sólo mueve las hojas en la dirección del parámetro p_mult (multiplica
por 1 o -1). Los dos restantes hacen comprobaciones y usan el primero.
*/

void Obstaculo_puerta::accion_hojas(float p_delta, short int p_mult)
{
	//En primer lugar calculamos las nuevas posiciones de X.
	//OJO: No podemos manipular float contra entero. Por eso calculamos 
	//primero en this->x_hoja_derecha_actual.

	float vel=p_mult * this->velocidad_hojas*(p_delta / No_jugador::divisor_movimiento);
	this->x_hoja_derecha_actual+=vel;
	this->hoja_derecha.x=this->x_hoja_derecha_actual;

	//En función de la nueva posición de X calculamos el nuevo ancho.

	int diferencia_derecha=this->hoja_derecha.x-this->posicion.x-W_MAPA_BITS_HOJAS;
	int ancho_calculado=W_MAPA_BITS_HOJAS - diferencia_derecha;

	this->hoja_izquierda.w=ancho_calculado;
	this->hoja_derecha.w=ancho_calculado;
}

bool Obstaculo_puerta::cerrar_hojas(float p_delta)
{
	this->accion_hojas(p_delta, -1);

	//Comprobamos finalmente si nos hemos pasado para restaurarlo todo.

	if(this->x_hoja_derecha_actual <= this->x_hoja_derecha_cerrada)
	{
		this->x_hoja_derecha_actual=this->x_hoja_derecha_cerrada;			
		return true;
	}
	else return false;
}

bool Obstaculo_puerta::abrir_hojas(float p_delta)
{	
	this->accion_hojas(p_delta, 1);	

	if(this->x_hoja_derecha_actual >= this->x_hoja_derecha_abierta)
	{
		this->x_hoja_derecha_actual=this->x_hoja_derecha_abierta;			
		return true;
	}
	else return false;
}

bool Obstaculo_puerta::es_activo() const
{
	return this->estado_actual==E_INICIO_ABRIR ||
		this->estado_actual==E_FIN_ABRIR ||
		this->estado_actual==E_INICIO_CERRAR ||
		this->estado_actual==E_FIN_CERRAR;
}

void Obstaculo_puerta::implementacion_procesar_turno(float p_delta)
{
	//La implementación por defecto es... no hacer nada
}
	
bool Obstaculo_puerta::en_colision_con(const Actor& p_actor) const
{
	return p_actor.en_colision_con(this->hoja_izquierda) 
		|| p_actor.en_colision_con(this->hoja_derecha);
}
