#include "representacion_texto_fija.h"

using namespace DLibV;

Representacion_texto_fija::Representacion_texto_fija(const Representacion_texto_fija& p_otra)
	:Representacion_texto(p_otra)
{
	
}

Representacion_texto_fija& Representacion_texto_fija::operator=(const Representacion_texto_fija& p_otra)
{
	Representacion_texto::operator=(p_otra);
	return *this;
}
