#include "escudo.h"

const float Escudo::MS_DESPLIEGUE_ESCUDO=500.0f; //Tiempo que tarda en desplegarse plenamente.	
const float Escudo::GASTO_CARGA_SEG=250.0f;	 //Carga gastada en un segundo.
const float Escudo::RECUPERACION_CARGA_SEG=150.0f; //Carga recuperada en un segundo.

Escudo::Escudo()
	:
	//recurso(NULL), 
	angulo(0.0f), angulo_despliegue(ANGULO), 
	multiplicador_despliegue(0.0f),
	partes_actuales(CANTIDAD_PARTES), estado(E_ACTIVO),
	max_carga_escudo(MAX_CARGA_INICIAL), carga_escudo_actual(MAX_CARGA_INICIAL),
	activo(false)
{
	//Generar el recurso gráfico...
	//this->generar_recurso_grafico();

	//Construir el vector de partes.
	this->construir_vector_de_partes();
	this->posicion.x=0;
	this->posicion.y=0;
	this->posicion.w=0;
	this->posicion.h=0;
}

Escudo::~Escudo()
{
	this->destruir_vector_de_partes();
}

void Escudo::construir_vector_de_partes()
{
	this->destruir_vector_de_partes();
	unsigned int i=0;
	
	while(i<partes_actuales)
	{
		this->partes.push_back(new Escudo_parte());
		i++;
	}

	this->iterador_turno=this->partes.begin();
	this->iterador_colision=this->partes.begin();
	this->iterador_representacion=this->partes.begin();
	this->iterador_fin=this->partes.end();
}

void Escudo::destruir_vector_de_partes()
{
	//Destruir el vector de partes.
	this->iterador_turno=this->partes.begin();
	this->iterador_fin=this->partes.end();

	while(this->iterador_turno < this->iterador_fin)
	{
		delete (*this->iterador_turno);
		++this->iterador_turno;
	}

	this->partes.clear();
}

void Escudo::establecer_posicion()
{
	this->posicion_desde_coordenadas_propias();
}

void Escudo::procesar_turno(float p_delta)
{
	if(!this->activo)
	{
		if(this->carga_escudo_actual < this->max_carga_escudo)
		{
			this->carga_escudo_actual+=(RECUPERACION_CARGA_SEG) * p_delta;
			if(this->carga_escudo_actual > this->max_carga_escudo) this->carga_escudo_actual=this->max_carga_escudo;
		}
	}
	else
	{
		if(this->carga_escudo_actual <=0)
		{
			this->estado=E_CIERRE;
			this->carga_escudo_actual=0.0f;	
		}
		else
		{
			float gasto=(GASTO_CARGA_SEG) * p_delta;
			this->carga_escudo_actual-=gasto;	
		}

		//Si delta vale MS_DESPLIEGUE_ESCUDO / 1000 es que ha pasado el 
		//tiempo necesario para el despliegue del escudo. De ahí podemos 
		//sacar la norma de crecimiento.

		float factor=(MS_DESPLIEGUE_ESCUDO / 100) * p_delta;

		switch(this->estado)
		{
			case E_DESPLIEGUE:

				this->multiplicador_despliegue+=factor;

				if(this->multiplicador_despliegue >= 1.0f)
				{
					this->multiplicador_despliegue=1.0f;
					this->estado=E_ACTIVO;
				}
			break;
			
			case E_ACTIVO:

			break;

			case E_CIERRE:

				this->multiplicador_despliegue-=factor;
				if(this->multiplicador_despliegue <= 0.0f)
				{
					this->multiplicador_despliegue=0.0f;
					this->estado=E_DESPLIEGUE;
					this->activo=false;
					this->establecer_transparencia_partes(Escudo_parte::ALPHA_INACTIVO);
				}
			break;
		}
	}	

//	if(this->partes.size())
//	{
		this->procesar_turno_escudos(p_delta);
//	}
}

void Escudo::establecer_transparencia_partes(unsigned short int alpha)
{
	this->iterador_turno=this->partes.begin();

	while(this->iterador_turno < this->iterador_fin)
	{
		(*iterador_turno)->establecer_transparencia(alpha);
		++this->iterador_turno;
	}
}

