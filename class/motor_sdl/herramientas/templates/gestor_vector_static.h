#ifndef GESTOR_VECTOR_STATIC_H
#define GESTOR_VECTOR_STATIC_H

#include <vector>
#include <iostream> //Para NULL...
/*
Unas palabras... Aquí hay estar un poco atento: el tipo es class T y siempre
se enuncia de ese modo. Si la intención del vector es recibir punteros lo
declarareos como Gestor_vector_static<clase_de_turno *>.
*/

namespace DLibH
{


template<class T>
class Gestor_vector_static
{
	protected:
	
	static std::vector<T> elementos;

	Gestor_vector_static(){}
	~Gestor_vector_static(){}

	public:	

	static const std::vector<T>& acc_elementos() {return Gestor_vector_static::elementos;}
	unsigned int cantidad() const {return Gestor_vector_static::elementos.size();}

	static T registrar(T p_elem)
	{
		int l=Gestor_vector_static::elementos.size(), i=0;
		bool encontrado=false;
		for(; i<l && !encontrado; i++)
			encontrado=p_elem==Gestor_vector_static::elementos[i];

		if(!encontrado)
			Gestor_vector_static::elementos.push_back(p_elem);

		return p_elem;
	}

	static T obtener(unsigned int p_indice)
	{
		int l=Gestor_vector_static::elementos.size();
		T resultado=NULL;

		if(p_indice >= 0 && p_indice<l)
		{
			resultado=Gestor_vector_static::elementos[p_indice];
		}

		return resultado;
	}

	static bool liberar(unsigned int p_indice)
	{
		bool resultado=false;
		int l=Gestor_vector_static::elementos.size(), i=0;
		
		if(p_indice >= 0 && p_indice<l)
		{	
			Gestor_vector_static::elementos.erase(Gestor_vector_static::elementos.begin()+p_indice);
			resultado=true;
		}

		return resultado;
	}

	static void vaciar()
	{
		while(!Gestor_vector_static::elementos.empty())
		{
			T elemento=Gestor_vector_static::elementos.back();
			delete elemento;
			Gestor_vector_static::elementos.pop_back();			
		}
	}

};

template <class T> std::vector<T> Gestor_vector_static<T>::elementos;

} //Fin namespace DLibH

#endif
