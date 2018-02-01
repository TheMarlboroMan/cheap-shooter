/*
Test de colisión al pixel usando la clase Mapa_colision_bitmap.

No hay limpieza de memoria ni nada, ¿para qué?.
*/

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include "../libDanSDL.h"
#include "../defDanSDL.h"

using namespace DLibA;
using namespace DLibV;
using namespace DLibH;
using namespace DLibI;

int main(int argc, char ** argv)
{	
	if(!Herramientas_SDL::iniciar_SDL())
	{
		return 1;
	}
	else
	{
		//Inicializar pantalla y controles...
		Pantalla * P=new Pantalla();	
		P->inicializar(800, 600, 0);
		P->establecer_titulo("TEST TEST");	
	
		Controles_SDL * C=new Controles_SDL();

		//Cargar recursos.
		Gestor_recursos_graficos::insertar(0, new Imagen("class/motor_sdl/tests_ejemplos/test_01.bmp"));
		Gestor_recursos_graficos::insertar(1, new Imagen("class/motor_sdl/tests_ejemplos/test_02.bmp"));
	
		Gestor_recursos_graficos::obtener(0)->establecer_transparencia(0x00,0xff,0xff);
		Gestor_recursos_graficos::obtener(1)->establecer_transparencia(0x00,0xff,0xff);

		//Crear mapas para recursos.
		Mapa_colision_bitmap m1;
		m1.inicializar(Gestor_recursos_graficos::obtener(0)->acc_superficie(), Gestor_color::color(Gestor_recursos_graficos::obtener(0)->acc_superficie(), 0x00, 0xff, 0xff));

		Mapa_colision_bitmap m2;
		m2.inicializar(Gestor_recursos_graficos::obtener(1)->acc_superficie(), Gestor_color::color(Gestor_recursos_graficos::obtener(1)->acc_superficie(), 0x00, 0xff, 0xff));

		//Crear sprites.
		Representacion_bitmap_dinamica b1(Gestor_recursos_graficos::obtener(0), 10, 10);
		Representacion_bitmap_dinamica b2(Gestor_recursos_graficos::obtener(1), 0, 0);

		//Crear rectángulos que harán de cajas de posición.		
		SDL_Rect pos1=Herramientas_SDL::nuevo_sdl_rect(10, 10, 8, 8);
		SDL_Rect pos2=Herramientas_SDL::nuevo_sdl_rect(0, 0, 8, 8);
		
		bool salir=false;
		SDL_Rect caja_comun=Herramientas_SDL::nuevo_sdl_rect(0,0,0,0);	//Esta es la supuesta caja de colisión...

		while(!salir)
		{
			C->recoger();	//Obtener input.

			SDL_Rect pos3=Herramientas_SDL::nuevo_sdl_rect(pos2.x, pos2.y, pos2.w, pos2.h);	//Esto es el "fantasma" del movimiento...

			if(C->es_senal_salida() || C->es_tecla_down(SDLK_ESCAPE)) salir=true;
			else if(C->es_tecla_down(SDLK_DOWN)) pos3.y++;
			else if(C->es_tecla_down(SDLK_UP)) pos3.y--;
			else if(C->es_tecla_down(SDLK_LEFT)) pos3.x--;
			else if(C->es_tecla_down(SDLK_RIGHT)) pos3.x++;

			bool colision=false;

			//Si hay colisión se captura la caja en "caja_comun".
			if(Herramientas_SDL::rectangulos_superpuestos(pos1, pos3, caja_comun))
			{
				//Estas dos cajas representan el espacio de colisión dentro de los mapas de colisión
				//de cada uno de ellos. Empiezan vacías.
				SDL_Rect c1=Herramientas_SDL::nuevo_sdl_rect(0,0,caja_comun.w,caja_comun.h);
				SDL_Rect c2=Herramientas_SDL::nuevo_sdl_rect(0,0,caja_comun.w,caja_comun.h);

				//Convertimos el espacio de colisión al espacio equivalente en cada mapa.
				//Alto y ancho ya han sido copiados.
				c1.x=abs(pos1.x-caja_comun.x);
				c1.y=abs(pos1.y-caja_comun.y);

				c2.x=abs(pos3.x-caja_comun.x);
				c2.y=abs(pos3.y-caja_comun.y);

				//Comparación a nivel de pixel.
				
				int w=0;
				int h=0;
/*				char * test=Mapa_colision_bitmap::prueba_colision_char_puntero(m1, m2, c1, c2,w,h);
				
				for(int y=0; y<h; y++)
				{
					for(int x=0; x<w; x++)
					{
						std::cout<<test++;
					}
					std::cout<<std::endl;
				}


				return 1;
*/
				if(Mapa_colision_bitmap::comparar_mapas(m1, m2, c1, c2))
				{
					colision=true;
				}
			}

			//Si no hay colisión el fantasma se convierte en la posición.
			if(!colision)
			{
				pos2=pos3;
				b2.establecer_posicion(pos2.x, pos2.y);
			}

			//Volcado en pantalla.	
			P->rellenar(0,0,0);
			P->volcar(b1);
			P->volcar(b2);

			P->flipar();

			SDL_Delay(10);
		}

		return 0;
	}
}
