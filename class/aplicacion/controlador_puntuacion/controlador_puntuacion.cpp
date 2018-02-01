#include "controlador_puntuacion.h"

const unsigned int Controlador_puntuacion::CANTIDAD_NIVEL_1_5=2;
const unsigned int Controlador_puntuacion::CANTIDAD_NIVEL_2=3;
const unsigned int Controlador_puntuacion::CANTIDAD_NIVEL_3=5;
const unsigned int Controlador_puntuacion::CANTIDAD_NIVEL_4=8;
const unsigned int Controlador_puntuacion::CANTIDAD_NIVEL_5=10;


const unsigned int Controlador_puntuacion::MAX_MULTIPLICADOR=5;

Controlador_puntuacion::Controlador_puntuacion():
	multiplicador_combo(1), contador_combo(0), cantidad_restante_combo(0), 
	al_maximo(false), puntuacion(0), puntuacion_representada(0), 
	tipo_combo(X1)
{
	cantidades_necesarias.push_back(CANTIDAD_NIVEL_1_5);
	cantidades_necesarias.push_back(CANTIDAD_NIVEL_2);
	cantidades_necesarias.push_back(CANTIDAD_NIVEL_3);
	cantidades_necesarias.push_back(CANTIDAD_NIVEL_4);
	cantidades_necesarias.push_back(CANTIDAD_NIVEL_5);

	calcular_cantidad_restante_combo();
}

Controlador_puntuacion::~Controlador_puntuacion()
{
	std::map<unsigned int, Decoracion_puntuacion *>::iterator	ini=this->decoraciones.begin(),
									fin=this->decoraciones.end();

	for(;ini!=fin; ini++)
	{
		delete (*ini).second;
	}

	this->decoraciones.clear();
}

void Controlador_puntuacion::calcular_cantidad_restante_combo()
{
	if(tipo_combo==X5) cantidad_restante_combo=0;
	else cantidad_restante_combo=cantidades_necesarias[tipo_combo]-contador_combo;
}

void Controlador_puntuacion::sumar_puntuacion(int p_puntuacion)
{
	int final=p_puntuacion*this->multiplicador_combo;
	if(this->puntuacion+final < MAX_PUNTUACION) this->puntuacion+=final;
}
void Controlador_puntuacion::sumar_puntuacion_representada(int p_puntuacion)
{
	this->puntuacion_representada+=p_puntuacion;
}
 
Decoracion_puntuacion * Controlador_puntuacion::obtener_decoracion_puntuacion(int p_obtener, float p_x, float p_y)
{

	int final=p_obtener*this->multiplicador_combo;

	//Esto no hace lo que se supone... original termina siendo 0x0.
	//Decoracion_puntuacion * original=this->decoraciones[final];
	std::map<unsigned int, Decoracion_puntuacion *>::iterator it;
	it=this->decoraciones.find(final);
	Decoracion_puntuacion * original=NULL;

	if(it==this->decoraciones.end())
	{
		original=new Decoracion_puntuacion();
		original->mut_puntuacion(final);
		original->preparar_maestra();
		this->decoraciones.insert(std::pair<unsigned int, Decoracion_puntuacion *>(final, original));
	}
	else
	{
		original=it->second;
	}
	
	//Ahora hacemos la copia.
	Decoracion_puntuacion * resultado=original->generar_copia();
	resultado->ir_a(p_x, p_y);
	resultado->poner_en_marcha();
	return resultado;
}


void Controlador_puntuacion::reiniciar_combo()
{
	multiplicador_combo=1;
	tipo_combo=X1;
	contador_combo=0;
	al_maximo=false;
	calcular_cantidad_restante_combo();

	LOG<<"REINICIANDO COMBO"<<std::endl;
}

void Controlador_puntuacion::bajar_combo()
{
	LOG<<"BAJANDO COMBO"<<std::endl;
	contador_combo=0;

	if(tipo_combo != X1) //Esto....
	{
		--tipo_combo;	
	}
	
	al_maximo=false;
	calcular_cantidad_restante_combo();
	calcular_multiplicador_combo();
}

bool Controlador_puntuacion::sumar_combo()
{
	float original=multiplicador_combo;

	if(multiplicador_combo >= MAX_MULTIPLICADOR)
	{
		return false;	//Has llegado al máximo.
	}
	else
	{
		++contador_combo;

		calcular_cantidad_restante_combo();
		
		if(!cantidad_restante_combo)
		{
			++tipo_combo;
			contador_combo=0;
			calcular_cantidad_restante_combo();
			calcular_multiplicador_combo();
		}

		return original != multiplicador_combo;
	}
}

void Controlador_puntuacion::calcular_multiplicador_combo()
{
	switch(tipo_combo)	
	{
		case X1: multiplicador_combo=1.0f; break;	//Aquí nunca llegaría...
		case X1_5: multiplicador_combo=1.5f; break;
		case X2: multiplicador_combo=2.0f; break;
		case X3: multiplicador_combo=3.0f; break;
		case X4: multiplicador_combo=4.0f; break;
		case X5: 
			multiplicador_combo=5.0f; 
			al_maximo=true;
		break;
	}
}
