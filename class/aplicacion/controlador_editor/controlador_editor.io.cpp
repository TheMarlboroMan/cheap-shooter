bool Controlador_editor::cargar()
{
	std::ifstream archivo(this->nombre_fichero.c_str(), std::ios::in | std::ios::binary);

	if(!archivo)
	{
		this->mensaje="Error al cargar el archivo ";
		this->mensaje+=this->nombre_fichero;

		return false;
	}
	else
	{
		this->reiniciar();

		U_ENTERO_16 cantidad=0;
		unsigned int i=0;

		INFO_NIVEL.deserializar(archivo);

		this->deserializar_mapa_items(this->TABLA_OBSTACULOS, archivo); 		//Tablas...
		this->deserializar_mapa_items(this->TABLA_BONUS, archivo);
		this->deserializar_mapa_items(this->TABLA_DESTRUIR_CAJA, archivo);
		this->deserializar_mapa_items(this->TABLA_DESTRUIR_OBSTACULO, archivo);
		
		archivo.read((char *) &cantidad, sizeof(U_ENTERO_16)); 	//Leer la cantidad de acciones y las acciones...
		i=0;
		while(i< cantidad)
		{
			this->acciones.push_back(Accion_trigger::deserializar(archivo));
			++i;
		}

		//Leemos la cantidad de actores...
		archivo.read((char *) &cantidad, sizeof(U_ENTERO_16));
		i=0;
		while(i < cantidad)
		{
			this->deserializar_actor_mapa(archivo);
			++i;
		}

		archivo.close();

		this->actualizar_maximo_tag();

		this->mensaje="Archivo ";
		this->mensaje+=this->nombre_fichero;
		this->mensaje+=" cargado correctamente";


		return true;
	}
}

void Controlador_editor::copiar_archivo()
{
	std::ifstream archivo(this->nombre_fichero.c_str(), std::ios::in | std::ios::binary);

	if(archivo)
	{
		std::string nombre_copia=this->nombre_fichero+".bak";
		std::ofstream salida(nombre_copia.c_str(), std::ios::out | std::ios::binary);

		while(true)
		{
			U_ENTERO_8 buf;
			archivo.read((char *) &buf, sizeof(U_ENTERO_8));
			salida.write((char *) &buf, sizeof(U_ENTERO_8));

			if(archivo.eof()) break;
		}
	}
}

bool Controlador_editor::guardar()
{
	this->copiar_archivo();
	std::ofstream archivo(this->nombre_fichero.c_str(), std::ios::out | std::ios::binary);
		
	if(!archivo)
	{
		this->mensaje="No ha sido posible guardar el archivo ";
		this->mensaje.append(this->nombre_fichero);
		return false;
	}
	else
	{			
		INFO_NIVEL.serializar(archivo);

		//Tablas...
		this->serializar_mapa_items(this->TABLA_OBSTACULOS, archivo);
		this->serializar_mapa_items(this->TABLA_BONUS, archivo);
		this->serializar_mapa_items(this->TABLA_DESTRUIR_CAJA, archivo);
		this->serializar_mapa_items(this->TABLA_DESTRUIR_OBSTACULO, archivo);

		//Serializar las acciones. Primero la cantidad, luego las acciones.
		U_ENTERO_16 cantidad_acciones=this->acciones.size();
		archivo.write((char *) &cantidad_acciones, sizeof(U_ENTERO_16));
		for(Accion_trigger& at : acciones) at.serializar(archivo);

		//Serializar los actores, primero la cantidad, luego los actores.

		U_ENTERO_16 cantidad=this->actores_mapa.size();
		archivo.write((char *) &cantidad, sizeof(U_ENTERO_16));

		Comparador_actor_parametrizado ordenador;
		std::sort(actores_mapa.begin(), actores_mapa.end(), ordenador);

		std::vector<Actor_serializado> vector_ser;
		for(T_TIPO_ACTOR* tta : actores_mapa) vector_ser.push_back(this->serializar_actor_mapa(tta));

//		Comparador_actor_serializado ordenador;
//		std::sort(vector_ser.begin(), vector_ser.end(), ordenador);
		for(Actor_serializado& s : vector_ser) s.serializar(archivo);

		archivo.close();

		this->mensaje="Archivo ";
		this->mensaje+=this->nombre_fichero;
		this->mensaje+=" guardado correctamente";

		return true;
	}
}

