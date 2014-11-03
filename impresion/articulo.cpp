#include "articulo.h"

articulo::articulo()
{
}

articulo::~articulo()
{
}


QString articulo::get_t_articulo()
{
    return t_articulo;
}

QString articulo::get_cantidad()
{
    return cantidad;
}

QString articulo::get_p_unitario()
{
    return p_unitario;
}

QString articulo::get_importe()
{
    return importe;
}

void articulo::set_t_articulo(QString tmp)
{
   t_articulo = tmp;
}

void articulo::set_cantidad(QString tmp)
{
    cantidad = tmp;
}

void articulo::set_p_unitario(QString tmp)
{
    p_unitario = tmp;
}

void articulo::set_importe(QString tmp)
{
    importe = tmp;
}
