#include "mapa_colision_bitmap.h"

using namespace DLibV;

Mapa_colision_bitmap::Mapa_colision_bitmap()
{
	this->mapa=NULL;
}

Mapa_colision_bitmap::~Mapa_colision_bitmap()
{
	if(this->mapa!=NULL)
	{
		delete [] this->mapa;
	}
}

bool Mapa_colision_bitmap::inicializar(SDL_Surface * p_superficie, Uint32 p_color)
{
	SDL_Rect temp=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, p_superficie->w, p_superficie->h);
	return this->inicializar(p_superficie, p_color, temp);
}

bool Mapa_colision_bitmap::inicializar(SDL_Surface * p_superficie , Uint32 p_color, SDL_Rect p_rect)
{
	//Comprobar que el rectángulo no tiene más ancho del que debe, hacer clip.
	//De lo contrario pueden ocurrir horribles errores...
	
	if(p_rect.x + p_rect.w > p_superficie->w)
	{
		this->w=p_rect.w - (p_rect.w - p_superficie->w) - p_rect.x;
	}
	else
	{
		this->w=p_rect.w;
	}

	if(p_rect.y + p_rect.h > p_superficie->h)
	{
		this->h=p_rect.h - (p_rect.h - p_superficie->h) - p_rect.y;
	}
	else
	{
		this->h=p_rect.h;
	}

	this->generar_mapa();
	this->rellenar_mapa(p_superficie, p_color, p_rect);

	return true;	
}

bool Mapa_colision_bitmap::inicializar(SDL_Rect p_caja, unsigned short int VALOR)
{
	this->w=p_caja.w;
	this->h=p_caja.h;
	this->generar_mapa(VALOR);

	return true;
}

/*Rellena el mapa...*/

void Mapa_colision_bitmap::rellenar_mapa(SDL_Surface * p_superficie , Uint32 p_color, SDL_Rect p_rect)
{
	int ini_x=p_rect.x;
	int ini_y=p_rect.y;

	int max_x=this->w+p_rect.x;
	int max_y=this->h+p_rect.y;

	int x=0;
	int y=0;
	int posicion=0;
	unsigned char bit=0;
	int i=0;
	int cantidad=0;

	//Lo almacenamos de izquierda a derecha y de arriba a abajo...

	for(y=ini_y; y<max_y; y++)
	{
		for(x=ini_x; x<max_x; x++, i++)
		{
			//En qué parte estaríamos?. Cada TAM_UNIDAD guardados avanzariamos una... 
			//Es cutre, pero puede funcionar :D.

			if(i && i%TAM_UNIDAD==0) posicion++;
	
			//Si hay un color que no sea transparente...
			Uint32 color=Utilidades_graficas_SDL::SDL_GetPixel(p_superficie, x, y);

			if(color!=p_color)
			{
				//Que bit toca?. Poco elegante, pero rápido.
				cantidad=i % TAM_UNIDAD;
				bit_para_cantidad(bit, cantidad);
				this->mapa[posicion]=this->mapa[posicion] | bit;
			}
		}
	}
}

/*Crea la memoria para el mapa...*/

void Mapa_colision_bitmap::generar_mapa(unsigned short int VALOR)
{
	//Tenemos que calcular la cantidad de chars nos hacen falta para meter lo que queremos.
	int longitud=this->w*this->h;
	int unidades=floor(longitud / TAM_UNIDAD);
	if(longitud % TAM_UNIDAD) unidades++;	//Si hay un resto metemos una más...

	this->mapa=new unsigned char[unidades];
	memset(this->mapa, VALOR, unidades); 
}

bool Mapa_colision_bitmap::comprobar(unsigned int p_x, unsigned int p_y) const
{
	if(p_x >= this->w || p_y >= this->h) 
	{
		return false;	//Ojo: Si nos pasamos dice que false.
	}
	else
	{
		//Calculamos el bloque de memoria en el que está almacenado
		//el píxel que queremos. Se guardaron de izquierda a derecha y
		//de arriba a abajo.

		int saltos=(p_y * this->w) + p_x;
		int posicion=floor(saltos / TAM_UNIDAD);
		int resto=saltos % TAM_UNIDAD;

		//Calculamos el bit que sería...
		unsigned char bit=0;
		bit_para_cantidad(bit, resto);
		
		return this->mapa[posicion] & bit;
	}
}