void Escudo::procesar_turno_escudos(float p_delta)
{
	this->iterador_turno=this->partes.begin();

	//Si no está activo veríamos como estaría desplegado.
	float multiplicador_despliegue=this->activo ? this->multiplicador_despliegue : 1;

	//Se despliega de izquierda a derecha. 
	float mitad_angulo=this->angulo_despliegue / 2;
	float angulo=this->angulo-mitad_angulo;
	//Dividimos en arco en tantas partes como puntos haya menos uno, para que haya un punto en cada corte.
	float salto=(this->angulo_despliegue / (this->partes_actuales-1)) * multiplicador_despliegue;

	int x=0, y=0;
	
	//Establecer la posición de cada una de las partes.
	while(this->iterador_turno < this->iterador_fin)
	{
		x=cos(DLibH::Herramientas::grados_a_radianes(angulo))*DISTANCIA;
		y=sin(DLibH::Herramientas::grados_a_radianes(angulo))*DISTANCIA;

		x+=this->x-(Escudo_parte::W / 2);
		y+=this->y-(Escudo_parte::H / 2);

		(*iterador_turno)->ir_a(x, y);

		angulo+=salto;
		++this->iterador_turno;
	}
}

void Escudo::establecer_posicion_y_angulo_desde_actores(const Actor& p_actor_pos, const Actor& p_actor_angulo)
{
	int w=p_actor_pos.acc_w() / 2;
	int h=p_actor_pos.acc_h() / 2;

	this->establecer_posicion_desde_actor(p_actor_pos, w, h);

 	//Establecer el ángulo a partir de la posición de la mirilla...
	float pos_ax=p_actor_pos.acc_x_centro();
	float pos_ay=p_actor_pos.acc_y_centro();
	float pos_bx=p_actor_angulo.acc_x_centro();
	float pos_by=p_actor_angulo.acc_y_centro();

	DLibH::Vector_2d v=DLibH::Vector_2d::obtener_para_puntos(pos_bx, pos_by, pos_ax, pos_ay);
	this->angulo=DLibH::Vector_2d::obtener_angulo_para_vector_unidad_grados(v);
}

void Escudo::reiniciar_pasos_representacion()
{
	this->iterador_representacion=this->partes.begin();
}

DLibV::Representacion * Escudo::acc_representacion()
{
	if(this->iterador_representacion==this->iterador_fin)
	{
		return NULL;
	}
	else
	{
		DLibV::Representacion * resultado=(*iterador_representacion)->acc_representacion();
		++iterador_representacion;
		return resultado;
	}
}

bool Escudo::comprobar_colision_con(const Proyectil& p_pr)
{
	this->iterador_colision=this->partes.begin();

	//Comprobar con cada uno de los escudos...
	while(this->iterador_colision < this->iterador_fin)
	{
		if( (*iterador_colision)->en_colision_con(p_pr))
		{
			return true;
		}

		++this->iterador_colision;
	}

	return false;
}

void Escudo::reiniciar()
{
	this->multiplicador_despliegue=0.0f;
	this->partes_actuales=CANTIDAD_PARTES;
	this->estado=E_ACTIVO;
	this->max_carga_escudo=MAX_CARGA_INICIAL;
	this->carga_escudo_actual=MAX_CARGA_INICIAL;
	this->angulo_despliegue=ANGULO;
	this->activo=false;

	this->construir_vector_de_partes();
}

bool Escudo::desactivar()
{
	this->estado=E_CIERRE;
	return true;
}

bool Escudo::activar()
{
	if(!this->es_usable())
	{
		this->activo=false;
		return false;
	}
	else
	{
		this->activo=true;
		this->estado=E_DESPLIEGUE;
		this->establecer_transparencia_partes(Escudo_parte::ALPHA_ACTIVO);
		return true;
	}
}

bool Escudo::sumar_duracion()
{
	if(this->max_carga_escudo==MAX_CARGA_AMPLIADA)
	{
		return false;
	}
	else
	{
		this->max_carga_escudo+=CANTIDAD_CARGA_AMPLIADA;
		return true;
	}
}

bool Escudo::sumar_escudos()
{
	if(this->partes_actuales==MAX_CANTIDAD_PARTES)
	{
		return false;
	}
	else
	{
		this->partes_actuales+=CANTIDAD_PARTES_AMPLIADO;
		this->angulo_despliegue+=CANTIDAD_ANGULO_AMPLIADO;
		this->construir_vector_de_partes();
		return true;
	}
}
