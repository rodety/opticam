#ifndef LUNA_H
#define LUNA_H

#include <producto/producto.h>

class luna:public producto
{
private:
    diametro pDiametro;
    calidadluna pCalidad;
    tipoLuna pTipoLuna;
    tratamiento pTratamiento;
    QString valorInicial;
    QString valorFinal;
    QString precio;
    QString cilindro;
    QString cilindro_final;
    QString detalle;

public:
    luna();

    diametro getDiametro();
    calidadluna getCalidad();
    tipoLuna getTipoLuna();
    tratamiento getTratamiento();
    QString getValorInicial();
    QString getValorFinal();
    QString getPrecio();
    QString getCilindro();
    QString getCilindro_final();


    void setDiametro(diametro tmp);
    void setCalidad(calidadluna tmp);
    void setTipoLuna(tipoLuna tmp);
    void setTratamiento(tratamiento tmp);
    void setValorInicial(QString tmp);
    void setValorFinal(QString tmp);
    void setPrecio(QString tmp);
    void setCilindro(QString tmp);
    void setCilindro_final(QString tmp);
    void setDetalle(QString tmp);    

    bool agregar();
    bool actualizar();
    bool eliminar();

    static QSqlQueryModel* mostrar();
    bool completar();

    bool verificarPrecio();
};

#endif // LUNA_H
