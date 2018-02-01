#include "log_base.h"

using namespace DLibH;

Log_base * Log_motor::l=NULL;
std::stringstream Log_base::sstream;

Log_base& Log_base::operator<<(std::ostream& ( *pf )(std::ostream&))
{
	if(es_usable())
	{
		s<<pf;
		if(this->nivel_actual >= this->nivel_min) en_cadena(pf);
	}
	return *this;
}

Log_base& Log_base::operator<<(std::ios& ( *pf )(std::ios&))
{
	if(es_usable())
	{
		s<<pf;
		if(this->nivel_actual >= this->nivel_min) en_cadena(pf);
	}
	return *this;
}

Log_base& Log_base::operator<<(std::ios_base& ( *pf )(std::ios_base&))
{
	if(es_usable())
	{
		s<<pf;
		if(this->nivel_actual >= this->nivel_min) en_cadena(pf);
	}
	return *this;
}

