#include "object_Luna.h"

object_Luna::object_Luna()
{
	//file Luna
	//function construct_0
	//w!
}

object_Luna::object_Luna(_QSTR Producto_idProducto, _QSTR Diametro_idDiametro, _QSTR Calidad_idCalidad, _QSTR TipoLuna_idTipoLuna, _QSTR Tratamiento_idTratamiento, _QSTR valorInicial, _QSTR valorFinal, _QSTR precio, _QSTR cilindro)
{
	//file Luna
	//function construct_1
	//w!

	md_o_Producto_idProducto = Producto_idProducto;
	md_o_Diametro_idDiametro = Diametro_idDiametro;
	md_o_Calidad_idCalidad = Calidad_idCalidad;
	md_o_TipoLuna_idTipoLuna = TipoLuna_idTipoLuna;
	md_o_Tratamiento_idTratamiento = Tratamiento_idTratamiento;
	md_o_valorInicial = valorInicial;
	md_o_valorFinal = valorFinal;
	md_o_precio = precio;
	md_o_cilindro = cilindro;
}
object_Luna::object_Luna(_QSTR Diametro_idDiametro, _QSTR Calidad_idCalidad, _QSTR TipoLuna_idTipoLuna, _QSTR Tratamiento_idTratamiento, _QSTR valorInicial, _QSTR valorFinal, _QSTR precio, _QSTR cilindro)
{
	//file Luna
	//function construct_2
	//w!

	md_o_Diametro_idDiametro = Diametro_idDiametro;
	md_o_Calidad_idCalidad = Calidad_idCalidad;
	md_o_TipoLuna_idTipoLuna = TipoLuna_idTipoLuna;
	md_o_Tratamiento_idTratamiento = Tratamiento_idTratamiento;
	md_o_valorInicial = valorInicial;
	md_o_valorFinal = valorFinal;
	md_o_precio = precio;
	md_o_cilindro = cilindro;
}
object_Luna::~object_Luna()
{

}


void object_Luna::mf_set_Producto_idProducto(_QSTR Producto_idProducto)
{
	//function mf_set_Producto_idProducto
	//w!

	md_o_Producto_idProducto = Producto_idProducto;
}
void object_Luna::mf_set_Diametro_idDiametro(_QSTR Diametro_idDiametro)
{
	//function mf_set_Diametro_idDiametro
	//w!

	md_o_Diametro_idDiametro = Diametro_idDiametro;
}
void object_Luna::mf_set_Calidad_idCalidad(_QSTR Calidad_idCalidad)
{
	//function mf_set_Calidad_idCalidad
	//w!

	md_o_Calidad_idCalidad = Calidad_idCalidad;
}
void object_Luna::mf_set_TipoLuna_idTipoLuna(_QSTR TipoLuna_idTipoLuna)
{
	//function mf_set_TipoLuna_idTipoLuna
	//w!

	md_o_TipoLuna_idTipoLuna = TipoLuna_idTipoLuna;
}
void object_Luna::mf_set_Tratamiento_idTratamiento(_QSTR Tratamiento_idTratamiento)
{
	//function mf_set_Tratamiento_idTratamiento
	//w!

	md_o_Tratamiento_idTratamiento = Tratamiento_idTratamiento;
}
void object_Luna::mf_set_valorInicial(_QSTR valorInicial)
{
	//function mf_set_valorInicial
	//w!

	md_o_valorInicial = valorInicial;
}
void object_Luna::mf_set_valorFinal(_QSTR valorFinal)
{
	//function mf_set_valorFinal
	//w!

	md_o_valorFinal = valorFinal;
}
void object_Luna::mf_set_precio(_QSTR precio)
{
	//function mf_set_precio
	//w!

	md_o_precio = precio;
}
void object_Luna::mf_set_cilindro(_QSTR cilindro)
{
	//function mf_set_cilindro
	//w!

    md_o_cilindro = cilindro;
}

void object_Luna::mf_set_cilindrof(_QSTR cilindrof)
{
    //function mf_set_cilindro
    //w!

    md_o_cilindrof = cilindrof;
}