/*
Se compara un mapa con el otro dentro de las regiones escogidas para cada uno de 
los mapas, punto a punto. Se devuelve verdadero si hay algún tipo de colisión.
No se comprueba si los rectángulos realmente tienen los tamaños que deben o no.

Ojo: si usamos DLibH::Herramientas_SDL::rectangulos_superpuestos para sacar un 
rectángulo R de la colisión para ambas cajas tenemos que "transformarlo" al 
espacio de la propia caja para ver la parte que correspondería: es fácil:

x=R.x-A.x, y=R.y-A.y, w=R.w, h=R.h.

Ejemplo: A es caja A, B es Caja B y R es rectángulo de colisión, luego

A(x2,y2,w3,h3)
B(x4,y1,w3,h5)
R(x4,y2,w1,h3) 

 0123456789
0          
1    BBB   
2  AARBB  
3  AARBB  
4  AARBB  
5    BBB   
    
Ahora, dentro de A y B ocupa otras posiciones:

 012		 0123456
0AAR		0BBB
1AAR 		1RBB
2AAR		3RBB
		4RBB
		5BBB

En A=> R(x2,y0, w1, h3) (A.x-R.x, A.y-R.y, R.w, R.y)
En B=> R(x0,y1, w1, h3) (B.x-R.x, B.y-R.y, R.w, R.y)
*/


bool Mapa_colision_bitmap::comparar_mapas(const Mapa_colision_bitmap & p_mapa_1, const Mapa_colision_bitmap & p_mapa_2, const SDL_Rect& p_r1, const SDL_Rect& p_r2)
{
	unsigned int x1,x2,y1,y2;

	unsigned int lim_y_1=p_r1.y+p_r1.h;
	unsigned int lim_y_2=p_r2.y+p_r2.h;
	unsigned int lim_x_1=p_r1.x+p_r1.w;
	unsigned int lim_x_2=p_r2.x+p_r2.w;

	for(
		y1=p_r1.y, y2=p_r2.y; 
		y1 < lim_y_1 && y2 < lim_y_2;
		y1++, y2++
	)
	{
		for(
			x1=p_r1.x, x2=p_r2.x; 
			x1 < lim_x_1 && x2 < lim_x_2; 
			x1++, x2++
		)
		{
			if(p_mapa_1.comprobar(x1, y1) && p_mapa_2.comprobar(x2, y2))
			{
				return true;
			}
		}
	}

	return false;
}

bool Mapa_colision_bitmap::comparar_mapas(const Mapa_colision_bitmap & p_mapa_1, const Mapa_colision_bitmap & p_mapa_2)
{
	SDL_Rect caja_1=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, p_mapa_1.w, p_mapa_1.h);
	SDL_Rect caja_2=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, p_mapa_2.w, p_mapa_2.h);
	return comparar_mapas(p_mapa_1, p_mapa_2, caja_1, caja_2);
}

/*Este método existe sólo para hacer las operaciones que se requieren según
el comentario encima de comparar_mapas()*/

bool Mapa_colision_bitmap::comparar_mapas_posicionados(const Mapa_colision_bitmap & p_mapa_1, const Mapa_colision_bitmap & p_mapa_2, const SDL_Rect& p_r1, const SDL_Rect& p_r2)
{
	SDL_Rect caja=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0);
	if(!DLibH::Herramientas_SDL::rectangulos_superpuestos(p_r1, p_r2, caja)) 
	{
		return false;
	}
	else 
	{
		return comparar_mapas_posicionados(p_mapa_1, p_mapa_2, p_r1, p_r2, caja);
/*
		//Creamos dos cajas de SDL rect a partir de la caja
		//de colisión y las convertimos ahora el tipo
		//de caja que espera "comparar_mapas".

		SDL_Rect caja_a=DLibH::Herramientas_SDL::copiar_sdl_rect(caja);
		SDL_Rect caja_b=DLibH::Herramientas_SDL::copiar_sdl_rect(caja);

		caja_a.x=caja.x-p_r1.x;
		caja_a.y=caja.y-p_r1.y;

		caja_b.x=caja.x-p_r2.x;
		caja_b.y=caja.y-p_r2.y;

		return Mapa_colision_bitmap::comparar_mapas(p_mapa_1, p_mapa_2, caja_a, caja_b);
*/
	}
}

