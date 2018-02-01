#ifndef COLA_SONIDO_H
#define COLA_SONIDO_H

#include <queue>
#include "../sonido/sonido.h"
#include "../controlador_audio_sdl/controlador_audio_sdl.h"

/*
La cola de sonido no es más que una envoltura para una cola de punteros a 
Sonido. El gestor de audio puede solicitar a la cola que le cuente cosas por
cada turno y volcaría sonidos mientras hubiera canales disponibles. O 
algo así. La se supone que el primer sonido en entrar es el primer sonido en 
salir (FIFO).

Esta es una clase amiga del controlador_audio_sdl, que está implementada como
un singleton. Al ser amiga puede tener un miembro "turno" que se encargue
de conectar con la instancia del controlador de audio, darle los sonidos y 
eliminarlos de la cola si procede... Tenemos acceso a los métodos manuales
o a "turno", que lo haría por nosotros. Bien pensado no hace falta que sea
clase amiga, pero bueno, ya que está lo dejamos, por si en el futuro es 
aplicable para acceder a los canales y todo eso.

Para un uso seguro se supone que los sonidos están en el gestor de audio.

Cuando hemos creado la Estructura_sonido hacemos cambios para que esto gestione
objetos de ese tipo, con los valores por defecto.
*/

namespace DLibA
{
	class Cola_sonido
	{
		private:

		std::queue<Estructura_sonido> cola;
		void vaciar();
		Controlador_audio_SDL * controlador;

		public:

		Cola_sonido();
		~Cola_sonido();

		void inicializar();
		void insertar(const Estructura_sonido&);
		Estructura_sonido obtener_primero();
		void eliminar_primero();
		void procesar_cola(bool=true);
	};
}
#endif
