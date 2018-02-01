#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdint>

#include "class/motor_sdl/libDanSDL.h"
#include "class/motor_sdl/defDanSDL.h"

#include "class/aplicacion/definiciones/definiciones.h"

#include "class/aplicacion/controlador/controlador.h"
#include "class/aplicacion/controlador_juego/controlador_juego.h"
#include "class/aplicacion/no_jugador/no_jugador.h"
#include "class/aplicacion/lector_puntuaciones/lector_puntuaciones.h"

/*TODO 
	REVISIONES

- Comprobar uno por uno todos los trofeos.
- Al finalizar todo el juego, hacer un paseo de todo y ver si los destructores funcionan.

	BUGS

- Sala parada nivel 03, se puede joder si le das al botón antes de tiempo. Poner puertas.
- Si te liquidan al cambiar de artefacto se conserva el mismo (o no se cargan los originales).

	POR HACER

- Puntos de control: eliminar el "mínimo de gemas para nivel" y reemplazarlo por
	máximo de gemas a perder antes del próximo puntos de control. Conservar
	en memoria de alguna forma las cosas destruidas y recogidas (???). 
	El punto de control te diría cuantas gemas puedes perder antes del siguiente.
	Usar alguna barra del HUD para indicar cuánto queda del nivel aún y 
	dónde están los puntos de control????.
- Un cargando mejorado.
- Algún tipo de efecto al parar el tiempo??. Algo en los bordes con alpha 128??.
- Diseñar niveles
- Redactar Instrucciones...
- Música
- Gráficos.
- El logo de Cheap Studios debe tener la definición de cheap por detrás.

	IDEAS NIVELES:

- Sala parada, con varios nichos y puertas, mini puzzle.
- Sala circular con cañón giratorio.
- Estructura de piezas que se mueven, el Cheap Cruiser debe navegarlas.
- Red de proyectiles que se "enciende y apaga"
- Cañón apuntado que persigue al Cheap Cruiser en un laberinto de cobertura, por un rail.

	LISTA NIVELES:

v1 - Tutorial
v2 - Aleatorio (I) 	[Zona #32]
v3 - Minas		[Minas de piroxeno] lol.
4 - Hielo		[Satélite de hielo]
5 - Aleatorio (II)	[Zona #66]
6 - Gran nave - templo something en el espacio something.	[Templo de la Máquina]
7 - Persecución de cañón por rail.
8 - Aleatorio (III)	[Zona #104]
9 - ???
s1 - Pirámides :D
s2 - 
s3 - 

Referencias culturales:
	Bloques del 1-1 en super mario bros.

#define NDEBUG para que no haga efecto.
#include <cassert>

assert(a==b);
*/

DLibH::Log_base LOG;
const float Definiciones::T_MAX_DELTA=0.016f; //Este es el máximo salto permisible.

int main(int argc, char ** argv)
{	
	try
	{
		DLibH::Controlador_argumentos CARG(argc, argv);

		if(!DLibH::Herramientas_SDL::iniciar_SDL(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK))
		{
			DLibH::Herramientas_SDL::error();
			return 1;
		}
		else
		{
			Controlador C=Controlador();

			if(!C.inicializar())
			{
				std::cout<<"ERROR AL INICIALIZAR: "<<C.acc_cadena_error()<<std::endl;
				return 1;
			}
			else
			{
				//Si ponemos la cadena "LOG" al ejecutar, se activan los logs de juego y motor.
				if(CARG.buscar("LOG") != -1)
				{
					LOG.inicializar("log_juego.log");
					LOG.activar();
					DLibH::Log_motor::arrancar();
				}
			
				//Si usamos "editor", se abrirá el editor con el siguiente parámetro que demos.
				int pos_editor=CARG.buscar("editor");
				if(pos_editor != -1)
				{
					C.iniciar_y_procesar_editor(CARG.acc_argumento(pos_editor+1));
					return 0;
				}
				else
				{
					C.loop_principal();
					return 0;
				}

				DLibH::Log_motor::finalizar();
				DLibA::Controlador_audio_SDL::desmontar();
				DLibH::Herramientas_SDL::apagar_SDL();
			}
		}
	}
	catch(Excepcion_salida& e)
	{
		DLibH::Log_motor::finalizar();
		DLibA::Controlador_audio_SDL::desmontar();
		DLibH::Herramientas_SDL::apagar_SDL();	
		std::cout<<"ABORTANDO EJECUCION: COD "<<e.cod<<" :: "<<Excepcion_salida::traducir(e.cod)<<std::endl;
		exit(e.cod);
	}
}

