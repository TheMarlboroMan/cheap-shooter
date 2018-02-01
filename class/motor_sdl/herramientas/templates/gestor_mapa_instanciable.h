#ifndef GESTOR_MAPA_INSTANCIABLE_H
#define GESTOR_MAPA_INSTANCIABLE_H

#include <map>
#include <iostream> //Para NULL.

/*
Unas palabras... Aquí hay estar un poco atento: el tipo es class T y siempre
se enuncia de ese modo. Si la intención del vector es recibir punteros lo
declarareos como Gestor_mapa_instanciable<clase_de_turno *>.

Este template es una lista pensada para guardar cosas e indexarlas por su clave.
La clave es "class C" (por ejemplo, int o std::string) y el valor es "class V"
que ya sería cualquier otra cosa.

Está pensado, evidentemente, para punteros.
*/

namespace DLibH
{


template<class C, class V>
class Gestor_mapa_instanciable
{
	protected:
	
	static std::map<C, V> elementos;

	public:	

	Gestor_mapa_instanciable(){}
	~Gestor_mapa_instanciable(){}

	const std::map<C, V>& acc_elementos() {return this->elementos;}
	unsigned int cantidad() {return this->elementos.size();}

	V obtener(C p_c)
	{
		typename std::map<C, V>::iterator it;
		it=this->elementos.find(p_c);
		
		if(it == this->elementos.end())
		{
			return NULL;
		}
		else
		{
			return it->second;
		}
	}

	void eliminar(C p_c)
	{
		this->elementos.erase(p_c);
	}

	C insertar(C p_c, V p_v)
	{
		this->elementos.insert(std::pair<C,V>(p_c, p_v));
		return p_c;
	}

	void vaciar()
	{
		while(!this->elementos.empty())
		{
			typename std::map<C, V>::iterator it=this->elementos.begin();
			delete it->second;
			this->elementos.erase(it);		
		}
	}
};

} //Fin namespace DLibH

#endif
