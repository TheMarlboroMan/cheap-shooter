#ifndef OBSTACULO_PUERTA_H
#define OBSTACULO_PUERTA_H

#include "obstaculo_actividad.h"

/*
Este es una puerta de ciertas dimensiones. La usaremos como clase abstracta
para hacer puertas que se abran al disparar, por ejemplo, que se abran
y se cierren constantemente...
La puerta es horizontal y tiene dos rectángulos de colisión internos, la hoja
izquierda y la hoja derecha.
La hoja izquierda sólo cambia de ancho hasta hacerse cero. La hoja derecha
se mueve y pliega hasta hacerse cero en la derecha del todo. La hoja derecha
es la que se usa como control.
*/

class Obstaculo_puerta:public Obstaculo_actividad
{
	private:

	void actualizar_representacion_hojas();
	void accion_hojas(float, short int);
	bool abrir_hojas(float);
	bool cerrar_hojas(float);

	static const int Y_MAPA_BITS_HOJA_IZQUIERDA=208;	
	static const int Y_MAPA_BITS_HOJA_DERECHA=192;
	static const int W_MAPA_BITS_HOJAS=48;
	static const int H_MAPA_BITS_HOJAS=16;
	static const int X_MAPA_BITS_HOJA_IZQUIERDA=80;
	static const int X_MAPA_BITS_HOJA_DERECHA=80;

	protected:

	DLibV::Representacion_bitmap_dinamica * rep_hoja_izquierda;
	DLibV::Representacion_bitmap_dinamica * rep_hoja_derecha;

	SDL_Rect hoja_izquierda;
	SDL_Rect hoja_derecha;

	//Los estados determinan
	enum ESTADOS{
	E_CERRADO,
	E_INICIO_ABRIR,
	E_ABRIENDO,
	E_FIN_ABRIR,
	E_ABIERTO,
	E_INICIO_CERRAR,
	E_FIN_CERRAR,
	E_CERRANDO
	};

	unsigned int estado_actual;

	void establecer_representacion();
	void establecer_caracteristicas();
	void cambiar_estado(unsigned int);    //Establece un nuevo estado para la puerta.
	
	float vel_max() {return 0;}
	float vel_min() {return 0;}

	float velocidad_hojas;    //La velocidad a la que se moverán las hojas.

	float x_hoja_derecha_actual;
	float x_hoja_derecha_cerrada;
	float x_hoja_derecha_abierta;

	public:

	Obstaculo_puerta(unsigned int);
	virtual ~Obstaculo_puerta();

	bool es_afectado_por_edu() const {return false;}
	short int puntuacion_por_destruir() {return 0;}
	void procesar_turno(float=0);    //Esto debe escanear el estado y hacer lo que tenga que hacer. Luego, además, pasará el control a un método particular para implementar.
	virtual void implementacion_procesar_turno(float);    //Este viene implementado vacio, pero lo puedes llamar con cada puerta para hacer algo después de procesar el turno.
	bool en_colision_con(Actor const&) const;
	virtual bool es_fuera_de_juego() const {return false;}
	void preparar_actor_mapa_manual(float, float, float, float);

	//Todas estas deben ser implementadas.
	virtual unsigned short int actuar_sonido()=0;
	virtual void parametrizar_actor_en_nivel_preparado(const Parametros_actor&)=0;
	virtual void cuentame()=0;
	virtual short unsigned int tipo_actor_mapa()=0;
	virtual bool es_activo() const;
	virtual void aceptar_visitante(Visitante& v)=0;
};

#endif
