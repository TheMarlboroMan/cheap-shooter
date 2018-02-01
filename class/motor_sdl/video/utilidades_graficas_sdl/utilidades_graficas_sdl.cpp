#include "utilidades_graficas_sdl.h"

using namespace DLibV;


SDL_Surface * Utilidades_graficas_SDL::nueva_superficie_formato(SDL_Surface const * p_origen)
{
	SDL_Rect caja;
	caja.x=0;
	caja.y=0;
	caja.w=p_origen->w;
	caja.h=p_origen->h;

	return nueva_superficie_formato(p_origen, caja, 0, 0);
}

SDL_Surface * Utilidades_graficas_SDL::nueva_superficie_formato(SDL_Surface const * p_origen, const SDL_Rect& p_caja, Uint32 p_flags, Uint32 p_color)
{
	SDL_Surface * origen=const_cast <SDL_Surface *> (p_origen);
	SDL_Surface * copia=NULL;

	copia=SDL_CreateRGBSurface(
//		origen->flags|p_flags,
		p_flags,
		p_caja.w, p_caja.h, origen->format->BitsPerPixel,
		origen->format->Rmask, origen->format->Gmask, origen->format->Bmask, origen->format->Amask);

	SDL_FillRect(copia, NULL, p_color);

	return copia;
}

SDL_Surface * Utilidades_graficas_SDL::copiar_superficie(SDL_Surface const * p_origen)
{
	SDL_Rect p_caja;
	p_caja.x=0;
	p_caja.y=0;
	p_caja.w=p_origen->w;
	p_caja.h=p_origen->h;

	Uint32 p_flags=p_origen->flags;	
	Uint32 p_color=0;	

	return copiar_superficie(p_origen, p_caja, p_flags, p_color);
}

SDL_Surface * Utilidades_graficas_SDL::copiar_superficie(SDL_Surface const * p_origen, const SDL_Rect& p_caja, Uint32 p_flags, Uint32 p_color)
{
	SDL_Surface * copia=Utilidades_graficas_SDL::nueva_superficie_formato(p_origen, p_caja, p_flags, p_color);
	SDL_Surface * origen=const_cast <SDL_Surface *> (p_origen);
	SDL_Rect * caja=const_cast <SDL_Rect *> (&p_caja);

	if(copia)
	{

/*		
		Carajo. Si tiene colorkey que se lo quite el código cliente.

		if(origen->format->colorkey) //Y si es negro??... Pues te jodes. Supongo.
		{
			SDL_Surface * origen=const_cast<SDL_Surface *>(p_origen);

			//Eliminar temporalmente color key, para que se blitee...
			Uint32 c=origen->format->colorkey;
			SDL_SetColorKey(origen, 0, 0);
			SDL_BlitSurface(origen, caja, copia, NULL);

			SDL_SetColorKey(origen, SDL_SRCCOLORKEY, c); //Y si está con RLE... Pues te jodes. Again.
			SDL_SetColorKey(copia, SDL_SRCCOLORKEY, c);
		}
		else
		{
*/
			SDL_BlitSurface(origen, caja, copia, NULL);
//		}
	}

	return copia;
}

SDL_Surface * Utilidades_graficas_SDL::cargar_imagen(const char * cadena)
{
        SDL_Surface * temporal=NULL, *  optimizada=NULL;  
	//temporal=SDL_LoadBMP(cadena);
	temporal=IMG_Load(cadena);
        if (!temporal) 
        {
		DLibH::Log_motor::L()<<DLibH::Log_base_n(1)<<DLibH::Log_base_t()<<"Utilidades_graficas_SDL::cargar_imagen() : Imagen no cargada:"<<cadena<<std::endl;
                return NULL;
        }
	else
	{
		optimizada = SDL_DisplayFormat(temporal);
                SDL_FreeSurface(temporal);
		return optimizada;
	}
}