_QSTR object_Luna::mf_get_Producto_idProducto()
{
	//function mf_get_Producto_idProducto
	//w!

	return md_o_Producto_idProducto;
}
_QSTR object_Luna::mf_get_Diametro_idDiametro()
{
	//function mf_get_Diametro_idDiametro
	//w!

	return md_o_Diametro_idDiametro;
}
_QSTR object_Luna::mf_get_Calidad_idCalidad()
{
	//function mf_get_Calidad_idCalidad
	//w!

	return md_o_Calidad_idCalidad;
}
_QSTR object_Luna::mf_get_TipoLuna_idTipoLuna()
{
	//function mf_get_TipoLuna_idTipoLuna
	//w!

	return md_o_TipoLuna_idTipoLuna;
}
_QSTR object_Luna::mf_get_Tratamiento_idTratamiento()
{
	//function mf_get_Tratamiento_idTratamiento
	//w!

	return md_o_Tratamiento_idTratamiento;
}
_QSTR object_Luna::mf_get_valorInicial()
{
	//function mf_get_valorInicial
	//w!

	return md_o_valorInicial;
}
_QSTR object_Luna::mf_get_valorFinal()
{
	//function mf_get_valorFinal
	//w!

	return md_o_valorFinal;
}
_QSTR object_Luna::mf_get_precio()
{
	//function mf_get_precio
	//w!

	return md_o_precio;
}
_QSTR object_Luna::mf_get_cilindro()
{
	//function mf_get_cilindro
	//w!

    return md_o_cilindro;
}

_QSTR object_Luna::mf_get_cilindrof()
{
    //function mf_get_cilindro
    //w!

    return md_o_cilindrof;
}

bool object_Luna::mf_load(_QSTR pk)
{
	//function mf_load
	//w!

	QSqlQuery query;

	query.prepare("SELECT * FROM Luna WHERE Producto_idProducto = ?");
	query.bindValue(0,pk);
	query.exec();

	if(query.next())
	{
		md_o_Producto_idProducto = query.value(0).toString();
		md_o_Diametro_idDiametro = query.value(1).toString();
		md_o_Calidad_idCalidad = query.value(2).toString();
		md_o_TipoLuna_idTipoLuna = query.value(3).toString();
		md_o_Tratamiento_idTratamiento = query.value(4).toString();
		md_o_valorInicial = query.value(5).toString();
		md_o_valorFinal = query.value(6).toString();
		md_o_precio = query.value(7).toString();
		md_o_cilindro = query.value(8).toString();
        md_o_cilindrof = query.value(9).toString();

		//state OK
		//w!

		return true;
	}else{
		//state FAILED
		//w!

		return false;
	}
}
bool object_Luna::mf_add()
{
	//function mf_add
	//w!

	QSqlQuery query;

	string str_query = "INSERT INTO Luna(";
	str_query += "Producto_idProducto";
	str_query += ", Diametro_idDiametro";
	str_query += ", Calidad_idCalidad";
	str_query += ", TipoLuna_idTipoLuna";
	str_query += ", Tratamiento_idTratamiento";
	str_query += ", valorInicial";
	str_query += ", valorFinal";
	str_query += ", precio";
	str_query += ", cilindro";
    str_query += ", cilindrof";
	str_query += ") VALUES(";
	str_query += "?";
	str_query += ", ?";
	str_query += ", ?";
	str_query += ", ?";
	str_query += ", ?";
	str_query += ", ?";
	str_query += ", ?";
	str_query += ", ?";
	str_query += ", ?";
    str_query += ", ?";
	str_query += ")";

	query.prepare(QString(str_query.c_str()));
	int integer = 0;
	if (md_o_Producto_idProducto != "")
	{
		query.bindValue(integer++, md_o_Producto_idProducto);
	}
	else
        query.bindValue(integer++, NULL);
	query.bindValue(integer++, md_o_Diametro_idDiametro);
	query.bindValue(integer++, md_o_Calidad_idCalidad);
	query.bindValue(integer++, md_o_TipoLuna_idTipoLuna);
	query.bindValue(integer++, md_o_Tratamiento_idTratamiento);
	query.bindValue(integer++, md_o_valorInicial);
	query.bindValue(integer++, md_o_valorFinal);
	query.bindValue(integer++, md_o_precio);
	query.bindValue(integer++, md_o_cilindro);
    query.bindValue(integer++, md_o_cilindrof);

	if(query.exec())
	{
		//state OK
		//w!

		return true;

	}else{
		//state FAILED
		//w!

		return false;
	}
}