Actor_serializado Controlador_editor::serializar_actor_mapa(T_TIPO_ACTOR * p_actor)
{
	Actor_serializado act;
	act.tipo=p_actor->actor->tipo_actor_mapa();
	act.x=p_actor->actor->acc_x();
	act.y=p_actor->actor->acc_y();
	act.tag=p_actor->actor->acc_tag();
	act.parametrizable=p_actor->actor->es_parametrizable() ? 1 : 0;
	act.representacion_flexible=p_actor->actor->es_flexible() ? 1 : 0;
	act.redimensionable=p_actor->actor->es_redimensionable() ? 1 : 0;
	act.ordenable_profundidad=p_actor->actor->es_ordenable_profundidad() ? 1 : 0;

	act.params_redim=p_actor->params_redim;
	act.parametros=p_actor->parametros;
	act.params_rep=p_actor->params_rep;
	act.params_profundidad=p_actor->params_profundidad;

	return act;
}

void Controlador_editor::deserializar_actor_mapa(std::ifstream &archivo)
{
	Actor_serializado act=Actor_serializado::deserializar(archivo);
	this->insertar_actor_en_posicion(act.x, act.y, act.tipo, act.tag, act.parametros, act.params_rep, act.params_redim, act.params_profundidad, true);
}

void Controlador_editor::deserializar_mapa_items(std::vector<Item_tabla_probabilidad> &p_tabla, std::ifstream &p_archivo)
{

	unsigned int cantidad=0;
	unsigned int i=0;
	unsigned int tipo=0;
	unsigned short int probabilidad=0; 

	//Leer la cantidad
	p_archivo.read((char *) &cantidad, sizeof(U_ENTERO_16));

	//Leer tantos items como la cantidad indique...

	while(i < cantidad)
	{
		p_archivo.read((char *) &tipo, sizeof(U_ENTERO_16));
		p_archivo.read((char *) &probabilidad, sizeof(U_ENTERO_8));

		//Buscamos el item con el tipo y aumentamos su probabilidad.
		this->aux_deserializar_mapa_items(p_tabla, tipo, probabilidad);
		++i;
	}
}	

void Controlador_editor::aux_deserializar_mapa_items(std::vector<Item_tabla_probabilidad> & p_tabla, unsigned int p_tipo, unsigned short int p_probabilidad)
{
	std::vector<Item_tabla_probabilidad>::iterator			ini=p_tabla.begin(),
									fin=p_tabla.end();

	while(ini < fin)
	{

		if( (*ini).tipo==p_tipo)
		{
			(*ini).probabilidad=p_probabilidad;
			break;
		}

		ini++;
	};
}

void Controlador_editor::serializar_mapa_items(std::vector<Controlador_editor::Item_tabla_probabilidad> const & p_tabla, std::ofstream &p_archivo)
{
	std::vector<Item_tabla_probabilidad>::const_iterator		ini=p_tabla.begin(),
									fin=p_tabla.end();
	unsigned short int probabilidad=0;
	unsigned int tipo=0;
	unsigned int cantidad=0;

	//Se escribe en primer lugar la cantidad de items que habrá...

	while(ini < fin)
	{
		if((*ini).probabilidad!=0) ++cantidad;
		++ini;
	}

	p_archivo.write((char *) &cantidad, sizeof(U_ENTERO_16));

	//Y ahora se escribe cada uno de los items.

	ini=p_tabla.begin();

	while(ini < fin)
	{
		probabilidad=(*ini).probabilidad;
		tipo=(*ini).tipo;

		if(probabilidad!=0)
		{
			p_archivo.write((char *) &tipo, sizeof(U_ENTERO_16));
			p_archivo.write((char *) &probabilidad, sizeof(U_ENTERO_8));
		}

		++ini;
	}
}
