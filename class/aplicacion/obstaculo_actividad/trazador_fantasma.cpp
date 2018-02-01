#include "trazador_fantasma.h"

const float Trazador_fantasma::pixeles_segundo=1000.0;

Trazador_fantasma::Trazador_fantasma():
	Obstaculo_actividad(), duracion(0),
	contador_reflejo((float) CONTADOR_REFLEJO / 1000)
{
	this->preparar_actor(0);
}

Trazador_fantasma::~Trazador_fantasma()
{

}

void Trazador_fantasma::establecer_caracteristicas()
{
	this->w=32;
	this->h=32;
}

/* Realmente no tiene representación... Tan fácil como eso.*/

void Trazador_fantasma::establecer_representacion()
{

}

bool Trazador_fantasma::debe_dejar_reflejo()
{
	if(contador_reflejo <= 0)
	{
		contador_reflejo=float(CONTADOR_REFLEJO) / 1000;
		return duracion > 0; //Originalmente decía "return true" pero si te teleportabas a un muro dejaba reflejos hasta el infinito por no borrarse. Añadimos esta comprobación extra.
	}
	else
	{
		return false;
	}
}

void Trazador_fantasma::configurar(const Actor * const origen, const Actor * const fin)
{
	vector_mov=Actor::obtener_vector_unidad_desde_hasta(*origen, *fin);
	vector_mov*=pixeles_segundo;

	int 	ox=origen->acc_x(), //_centro(),
		oy=origen->acc_y(), //_centro(),
		fx=fin->acc_x(), //_centro(),
		fy=fin->acc_y(); //_centro();
	
	float distancia=DLibH::Linea::distancia_entre_puntos(ox, oy, fx, fy);

	ir_a(ox, oy);

	duracion=distancia / pixeles_segundo;
	if(duracion < 0) duracion=-duracion;	//Por lo que sea el "abs" no fufa. Supongo que no tengo un template sino que es sólo para enteros.	
} 

void Trazador_fantasma::procesar_turno(float p_delta)
{
	float factor=p_delta / No_jugador::divisor_movimiento;;
	No_jugador::procesar_turno(p_delta);
	duracion-=factor;
	contador_reflejo-=factor;
	if(duracion <=0) 
	{
		marcar_para_borrar();
	}
}