bool object_Luna::mf_update()
{
	//function mf_update
	//w!

	QSqlQuery query;

    query.prepare("UPDATE Luna SET Diametro_idDiametro = ?, CalidadLuna_idCalidadLuna = ?, TipoLuna_idTipoLuna = ?, Tratamiento_idTratamiento = ?, valorInicial = ?, valorFinal = ?, precio = ?, cilindro = ?, cilindrof = ? WHERE Producto_idProducto = ?");
	query.bindValue(0, md_o_Diametro_idDiametro);
	query.bindValue(1, md_o_Calidad_idCalidad);
	query.bindValue(2, md_o_TipoLuna_idTipoLuna);
	query.bindValue(3, md_o_Tratamiento_idTratamiento);
	query.bindValue(4, md_o_valorInicial);
	query.bindValue(5, md_o_valorFinal);
	query.bindValue(6, md_o_precio);
	query.bindValue(7, md_o_cilindro);
    query.bindValue(8, md_o_cilindrof);
    query.bindValue(9, md_o_Producto_idProducto);

	if(query.exec())
	{
		//state OK
		//w!

		return true;

	}else{
		//state FAILED
		//w!

		return false;
	}
}

bool object_Luna::mf_remove()
{
	//function mf_remove
	//w!

	QSqlQuery query;

	query.prepare("DELETE Luna FROM Producto_idProducto = ?");
	query.bindValue(0, md_o_Producto_idProducto);

	if(query.exec())
	{
		//state OK
		//w!

		return true;

	}else{
	//state FAILED
	//w!

		return false;
	}
}

