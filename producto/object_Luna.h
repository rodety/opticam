#ifndef OBJECT_LUNA_H
#define OBJECT_LUNA_H

#include "share_include.h"

#define TABLE_NAME_LUNA "Luna"
class object_Luna:public object_general
{

private:
	_QSTR md_o_Producto_idProducto;
	_QSTR md_o_Diametro_idDiametro;
	_QSTR md_o_Calidad_idCalidad;
	_QSTR md_o_TipoLuna_idTipoLuna;
	_QSTR md_o_Tratamiento_idTratamiento;
	_QSTR md_o_valorInicial;
	_QSTR md_o_valorFinal;
	_QSTR md_o_precio;
	_QSTR md_o_cilindro;
    _QSTR md_o_cilindrof;

public:
	object_Luna();
	object_Luna(_QSTR Producto_idProducto, _QSTR Diametro_idDiametro, _QSTR Calidad_idCalidad, _QSTR TipoLuna_idTipoLuna, _QSTR Tratamiento_idTratamiento, _QSTR valorInicial, _QSTR valorFinal, _QSTR precio, _QSTR cilindro);
	object_Luna(_QSTR Diametro_idDiametro, _QSTR Calidad_idCalidad, _QSTR TipoLuna_idTipoLuna, _QSTR Tratamiento_idTratamiento, _QSTR valorInicial, _QSTR valorFinal, _QSTR precio, _QSTR cilindro);

	void mf_set_Producto_idProducto(_QSTR Producto_idProducto);
	void mf_set_Diametro_idDiametro(_QSTR Diametro_idDiametro);
	void mf_set_Calidad_idCalidad(_QSTR Calidad_idCalidad);
	void mf_set_TipoLuna_idTipoLuna(_QSTR TipoLuna_idTipoLuna);
	void mf_set_Tratamiento_idTratamiento(_QSTR Tratamiento_idTratamiento);
	void mf_set_valorInicial(_QSTR valorInicial);
	void mf_set_valorFinal(_QSTR valorFinal);
	void mf_set_precio(_QSTR precio);
	void mf_set_cilindro(_QSTR cilindro);
    void mf_set_cilindrof(_QSTR cilindrof);

	_QSTR mf_get_Producto_idProducto();
	_QSTR mf_get_Diametro_idDiametro();
	_QSTR mf_get_Calidad_idCalidad();
	_QSTR mf_get_TipoLuna_idTipoLuna();
	_QSTR mf_get_Tratamiento_idTratamiento();
	_QSTR mf_get_valorInicial();
	_QSTR mf_get_valorFinal();
	_QSTR mf_get_precio();
	_QSTR mf_get_cilindro();
    _QSTR mf_get_cilindrof();

	bool mf_load(_QSTR pk);
	bool mf_add();
	bool mf_update();
	bool mf_remove();
        QSqlQueryModel* mf_show();
        QSqlQueryModel* mf_show_user();


	~object_Luna();
};
#endif // OBJECT_LUNA_H
	/*
	object_Luna obj_Luna;
	obj_Luna.mf_set_Producto_idProducto();
	obj_Luna.mf_set_Diametro_idDiametro();
	obj_Luna.mf_set_Calidad_idCalidad();
	obj_Luna.mf_set_TipoLuna_idTipoLuna();
	obj_Luna.mf_set_Tratamiento_idTratamiento();
	obj_Luna.mf_set_valorInicial();
	obj_Luna.mf_set_valorFinal();
	obj_Luna.mf_set_precio();
	obj_Luna.mf_set_cilindro();

	obj_Luna.mf_get_Producto_idProducto();
	obj_Luna.mf_get_Diametro_idDiametro();
	obj_Luna.mf_get_Calidad_idCalidad();
	obj_Luna.mf_get_TipoLuna_idTipoLuna();
	obj_Luna.mf_get_Tratamiento_idTratamiento();
	obj_Luna.mf_get_valorInicial();
	obj_Luna.mf_get_valorFinal();
	obj_Luna.mf_get_precio();
	obj_Luna.mf_get_cilindro();
	*/
