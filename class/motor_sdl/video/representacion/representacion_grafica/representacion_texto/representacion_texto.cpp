#include "representacion_texto.h"

using namespace DLibV;

Representacion_texto::Representacion_texto(const Recurso_grafico * p_rep)
	:Representacion_manual(),
	ancho_glifo(0), alto_glifo(0), interlineado(0), interletra(0), cadena(""),
	color_fondo(0), superficie_alfabeto(NULL)
{
	this->establecer_recurso_fuente(p_rep);
}

Representacion_texto::Representacion_texto(const Representacion_texto& p_otra)
	:
	Representacion_manual(p_otra),
	ancho_glifo(p_otra.ancho_glifo), 
	alto_glifo(p_otra.alto_glifo), 
	interlineado(p_otra.interlineado), 
	interletra(p_otra.interletra), 
	cadena(p_otra.cadena),
	color_fondo(p_otra.color_fondo), 
	superficie_alfabeto(p_otra.superficie_alfabeto)
{
	this->establecer_posicion(p_otra.acc_posicion());
//	this->establecer_recurso_fuente(p_rep);

	//En lugar de copiar el puntero del recurso anterior creamos uno nuevo...
	anular_recurso();
	this->iniciar_recurso();
}

Representacion_texto::~Representacion_texto()
{
	this->destruir_recurso();
	this->superficie_alfabeto=NULL;	
}

Representacion_texto& Representacion_texto::operator=(const Representacion_texto& p_otra)
{
	Representacion_manual::operator=(p_otra);

	this->ancho_glifo=p_otra.ancho_glifo;
	this->alto_glifo=p_otra.alto_glifo;
	this->interlineado=p_otra.interlineado;
	this->interletra=p_otra.interletra;
	this->cadena=std::string(p_otra.cadena);
	this->ancho_area=p_otra.ancho_area;
	this->alto_area=p_otra.alto_area;
	this->superficie_alfabeto=p_otra.superficie_alfabeto;

	//En lugar de copiar el puntero del recurso anterior creamos uno nuevo...
	anular_recurso();
	this->iniciar_recurso();
	
	return *this;
}

bool Representacion_texto::establecer_recorte_para_glifo(unsigned char p_char, SDL_Rect & p_caja_glifo)
{
	//Convertimos el código a las coordenadas que necesitamos...
	unsigned char codigo=p_char;
	bool resultado=false;

	switch(codigo)
	{
		case 10: resultado=false; break; //No imprimiremos nada.
			
		default:
			unsigned int fila=floor(codigo/T_GLIFOS_FILA);
			unsigned int columna=codigo % (T_GLIFOS_FILA);
			unsigned int p_x=columna * this->ancho_glifo;
			unsigned int p_y=fila * this->alto_glifo;

			p_caja_glifo.x=p_x;
			p_caja_glifo.y=p_y;

			resultado=true;	//Imprimiremos.
		break;
	}

	return resultado;	
}

/*
Este método se encarga de actualizar la posición de dibujo en función del 
carácter que se haya pasado. Básicamente, para una nueva línea vamos a saltar
a otra línea y para el resto vamos a ir hacia adelante.
*/

void Representacion_texto::establecer_posicion_para_glifo(unsigned char p_char, SDL_Rect & p_posicion_cursor, unsigned int x_original)
{
	switch(p_char)
	{
		case 10:
			p_posicion_cursor.y+=this->alto_glifo+this->interlineado;
			p_posicion_cursor.x=x_original;
		break;
			
		default:
			p_posicion_cursor.x+=this->ancho_glifo+this->interletra;
		break;
	}
}

void Representacion_texto::iniciar_recurso()
{
	//Este recurso es único, no se almacena...
	//A partir de este momento esta clase es RESPONSABLE de
	//liberarlo. 

	if(!this->ref_recurso())
	{
		Lienzo * l=Lienzo::generar_nuevo(this->ancho_area, this->alto_area); 			

		if(this->superficie_alfabeto)
		{
			//El orden de los factores altera (y mucho) el producto:
			//si le decimos que establezca la clave de color sin
			//antes asignar un recurso explotará. Asignar la clave
			//de color tiene como conscuencia que se aplica a la
			//superficie de forma instantánea. 

			this->establecer_recurso(l);
			Representacion_manual::establecer_clave_color(this->superficie_alfabeto);
			this->color_fondo=Representacion_manual::acc_clave_color();
			this->ref_recurso()->rellenar(this->color_fondo);

			l=NULL;

			//Al crear la superficie ya le damos un ancho y alto. Ojo a las flags: no la estamos moviendo.
			this->establecer_posicion(0,0,this->ancho_area, this->alto_area, FRECT_W | FRECT_H);
		}	
	}
}