SDL_Surface * Utilidades_graficas_SDL::cargar_imagen_en_superficie(const char * p_ruta, int flags)
{
	SDL_Surface * superficie=NULL;

	std::ifstream archivo;
        archivo.open(p_ruta);	

	//Si existe lo cargamos.
        if(archivo)
	{
		superficie=Utilidades_graficas_SDL::cargar_imagen(p_ruta);
                archivo.close();
	}
        //Si no existe generamos una superficie roja para ocupar su sitio y evitar un casque.
        else
        {
		DLibH::Log_motor::L()<<DLibH::Log_base_n(1)<<DLibH::Log_base_t()<<"Utilidades_graficas_SDL::cargar_imagen_en_superficie(): Ruta no localizable: "<<p_ruta<<std::endl;

		superficie=SDL_CreateRGBSurface(flags, 32, 32, 32, (Uint32) 255, (Uint32) 0, (Uint32) 0, (Uint32) 0);
		SDL_FillRect(superficie, NULL, 128);
	}

	return superficie;
}

//Directamente de la documentación de la SDL.

Uint32 Utilidades_graficas_SDL::SDL_GetPixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	
	/* Here p is the address to the pixel we want to retrieve */
	if(SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	if(SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

	switch(bpp) 
	{
		case 1:
			return *p;
		break;

		case 2:
			return *(Uint16 *)p;
		break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) return p[0] << 16 | p[1] << 8 | p[2];
			else return p[0] | p[1] << 8 | p[2] << 16;
		break;

		case 4:
			return *(Uint32 *)p;
		break;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
		break;
	}
}

Uint32 Utilidades_graficas_SDL::combinar_pixel(SDL_Surface * superficie, Uint32 p_nuevo, Uint32 p_original, Uint8 p_alpha)
{
	switch(p_alpha)
	{
		case SDL_ALPHA_OPAQUE: return p_nuevo; break;
		case SDL_ALPHA_TRANSPARENT: return p_original; break;
		default:
		{
			auto media=[p_alpha](Uint8 colora, Uint8 colorb) -> Uint8
			{
				Uint8 complemento_alpha=SDL_ALPHA_OPAQUE - p_alpha;
				int val_a=colora * p_alpha;
				int val_b=colorb * complemento_alpha;
				Uint8 res=DLibH::Herramientas::round((val_a + val_b) / 2) / SDL_ALPHA_OPAQUE;
				return res;
			};

			Uint8 r=0, g=0, b=0, ro=0, go=0, bo=0;
			SDL_PixelFormat *formato=superficie->format;
			SDL_GetRGB(p_original, formato, &ro, &go, &bo);	//Sacar colores del original.
			SDL_GetRGB(p_nuevo, formato, &r, &g, &b);			//Sacar colores del nuevo.

			r=media(r, ro);
			g=media(g, go);
			b=media(b, bo);
/*

			if(combinar > 0)	//Sumar.
			{
				r=(int)r+(int)ro > 255 ? r : r+ro;
				g=(int)g+(int)go > 255 ? g : g+go;
				b=(int)b+(int)bo > 255 ? b : b+bo;
			}
			else			//Restar
			{
				r=r-ro < r ? r : r-ro;
				g=g-go < g ? g : g-go;
				b=b-bo < b ? b : b-bo;
			}
*/
			return SDL_MapRGB(formato, r, g, b);
		}
		break;
	}
}

/*
Idem que la anterior, directa del manual de la SDL
*/


//TODO: Esto podríamos separarlo en modos para cada bpp. Dentro de las representaciones
//podríamos almacenar una referencia al método y todos tan felices.

