#ifndef CONTROLADOR_RETOS_H
#define CONTROLADOR_RETOS_H

#include "../reto/reto.h"
#include "../definiciones/definicion_retos.h"
#include "../localizador/localizador.h"
#include "../perfil_datos/perfil_datos.h"
#include <vector>
//#include <algorithm>

/*Esta clase está aquí para controlar que los retos se van cumpliendo. 
Algunos retos se cumplirán comparando los datos guardados en el perfil del 
jugador y otros son eventos que se dispararán manualmente. La clase 
también guarda un vector con todos los retos que hay disponibles, para que
podamos tirar de ellos y cosas así. 

Para comprobar si el usuario va cumpliendo los retos se implementan los métodos 
"comprobar_retos" y "disparar_reto". El primero comprobaría automáticamente
los retos contra el perfil, activando aquellos que se hayan cumplido. El 
segundo forzaría el cumplimiento de un reto.

Para cumplir un reto comprobaremos antes que el usuario no lo ha conseguido
anteriormente. Si no lo ha conseguido se cumplirá y se meterá en un vector
que se servirá al controlador. Este vector contendrá los retos cumplidos desde
la última comprobación y se usará para dibujarlos en pantalla.

Cada reto tiene un identificador propio que está definido en la clase Reto.
Esta clase tiene también un puñado de constantes que se usarán para comprobar
que las cifras requeridas se están alcanzando.
*/

class Controlador_retos
{
	private:

	static const unsigned int N_PARTIDAS=3;
	static const unsigned int N_DISPAROS=1;
	static const unsigned int N_OBSTACULOS_DESTRUIDOS=5;
	static const unsigned int N_GEMAS=5;
	static const unsigned int N_COMBO=1;
	static const unsigned int N_VIDAS_PERDIDAS=2;
	static const unsigned int N_PARAR_TIEMPO=2;
	static const unsigned int N_TELETRANSPORTE=100;
	static const unsigned int N_EDU=200;
	static const unsigned int N_MINAS_DESTRUIDAS=5;
		
	//Esta debe leerse desde el controlador de juego.
	public: static const unsigned long int N_PUNTUACION=500;

	private:

	typedef std::vector<Reto *> t_vector_retos;
	
	t_vector_retos vector_retos;
	t_vector_retos retos_recien_cumplidos;	//Este es el vector de los cumplidos desde la última comprobación. De nuevo, son punteros.

	void insertar_reto(unsigned int, std::string, std::string);
	void conseguir_reto(Perfil_datos &, unsigned int, unsigned int=0, unsigned int=0);
	
	bool implementacion_reto_numerico(Perfil_datos &, unsigned int, unsigned int, unsigned int);
	
	public:	

	Controlador_retos();
	~Controlador_retos();
	void inicializar(Localizador &);	

	bool comprobar_retos(Perfil_datos &);
	bool disparar_reto(unsigned int, Perfil_datos &);
	Reto * turno_controlador(float);
	Reto * obtener_por_indice(unsigned int);
	t_vector_retos & acc_retos_recien_cumplidos() {return this->retos_recien_cumplidos;}
};

#endif