void Representacion_texto::destruir_recurso()
{
	Representacion_grafica::liberar_recurso();
}

void Representacion_texto::establecer_dimensiones_area(unsigned int p_ancho, unsigned int p_alto)
{
	this->ancho_area=p_ancho;
	this->alto_area=p_alto;
	Representacion_grafica::marcar_como_no_preparada();
}

void Representacion_texto::preparar()
{
	if(this->superficie_alfabeto)
	{			
		unsigned int x_retorno_carro=0;
		unsigned char c, c_aux;

		if(!this->ref_recurso())
		{
			DLibH::Log_motor::L()<<DLibH::Log_base_n(1)<<DLibH::Log_base_t()<<"Representacion_texto::preparar() NO HAY RECURSO"<<std::endl;
		}
		else
		{
			this->ref_recurso()->rellenar(this->color_fondo);

			SDL_Rect caja_glifo; 			
			caja_glifo.x=0;
			caja_glifo.y=0;
			caja_glifo.w=this->ancho_glifo;
			caja_glifo.h=this->alto_glifo;

			SDL_Rect posicion_cursor=caja_glifo;
			x_retorno_carro=caja_glifo.x;

			std::string::iterator it=this->cadena.begin(),
						fin=this->cadena.end();

			for(;it < fin; it++)
			{
				c=(unsigned char)*it;

				if(DLibH::Herramientas::es_inicio_utf8_2b(c))
				{
					it++;
					c_aux=*it;

					c=DLibH::Herramientas::utf8_2b_a_uchar(c, c_aux);
				}

				//OJO: No podemos mover el cursor ANTES de pintar... En este método se
				//seleccionaría la posición pero el movimiento sería después.

				if(this->establecer_recorte_para_glifo(c, caja_glifo))
				{
					SDL_BlitSurface(this->superficie_alfabeto, &caja_glifo, this->ref_superficie(), &posicion_cursor);
				}

				//Luego, haya que dibujar o no, movemos el cursor.
				this->establecer_posicion_para_glifo(c, posicion_cursor, x_retorno_carro);
			}

			this->establecer_recorte(0, 0, this->ancho_area, this->alto_area);
		}

		this->marcar_como_preparada();
	}
}

/*
Establece el recurso gráfico que será la fuente y también el ancho y alto
de los glifos a partir del tamaño de dicho recurso.
*/

void Representacion_texto::establecer_recurso_fuente(Recurso_grafico const * p_recurso)
{
	this->superficie_alfabeto=p_recurso->acc_superficie();	//Esto es la superficie de alfabeto.

	//Establecer tamaño de glifos... Debe haber 16 filas y 16 columnas.
	this->ancho_glifo=this->superficie_alfabeto->w / 16;
	this->alto_glifo=this->superficie_alfabeto->h / 16;
}

/*Destruye el original y pone el nuevo...*/

void Representacion_texto::modificar_fuente(Recurso_grafico const * p_recurso)
{
	this->establecer_recurso_fuente(p_recurso);
	this->asignar(this->cadena);
}

////////////////////////////////////////////////////////////////////////
//Estos métodos afectan a la cadena.

void Representacion_texto::asignar(const char * p_param)
{
	this->cadena.assign(p_param);
	Representacion_grafica::marcar_como_no_preparada();
}

void Representacion_texto::asignar(const char p_param)
{
	//Hay que joderse...
	std::string cosa("");
	cosa+=p_param;

	this->cadena.assign(cosa);
	Representacion_grafica::marcar_como_no_preparada();
}


void Representacion_texto::asignar(std::string const& p_param)
{
	this->cadena=p_param;
	Representacion_grafica::marcar_como_no_preparada();
}
