/*Esta es la clase que no hace nada de nada: tiene declarados los mismos métodos
que la clase "log_base", pero no tienen ningún efecto. Por no tener, no tiene
ninguna propiedad.*/

class Log_base
{
	private:

	template <class X> void en_cadena(const X &input){}
	void en_cadena(std::ostream& (*pf)(std::ostream&)){}
	void en_cadena(std::ios& (*pf)(std::ios&)){}
	void en_cadena(std::ios_base& (*pf)(std::ios_base&)){}

	public:
	std::string cadena() {return "";}
	void limpiar_cadena() {}
	bool es_cadena_actualizada() const {return false;}

	Log_base() {}
	Log_base(const char *) {}
	~Log_base() {}

//Esta es la entrada para todo lo que hay... Si el nivel actual es 
//mayor o igual que el nivel mínimo se logueará también en la cadena.

	template <class X> Log_base& operator<<(const X &input) {return *this;}
	Log_base& operator<<(const Log_base_t& n) {return *this;}
	Log_base& operator<<(const Log_base_n& n) {return *this;}
	Log_base& operator<<(const Log_base_m& n) {return *this;}
	Log_base& operator<<(std::ostream& (*)(std::ostream&)) {return *this;}
	Log_base& operator<<(std::ios& (*)(std::ios&)) {return *this;}
	Log_base& operator<<(std::ios_base& (*)(std::ios_base&)) {return *this;}
};