//void Utilidades_graficas_SDL::SDL_PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel, short int combinar)
void Utilidades_graficas_SDL::SDL_PutPixel(SDL_Surface *surface, int x, int y, Uint32 pixel, Uint8 p_alpha)
{
	if(x < 0 || x >= surface->w || y < 0 || y >= surface->h || p_alpha==SDL_ALPHA_TRANSPARENT) return;

	int bpp = surface->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to set */
//	SDL_LockSurface(surface);
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
//	SDL_UnlockSurface(surface);

	if(p_alpha!=SDL_ALPHA_OPAQUE)
	{
		Uint32 pixel_original=Utilidades_graficas_SDL::SDL_GetPixel(surface, x, y);
		pixel=combinar_pixel(surface, pixel, pixel_original, p_alpha);
	}
/*
	if(combinar!=0)
	{
		Uint32 pixel_original=Utilidades_graficas_SDL::SDL_GetPixel(surface, x, y);
		Uint8 r=0, g=0, b=0, ro=0, go=0, bo=0;

		SDL_PixelFormat *formato=surface->format;

		SDL_GetRGB(pixel_original, formato, &ro, &go, &bo);	//Sacar colores del original.
		SDL_GetRGB(pixel, formato, &r, &g, &b);			//Sacar colores del nuevo.

		if(combinar > 0)	//Sumar.
		{
			r=(int)r+(int)ro > 255 ? r : r+ro;
			g=(int)g+(int)go > 255 ? g : g+go;
			b=(int)b+(int)bo > 255 ? b : b+bo;
		}
		else			//Restar
		{
			r=r-ro < r ? r : r-ro;
			g=g-go < g ? g : g-go;
			b=b-bo < b ? b : b-bo;
		}

		pixel=SDL_MapRGB(formato, r, g, b);
	}
*/

	switch(bpp) 
	{
		case 1:
			*p = pixel;
		break;

		case 2:
			*(Uint16 *)p = pixel;
		break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) 
			{
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} 
			else 
			{
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
		break;

		case 4:
			*(Uint32 *)p = pixel;
		break;

		default:
			DLibH::Log_motor::L()<<DLibH::Log_base_n(1)<<DLibH::Log_base_t()<<"Utilidades_graficas_SDL::SDL_PutPixel: Sin bpp "<<std::endl;
		break;
	}
}

void Utilidades_graficas_SDL::SDL_PutPixel(SDL_Surface *surface, SDL_Rect const& p_caja, int x, int y, Uint32 pixel, Uint8 p_alpha)
{
	if(x < p_caja.x || x >= p_caja.w+p_caja.x || y < p_caja.y || y >= p_caja.y+p_caja.h || p_alpha==SDL_ALPHA_TRANSPARENT) 
	{
		return;	
	}

	int bpp = surface->format->BytesPerPixel;

	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	if(p_alpha!=SDL_ALPHA_OPAQUE)
	{
		Uint32 pixel_original=Utilidades_graficas_SDL::SDL_GetPixel(surface, x, y);
		pixel=combinar_pixel(surface, pixel, pixel_original, p_alpha);
	}

/*
	if(combinar!=0)
	{
		Uint32 pixel_original=Utilidades_graficas_SDL::SDL_GetPixel(surface, x, y);
		Uint8 r=0, g=0, b=0, ro=0, go=0, bo=0;

		SDL_PixelFormat *formato=surface->format;

		SDL_GetRGB(pixel_original, formato, &ro, &go, &bo);	//Sacar colores del original.
		SDL_GetRGB(pixel, formato, &r, &g, &b);			//Sacar colores del nuevo.

		if(combinar > 0)	//Sumar.
		{
			r=(int)r+(int)ro > 255 ? r : r+ro;
			g=(int)g+(int)go > 255 ? g : g+go;
			b=(int)b+(int)bo > 255 ? b : b+bo;
		}
		else			//Restar
		{
			r=r-ro < r ? r : r-ro;
			g=g-go < g ? g : g-go;
			b=b-bo < b ? b : b-bo;
		}

		pixel=SDL_MapRGB(formato, r, g, b);
	}
*/
	switch(bpp) 
	{
		case 1:
			*p = pixel;
		break;

		case 2:
			*(Uint16 *)p = pixel;
		break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) 
			{
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} 
			else 
			{
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
		break;

		case 4:
			*(Uint32 *)p = pixel;
		break;

		default:
			DLibH::Log_motor::L()<<DLibH::Log_base_n(1)<<DLibH::Log_base_t()<<"Utilidades_graficas_SDL::SDL_PutPixel: Sin bpp "<<std::endl;
		break;
	}
}

/*Este set de funciones se usan libremente la una a la otra. La primera de ellas
es para dibujar una línea a partir de dos puntos. La segunda toma esos dos puntos
y además añade la pendiente y la intersección. Las otras dos son casos especiales
para líneas horizontales y verticales*/

