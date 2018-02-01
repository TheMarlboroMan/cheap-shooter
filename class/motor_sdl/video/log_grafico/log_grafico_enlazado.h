#ifndef LOG_GRAFICO_ENLAZADO_H
#define LOG_GRAFICO_ENLAZADO_H

/*Este se enlaza a un objeto del tipo Log_base y toma el control de la cadena
del mismo, usándola como base de sus propias entradas.
El proceso es manual, obligatoriamente llamaríamos a "procesar".

Usando esto me he encontrado con un problema si queremos hacer un objeto
LOG_GRAFICO global para usarlo en cualquier parte. El problema es que no crear
un objeto de esta clase sin antes inicializar la SDL y cargar los recursos 
necesarios... No hay forma de inicializar la SDL antes de que se hayan 
inicializado los objetos del ámbito global, de modo que estamos en un problema 
cíclico.

La solución es relativamente sencilla: El Log_grafico_enlazado está enlazado
a un objeto del tipo Log_base que no depende más que del sistema de archivos 
del lenguaje, de modo que si podemos instanciar uno global.

Luego, en el controlador, podemos crear el objeto LOG_GRAFICO y enlazarlo con
el Log_base global sin problemas. Las entradas de log se hacen mediante el 
objeto "Log_base" y no mediante "LOG_GRAFICO" así que tampoco hay problemas por
ahí.

Si vamos a hacer esto he encontrado útil crear un archivo de cabecera que
incluya la clase "Log_base" y declare:

extern DLibH::Log_base LOG;

Este archivo lo incluimos luego allá donde queramos usar el log y ya lo
dejamos todo listo.
*/

#include "log_grafico.h"
#include "../../herramientas/log_base/log_base.h"

namespace DLibV
{

class Log_grafico_enlazado:public Log_grafico
{
	private:

	DLibH::Log_base &log;

	public:

	Log_grafico_enlazado(const Recurso_grafico *, const SDL_Rect&, DLibH::Log_base&);
	~Log_grafico_enlazado();
	void procesar();
};

}
#endif
