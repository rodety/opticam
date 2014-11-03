#ifndef HISTORIALCLINICO_H
#define HISTORIALCLINICO_H

#include <cliente/medidashistorial.h>

class historialClinico:ObjetoPersistente
{
private:
    QString idHistorialClinico;
    QString idCliente;
    QString doctor;
    QString fecha;
    medidasHistorial mCercaDerecha;
    medidasHistorial mCercaIzquierda;
    medidasHistorial mLejosDerecha;
    medidasHistorial mLejosIzquierda;

    bool fechaCambiada;
public:
    historialClinico();

    QString getIdHistorialClinico();
    QString getIdCliente();
    QString getDoctor();
    QString getFecha();
    medidasHistorial& getMedidasCercaDerecha();
    medidasHistorial& getMedidasCercaIzquierda();
    medidasHistorial& getMedidasLejosDerecha();
    medidasHistorial& getMedidasLejosIzquierda();

    void setIdHistorialClinico(QString);
    void setIdCliente(QString);
    void setDoctor(QString);
    void setFecha(QString);
    void setMedidasCercaDerecha(medidasHistorial);
    void setMedidasCercaIzquierda(medidasHistorial);
    void setMedidasLejosDerecha(medidasHistorial);
    void setMedidasLejosIzquierda(medidasHistorial);
    void setFlag(bool);

    bool agregar();
    bool actualizar();
    bool eliminar();

    QSqlQueryModel* mostrar();
    bool buscar();
};

#endif // HISTORIALCLINICO_H