bool Mapa_colision_bitmap::comparar_mapas_posicionados(const Mapa_colision_bitmap & p_mapa_1, const Mapa_colision_bitmap & p_mapa_2, const SDL_Rect& p_r1, const SDL_Rect& p_r2, const SDL_Rect &p_caja_comun)
{
	SDL_Rect caja_a=DLibH::Herramientas_SDL::copiar_sdl_rect(p_caja_comun);
	SDL_Rect caja_b=DLibH::Herramientas_SDL::copiar_sdl_rect(p_caja_comun);

	caja_a.x=p_caja_comun.x-p_r1.x;
	caja_a.y=p_caja_comun.y-p_r1.y;

	caja_b.x=p_caja_comun.x-p_r2.x;
	caja_b.y=p_caja_comun.y-p_r2.y;

	return Mapa_colision_bitmap::comparar_mapas(p_mapa_1, p_mapa_2, caja_a, caja_b);
}


/*Compara un mapa contra una caja...*/

bool Mapa_colision_bitmap::comparar_mapa_con_rectangulo(const Mapa_colision_bitmap& p_mapa, const SDL_Rect& p_caja)
{
	SDL_Rect caja_1=DLibH::Herramientas_SDL::nuevo_sdl_rect(0, 0, p_mapa.w, p_mapa.h);
	return comparar_mapa_con_rectangulo(p_mapa, caja_1, p_caja);
} 

//Básicamente puede ser buscar cada pixel activo dentro de una caja.

bool Mapa_colision_bitmap::comparar_mapa_con_rectangulo(const Mapa_colision_bitmap& p_mapa, const SDL_Rect& p_caja_mapa, const SDL_Rect& p_caja)
{
	SDL_Rect caja_colision=DLibH::Herramientas_SDL::nuevo_sdl_rect(0,0,0,0);

	if(!DLibH::Herramientas_SDL::rectangulos_superpuestos(p_caja_mapa, p_caja, caja_colision))
	{
		return false;
	}
	else
	{
		//caja_colision contiene el rectángulo que tienen en común cada
		//una de ellas. Calcularemos las posiciones relativas y haremos
		//las comprobaciones para ver si los puntos quedan dentro de la caja.

		//Calculamos los puntos de inicio en el mapa según la caja
		//de colisión.

		int mapa_x=caja_colision.x-p_caja_mapa.x;
		int mapa_y=caja_colision.y-p_caja_mapa.y;
		int limite_mapa_x=mapa_x+caja_colision.w;
		int limite_mapa_y=mapa_y+caja_colision.h;

		//Recorreremos esos puntos en el mapa y si están activos 
		//querremos ver si quedarían dentro de la caja. Si está activo
		//les sumaremos la posición de la caja para situarlos en la
		//posición relativa correcta.

		for(; mapa_y < limite_mapa_y; mapa_y++)
		{
			for(int tx=mapa_x; tx < limite_mapa_x; tx++)
			{
				if(p_mapa.comprobar(tx, mapa_y))
				{
					if(DLibH::Herramientas_SDL::punto_en_caja(p_caja, tx+p_caja_mapa.x, mapa_y+p_caja_mapa.y)) 
					{
						return true;
					}
				}
			}
		}

		return false;
	}
} 

/*Este método coge dos cajas en colisión y tres mapas vacíos que le pasemos.
Rellenara el primer mapa vacío con el mapa de colisión resultante de aplicar la 
caja al primer mapa y el segundo hará lo propio.*/

Mapa_colision_bitmap Mapa_colision_bitmap::obtener_mapeado(Mapa_colision_bitmap const& p_mapa_1, SDL_Rect p_caja)
{
	//Generar el mapa al tamaño necesario...
	Mapa_colision_bitmap resultado=Mapa_colision_bitmap();
	resultado.inicializar(p_caja);

	//Rellenar...

	unsigned int x, y;
	int posicion=0;
	unsigned char bit=0;
	int i=0;
	int cantidad=0;

	for(y=p_caja.y; y < p_mapa_1.h; y++)
	{
		for(x=p_caja.x; x < p_mapa_1.w; x++)
		{
			if(i && i%TAM_UNIDAD==0) posicion++;
	
			if(p_mapa_1.comprobar(x, y))
			{
				//Que bit toca?. Poco elegante, pero rápido.
				cantidad=i % TAM_UNIDAD;
				bit_para_cantidad(bit, cantidad);
				resultado.mapa[posicion]=resultado.mapa[posicion] | bit;
			}
		}
	}

	return resultado;
}

std::string Mapa_colision_bitmap::debug() const
{
	std::string resultado;

	unsigned int y=0, x=0;

	for(; y<this->h; y++)
	{
		for(x=0; x<this->w; x++)
		{
			if(this->comprobar(x, y)) resultado+="1";
			else resultado+="0";
		}

		resultado+="\n";
	}
	
	return resultado;
}