QSqlQueryModel* object_Luna::mf_show()
{

    QSqlQueryModel* model=new QSqlQueryModel;
    QString v_luna;
    QString v_cilindro;
    QString consulta;
//AQUI ESTA LA PENDEJADA, MYSQL ES LO MAXIMO
    if(md_o_Diametro_idDiametro == "0")
        md_o_Diametro_idDiametro = "%";
    if(md_o_Calidad_idCalidad == "0")
        md_o_Calidad_idCalidad = "%";
    if(md_o_TipoLuna_idTipoLuna == "0")
        md_o_TipoLuna_idTipoLuna = "%";
    if(md_o_Tratamiento_idTratamiento == "0")
        md_o_Tratamiento_idTratamiento = "%";

    if(md_o_valorInicial == "0" && md_o_cilindro == "0")
    {
        //consulta todos
        consulta = "SELECT idProducto as 'Codigo',descripcion as 'Descripcion',e.nombre as 'Estado',precioCompra,precioVenta,precioDescuento,stock,d.valor as 'Diametro',c.nombre as 'Calidad',tl.nombre as 'Tipo de Luna',t.nombre as 'Tratamiento',l.valorInicial,l.valorFinal,l.cilindro as 'Cil. Ini',l.cilindrof as 'Cil. Fin',l.precio as 'Precio', observaciones as 'Obs.' FROM Producto p,Estado e,Luna l,Diametro d,CalidadLuna c,TipoLuna tl,Tratamiento t WHERE p.Estado_idEstado=e.idEstado AND p.idProducto=l.Producto_idProducto AND l.Diametro_idDiametro=d.idDiametro AND l.CalidadLuna_idCalidadLuna=c.idCalidadLuna AND l.TipoLuna_idTipoLuna=tl.idTipoLuna AND l.Tratamiento_idTratamiento=t.idTratamiento AND d.idDiametro like '"+md_o_Diametro_idDiametro+"' AND c.idCalidadLuna like '"+md_o_Calidad_idCalidad+"' AND tl.idTipoLuna like '"+md_o_TipoLuna_idTipoLuna+"' AND t.idTratamiento like '"+md_o_Tratamiento_idTratamiento+"'";

        model->setQuery(consulta);
        return model;

    }
    if(md_o_valorInicial == "0")
    {
        v_cilindro = QString::number(fabs(md_o_cilindro.toDouble()));
        consulta = "SELECT idProducto as 'Codigo',descripcion as 'Descripcion',e.nombre as 'Estado',precioCompra,precioVenta,precioDescuento,stock,d.valor as 'Diametro',c.nombre as 'Calidad',tl.nombre as 'Tipo de Luna',t.nombre as 'Tratamiento',l.valorInicial,l.valorFinal,l.cilindro as 'Cil. Ini',l.cilindrof as 'Cil. Fin',l.precio as 'Precio', observaciones as 'Obs.' FROM Producto p,Estado e,Luna l,Diametro d,CalidadLuna c,TipoLuna tl,Tratamiento t WHERE p.Estado_idEstado=e.idEstado AND p.idProducto=l.Producto_idProducto AND l.Diametro_idDiametro=d.idDiametro AND l.CalidadLuna_idCalidadLuna=c.idCalidadLuna AND l.TipoLuna_idTipoLuna=tl.idTipoLuna AND l.Tratamiento_idTratamiento=t.idTratamiento AND d.idDiametro like '"+md_o_Diametro_idDiametro+"' AND c.idCalidadLuna like '"+md_o_Calidad_idCalidad+"' AND tl.idTipoLuna like '"+md_o_TipoLuna_idTipoLuna+"' AND t.idTratamiento like '"+md_o_Tratamiento_idTratamiento+"' AND l.cilindro <= '"+v_cilindro+"'AND l.cilindrof >= '"+v_cilindro+"'";

        model->setQuery(consulta);
        return model;
    }
    if(md_o_cilindro == "0")
    {
        v_luna = QString::number(fabs(md_o_valorInicial.toDouble()+md_o_cilindro.toDouble()));
        consulta = "SELECT idProducto as 'Codigo',descripcion as 'Descripcion',e.nombre as 'Estado',precioCompra,precioVenta,precioDescuento,stock,d.valor as 'Diametro',c.nombre as 'Calidad',tl.nombre as 'Tipo de Luna',t.nombre as 'Tratamiento',l.valorInicial,l.valorFinal,l.cilindro as 'Cil. Ini',l.cilindrof as 'Cil. Fin',l.precio as 'Precio', observaciones as 'Obs.' FROM Producto p,Estado e,Luna l,Diametro d,CalidadLuna c,TipoLuna tl,Tratamiento t WHERE p.Estado_idEstado=e.idEstado AND p.idProducto=l.Producto_idProducto AND l.Diametro_idDiametro=d.idDiametro AND l.CalidadLuna_idCalidadLuna=c.idCalidadLuna AND l.TipoLuna_idTipoLuna=tl.idTipoLuna AND l.Tratamiento_idTratamiento=t.idTratamiento AND d.idDiametro like '"+md_o_Diametro_idDiametro+"' AND c.idCalidadLuna like '"+md_o_Calidad_idCalidad+"' AND tl.idTipoLuna like '"+md_o_TipoLuna_idTipoLuna+"' AND t.idTratamiento like '"+md_o_Tratamiento_idTratamiento+"' AND l.valorInicial <= '"+v_luna+"' AND l.valorFinal >= '"+v_luna+"'";

        model->setQuery(consulta);
        return model;
    }

     v_cilindro = QString::number(fabs(md_o_cilindro.toDouble()));
     v_luna = QString::number(fabs(md_o_valorInicial.toDouble()+md_o_cilindro.toDouble()));

    consulta = "SELECT idProducto as 'Codigo',descripcion as 'Descripcion',e.nombre as 'Estado',precioCompra,precioVenta,precioDescuento,stock,d.valor as 'Diametro',c.nombre as 'Calidad',tl.nombre as 'Tipo de Luna',t.nombre as 'Tratamiento',l.valorInicial,l.valorFinal,l.cilindro as 'Cil. Ini',l.cilindrof as 'Cil. Fin',l.precio as 'Precio', observaciones as 'Obs.' FROM Producto p,Estado e,Luna l,Diametro d,CalidadLuna c,TipoLuna tl,Tratamiento t WHERE p.Estado_idEstado=e.idEstado AND p.idProducto=l.Producto_idProducto AND l.Diametro_idDiametro=d.idDiametro AND l.CalidadLuna_idCalidadLuna=c.idCalidadLuna AND l.TipoLuna_idTipoLuna=tl.idTipoLuna AND l.Tratamiento_idTratamiento=t.idTratamiento AND d.idDiametro like '"+md_o_Diametro_idDiametro+"' AND c.idCalidadLuna like '"+md_o_Calidad_idCalidad+"' AND tl.idTipoLuna like '"+md_o_TipoLuna_idTipoLuna+"' AND t.idTratamiento like '"+md_o_Tratamiento_idTratamiento+"' AND l.valorInicial <= '"+v_luna+"' AND l.valorFinal >= '"+v_luna+"' AND l.cilindro <= '"+v_cilindro+"'AND l.cilindrof >= '"+v_cilindro+"'";

    model->setQuery(consulta);
    return model;
}
QSqlQueryModel* object_Luna::mf_show_user()
{

    QSqlQueryModel* model=new QSqlQueryModel;
//AQUI ESTA LA PENDEJADA, MYSQL ES LO MAXIMO
    if(md_o_Diametro_idDiametro == "0")
        md_o_Diametro_idDiametro = "%";
    if(md_o_Calidad_idCalidad == "0")
        md_o_Calidad_idCalidad = "%";
    if(md_o_TipoLuna_idTipoLuna == "0")
        md_o_TipoLuna_idTipoLuna = "%";
    if(md_o_Tratamiento_idTratamiento == "0")
        md_o_Tratamiento_idTratamiento = "%";
    if(md_o_valorInicial == "0")
        md_o_valorInicial = "%";
    if(md_o_valorFinal == "0")
        md_o_valorFinal = "%";
    if(md_o_cilindro == "0")
        md_o_cilindro = "%";
    if(md_o_cilindrof == "0")
        md_o_cilindrof = "%";


    model->setQuery("SELECT idProducto as 'Codigo',descripcion as 'Descripcion',e.nombre as 'Estado',precioCompra,precioVenta,precioDescuento,stock,d.valor as 'Diametro',c.nombre as 'Calidad',tl.nombre as 'Tipo de Luna',t.nombre as 'Tratamiento',l.valorInicial,l.valorFinal,l.cilindro as 'Cilindro',l.precio as 'Precio' FROM Producto p,Estado e,Luna l,Diametro d,CalidadLuna c,TipoLuna tl,Tratamiento t WHERE p.Estado_idEstado=e.idEstado AND p.idProducto=l.Producto_idProducto AND l.Diametro_idDiametro=d.idDiametro AND l.CalidadLuna_idCalidadLuna=c.idCalidadLuna AND l.TipoLuna_idTipoLuna=tl.idTipoLuna AND l.Tratamiento_idTratamiento=t.idTratamiento AND d.idDiametro like '"+md_o_Diametro_idDiametro+"' AND c.idCalidadLuna like '"+md_o_Calidad_idCalidad+"' AND tl.idTipoLuna like '"+md_o_TipoLuna_idTipoLuna+"' AND t.idTratamiento like '"+md_o_Tratamiento_idTratamiento+"' AND l.valorInicial like '"+md_o_valorInicial+"' AND l.valorFinal like '"+md_o_valorFinal+"' AND l.cilindro like '"+md_o_cilindro+"' AND l.cilindrof like '"+md_o_cilindrof+"'");
    return model;
}
