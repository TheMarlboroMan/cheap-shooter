#include "herramientas_sdl.h"

using namespace DLibH;

void Herramientas_SDL::error()
{
	std::cout<<SDL_GetError();
}

bool Herramientas_SDL::iniciar_SDL(Uint32 p_flags)
{
	return SDL_Init(p_flags) >= 0;
}

void Herramientas_SDL::apagar_SDL()
{
	SDL_Quit();
}

bool Herramientas_SDL::arrancar_sistema_SDL(Uint32 p_flags)
{
	return SDL_InitSubSystem(p_flags)==0;
}

bool Herramientas_SDL::es_sistema_arrancado_SDL(Uint32 p_flags)
{
	return SDL_WasInit(p_flags)!=0;
}

void Herramientas_SDL::detener_sistema_SDL(Uint32 p_flags)
{
	SDL_QuitSubSystem(p_flags);
}

SDL_Rect Herramientas_SDL::copiar_sdl_rect(SDL_Rect const& p_original, Sint16 mod_x,  Sint16 mod_y, Uint16 mod_w, Uint16 mod_h)
{
	SDL_Rect resultado;
	resultado.x=p_original.x+mod_x;
	resultado.y=p_original.y+mod_y;
	resultado.w=p_original.w+mod_w;
	resultado.h=p_original.h+mod_h;

	return resultado;
}

SDL_Rect Herramientas_SDL::copiar_sdl_rect(SDL_Rect const* p_original, Sint16 mod_x,  Sint16 mod_y, Uint16 mod_w, Uint16 mod_h)
{
	return Herramientas_SDL::copiar_sdl_rect(*p_original, mod_x, mod_y, mod_w, mod_h);
}

SDL_Rect * Herramientas_SDL::copiar_sdl_rect_dinamico(SDL_Rect const& p_original, Sint16 mod_x,  Sint16 mod_y, Uint16 mod_w, Uint16 mod_h)
{
	SDL_Rect * resultado=new SDL_Rect();
	resultado->x=p_original.x+mod_x;
	resultado->y=p_original.y+mod_y;
	resultado->w=p_original.w+mod_w;
	resultado->h=p_original.h+mod_h;

	return resultado;
}

SDL_Rect * Herramientas_SDL::copiar_sdl_rect_dinamico(SDL_Rect const* p_original, Sint16 mod_x,  Sint16 mod_y, Uint16 mod_w, Uint16 mod_h)
{
	return Herramientas_SDL::copiar_sdl_rect_dinamico(*p_original, mod_x, mod_y, mod_w, mod_h);
}

SDL_Rect Herramientas_SDL::nuevo_sdl_rect(Sint16 mod_x, Sint16 mod_y, Uint16 mod_w, Uint16 mod_h)
{
	SDL_Rect resultado;
	resultado.x=mod_x;
	resultado.y=mod_y;
	resultado.w=mod_w;
	resultado.h=mod_h;

	return resultado;
}

 /*
Comprueba si dos rectángulos están superpuestos en sus dimensiones. El 
mátodo más rápido es el contrario: es decir, comprobar que NO se
superponen y luego negar el resultado.
Devuelve true si se superponen, false si no se superponen.
El tercer parámetro sirve para el tipo de comparación. Por defecto para
que haya colisión los valores tienen que superponerse y no simplemente
ser iguales.
*/

bool Herramientas_SDL::rectangulos_superpuestos(SDL_Rect const&plantilla, SDL_Rect const&comparado, bool roce_es_colision)
{
        bool dentro_x;
        bool dentro_y;

        //Comparamos si está fuera y lo negamos...
        if(roce_es_colision)
        {
                dentro_x=!( (comparado.x+comparado.w < plantilla.x) || (comparado.x > plantilla.x+plantilla.w) );
                dentro_y=!( (comparado.y+comparado.h < plantilla.y) || (comparado.y > plantilla.y+plantilla.h) );
        }
        else
        {
                dentro_x=!( (comparado.x+comparado.w <= plantilla.x) || (comparado.x >= plantilla.x+plantilla.w) );
                dentro_y=!( (comparado.y+comparado.h <= plantilla.y) || (comparado.y >= plantilla.y+plantilla.h) );
        }

	/*if(! (dentro_x&&dentro_y))
	{
		std::cout<<comparado.x<<" "<<comparado.w<<" vs "<<plantilla.x<<" "<<plantilla.w<<std::endl;
		std::cout<<comparado.y<<" "<<comparado.h<<" vs "<<plantilla.y<<" "<<plantilla.h<<std::endl;
	}*/

        return dentro_x&&dentro_y;
}

bool Herramientas_SDL::segmentos_superpuestos(int aini, int afin, int bini, int bfin, bool roce_es_colision)
{
 	//Comparamos si está fuera y lo negamos...
        if(roce_es_colision)
        {
                return !( (bfin < aini) || (bini > afin) );
        }
        else
        {
                return !( (bfin <= aini) || (bini >= afin) );
        }
}

/*Con estos dos parámetros devuelve también la parte de las cajas que está DENTRO 
de la colisión dentro de esas propias cajas. Esto lo podemos usar para muchas
cosas luego, como para colisiones por píxeles y esas historias, o para determinar
el origen de una colisión.*/

