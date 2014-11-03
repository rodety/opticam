#include "montura.h"
#include <producto/genero.h>
#include <configuracion/sesion.h>
class Sesion;
montura::montura()
{
}

forma montura::getForma()
{
    return pForma;
}
color montura::getColor()
{
    return pColor;
}
tamanio montura::getTamanio()
{
    return pTamanio;
}
calidad montura::getCalidad()
{
    return pCalidad;
}
genero montura::getGenero()
{
    return pGenero;
}


void montura::setForma(forma tmp)
{
    pForma=tmp;
}
void montura::setColor(color tmp)
{
    pColor=tmp;
}
void montura::setTamanio(tamanio tmp)
{
    pTamanio=tmp;
}
void montura::setCalidad(calidad tmp)
{
    pCalidad=tmp;
}

void montura::setGenero(genero tmp)
{
    pGenero=tmp;
}

bool montura::agregar()
{
    Sesion* s=Sesion::getSesion();
    int cVitri=0, cAlma=0;
    QSqlQuery query;
    query.prepare("INSERT INTO Producto(codigo,descripcion,precioCompra,precioVenta,precioDescuento,accesorios,stock,observaciones,Estado_idEstado,Marca_idMarca,idColaborador,cantidadVitrina,cantidadAlmacen,tipo) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.bindValue(0,codigo);
    query.bindValue(1,descripcion);
    query.bindValue(2,precioCompra);
    query.bindValue(3,precioVenta);
    query.bindValue(4,precioDescuento);
    query.bindValue(5,accesorio);
    query.bindValue(6,stock);
    query.bindValue(7,observaciones);
    query.bindValue(8,pEstado.getIdEstado());
    query.bindValue(9,pMarca.getIdMarca());
    query.bindValue(10,s->getIdColaborador());
    query.bindValue(11,cVitri);
    query.bindValue(12,cAlma);
    query.bindValue(13,"2");
    if(query.exec())
    {
        query.clear();
        query.prepare("SELECT @@identity AS id;");
        query.exec();
        if(query.size()!=0)
        {
            query.first();
            idProducto=query.value(0).toString();
        }
        if(!registrarKardex(stock.toInt(),0,"Aumento de Stock",1))
        {
            return false;
        }
        query.clear();
        query.prepare("INSERT INTO Montura(Producto_idProducto,Forma_idForma,Color_idColor,Tamanio_idTamanio,Calidad_idCalidad,Genero_idGenero) VALUES(?,?,?,?,?,?)");
        query.bindValue(0,idProducto);
        query.bindValue(1,pForma.getIdForma());
        query.bindValue(2,pColor.getIdColor());
        query.bindValue(3,pTamanio.getIdTamanio());
        query.bindValue(4,pCalidad.getIdCalidad());
        query.bindValue(5,pGenero.getIdgenero());
        if(query.exec())
            return true;
        else
            return false;
    }
    else
        return false;
}
bool montura::actualizar()
{
    Sesion* s=Sesion::getSesion();
    QSqlQuery query;
    query.prepare("UPDATE Producto SET codigo=?,descripcion=?,precioCompra=?,precioVenta=?,precioDescuento=?,accesorios=?,stock=?,observaciones=?,Estado_idEstado=?,Marca_idMarca=?,idColaborador=?  WHERE idProducto=?");
    query.bindValue(0,codigo);
    query.bindValue(1,descripcion);
    query.bindValue(2,precioCompra);
    query.bindValue(3,precioVenta);
    query.bindValue(4,precioDescuento);
    query.bindValue(5,accesorio);
    query.bindValue(6,stock);
    query.bindValue(7,observaciones);
    query.bindValue(8,pEstado.getIdEstado());
    query.bindValue(9,pMarca.getIdMarca());
    query.bindValue(10,s->getIdColaborador());
    query.bindValue(11,idProducto);
    if(query.exec())
    {
        if(stock.toInt() > stock_last.toInt())
        {
            if(!registrarKardex(stock.toInt() - stock_last.toInt(),stock_last.toInt(),"Aumento de Stock",1))
            {
                return false;
            }

        }

        if(stock.toInt() < stock_last.toInt())
        {
            if(!registrarKardex(stock_last.toInt() - stock.toInt(),stock_last.toInt(),"Decremento de Stock",2))
            {
                return false;
            }

        }
        query.clear();
        query.prepare("UPDATE Montura SET Forma_idForma=?,Color_idColor=?,Tamanio_idTamanio=?,Calidad_idCalidad=?,Genero_idGenero=? WHERE Producto_idProducto=?");
        query.bindValue(0,pForma.getIdForma());
        query.bindValue(1,pColor.getIdColor());
        query.bindValue(2,pTamanio.getIdTamanio());
        query.bindValue(3,pCalidad.getIdCalidad());
        query.bindValue(4,pGenero.getIdgenero());
        query.bindValue(5,idProducto);
        if(query.exec())
            return true;
        else
            return false;
    }
    else
        return false;
}
bool montura::eliminar()
{
    QSqlQuery query;
    query.prepare("DELETE FROM Montura WHERE Producto_idProducto=?");
    query.bindValue(0,idProducto);
    if(query.exec())
    {
        query.clear();
        query.prepare("DELETE FROM Producto WHERE idProducto=?");
        query.bindValue(0,idProducto);
        if(query.exec())
            return true;
        else
            return false;
    }
    else
        return false;
}

QSqlQueryModel* montura::mostrar()
{
    QSqlQueryModel* model=new QSqlQueryModel;
    model->setQuery("SELECT codigo as 'Cod',descripcion as 'Descrip.',e.nombre as 'Estado',precioCompra as 'Compra',precioVenta as 'Venta',precioDescuento as 'Desct.',stock,m.nombre as 'Marca',f.nombre as 'Forma',c.nombre as 'Color',t.nombre as 'Tamaño',ca.nombre as 'Calidad',ge.nombre as 'Genero' FROM Producto p,Estado e,Montura mn,Marca m,Forma f,Color c,Tamanio t,Calidad ca, Genero ge WHERE p.Estado_idEstado=e.idEstado AND p.Marca_idMarca=m.idMarca AND p.idProducto=mn.Producto_idProducto AND mn.Forma_idForma=f.idForma AND mn.Color_idColor=c.idColor AND mn.Tamanio_idTamanio=t.idTamanio AND mn.Calidad_idCalidad=ca.idCalidad AND mn.Genero_idGenero=ge.idGenero");    return model;
}
QSqlQueryModel* montura::mostrarcod()
{
    QSqlQueryModel* model=new QSqlQueryModel;
    model->setQuery("SELECT Producto_idProducto as 'Id',codigo as 'Cod',descripcion as 'Descp',e.nombre as 'Estado',precioCompra as 'Compra',precioVenta as 'Venta',precioDescuento as 'Desct',stock,m.nombre as 'Marca',f.nombre as 'Forma',c.nombre as 'Color',t.nombre as 'Tamaño',ca.nombre as 'Calidad',ge.nombre as 'Genero', accesorios as 'Accesorios', observaciones as 'Obs.' FROM Producto p,Estado e,Montura mn,Marca m,Forma f,Color c,Tamanio t,Calidad ca, Genero ge WHERE p.Estado_idEstado=e.idEstado AND p.Marca_idMarca=m.idMarca AND p.idProducto=mn.Producto_idProducto AND mn.Forma_idForma=f.idForma AND mn.Color_idColor=c.idColor AND mn.Tamanio_idTamanio=t.idTamanio AND mn.Calidad_idCalidad=ca.idCalidad AND mn.Genero_idGenero=ge.idGenero");    return model;
}
bool montura::completar()
{
    QSqlQuery query;
    query.prepare("SELECT p.idProducto,p.accesorios,p.observaciones FROM Producto p,Montura m WHERE p.idProducto=m.Producto_idProducto AND p.codigo=? AND p.descripcion=? AND p.precioCompra=? AND p.precioVenta=? AND p.precioDescuento=? AND p.stock=? AND p.Estado_idEstado=? AND p.Marca_idMarca=? AND m.Forma_idForma=? AND m.Color_idColor=? AND m.Tamanio_idTamanio=? AND Calidad_idCalidad=? AND Genero_idGenero=?");//falta seleccionar colaborador
    query.bindValue(0,codigo);
    query.bindValue(1,descripcion);
    query.bindValue(2,precioCompra);
    query.bindValue(3,precioVenta);
    query.bindValue(4,precioDescuento);
    query.bindValue(5,stock);
    query.bindValue(6,pEstado.getIdEstado());
    query.bindValue(7,pMarca.getIdMarca());
    query.bindValue(8,pForma.getIdForma());
    query.bindValue(9,pColor.getIdColor());
    query.bindValue(10,pTamanio.getIdTamanio());
    query.bindValue(11,pCalidad.getIdCalidad());
    query.bindValue(12,pGenero.getIdgenero());
    if(query.exec())
    {
        if(query.size()!=0)
        {
            query.first();
            idProducto=query.value(0).toString();
            accesorio=query.value(1).toString();
            observaciones=query.value(2).toString();
            return true;
        }
        else
            return false;
    }
    else
        return false;
}
