#include "pantalla.h"

using namespace DLibV;

Pantalla::Pantalla(int p_w, int p_h, int p_bpp, int p_flags):
	superficie(NULL), volcados(0), 
	w(p_w), h(p_h), bpp(p_bpp), flags(p_flags)
{
	this->simulacro_caja.w=0;
 	this->simulacro_caja.h=0;
	this->simulacro_caja.x=0;
	this->simulacro_caja.y=0;
}

Pantalla::~Pantalla()
{
	this->eliminar_procesos();

	if(this->superficie)
	{
		SDL_FreeSurface(this->superficie);	
	}
}

void Pantalla::inicializar(int p_w, int p_h, int p_bpp, int p_flags)
{
	this->w=p_w;
	this->h=p_h;
	this->bpp=p_bpp;
	this->flags=p_flags;
	this->establecer_modo_video();
}


void Pantalla::inicializar()
{
	this->establecer_modo_video();
}

void Pantalla::establecer_titulo(const char * p_cadena)
{
	SDL_WM_SetCaption(p_cadena, p_cadena);
}

void Pantalla::rellenar(Uint8 p_r, Uint8 p_g, Uint8 p_b, const Camara& camara)
{
	this->rellenar(Gestor_color::color(p_r, p_g, p_b), camara);
}

void Pantalla::rellenar(Uint32 p_color, const Camara& camara)
{
	SDL_Rect cosa;
	cosa.x=0; cosa.y=0; cosa.w=this->superficie->w; cosa.h=this->superficie->h;
	this->rellenar(p_color, cosa, camara);
}

void Pantalla::rellenar(Uint8 p_r, Uint8 p_g, Uint8 p_b, Uint16 p_x, Uint16 p_y, Sint16 p_w, Sint16 p_h, const Camara& camara)
{
	SDL_Rect cosa=DLibH::Herramientas_SDL::nuevo_sdl_rect(p_x, p_y, p_w, p_h);
	this->rellenar(p_r, p_g, p_b, cosa, camara);
}

void Pantalla::rellenar(Uint8 p_r, Uint8 p_g, Uint8 p_b, SDL_Rect const& p_caja, const Camara& camara)
{
	this->rellenar(Gestor_color::color(p_r, p_g, p_b), p_caja, camara);
}

void Pantalla::rellenar(Uint32 p_color, SDL_Rect const& p_caja, const Camara& camara)
{
	SDL_Rect cosa=DLibH::Herramientas_SDL::copiar_sdl_rect(p_caja);

	cosa.x-=camara.acc_x();
	cosa.y-=camara.acc_y();

	SDL_FillRect(this->superficie, &cosa, p_color);
}

void Pantalla::rellenar(Uint8 p_r, Uint8 p_g, Uint8 p_b)
{
	this->rellenar(Gestor_color::color(p_r, p_g, p_b));
}

void Pantalla::rellenar(Uint32 p_color)
{
	SDL_Rect cosa;
	cosa.x=0; cosa.y=0; cosa.w=this->superficie->w; cosa.h=this->superficie->h;
	this->rellenar(p_color, cosa);
}

void Pantalla::rellenar(Uint8 p_r, Uint8 p_g, Uint8 p_b, Uint16 p_x, Uint16 p_y, Sint16 p_w, Sint16 p_h)
{
	SDL_Rect cosa=DLibH::Herramientas_SDL::nuevo_sdl_rect(p_x, p_y, p_w, p_h);
	this->rellenar(p_r, p_g, p_b, cosa);
}

void Pantalla::rellenar(Uint8 p_r, Uint8 p_g, Uint8 p_b, SDL_Rect const& p_caja)
{
	this->rellenar(Gestor_color::color(p_r, p_g, p_b), p_caja);
}

void Pantalla::rellenar(Uint32 p_color, SDL_Rect const& p_caja)
{
	SDL_Rect cosa=DLibH::Herramientas_SDL::copiar_sdl_rect(p_caja);
	SDL_FillRect(this->superficie, &cosa, p_color);
}

