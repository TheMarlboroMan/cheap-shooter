#include <iostream>
#include <vector>
#include <string>
#include <cmath>

/*Este es un ejemplo para ver cómo se usa un método de píxeles para generar el
color de un pixel a la hora de usar una primitiva gráfica. Estos métodos deben
ser globales o un método estático y público de alguna clase. En función del
tipo de primitiva recibe una serie de parámetros que representarán diferentes
parámetros de la primitiva, como el pixel actual, el inicio, el fin... 
Siempre reciben además un parámetro void que podemos usar de forma personalizada
para pasar cualquier cosa: una instancia de una clase, un contador... Este
parámero lo podemos luego castear dentro del método para usarlo sin problemas.

El ejemplo actual dibuja una línea de colores cambiantes sin mucho sentido.

Este tipo de métodos puede tener una relación estrecha con la superficie de
dibujo, de modo que en el futuro quiźa sea posible plantearse pasar una
referencia a la superficie de turno.
*/


#include "class/motor_sdl/libDanSDL.h"
#include "class/motor_sdl/defDanSDL.h"

Uint32 metodo_pixeles(int x, int y, int inix, int iniy, int finx, int finy, void * datos)
{
	int test=0;

	if(datos!=NULL)
	{
		int * cosa=static_cast <int *> (datos);
		test=*cosa;
	}

	int r=x + test;
	int g=y + test;
	int b=inix+iniy / finx-finy;

	return Gestor_color::color(r, g, b);
}

void cosa()
{
	Pantalla P=Pantalla();
	P.inicializar(800, 600);
	Gestor_color::establecer_formato(P.obtener_formato_pixeles()); 
	Controles_SDL C=Controles_SDL();

	Primitiva_grafica_linea_dinamica L=Primitiva_grafica_linea_dinamica(10, 10, 600, 400); //, Gestor_color::color(255, 40, 40), false);
	L.establecer_metodo_pixeles(metodo_pixeles);

	bool salir=false;
	int i=0;

	while(!salir)
	{
		int a=++i % 255;		

		L.establecer_parametro_metodo_pixeles(&a);
		C.recoger();

		if(C.es_senal_salida() || C.es_tecla_down(SDLK_ESCAPE))
		{
			salir=true;
		}

		P.rellenar(128,128,128);
		P.volcar(L);
		P.flipar();

		SDL_Delay(10);
	}		
}

int main(int argc, char ** argv)
{	
	if(!Herramientas_SDL::iniciar_SDL())
	{
		Herramientas_SDL::error();
	}
	else
	{
		cosa();
		Herramientas_SDL::apagar_SDL();
	}

	return 1;
}
