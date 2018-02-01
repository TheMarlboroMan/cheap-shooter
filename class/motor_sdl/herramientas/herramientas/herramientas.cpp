#include "herramientas.h"

using namespace DLibH;

std::stringstream Herramientas::sstream;

float Herramientas::angulo_entre_puntos(float x1, float y1, float x2, float y2)
{
	float theta = atan2(y2 - y1, x2 - x1);
	theta += M_PI/2.0; //0 apunta ahora hacia el norte...
	float angulo=theta * 180 / M_PI; //Convertir a grados.
	if(angulo < 0) angulo+=360;
	return angulo;
}

int Herramientas::obtener_aleatorio_tramo(int ini, int fin)
{
	int resultado=rand() % (fin-ini) + ini;
	return resultado;
}

int Herramientas::cadena_a_entero(const std::string& p_cadena)
{
	Herramientas::sstream.str("");
	Herramientas::sstream.clear();

	if(p_cadena[0]=='0') 
	{
		return 0;
	}
	else
	{
		int resultado=0;
		Herramientas::sstream<<p_cadena<<std::endl;
		Herramientas::sstream>>resultado;
	
		return resultado;
	}
}

unsigned int Herramientas::digitos_en_entero(const int& p_entero)
{
	if(p_entero==0) return 1;

	int copia=p_entero;
	int resultado=0;

	if(copia < 0) copia=-copia;
		
	while(copia > 0)
	{
		resultado++;
		copia/=10;
	}
	return resultado;
}

unsigned short int Herramientas::tipo_inicio_utf8(const char c)
{
	if(es_inicio_utf8_6b(c)) return 6;
	else if(es_inicio_utf8_5b(c)) return 5;
	else if(es_inicio_utf8_4b(c)) return 4;
	else if(es_inicio_utf8_3b(c)) return 3;
	else if(es_inicio_utf8_2b(c)) return 2;
	else return 0;
}

bool Herramientas::es_inicio_utf8_2b(const char c){return c & 128;}
bool Herramientas::es_inicio_utf8_3b(const char c){return es_inicio_utf8_2b(c) && (c & 64);}
bool Herramientas::es_inicio_utf8_4b(const char c){return es_inicio_utf8_3b(c) && (c & 32);}
bool Herramientas::es_inicio_utf8_5b(const char c){return es_inicio_utf8_4b(c) && (c & 16);}
bool Herramientas::es_inicio_utf8_6b(const char c){return es_inicio_utf8_5b(c) && (c & 8);}

unsigned char Herramientas::utf8_2b_a_uchar(char a, char b)
{
	a=a & 3;	//Nos quedamos con los 2 bits de la derecha.
	a=a<<6;		//Desplazamos 6 a la izquierda para que esos dos estén a la izquierda del todo.
	b=b & 63;	//Nos quedamos con los 6 bits de la derecha.

	unsigned char r=a|b;	//Combinamos.
	return r;
}

int Herramientas::abs(int &p_valor)
{	
	return (p_valor<0 ? p_valor*-1 : p_valor);
}

char * Herramientas::cadena_w_a_8(const wchar_t * p_cadena)
{
	char * buffer=new char[1024];
	unsigned int i=0;

	std::locale loc;

	while(p_cadena[i]!='\0')
	{
		buffer[i]=std::use_facet<std::ctype<wchar_t> >(loc).widen(p_cadena[i]);
		i++;
	}

	buffer[i]='\0';

//			=st.narrow(p_cadena[i], '?');

	return buffer;
}

int Herramientas::cadena_a_entero(const wchar_t * p_cadena)
{
	char * buffer=new char[1024];
	int resultado=0;

	buffer=cadena_w_a_8(p_cadena);
	resultado=cadena_a_entero(buffer);

	delete [] buffer;
	return resultado;
}

int Herramientas::cadena_a_entero(const char * p_cadena)
{
	int cantidad=0, i=0, valor=0;
	long int multiplicador=1;
	const char * p=const_cast<char *>(p_cadena);
	int resultado=0;

	//Primero buscamos el final de la cadena...

	while(*p!='\0')
	{
		cantidad++;
		p++;			
	}
	
	p--;	//Vamos al último valor.

	//Y ahora al revés...
	for(; i<cantidad; i++)
	{
		if(*p>=48 && *p<=57)	//Dígitos, siendo 48 el 0
		{
			valor=(*p)-48;
			resultado+=valor*multiplicador;
			multiplicador=multiplicador*10;
		}
		else if(*p==45)		//Signo menos
		{
			resultado=-1*resultado;
			break;
		}
		else
		{
			break;
		}

		p--;
	}

	return resultado;		
}

std::string Herramientas::float_a_cadena(float numero)
{
	std::string resultado;
	char buffer[40];	
	std::snprintf(buffer, 40, "%f", numero);
	resultado=buffer;
	return resultado;
}

