#ifndef LOCALIZADOR_BASE_H
#define LOCALIZADOR_BASE_H

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <cstring> //Para memset
#include <utility> //para "pair"
#include "../herramientas/herramientas.h"
#include <vector>


namespace DLibH
{

//typedef std::map<unsigned int, std::wstring> t_mapa;	//Tipo del mapa.
//typedef std::wstring t_cadena;				//Tipo de cadena que devuelve.
typedef std::map<unsigned int, std::string> t_mapa;	//Tipo del mapa.
typedef std::string t_cadena;				//Tipo de cadena que devuelve.
typedef std::string t_cadena_stream;				//Tipo de cadena que devuelve.
typedef char t_char_buffer;				//Tipo del buffer de lectura del archivo.
typedef std::ifstream t_stream_in;			//Tipo del archivo que abre.
typedef std::vector<t_cadena> t_lista_nombres_archivo;	//Tipo de dato que devuelve componer_nombre_archivo;

class Localizador_base
{
	protected:

	t_mapa cadenas;
	unsigned short int idioma;

	int delimitador_inicio_en_cadena(t_cadena const&);
	bool delimitador_fin_en_cadena(t_cadena const&);
	void limpiar_cadenas();
	void insertar_cadena(unsigned int, t_cadena const&);
	t_cadena generar_nombre_archivo(t_cadena const&);
	void procesar_fichero(t_cadena const&);

	public:

	enum IDIOMAS{I_ESPANOL=0, I_INGLES=1};

	Localizador_base(unsigned short int);
	virtual ~Localizador_base();

	void cambiar_idioma(unsigned short int);
	t_cadena const& obtener(unsigned int);

	//A implementar...

	protected:
	virtual t_lista_nombres_archivo obtener_lista_archivos()=0;
	virtual t_cadena const& cadena_no_cargado()=0;
	virtual t_cadena const& cadena_no_encontrado()=0;

	public:
	virtual void inicializar();
	void recargar() {inicializar();}

};

} //Fin namespace DLibH
#endif