bool Herramientas_SDL::rectangulos_superpuestos(SDL_Rect const&plantilla, SDL_Rect const&comparado, SDL_Rect &resultado, bool roce_es_colision)
{
	if(!Herramientas_SDL::rectangulos_superpuestos(plantilla, comparado, roce_es_colision))
	{
		return false;
	}
	else
	{
		//Esta caja es la que ambos tienen en común... 
		posicion_dimension_segmentos(plantilla.x, plantilla.x+plantilla.w, comparado.x, comparado.x+comparado.w, resultado.x, resultado.w);
		posicion_dimension_segmentos(plantilla.y, plantilla.y+plantilla.h, comparado.y, comparado.y+comparado.h, resultado.y, resultado.h);

		return true;
	}
}

/*
Suponiendo dos segmentos que se superponen se puede sacar el inicio y el fin de
dicha superposición.
En el archivo tests_ejemplos/test_01.cpp tenemos un ejemplillo de todo esto
y las explicaciones pertintentes...
*/

void Herramientas_SDL::posicion_dimension_segmentos(int aini, int afin, int bini, int bfin, Sint16 &resultado_pos, Uint16 &resultado_dim)
{
	bool c1=aini < bini;
	bool c2=afin < bfin;
	bool c3=bini < aini;
	bool c4=bfin < afin;
	bool c5=aini == bini;
	bool c6=afin == bfin;
	
	//Idénticos...
	if(c5 && c6)
	{
		posicion_dimension_segmentos_solucion_completa(bini, bfin, aini, afin, resultado_pos, resultado_dim);
	}
	//A a la izquierda de B
	else if(c1 && c2) 
	{
		posicion_dimension_segmentos_solucion_parcial(aini, afin, bini, bfin, resultado_pos, resultado_dim); //Caso 1
	}
	//B a la izquierda de A
	else if(c3 && c4) 
	{
		posicion_dimension_segmentos_solucion_parcial(bini, bfin, aini, afin, resultado_pos, resultado_dim); //Caso 2
	}
	//A dentro de B
	else if(!c1 && !c4) 
	{
		posicion_dimension_segmentos_solucion_completa(aini, afin, bini, bfin, resultado_pos, resultado_dim); //Caso 3
	}
	//B dentro de A
	else if(!c2 && !c3) 
	{
		posicion_dimension_segmentos_solucion_completa(bini, bfin, aini, afin, resultado_pos, resultado_dim); //Caso 4
	}
}

/*
Un segmento está parcialmente dentro de otro, como en este ejemplo:

	|------ SEGMENTO A ---------|
			     XXXXXXXX
			     |-------- SEGMENTO B ------|
*/

void Herramientas_SDL::posicion_dimension_segmentos_solucion_parcial(int aini, int afin, int bini, int bfin, Sint16 &resultado_pos, Uint16 &resultado_dim)
{
	resultado_pos=bini;
	resultado_dim=afin-bini;
}

/*Un segmento está completamente dentro de otro....

		|--- SEGMENTO A ----|
	|-------XXXX SEGMENTO B XXXXX ---|
*/

void Herramientas_SDL::posicion_dimension_segmentos_solucion_completa(int aini, int afin, int bini, int bfin, Sint16 &resultado_pos, Uint16 &resultado_dim)
{
	resultado_pos=aini;
	resultado_dim=afin-aini;
}

/*
Calcula si las coordenadas dadas están dentro del bloque que
se pasa por referencia. Devuelve true si el par queda dentro del
rectángulo y false si queda fuera.
*/

bool Herramientas_SDL::punto_en_caja(SDL_Rect const& posicion, int x, int y)
{
        return  (posicion.x <= x && posicion.x+posicion.w >= x)
        &&
        (posicion.y <= y && posicion.y+posicion.h >= y);
}

//Esto mide en relación con coordenadas de pantalla y no con un sistema
//cartesiano. Mide si la caja pequeña está contenida en la grande
//o si son iguales. En ambos casos devolvería verdadero.

bool Herramientas_SDL::caja_en_caja(SDL_Rect const& p_pequena, SDL_Rect const& p_grande)
{
	bool resultado=false;

	int grande_xf=p_grande.x+p_grande.w,
	pequena_xf=p_pequena.x+p_pequena.w,
	grande_yf=p_grande.y+p_grande.h,
	pequena_yf=p_pequena.y+p_pequena.h;

	resultado=(p_grande.x <= p_pequena.x
	&& grande_xf >= pequena_xf
	&& p_grande.y <= p_pequena.y
	&& grande_yf >= pequena_yf);

	return resultado;			
}

void Herramientas_SDL::copiar_caja_de_a(SDL_Rect const& p_origen, SDL_Rect & p_destino)
{
	p_destino.x=p_origen.x;
	p_destino.y=p_origen.y;
	p_destino.w=p_origen.w;
	p_destino.h=p_origen.h;
}

bool Herramientas_SDL::son_cajas_iguales(SDL_Rect const& p_a, SDL_Rect const& p_b)
{
	return p_a.x==p_b.x 
	&& p_a.y==p_b.y
	&& p_a.w==p_b.w
	&& p_a.h==p_b.h;
}

void Herramientas_SDL::describe_caja(SDL_Rect const & p_caja)
{
	std::cout<<p_caja.x<<","<<p_caja.y<<" ["<<p_caja.w<<"x"<<p_caja.h<<"]"<<std::endl; 
}
/*
float Herramientas_SDL::round(float r) {return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);}

void Herramientas_SDL::posicion_decimal_caja(SDL_Rect& p_caja, float p_x, float p_y)
{
	p_caja.x=Herramientas_SDL::round(p_x);
	p_caja.y=Herramientas_SDL::round(p_y);
}
*/
