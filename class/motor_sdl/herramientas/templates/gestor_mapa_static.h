#ifndef GESTOR_MAPA_STATIC_H
#define GESTOR_MAPA_STATIC_H

#include <map>
#include <iostream> //Para NULL.

/*
Unas palabras... Aquí hay estar un poco atento: el tipo es class T y siempre
se enuncia de ese modo. Si la intención del vector es recibir punteros lo
declarareos como Gestor_mapa_static<clase_de_turno *>.

Este template es una lista pensada para guardar cosas e indexarlas por su clave.
La clave es "class C" (por ejemplo, int o std::string) y el valor es "class V"
que ya sería cualquier otra cosa.

Está pensado, evidentemente, para punteros.
*/

namespace DLibH
{


template<class C, class V>
class Gestor_mapa_static
{
	protected:
	
	static std::map<C, V> elementos;

	Gestor_mapa_static(){}
	~Gestor_mapa_static(){}

	public:	

	static const std::map<C, V>& acc_elementos() {return Gestor_mapa_static::elementos;}

	//Const????
	static unsigned int cantidad() {return Gestor_mapa_static::elementos.size();}

	static V obtener(C p_c)
	{
		typename std::map<C, V>::iterator it;
		it=Gestor_mapa_static::elementos.find(p_c);
		
		if(it == Gestor_mapa_static::elementos.end())
		{
			return NULL;
		}
		else
		{
			return it->second;
		}
	}

	static void eliminar(C p_c)
	{
		Gestor_mapa_static::elementos.erase(p_c);
	}

	static C insertar(C p_c, V p_v)
	{
		std::pair<C,V> insertar=std::pair<C,V>(p_c, p_v), resultado;
		Gestor_mapa_static::elementos.insert(insertar);
		return p_c;
	}

	static bool comprobar(C p_c)
	{
		typename std::map<C, V>::iterator it;
		it=Gestor_mapa_static::elementos.find(p_c);
		return it != Gestor_mapa_static::elementos.end();
	}

	static void vaciar()
	{
		while(!Gestor_mapa_static::elementos.empty())
		{
			typename std::map<C, V>::iterator it=Gestor_mapa_static::elementos.begin();
			delete it->second;
			Gestor_mapa_static::elementos.erase(it);		
		}
	}
};

template <class C, class V> std::map<C, V> Gestor_mapa_static<C,V>::elementos;

} //Fin namespace DLibH

#endif