void Pantalla::flipar()
{
	if(!this->cajas_para_actualizar.size())
	{
	//	SDL_UpdateRect(this->superficie, 0,0,0,0);
		//TODO: Para modo HW.
		SDL_Flip(this->superficie);
	}
	else
	{
		int cantidad=this->cajas_para_actualizar.size();

		if(cantidad)
		{
			this->optimizar_cajas_para_actualizar();

			vector_sdl_rect::iterator ini=this->cajas_para_actualizar.begin(),
						fin=this->cajas_para_actualizar.end();

			for(;ini<fin;ini++) this->cortar_caja_a_pantalla(&*ini);
			ini=this->cajas_para_actualizar.begin();

			SDL_UpdateRects(this->superficie, cantidad, &*ini);
		}
	}

	this->volcados=0;
}

void Pantalla::insertar_caja_para_actualizar(SDL_Rect const& p_caja)
{
	SDL_Rect nueva=DLibH::Herramientas_SDL::copiar_sdl_rect(p_caja);
	this->cajas_para_actualizar.push_back(nueva);
}

void Pantalla::optimizar_cajas_para_actualizar()
{
	vector_sdl_rect::iterator ini_externo, fin_externo, ini_interno, fin_interno;
	ini_externo=this->cajas_para_actualizar.begin();
	fin_externo=this->cajas_para_actualizar.end();

	for(;ini_externo < fin_externo; ini_externo++)
	{
		ini_interno=this->cajas_para_actualizar.begin();
		fin_interno=this->cajas_para_actualizar.end();

		for(;ini_interno < fin_interno; ini_interno++)
		{
			if( distance(ini_interno, ini_externo)!=0 && DLibH::Herramientas_SDL::caja_en_caja(*ini_interno, *ini_externo))
			{
				this->cajas_para_actualizar.erase(ini_interno);
				ini_interno=this->cajas_para_actualizar.begin();
				fin_interno=this->cajas_para_actualizar.end();
			}
		}
	}

/*
Este código es una comprobación empírica del anterior.
int i=0, j=0;

std::vector<SDL_Rect> cajas_test;
for(;i<12; i++) cajas_test.push_back(SDL_Rect());

cajas_test[0].x=40;	cajas_test[0].y=20;	cajas_test[0].w=20;	cajas_test[0].h=20;
cajas_test[1].x=20;	cajas_test[1].y=10;	cajas_test[1].w=15;	cajas_test[1].h=10;
cajas_test[2].x=30;	cajas_test[2].y=10;	cajas_test[2].w=10;	cajas_test[2].h=10;
cajas_test[3].x=10;	cajas_test[3].y=30;	cajas_test[3].w=40;	cajas_test[3].h=20;
cajas_test[4].x=10;	cajas_test[4].y=30;	cajas_test[4].w=40;	cajas_test[4].h=20;
cajas_test[5].x=20;	cajas_test[5].y=35;	cajas_test[5].w=5;	cajas_test[5].h=5;
cajas_test[6].x=45;	cajas_test[6].y=25;	cajas_test[6].w=5;	cajas_test[6].h=5;
cajas_test[7].x=55;	cajas_test[7].y=25;	cajas_test[7].w=5;	cajas_test[7].h=5;
cajas_test[8].x=45;	cajas_test[8].y=35;	cajas_test[8].w=5;	cajas_test[8].h=5;
cajas_test[9].x=55;	cajas_test[9].y=35;	cajas_test[9].w=5;	cajas_test[9].h=5;
cajas_test[10].x=70;	cajas_test[10].y=10;	cajas_test[10].w=20;	cajas_test[10].h=40;
cajas_test[11].x=75;	cajas_test[11].y=15;	cajas_test[11].w=10;	cajas_test[11].h=30;

for(i=0, j=100; i<cajas_test.size(); i++, j+=10) this->pantalla->rellenar(j, j, j, cajas_test[i]);

this->flipar();
SDL_Delay(1500);

this->rellenar(0,0,0);
this->flipar();
SDL_Delay(500);

for(i=0; i<cajas_test.size(); i++) this->insertar_caja_para_actualizar(cajas_test[i]);

this->optimizar_cajas_para_actualizar();
std::vector<SDL_Rect> cajas=this->pantalla->acc_cajas_para_actualizar();
std::cout<<"QUEDAN "<<cajas.size()<<std::endl;

std::vector<SDL_Rect>::iterator ini=cajas.begin(), fin=cajas.end();
i=60;
for(;ini < fin; ini++, i+=20) this->rellenar(i,i,i, *ini);

this->flipar();
SDL_Delay(1500);
*/
}