void Utilidades_graficas_SDL::linea(int xa, int ya, int xb, int yb, SDL_Surface * superficie, Uint32 pixel, Uint8 alpha)
{
	float m, b;

	//Si no es vertical...
	if(xa!=xb)
	{       
		//La pendiente: m=(yb-ya)/(xb-xa)...            
		m=((float)yb-(float)ya)/((float)xb-(float)xa);  

		//La constante b... b=y-(mx)
		b=ya-(m*xa);

		//Horizontal, sin pendiente...
		if(m==0)
		{
			Utilidades_graficas_SDL::linea_horizontal(xa, ya, xb, superficie, pixel, alpha);
		}
		//Pendiente...
		else
		{
			Utilidades_graficas_SDL::linea(xa, ya, xb, yb, m, b, superficie, pixel, alpha);
		}
	}
	else
	{
		Utilidades_graficas_SDL::linea_vertical(xa, ya, yb, superficie, pixel, alpha);
	}
}

void Utilidades_graficas_SDL::linea(int xa, int ya, int xb, int yb, float m, float b, SDL_Surface * superficie, Uint32 pixel, Uint8 alpha)
{
	if(m==0)
	{
		Utilidades_graficas_SDL::linea_horizontal(xa, ya, xb, superficie, pixel, alpha);
	}
	else if(xa==xb)
	{
		Utilidades_graficas_SDL::linea_vertical(xa, ya, xb, superficie, pixel, alpha);
	}
	else
	{
		int xini, xfin, y, old_y;

		//Esto es terrible, parece determinar la dirección de la línea de esta manera.
		xini=xb > xa ? xa : xb;
		xfin=xb > xa ? xb : xa;

		old_y=(m*xini)+b;       //Para la primera iteraci��n igualamos...

		if(SDL_MUSTLOCK(superficie)) SDL_LockSurface(superficie);

		for(;xini<=xfin; xini++)
		{
			//y=mx+b...
			y=(m*xini)+b;

			//Calculamos a raíz de X. Para líneas con una 
			//pendiente muy pronunciada tendremos que rellenar
			//más píxeles en y...

			if(abs(y-old_y) > 1)    
			{
				for(; old_y !=y; old_y < y ? old_y++ : old_y--)
				{
					Utilidades_graficas_SDL::SDL_PutPixel(superficie, xini, old_y, pixel, alpha);
				}                               
			}                               

			Utilidades_graficas_SDL::SDL_PutPixel(superficie, xini, y, pixel, alpha);
			old_y=y;
		}

		if(SDL_MUSTLOCK(superficie)) SDL_UnlockSurface(superficie);
	}
}

void Utilidades_graficas_SDL::linea_horizontal(int xa, int y, int xb, SDL_Surface * superficie, Uint32 pixel, Uint8 alpha)
{
	if(SDL_MUSTLOCK(superficie)) SDL_LockSurface(superficie);

	for(;xa!=xb; xa < xb ? xa++ : xa--)
	{
		Utilidades_graficas_SDL::SDL_PutPixel(superficie, xa, y, pixel, alpha);
	}	

	if(SDL_MUSTLOCK(superficie)) SDL_UnlockSurface(superficie);
}

void Utilidades_graficas_SDL::linea_vertical(int x, int ya, int yb, SDL_Surface * superficie, Uint32 pixel, Uint8 alpha)
{
	if(SDL_MUSTLOCK(superficie)) SDL_LockSurface(superficie);

	for(; ya!=yb; ya<yb ? ya++ : ya--)
	{                               
		Utilidades_graficas_SDL::SDL_PutPixel(superficie, x, ya, pixel, alpha);
	}

	if(SDL_MUSTLOCK(superficie)) SDL_UnlockSurface(superficie);
}

void Utilidades_graficas_SDL::mostrar_ocultar_cursor(bool p_modo)
{
	int valor=p_modo ? SDL_ENABLE : SDL_DISABLE;
	SDL_ShowCursor(valor);
}

/*Implementación del "Midpoint Circle Algorithm". No recuerdo muy bien
como iba pero creo que partía desde el centro haciendo los píxeles directamente
arriba, abajo, izquierda y derecha y luego iba rellenando la circunferencia.
Simplemente la hemos copiado de algún lado.*/

