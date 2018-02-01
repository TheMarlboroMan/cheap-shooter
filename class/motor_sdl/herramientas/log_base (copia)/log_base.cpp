#include "log_base.h"

using namespace DLibH;

Log_base * Log_motor::l=NULL;

/*Si se ha definido la compilación del log silencioso, nos saltaremos la 
definición de estos métodos y de la propiedad estática sstream.*/

#ifndef LOG_BASE_SILENCIOSO
std::stringstream Log_base::sstream;

Log_base& Log_base::operator<<(std::ostream& ( *pf )(std::ostream&))
{
	s<<pf;
	if(this->nivel_actual >= this->nivel_min) en_cadena(pf);
	return *this;
}

Log_base& Log_base::operator<<(std::ios& ( *pf )(std::ios&))
{
	s<<pf;
	if(this->nivel_actual >= this->nivel_min) en_cadena(pf);
	return *this;
}

Log_base& Log_base::operator<<(std::ios_base& ( *pf )(std::ios_base&))
{
	s<<pf;
	if(this->nivel_actual >= this->nivel_min) en_cadena(pf);
	return *this;
}
#endif

