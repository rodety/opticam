#ifndef ARTICULO_H
#define ARTICULO_H
#include <QString>

class articulo
{
public:
    articulo();
    ~articulo();    
    QString get_t_articulo();
    QString get_cantidad();
    QString get_p_unitario();
    QString get_importe();

     void set_t_articulo(QString tmp);
     void set_cantidad(QString tmp);
     void set_p_unitario(QString tmp);
     void set_importe(QString tmp);

private:

    QString t_articulo;
    QString cantidad;
    QString p_unitario;
    QString importe;
};

#endif // ARTICULO_H
