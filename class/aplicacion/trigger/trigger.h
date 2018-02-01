#ifndef TRIGGER_H
#define TRIGGER_H

/*Las cosas propias del trigger están separadas del obstáculo trigger. En 
un principio no era así pero para el trigger automático, que es mitad trigger
y mitad actor actividad, era obligatorio.*/

class Trigger
{
	private:

	unsigned int tag_accion;
	bool repetible;
	mutable unsigned int veces_activado;
	bool en_curso;

	public:

	Trigger();
	~Trigger();

	void mut_tag_accion(unsigned int p_valor) {this->tag_accion=p_valor;}
	void mut_repetible(bool p_valor) {this->repetible=p_valor;}

	bool es_repetible() const {return this->repetible && !this->en_curso;}
	unsigned int acc_tag_accion() const {return this->tag_accion;}

	bool puede_ser_activado() const;
	void activar() const;
	virtual void recibir_inicio_accion();
	virtual void recibir_finalizacion_accion();
};

#endif
