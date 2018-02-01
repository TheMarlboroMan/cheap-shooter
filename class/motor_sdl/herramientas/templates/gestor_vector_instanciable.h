#ifndef GESTOR_VECTOR_INSTANCIABLE_H
#define GESTOR_VECTOR_INSTANCIABLE_H

#include <vector>
#include <iostream> //Para NULL...

/*
Este es instanciable... Igual que el otro, pero instanciable.
*/

namespace DLibH
{


template<class T>
class Gestor_vector_instanciable
{
	protected:
	
	std::vector<T> elementos;

	public:	

	Gestor_vector_instanciable(){}
	~Gestor_vector_instanciable(){}

	const std::vector<T>& acc_elementos() const {return this->elementos;}
	unsigned int cantidad() const {return this->elementos.size();}

	T registrar(T p_elem)
	{
		unsigned int l=this->elementos.size(), i=0;
		bool encontrado=false;
		for(; i<l && !encontrado; i++)
			encontrado=p_elem==this->elementos[i];

		if(!encontrado)
			this->elementos.push_back(p_elem);

		return p_elem;
	}

	T obtener(unsigned int p_indice) const
	{
		unsigned int l=this->elementos.size();
		T resultado=NULL;

		if(p_indice >= 0 && p_indice<l)
		{
			resultado=this->elementos[p_indice];
		}

		return resultado;
	}

	bool liberar(unsigned int p_indice)
	{
		bool resultado=false;
		unsigned int l=this->elementos.size(), i=0;
		
		if(p_indice >= 0 && p_indice<l)
		{	
			this->elementos.erase(this->elementos.begin()+p_indice);
			resultado=true;
		}

		return resultado;
	}

	void vaciar()
	{
		while(!this->elementos.empty())
		{
			T elemento=this->elementos.back();
			delete elemento;
			this->elementos.pop_back();			
		}
	}

};

} //Fin namespace DLibH
#endif