void Pantalla::cortar_caja_a_pantalla(SDL_Rect * p_caja)
{
	if(p_caja->x < 0) 
	{
		p_caja->w=p_caja->w-p_caja->x;
		p_caja->x=0;
	}

	if(p_caja->y < 0) 
	{
		p_caja->h=p_caja->h-p_caja->y;
		p_caja->y=0;
	}

	if(p_caja->x+p_caja->w > this->w)
	{
		p_caja->w=this->w-p_caja->w;
	}

	if(p_caja->y+p_caja->h > this->h)
	{
		p_caja->h=this->h-p_caja->h;
	}
}

void Pantalla::eliminar_caja_para_actualizar(SDL_Rect const& p_caja)	
{
	//Podríamos usar el template find de <algorithm> pero 
	//no sé cómo se comparan dos SDL_Rect...

	vector_sdl_rect::iterator ini=this->cajas_para_actualizar.begin(),
	fin=this->cajas_para_actualizar.end();

	for(;ini<fin; ini++)
	{
		if(DLibH::Herramientas_SDL::son_cajas_iguales(p_caja, *ini))
		{
			this->cajas_para_actualizar.erase(ini);
			break;
		}
	}
}

void Pantalla::preparar_para_camara(Camara const& p_camara)
{
	if(p_camara.es_con_clip()) this->establecer_clip_para_camara(p_camara);
}

void Pantalla::establecer_modo_video()
{
	this->superficie=SDL_SetVideoMode(this->w, this->h, this->bpp, this->flags);

	this->simulacro_caja.w=this->w;
	this->simulacro_caja.h=this->h;
	this->simulacro_caja.x=0;
	this->simulacro_caja.y=0;
}

void Pantalla::reiniciar_clip_completo()
{
	SDL_Rect caja;
	caja.x=0; 
	caja.y=0;
	caja.w=this->superficie->w;
	caja.h=this->superficie->h;

	SDL_SetClipRect(this->superficie, &caja);
}

void Pantalla::establecer_clip_para_camara(Camara const& p_camara)
{
	SDL_Rect caja=p_camara.acc_caja_pos();	
	SDL_SetClipRect(this->superficie, &caja);
}

void Pantalla::establecer_clip(SDL_Rect p_caja)
{
	SDL_SetClipRect(this->superficie, &p_caja);
}

/*La pantalla puede tener adjuntos una serie de procesos que se aplicarán a 
su superficie. La idea es adjuntar un proceso y llamar a "procesar_procesos" 
justo antes de volcar la pantalla. 

Los procesos se eliminan automáticamente de la cola una vez que están 
finalizados. Para procesos infinitos habría que guardar una referencia en el
código cliente y finalizarlos manualmente.

Ojo con esto: una vez que se adjunta un proceso a la pantalla pasa a ser 
PROPIEDAD de esta última y es ella quien se encarga de eliminarlo!!!!.

Si alguna vez esto nos da dolores de cabeza siempre podemos crear métodos 
para adjuntar procesos y encerrarlo todo.
*/

void Pantalla::adjuntar_proceso(Proceso_superficie * p_proceso)
{
	this->vector_procesos.push_back(p_proceso);
} 

void Pantalla::procesar_procesos(float p_delta)
{
	t_vector_procesos::iterator 	ini=this->vector_procesos.begin(),
					fin=this->vector_procesos.end();

	//Primero los procesamos...

	while(ini < fin)
	{
		(*ini)->actuar(this->superficie, p_delta);
		ini++;
	}

	//Y ahora, si hay alguno finalizado lo eliminamos.

	ini=this->vector_procesos.begin();

	while(ini < fin)
	{
		if((*ini)->es_finalizado())
		{			
			delete (*ini);
			ini=this->vector_procesos.erase(ini);
			fin=this->vector_procesos.end();	//Todos los iteradores se invalidan al borrar uno!.
		}
		else
		{
			ini++;
		}
	}
}

void Pantalla::eliminar_procesos()
{
	t_vector_procesos::iterator 	ini=this->vector_procesos.begin(),
					fin=this->vector_procesos.end();

	while(ini < fin)
	{
		delete (*ini);
		ini=this->vector_procesos.erase(ini);
		fin=this->vector_procesos.end();	//Todos los iteradores se invalidan al borrar uno!.
	}
}
