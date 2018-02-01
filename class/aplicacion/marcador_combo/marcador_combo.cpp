#include "marcador_combo.h"

const float Marcador_combo::CANTIDAD_MOVIMIENTO=300.0;

Marcador_combo::Marcador_combo(unsigned int p_tipo_combo, DLibV::Recurso_grafico * rec):
	tipo_combo(p_tipo_combo), estado(E_ENTRADA), x(X_INICIAL), y(Y_INICIAL), 
	representacion(rec)
{
	this->establecer_recorte();
}

Marcador_combo::Marcador_combo(const Marcador_combo& o):
	tipo_combo(o.tipo_combo), estado(o.estado), x(o.x), y(o.y), 
	representacion(o.representacion)
{

}

Marcador_combo& Marcador_combo::operator=(const Marcador_combo& o)
{
	tipo_combo=o.tipo_combo;
	estado=o.estado;
	x=o.x;
	y=o.y;
	representacion=o.representacion;

	return *this;
}

Marcador_combo::~Marcador_combo()
{

}

void Marcador_combo::establecer_recorte()
{
	unsigned int x=ANCHO * this->tipo_combo;
	this->representacion.establecer_recorte(x, 0, ANCHO, ALTO);
}	

bool Marcador_combo::procesar_turno(float p_delta)
{
	bool resultado=true;

	switch(this->estado)
	{
		case E_ENTRADA:
			//Viene de la derecha hacia la izquierda...
			this->x-=p_delta*CANTIDAD_MOVIMIENTO;
			if(this->x <= X_ACTIVA) 
			{
				this->x=X_ACTIVA;
				this->estado=E_ACTIVO;
			}
	
			this->representacion.establecer_posicion(this->x, this->y);
		break;

		case E_ACTIVO:
			//Nada de nada... Había pensado en algo de alpha.
		break;

		case E_SALIDA:
			//Podemos entrar en este estado sin haber finalizado aún la "entrada"...
			//En este caso intentamos ver si ha finalizado antes de "subir".

			if(this->x>X_ACTIVA)
			{
				this->x-=p_delta*CANTIDAD_MOVIMIENTO;
			}
			else
			{
				//Va hacia arriba.
				this->y-=p_delta*CANTIDAD_MOVIMIENTO;
				if(this->y < Y_FINAL)
				{
					resultado=false;
				}
			}
	
			this->representacion.establecer_posicion(this->x, this->y);
		break;
	}

	return resultado;
}

void Marcador_combo::iniciar_salida()
{
	this->estado=E_SALIDA;
}
