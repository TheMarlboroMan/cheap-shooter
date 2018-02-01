#include "campo_estrellas.h"

Campo_estrellas::Campo_estrellas()
	:max_puntos(400), divisor_movimiento(1), rojo(0), azul(0), verde(0),
	nrojo(0), nazul(0), nverde(0), salto_color(0),
	cambio_color(0)
{
	this->establecer_representacion();
	this->preparar_puntos();
	this->aplicar_puntos();	
}

Campo_estrellas::~Campo_estrellas()
{
	if(this->representacion)
	{
		delete this->representacion;
		this->representacion=NULL;
	}
}

void Campo_estrellas::establecer_representacion()
{
	representacion=new DLibV::Representacion_pixeles_estatica(Medidas::ANCHO_TERRENO_JUEGO, Medidas::ALTO_TERRENO_JUEGO);
	representacion->establecer_posicion(0,0,Medidas::ANCHO_TERRENO_JUEGO, Medidas::ALTO_TERRENO_JUEGO);
	representacion->limpiar_transparencia();
	representacion->establecer_autolock(false);
}

void Campo_estrellas::preparar_puntos()
{
	unsigned int i=0;
	for(;i<max_puntos; i++)
		this->generar_punto(true);		
}

void Campo_estrellas::generar_punto(bool y_aleatorio)
{
	Punto p=Punto();
	p.establecer_valores(y_aleatorio);
	this->puntos.push_back(p);
}	

void Campo_estrellas::aplicar_puntos()
{
	t_iterator_lista_puntos		ini=this->puntos.begin(),
					fin=this->puntos.end();

	this->representacion->rellenar(this->rojo, this->verde, this->azul);

	unsigned int x, y, i=0;

	representacion->bloquear_superficie();

	while(ini!=fin && i++!=this->max_puntos)
	{
		x=DLibH::Herramientas::round((*ini).x);
		y=DLibH::Herramientas::round((*ini).y);

		this->representacion->cambiar_color((*ini).color);
		this->representacion->pixel(x, y);

		ini++;
	}

	representacion->desbloquear_superficie();
}

void Campo_estrellas::establecer_divisor_movimiento(float p_v) 
{
	this->divisor_movimiento=p_v;
}

void Campo_estrellas::procesar_turno(float p_delta)
{
	float deltadiv=p_delta / this->divisor_movimiento;
	
	t_iterator_lista_puntos		ini=this->puntos.begin(),
					fin=this->puntos.end();
		
	unsigned int i=0;

	while(ini!=fin && i++!=this->max_puntos)
	{

//Esto supone una optimización enooorme (de 120 a 180 fps).
//		(*ini).turno(deltadiv);

		(*ini).y+=(*ini).velocidad * deltadiv;
		if((*ini).fuera_de_juego()) (*ini).establecer_valores();
		ini++;
	}

	if(this->cambio_color)
	{
		this->procesar_cambio_color(p_delta);
	}
	
	this->aplicar_puntos();
}

void Campo_estrellas::procesar_cambio_color(float p_delta)
{
	auto cambio=[p_delta, this](float &original, Uint8 &nuevo) -> int
	{
		if(original!=nuevo)
		{
			float factor=(float)this->salto_color * p_delta;

			if(original < nuevo)
			{	
				original+=factor;
				if(original > nuevo) original=nuevo;
			}
			else if(original > nuevo)
			{
				original-=factor;
				if(original < nuevo) original=nuevo;
			}
		}

		if(original==nuevo) return 1;
		else return 0;

	};

	unsigned int ok=0;

	ok+=cambio(this->rojo, this->nrojo);
	ok+=cambio(this->verde, this->nverde);
	ok+=cambio(this->azul, this->nazul);

	if(ok==3)
	{
		this->cambio_color=false;
	}
}

void Campo_estrellas::establecer_color_fondo(Uint8 p_r, Uint8 p_g, Uint8 p_b)
{
	this->rojo=p_r;
	this->verde=p_g;
	this->azul=p_b;
}

void Campo_estrellas::transformar_color_fondo(Uint8 p_r, Uint8 p_g, Uint8 p_b, Uint8 p_salto)
{
	if(p_salto==0)
	{
		return;
	}
	else
	{
		this->nrojo=p_r;
		this->nverde=p_g;
		this->nazul=p_b;
		this->salto_color=p_salto;
		this->cambio_color=true;
	}
}

void Campo_estrellas::restaurar()
{
	this->rojo=0;
	this->verde=0;
	this->azul=0;
	this->salto_color=0;
	this->cambio_color=0;
	this->max_puntos=MAX_ESTRELLAS;
	this->divisor_movimiento=1;
}