void Utilidades_graficas_SDL::circunferencia(int p_cx, int p_cy, int p_radio, SDL_Surface * p_superficie, Uint32 p_pixel)
{
	double error = (double)-p_radio;
	double x = (double)p_radio -0.5;
	double y = (double)0.5;
	double cx = p_cx - 0.5;
	double cy = p_cy - 0.5;

	if(SDL_MUSTLOCK(p_superficie)) SDL_LockSurface(p_superficie);

	while(x >= y)
	{
		SDL_PutPixel(p_superficie, (int)(cx + x), (int)(cy + y), p_pixel);
		SDL_PutPixel(p_superficie, (int)(cx + y), (int)(cy + x), p_pixel);

		if (x!=0)
		{
			SDL_PutPixel(p_superficie, (int)(cx - x), (int)(cy + y), p_pixel);
			SDL_PutPixel(p_superficie, (int)(cx + y), (int)(cy - x), p_pixel);
		}

		if(y!=0)
		{
			SDL_PutPixel(p_superficie, (int)(cx + x), (int)(cy - y), p_pixel);
			SDL_PutPixel(p_superficie, (int)(cx - y), (int)(cy + x), p_pixel);
		}

		if (x!=0 && y!=0)
		{
			SDL_PutPixel(p_superficie, (int)(cx - x), (int)(cy - y), p_pixel);
			SDL_PutPixel(p_superficie, (int)(cx - y), (int)(cy - x), p_pixel);
		}

		error+=y;
		++y;
		error+=y;

		if(error >= 0)
		{
			--x;
			error-=x;
			error-=x;
		}
	}

	if(SDL_MUSTLOCK(p_superficie)) SDL_UnlockSurface(p_superficie);
}

SDL_Surface * Utilidades_graficas_SDL::invertir_superficie(const SDL_Surface * p_superficie, unsigned int p_flags)
{
	SDL_Rect caja;
	caja.x=0;
	caja.y=0;
	caja.w=p_superficie->w;
	caja.h=p_superficie->h;

	return invertir_superficie(p_superficie, caja, p_flags);
}

/*
El proceso es:

- Crear nueva superficie del tamaño de la caja. 
- Recorrer la original e ir pintando en la nueva.
- Devolver la nueva.

Un ejemplo estúpido de uso es (asumiendo que ya hay una pantalla)

SDL_Surface * original=DLibV::Utilidades_graficas_SDL::cargar_imagen_en_superficie("data/imagenes/titulo_1.png");
SDL_Surface * cosa=DLibV::Utilidades_graficas_SDL::invertir_superficie(original, DLibV::Utilidades_graficas_SDL::VERTICAL | DLibV::Utilidades_graficas_SDL::HORIZONTAL);
DLibV::Imagen * img=new DLibV::Imagen(cosa);	
DLibV::Representacion_bitmap_dinamica rep=DLibV::Representacion_bitmap_dinamica(img, 0, 0);

rep.volcar(pantalla);
pantalla.flipar();
SDL_Delay(1000);
*/

SDL_Surface * Utilidades_graficas_SDL::invertir_superficie(const SDL_Surface * p_superficie, const SDL_Rect& p_caja, unsigned int p_flags)
{
	SDL_Surface * resultado=nueva_superficie_formato(p_superficie, p_caja, p_superficie->flags, 0);

	//Si no especificamos las flags... Pues nada, ahí te quedas.
	if(p_flags)
	{
		int x=p_caja.x, y=p_caja.y, fx=p_caja.w, fy=p_caja.h, rx=0, ry=0;
		Uint32 pixel=0;
		SDL_Surface * no_const=const_cast<SDL_Surface *>(p_superficie);

		if(SDL_MUSTLOCK(no_const)) SDL_LockSurface(no_const);

		for(; y<fy; ++y)
		{
			if(p_flags & VERTICAL) ry=fy-y-1;
			else ry=y;

			for(x=p_caja.x; x<fx; ++x)
			{
				if(p_flags & HORIZONTAL) rx=fx-x-1;
				else rx=x;

				pixel=SDL_GetPixel(no_const, x, y);
				SDL_PutPixel(resultado, rx, ry, pixel, false);
			}
		}

		if(SDL_MUSTLOCK(no_const)) SDL_UnlockSurface(no_const);
	}

	return resultado;
}

