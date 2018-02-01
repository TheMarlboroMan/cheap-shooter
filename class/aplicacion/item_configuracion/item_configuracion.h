#ifndef ITEM_CONFIGURACION_H
#define ITEM_CONFIGURACION_H

#include <string>
#include <vector>

class Item_configuracion
{
	private:

	class Opcion_configuracion
	{
		public:

		std::string mostrar;
		int valor;

		Opcion_configuracion(int p_valor, std::string p_mostrar)
		{
			this->valor=p_valor;
			this->mostrar=p_mostrar;
		}
	};
	
	std::string titulo;
	std::vector<Opcion_configuracion> opciones;
	std::vector<Opcion_configuracion>::iterator iterador_opciones;

	public:

	std::string acc_titulo() const {return this->titulo;}
	std::string acc_titulo_seleccionado() const 
	{
		if(this->opciones.size()) return (*iterador_opciones).mostrar;
		else return std::string("");
	}
	int acc_valor_seleccionado() const 
	{
		if(this->opciones.size()) return (*iterador_opciones).valor;
		else return -1;
	}

	void establecer_actual(int valor_opcion)
	{
		std::vector<Opcion_configuracion>::iterator 	ini=this->opciones.begin(),
								fin=this->opciones.end();
		for(;ini<fin;ini++)
		{
			if( (*ini).valor==valor_opcion)
			{
				this->iterador_opciones=ini;
				break;
			}
		}
	}

	void cambiar_seleccion(int p_direccion)
	{
		std::vector<Opcion_configuracion>::iterator 	ini=this->opciones.begin(),
								fin=this->opciones.end();

		if(p_direccion > 0) 
		{
			this->iterador_opciones++;
			if(this->iterador_opciones==fin) this->iterador_opciones=ini;
		}
		else 
		{	
			if(this->iterador_opciones==ini) this->iterador_opciones=fin;
			this->iterador_opciones--;
		}
	}

	void insertar_opcion(int p_valor, std::string p_mostrar)
	{
		this->opciones.push_back(Opcion_configuracion(p_valor, p_mostrar));
		this->iterador_opciones=this->opciones.begin();
	}

	Item_configuracion(std::string);
	~Item_configuracion();	
};

#endif
