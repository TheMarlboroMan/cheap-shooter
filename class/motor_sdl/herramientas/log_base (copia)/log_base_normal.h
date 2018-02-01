class Log_base
{
	private:

	static std::stringstream sstream;
	std::ofstream s;	
	std::string c;
	unsigned int nivel_min;
	unsigned int nivel_actual;
	bool cadena_actualizada;

	template <class X> void en_cadena(const X &input)
	{
		sstream.clear();
		sstream<<input;
		c+=sstream.str();
		sstream.str(" ");
		cadena_actualizada=true;
	}

	void en_cadena(std::ostream& (*pf)(std::ostream&))
	{
		sstream.clear();
		sstream<<pf;
		c+=sstream.str();
		sstream.str(" ");
		cadena_actualizada=true;
	}

	void en_cadena(std::ios& (*pf)(std::ios&))
	{
		sstream.clear();
		sstream<<pf;
		c+=sstream.str();
		sstream.str(" ");
		cadena_actualizada=true;
	}

	void en_cadena(std::ios_base& (*pf)(std::ios_base&))
	{
		sstream.clear();
		sstream<<pf;
		c+=sstream.str();
		sstream.str(" ");
		cadena_actualizada=true;
	}

	public:

	const std::string& cadena()
	{
		cadena_actualizada=false;
		return c;
	}

	void limpiar_cadena() {c="";}
	bool es_cadena_actualizada() const {return cadena_actualizada;}

	Log_base()
		:s("log.txt"), c(), nivel_min(10), nivel_actual(10),
		cadena_actualizada(false)
	{	
		s<<"INICIANDO SESION "<<time(NULL)<<std::endl;
	}

	Log_base(const char * nombre_fichero)
		:s(nombre_fichero), c(), nivel_min(10), nivel_actual(10),
		cadena_actualizada(false)
	{
		s<<"INICIANDO SESION "<<time(NULL)<<std::endl;
	}

	~Log_base()
	{
		s<<std::endl<<"FINALIZANDO SESION "<<time(NULL);
		s.close();
	}

//Esta es la entrada para todo lo que hay... Si el nivel actual es 
//mayor o igual que el nivel mínimo se logueará también en la cadena.

	template <class X> Log_base& operator<<(const X &input)
	{
		s<<input;
		if(this->nivel_actual >= this->nivel_min) en_cadena(input);
		s.flush();
		return *this;
	}

	//Coloca la hora.
	Log_base& operator<<(const Log_base_t& n)
	{
		char * t=new char[14];
		memset(t, '\0', 14);
		time_t tiempo=time(NULL);
		strftime(t, 14, "%H:%M:%S >> ", localtime(&tiempo));

		s<<t;
		delete [] t ;
		return *this;
	}

	//Establece un nuevo nivel actual de log
	Log_base& operator<<(const Log_base_n& n)
	{
		this->nivel_actual=n.niv;
		return *this;
	}

	//Establece un nuevo nivel minimo de log
	Log_base& operator<<(const Log_base_m& n)
	{
		this->nivel_min=n.niv;
		return *this;
	}

	Log_base& operator<<(std::ostream& (*)(std::ostream&));
	Log_base& operator<<(std::ios& (*)(std::ios&));
	Log_base& operator<<(std::ios_base& (*)(std::ios_base&));
};
