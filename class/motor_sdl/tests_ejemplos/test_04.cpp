#include "class/motor_sdl/libDanSDL.h"
#include "class/motor_sdl/defDanSDL.h"

/*Primitivas gráficas, recortes y cámaras: un ejemplo práctico para hacer
pruebas de debug sobre el código que ya tenemos. Prácticamente cada tecla hace
una cosa: mucho ojo con las posiciones ilegales: no se controlan!!!.
*/
int main(int argc, char ** argv)
{	
	if(!Herramientas_SDL::iniciar_SDL(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK))
	{
		Herramientas_SDL::error();
		return 1;
	}
	else
	{	
		Pantalla P=Pantalla();
		P.inicializar(800, 600);
		P.establecer_titulo("DA TEST");
		Gestor_color::establecer_formato(P.obtener_formato_pixeles());
		Controles_SDL C=Controles_SDL();

		Camara CAM=Camara(0, 0, 400, 400);
		CAM.mut_pos_x(400);
		CAM.mut_pos_y(32);

		int x=10, y=10, w=50, h=50;
		int xcam=0, ycam=0;
		int xposc=60, yposc=60, rc=60, ac=1;
		int xlini=0, ylini=0, xlfin=120, ylfin=120;

		Primitiva_grafica_circunferencia_dinamica cd(xposc, yposc, rc, ac, 0);
		Primitiva_grafica_linea_dinamica ld(xlini, ylini, xlfin, ylfin);

		do
		{
			cd.establecer_recorte(x, y, w, h);
			ld.establecer_recorte(x, y, w, h);

			ld.mut_puntos(xlini, ylini, xlfin, ylfin);
			cd.mut_circunferencia(xposc, yposc, rc, ac);

			C.recoger();				
			CAM.enfocar_a(xcam, ycam);

			SDL_Rect pos=cd.acc_posicion();
			SDL_Rect rec=cd.acc_recorte();
			SDL_Rect cosa=Herramientas_SDL::nuevo_sdl_rect(pos.x+rec.x, pos.y+rec.y, rec.w, rec.h);

			cd.mut_pixel_color(Gestor_color::color(255, 10, 10));
			ld.mut_pixel_color(Gestor_color::color(255, 10, 10));
			P.rellenar(192, 192, 192);
			P.rellenar(64, 64, 64, cosa);
			P.volcar(cd);
			P.volcar(ld);

			P.rellenar(64, 64, 64, CAM.acc_caja_pos());
			cd.mut_pixel_color(Gestor_color::color(10, 255, 10));
			ld.mut_pixel_color(Gestor_color::color(10, 255, 10));
			P.volcar(cd, CAM);
			P.volcar(ld, CAM);

			P.flipar();

			if(C.es_tecla_pulsada(SDLK_LEFT)) x--;
			if(C.es_tecla_pulsada(SDLK_RIGHT)) x++;
			if(C.es_tecla_pulsada(SDLK_UP)) y--;
			if(C.es_tecla_pulsada(SDLK_DOWN)) y++;

			if(C.es_tecla_pulsada(SDLK_z)) xcam--;
			else if(C.es_tecla_pulsada(SDLK_x)) xcam++;
			if(C.es_tecla_pulsada(SDLK_c)) ycam--;
			else if(C.es_tecla_pulsada(SDLK_v)) ycam++;

			if(C.es_tecla_pulsada(SDLK_1)) w--;
			if(C.es_tecla_pulsada(SDLK_2)) w++;
			if(C.es_tecla_pulsada(SDLK_3)) h--;
			if(C.es_tecla_pulsada(SDLK_4)) h++;

			//Cosas del circulo..
			if(C.es_tecla_pulsada(SDLK_q)) xposc--;
			else if(C.es_tecla_pulsada(SDLK_w)) xposc++;
			if(C.es_tecla_pulsada(SDLK_e)) yposc--;
			else if(C.es_tecla_pulsada(SDLK_r)) yposc++;
			if(C.es_tecla_pulsada(SDLK_t)) rc--;
			else if(C.es_tecla_pulsada(SDLK_y)) rc++;
			if(C.es_tecla_pulsada(SDLK_u)) ac--;
			else if(C.es_tecla_pulsada(SDLK_i)) ac++;

			//Cosas de la línea...
			if(C.es_tecla_pulsada(SDLK_a)) xlini--;
			else if(C.es_tecla_pulsada(SDLK_s)) xlini++;
			if(C.es_tecla_pulsada(SDLK_d)) ylini--;
			else if(C.es_tecla_pulsada(SDLK_f)) ylini++;
			if(C.es_tecla_pulsada(SDLK_g)) xlfin--;
			else if(C.es_tecla_pulsada(SDLK_h)) xlfin++;
			if(C.es_tecla_pulsada(SDLK_j)) ylfin--;
			else if(C.es_tecla_pulsada(SDLK_k)) ylfin++;

			SDL_Delay(10);	
		}while(!C.es_senal_salida());
		
		Herramientas_SDL::apagar_SDL();	
	}
}
