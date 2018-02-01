#include "linea.h"
#include <iostream>

using namespace DLibH;

Linea::Linea()
{

}

Linea::~Linea()
{

}

float Linea::distancia_entre_puntos(float xa, float ya, float xb, float yb)
{
	float x=(xa-xb)*(xa-xb);
	float y=(ya-yb)*(ya-yb);

	return sqrt(x+y);
}

float Linea::obtener_y_para_x(float p_x) const
{
	//y=mx+b;
	float resultado;

	if(this->vertical)
	{
		resultado=0;
	}
	else if(this->horizontal)
	{
		resultado=this->b;
	}
	else
	{
		if(p_x==0) 
		{
			resultado=this->b;
		}
		else
		{
			resultado=(this->m*p_x) + this->b;
		}
	}

	return resultado;

}

float Linea::obtener_x_para_y(float p_y) const
{
	//x=(y-b)/m
	float resultado;

	if(this->horizontal)
	{
		resultado=0;
	}
	else if(this->vertical)
	{
		resultado=this->b;	//Quien sabe...
	}
	else
	{
		resultado=(p_y-this->b) / this->m;
	}

	return resultado;
}

Linea Linea::crear_desde_parametros(float p_m, float p_b)
{
	Linea resultado=Linea();
	resultado.m=p_m;
	resultado.b=p_b;
	resultado.horizontal=false;	
	resultado.vertical=false;

	if(p_b==0) 
	{
		resultado.horizontal=true;
	}

	return resultado;
}

Linea Linea::crear_desde_puntos(float xa, float ya, float xb, float yb)
{
	Linea resultado=Linea();

	//Es vertical??
	if(xa==xb)
	{
		resultado.vertical=true;
		resultado.horizontal=false;
		resultado.m=0;
		resultado.b=0;
	}
	//Es horizontal
	else if(ya==yb)
	{
		resultado.vertical=false;
		resultado.horizontal=true;
		resultado.m=0;
		resultado.b=0;
	}
	else
	{       
		resultado.vertical=false;
		resultado.horizontal=false;

		//La pendiente: m=(yb-ya)/(xb-xa)...            
		resultado.m=((float)yb-(float)ya)/((float)xb-(float)xa);  

		//La constante b... b=y-(mx)
		resultado.b=ya-(resultado.m*xa);
	}

	return resultado;
}

/*Obtiene una línea metida dentro de una caja recortándo
la existente, si procede... Los cuatro primeros parámetros
son la caja, el resto los puntos... Ojo: sólo funciona
cuando realmente la línea PASA por el rectángulo. 
Realmente no devuelve una línea, sino dos puntos en un
*/

Linea::Set_puntos Linea::obtener_para_linea_y_caja_limite(float x, float y, float w, float h, Linea linea)
{
	float t;
		
	//En primer lugar, buscamos los puntos en los que la línea corta
	//con cada "lado". Los ejes serán 1, 2, 3 y 4 como 
	//arriba, derecha, abajo e izquierda.
	
	//La línea que va por arriba... Buscamos la x para la y enviada.
	t=linea.obtener_x_para_y(y);
	Punto p1=Punto(t, y);

	//La línea de la derecha... Buscamos la y para una x.
	t=linea.obtener_y_para_x(x+w);
	Punto p2=Punto(x+w, t);	

	//La línea de abajo: buscamos la x para una y.
	t=linea.obtener_x_para_y(y+h);
	Punto p3=Punto(t, y);

	//La línea de la izquierda...
	t=linea.obtener_y_para_x(x);
	Punto p4=Punto(x, t);

	//De estos cuatro puntos ahora vemos cuales estarían "dentro"
	//del rectángulo. Serán aquellos en los que al menos una coordenada
	//coincida...

	Set_puntos resultado=Set_puntos(Punto(0,0), Punto(0,0));

	if(p1.x >= x && p1.y <= x+w)
	{
		resultado.p1=p1;
	}
	else if(p3.x >= x && p3.y <= x+w)
	{
		resultado.p1=p3;
	}
	else
	{
		resultado.erroneo=true;
	}

	if(p2.y >= y && p2.y <= y+h)
	{
		resultado.p2=p2;
	}
	else if(p4.y >= y && p4.y <= y+h)
	{
		resultado.p2=p4;
	}
	else
	{
		resultado.erroneo=true;
	}

	return resultado;
}

Linea::Set_puntos Linea::obtener_para_linea_y_caja_limite(float x, float y, float w, float h, float px1, float py1, float px2, float py2)
{
	Linea linea=crear_desde_puntos(px1, py1, px2, py2);
	return obtener_para_linea_y_caja_limite(x, y, w, h, linea);	

/*
	Linea linea=crear_desde_puntos(px1, py1, px2, py2);
	float dx1, dy1, dx2, dy2, dist1, dist2, defx, defy;

	if(linea.horizontal)
	{
		defx=px1 < px2 ? x+w : x;
		defy=py1;
	}
	else if(linea.vertical)
	{
		defx=px1;
		defy=py1 > py2 ? -(y+h) : y;
	}
	else
	{	
		dx1=px1 < px2 ? x+w : x;
		dy1=linea.obtener_y_para_x(dx1);

		dy2=py1 > py2 ? -(y+h) : y;
		dx2=linea.obtener_x_para_y(dy2);

		dist1=abs(Linea::distancia_entre_puntos(px1, py1, dx1, dy1));
		dist2=abs(Linea::distancia_entre_puntos(px1, py1, dx2, dy2));

		if(dist1 < dist2)
		{
			defx=dx1;
			defy=dy1;
		}
		else
		{
			defx=dx2;
			defy=dy2;
		}
	}

	Linea resultado=crear_desde_puntos(dx1, dy1, dx2, dy2);
	return resultado;
*/
}