std::string Herramientas::entero_a_cadena(int numero)
{
	int i;
	int valor=numero;
	int longitud=1; //Necesitamos calcular la longitud de la cadena...
	std::string resultado, reverso;

	valor=abs(valor);

	//Cuantos dígitos tendrá???.
	while(valor >= 10)
	{
	        valor=valor/10;
	        longitud++;
	}

	//Ahora tendremos que recorrer el entero y plantarlo en la cadena.
	valor=Herramientas::abs(numero);

	for(i=longitud-1; i>=0; i--)
	{               
	        char temp=48+(valor%10);
	        resultado.append(1, temp);      //Colocar temp 1 vez.
	        valor=(valor-(valor%10))/10;    //Tiene su cierta gracia... 
	}

	if(numero<0)
	    resultado.append("-");

	//Y ahora le damos la vuelta... Hay otras formas haciendo otros includes
	//y con iteradores, pero mejor otro día...

	longitud=resultado.length()-1;

	for(i=longitud; i>=0; i--)
	        reverso.append(1, resultado[i]);

	return reverso;
}

float Herramientas::round(float r) 
{
	return (r > 0.0f) ? floor(r + 0.5f) : ceil(r - 0.5f);
}

float Herramientas::grados_a_radianes(float cosa)
{
    return (cosa * M_PI) / 180;
}


//Estas dos sirven para coger una cadena cualquiera y reemplazar cualquier
//caracter que no sea alfanumérico por el caracter p_reemplazo.

std::string Herramientas::cadena_a_alfanumerica_normalizada(const char * p_cadena, const char p_reemplazo)
{
	std::string temp(p_cadena);
	return cadena_a_alfanumerica_normalizada(temp, p_reemplazo);
}

std::string Herramientas::cadena_a_alfanumerica_normalizada(std::string const & p_cadena, const char p_reemplazo)
{
	std::string resultado;	
	unsigned int l=p_cadena.size();
	unsigned int i=0;
	unsigned char buffer='0';

	while(i<l)
	{
		buffer=p_cadena[i];

		if(!isalnum(buffer))
		{		
			buffer=p_reemplazo;
		}

		resultado+=buffer;
		++i;
	}

	return resultado;
}

std::string Herramientas::cadena_a_maximo_ancho(std::string const &p_cadena, unsigned int p_maximo)
{
	unsigned int leidos=0;
	std::string resultado;
	auto it=p_cadena.begin(), fin=p_cadena.end();
	
	while(it != fin)
	{
		++leidos;
		resultado+=(*it);

		//TODO: REvisar: si hacemos líneas de exactamente el mismo
		//ancho hace dos veces \n.

		if((*it)=='\n') 
		{
			leidos=0;
		}
		else if(leidos % p_maximo==0)
		{
			//TODO
			//Aquí cabe un poco más de rollo... 
			//Por ejemplo, si lo que hemos leido es un 
			//espacio lo podemos quitar. Si vamos a partir
			//una cadena en dos podemos directamente cortar
			//hacia atrás (siempre que haya un espacio cerca)
			//Y pasar a la siguiente.

			resultado+='\n';
		}

		++it;
	}

	return resultado;
}

bool Herramientas::validar_email(std::string p_email)
{
	bool resultado=true;

	unsigned int pos_arroba=0;
	unsigned int pos_punto=0;

	if(!p_email.size())
	{
		resultado=false;
	}
	else
	{
		//El primer carácter debe ser alfabético...

		if(!isalpha(p_email[0]))
		{
			resultado=false;
		}
		else
		{
			//Buscamos una arroba.
			pos_arroba=p_email.find('@');
		
			if(pos_arroba==std::string::npos)
			{
				resultado=false;
			}
			else
			{
				//Buscamos un punto tras la arroba.
				pos_punto=p_email.find('.', pos_arroba);

				if(pos_punto==std::string::npos)
				{
					resultado=false;
				}
				else
				{
					//Finalmente miramos si hay texto tras el punto.
					if(!p_email.substr(pos_punto+1, std::string::npos).size())
					{
						resultado=false;
					}
				}
			}
		}			
	}

	return resultado;
}

std::vector<std::string> Herramientas::explotar(const std::string & p_cadena, const char p_delimitador)
{
	std::vector<std::string> resultado;

	unsigned int actual=0;
	unsigned int siguiente=0;

	while(siguiente != std::string::npos)
	{
		siguiente=p_cadena.find(p_delimitador, actual);
		resultado.push_back(p_cadena.substr(actual, siguiente-actual) );
		actual=siguiente+1;	//Esto es un dolor de cabeza... seguro.
	};
	return resultado;
}

float Herramientas::porcentaje(float p_parte, float p_total)
{
	if(p_total==0) return 0;
	else return (p_parte * 100) / p_total;
}

std::string Herramientas::cadena_desde_fichero(const char * p_nombre_fichero)
{
	std::ifstream archivo(p_nombre_fichero, std::ios::in);
	std::string resultado;	
	char buffer=' ';

	if(archivo)
	{
		while(true) 
		{
			buffer=archivo.get();
			if(!archivo.eof()) resultado+=buffer;
			else break;			
		}
		archivo.close();
	}

	return resultado;
}
