#include "uiventas.h"
#include "ui_uiventas.h"
#include <vitrina/empresa.h>
#include <cliente/object_Cliente.h>

uiventas::uiventas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::uiventas)
{
    ui->setupUi(this);
    configuracionesIniciaciales();
    configTableVentas();
    configuracionReportes();
}

uiventas::~uiventas()
{
    //delete validator;
    delete ui;
}

void uiventas::configuracionesIniciaciales()
{
    count_row=0;
    total_venta = 0;
    sub_total = 0;
    monto_igv = 0;
    igv = Sesion::getIgv();


    ///Validadores
    QValidator * validator = new QDoubleValidator(0.00,99999.99,3,this);
    ui->lineEdit_efectivo->setValidator(validator);
    ui->lineEdit_tarjeta->setValidator(validator);


    //Fechas

    ui->dateTimeEdit_fecha_preventa->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_entrega->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_entrega->setMinimumDateTime(QDateTime::currentDateTime());
    //Consiguiendo Ubicacion

    if(!tienda_actual.mf_load(QString::number(Sesion::getUbicacion().second)))
    {
        QMessageBox *msgBox =new QMessageBox;
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setWindowIcon(QIcon(":/Icons/abiword.png"));
        msgBox->setText("Error en la Configuracion inicial de Tienda");
        msgBox->show();
        return;
    }

    //Nombre de Tienda
    ui->label_nombre_Tienda->setText(tienda_actual.mf_get_nombre());

    if(!configuracion.mf_load(QString::number(Sesion::getUbicacion().second)))
    {
        QMessageBox *msgBox =new QMessageBox;
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setWindowIcon(QIcon(":/Icons/abiword.png"));
        msgBox->setText("Error en la Configuracion relacionada a Tienda");
        msgBox->show();
        return;
    }

    //Estamos asumiendo que el siguiente de la serie sera el siguiente del documento
    if(ui->radioButton_Boleta->isChecked())
        ui->label_numero_documento->setText(QString::number(configuracion.mf_get_serieBoleta().toInt()+1));
    if(ui->radioButton_Factura->isChecked())
        ui->label_numero_documento->setText(QString::number(configuracion.mf_get_serieFactura().toInt()+1));
    if(ui->radioButton_cotizacion->isChecked())
        ui->label_numero_documento->setText(QString::number(configuracion.mf_get_serieCotizacion().toInt()+1));

    //Datos de colaborador
    object_Colaborador colaborador;
    if(!colaborador.mf_load(QString::number(Sesion::getIdColaborador())))
    {
        QMessageBox *msgBox =new QMessageBox;
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setWindowIcon(QIcon(":/Icons/abiword.png"));
        msgBox->setText("Error en la Configuracion de personal colaborador , ");
        msgBox->show();
        return;
    }
    ui->lineEdit_usuario->setText(colaborador.mf_get_nombres()+ " "+ colaborador.mf_get_primer_apellido());

    //DESABILITANDO BOTON DE TARJETA
    ui->pushButton_anular->setEnabled(false);
    ui->lineEdit_total->setEnabled(false);
    ui->lineEdit_total_cancelado->setEnabled(false);
    ui->lineEdit_usuario->setEnabled(false);
    ui->lineEdit_restante->setEnabled(false);
    ui->radioButton_Boleta->click();
    comportamiento = true;
    //Desactivando calcular reporte
    flag_reporte = false;
    ui->pushButton_registro_tarjeta->setEnabled(false);

    ui->pushButton_imprimir->hide();

}
void uiventas::on_pushButton_buscarCliente_clicked()
{
    ui_cliente* form=new ui_cliente;
    form->setComportamiento(1);
    form->setWindowTitle("Cliente");
    form->show();
    connect(form,SIGNAL(sentCliente(QString,QString,QString,QString)),this,SLOT(recojeCliente(QString,QString,QString,QString)));
    connect(form,SIGNAL(sentCliente(QString,QString,QString,QString)),form,SLOT(close()));
}
void uiventas::recojeCliente(QString id,QString razon,QString ruc,QString direccion)
{
    idCliente = id;
    ui->lineEdit_razonSocial->setText(razon);
    ui->lineEdit_ruc->setText(ruc);
    ui->lineEdit_direccion->setText(direccion);
}


void uiventas::recojeProducto(QString codigo,QString descripcion,QString precioVenta,QString precioDescuento,int cant,QString pos,int tipo,int stock_producto,QString precioCompra)
{
    float subtotal_item = cant*(precioVenta.toDouble()-precioDescuento.toDouble());    
    total_venta += subtotal_item;
    monto_igv = total_venta -(total_venta / ((igv/100)+1));
    ui->lineEdit_total->setText(QString::number(total_venta));
    double total_adelanto = ui->lineEdit_tarjeta->text().toDouble() +  ui->lineEdit_efectivo->text().toDouble();
    calculaprecio(total_adelanto);

    seleccionados_model->setItem(count_row,0,new QStandardItem(codigo));
    seleccionados_model->setItem(count_row,1,new QStandardItem(descripcion));
    seleccionados_model->setItem(count_row,2,new QStandardItem(precioVenta));
    seleccionados_model->setItem(count_row,3,new QStandardItem(precioDescuento));
    seleccionados_model->setItem(count_row,4,new QStandardItem(QString::number(cant)));
    seleccionados_model->setItem(count_row,5,new QStandardItem("Si"));
    seleccionados_model->setItem(count_row,6,new QStandardItem(QString::number(subtotal_item)));
    seleccionados_model->setItem(count_row,7,new QStandardItem(pos));
    seleccionados_model->setItem(count_row,8,new QStandardItem(QString::number(tipo)));
    seleccionados_model->setItem(count_row,9,new QStandardItem(QString::number(stock_producto)));
    seleccionados_model->setItem(count_row,10,new QStandardItem(precioCompra));
    seleccionados_model->setItem(count_row,11,new QStandardItem(QString::number(fuente)));


/*
    for ( int i = 0; i < seleccionados_model->rowCount(); ++i )
      {
        ui->tableView_Productos->openPersistentEditor(seleccionados_model->index(i, 5) );
      }
*/
    seleccionados_model->setHeaderData(0,Qt::Horizontal,"Codigo");
    seleccionados_model->setHeaderData(1,Qt::Horizontal,"Descripcion");
    seleccionados_model->setHeaderData(2,Qt::Horizontal,"P. Venta");
    seleccionados_model->setHeaderData(3,Qt::Horizontal,"P. Descuento");
    seleccionados_model->setHeaderData(4,Qt::Horizontal,"Cantidad");
    seleccionados_model->setHeaderData(5,Qt::Horizontal,"Entregado");
    seleccionados_model->setHeaderData(6,Qt::Horizontal,"Sub Total");
    count_row++;
    //OCULTANDO ALGUNAS COLUMNAS
    ui->tableView_Productos->setColumnWidth(0,0);//idProducto
    ui->tableView_Productos->setColumnWidth(1,450);//Descripcion Resumida del Producto
    ui->tableView_Productos->setColumnWidth(7,0);//idUbicacicion o IdContenedor
    ui->tableView_Productos->setColumnWidth(8,0);//Tipo de Producto
    ui->tableView_Productos->setColumnWidth(9,0); //Stock Producto
    ui->tableView_Productos->setColumnWidth(10,0); //Precio Compra
    ui->tableView_Productos->setColumnWidth(11,0); //Fuente de Producto

}


void uiventas::on_lineEdit_efectivo_textChanged(const QString &arg1)
{
    //COMPORTAMIENTO 1 AGREGAR NUEVO 0 EDITAR
    double total_adelanto = 0;
    if(comportamiento)
    {
        double adelantoEfectivo = arg1.toDouble();
        total_adelanto = adelantoEfectivo + ui->label_tarjeta->text().toDouble();

        if(total_adelanto >= total_venta) //SUPERADO
        {
            if(adelantoEfectivo >= total_venta) //SOLO EL ADELANTO
            {
                ui->label_efectivo->setText(QString::number(total_venta));
                ui->lineEdit_tarjeta->clear();
                ui->label_tarjeta->clear();
                ui->lineEdit_tarjeta->setEnabled(false);
                calculaprecio(adelantoEfectivo);
            }
            else
            {
                if((total_adelanto - total_venta) > 0)//SI EXISTE VUELTO
                {
                    ui->label_efectivo->setText(QString::number(adelantoEfectivo-(total_adelanto - total_venta))); //MENOS EL VUELTO
                    ui->lineEdit_tarjeta->setEnabled(true);
                    calculaprecio(total_adelanto);
                }
                else
                {
                    ui->label_efectivo->setText(QString::number(adelantoEfectivo)); //MENOS EL VUELTO
                    ui->lineEdit_tarjeta->setEnabled(true);
                    calculaprecio(total_adelanto);
                }
            }

        }
        else
        {
            ui->label_efectivo->setText(QString::number(adelantoEfectivo));
            ui->lineEdit_tarjeta->setEnabled(true);
            calculaprecio(total_adelanto);
        }

    }
    else
    {
        double adelantoEfectivo = arg1.toDouble();
        double adelantoOtro = ui->lineEdit_tarjeta->text().toDouble();
        double adelantoVentaAnterior = efectivo_pasado + tarjeta_pasado + adelantoOtro;
        total_adelanto = adelantoEfectivo + adelantoVentaAnterior;

        if(total_adelanto >= (total_venta - adelantoVentaAnterior)) //SUPERADO
        {
            if(adelantoEfectivo > (total_venta - adelantoVentaAnterior)) //SOLO EL ADELANTO
            {
                ui->label_efectivo->setText(QString::number(total_venta-efectivo_pasado));
                ui->lineEdit_tarjeta->clear();
                ui->lineEdit_tarjeta->setEnabled(false);
                calculaprecio(adelantoEfectivo + adelantoVentaAnterior);
            }
            else
            {
                if((total_adelanto - total_venta) > 0)//SI EXISTE VUELTO
                {
                    ui->label_efectivo->setText(QString::number(adelantoEfectivo-(total_adelanto - total_venta))); //MENOS EL VUELTO
                    ui->lineEdit_tarjeta->setEnabled(true);
                    calculaprecio(total_adelanto);
                }
                else
                {
                    ui->label_efectivo->setText(QString::number(adelantoEfectivo+tarjeta_pasado)); //MENOS EL VUELTO
                    ui->lineEdit_tarjeta->setEnabled(true);
                    calculaprecio(total_adelanto);
                }
            }

        }
        else
        {
            ui->label_efectivo->setText(QString::number(adelantoEfectivo + efectivo_pasado));
            ui->lineEdit_tarjeta->setEnabled(true);
            calculaprecio(total_adelanto);
        }
    }

}

void uiventas::on_lineEdit_tarjeta_textChanged(const QString &arg1)
{
    double total_adelanto = 0;
    if(comportamiento)
    {
        double adelantoTarjeta = arg1.toDouble();
        total_adelanto = adelantoTarjeta + ui->label_efectivo->text().toDouble();

        if(total_adelanto >= total_venta)
        {
            if(adelantoTarjeta >= total_venta) // SOLO EL ADELANTO ES MAYOR
            {
                ui->label_tarjeta->setText(QString::number(total_venta));
                ui->lineEdit_efectivo->clear();
                ui->label_efectivo->clear();
                ui->lineEdit_efectivo->setEnabled(false);
                calculaprecio(adelantoTarjeta);
            }
            else
            {
                if((total_adelanto - total_venta) > 0)//SI EXISTE VUELTO
                {
                    ui->label_tarjeta->setText(QString::number(adelantoTarjeta-(total_adelanto - total_venta))); //MENOS EL VUELTO
                    ui->lineEdit_efectivo->setEnabled(true);
                    calculaprecio(total_adelanto);
                }
                else
                {
                    ui->label_tarjeta->setText(QString::number(adelantoTarjeta)); //MENOS EL VUELTO
                    ui->lineEdit_efectivo->setEnabled(true);
                    calculaprecio(total_adelanto);
                }

            }
        }
        else
        {
            ui->label_tarjeta->setText(QString::number(adelantoTarjeta));
            ui->lineEdit_efectivo->setEnabled(true);
            calculaprecio(total_adelanto);
        }

    }
    else
    {

        double adelantoEfectivo = arg1.toDouble();
        double adelantoOtro = ui->lineEdit_efectivo->text().toDouble();
        double adelantoVentaAnterior = efectivo_pasado + tarjeta_pasado;
        total_adelanto = adelantoEfectivo + adelantoVentaAnterior + adelantoOtro;

        if(total_adelanto >= (total_venta - adelantoVentaAnterior)) //SUPERADO
        {
            if(adelantoEfectivo >= (total_venta - adelantoVentaAnterior)) //SOLO EL ADELANTO
            {
                ui->label_tarjeta->setText(QString::number(total_venta-efectivo_pasado));
                ui->lineEdit_efectivo->clear();
                ui->lineEdit_efectivo->setEnabled(false);
                calculaprecio(adelantoEfectivo + adelantoVentaAnterior);
            }
            else
            {
                if((total_adelanto - total_venta) > 0)//SI EXISTE VUELTO
                {
                    ui->label_tarjeta->setText(QString::number(adelantoEfectivo-(total_adelanto - total_venta))); //MENOS EL VUELTO
                    ui->lineEdit_tarjeta->setEnabled(true);
                    calculaprecio(total_adelanto);
                }
                else
                {
                    ui->label_tarjeta->setText(QString::number(adelantoEfectivo+tarjeta_pasado)); //MENOS EL VUELTO
                    ui->lineEdit_efectivo->setEnabled(true);
                    calculaprecio(total_adelanto);
                }
            }

        }
        else
        {
            ui->label_tarjeta->setText(QString::number(adelantoEfectivo + efectivo_pasado));
            ui->lineEdit_efectivo->setEnabled(true);
            calculaprecio(total_adelanto);
        }

    }
}

void uiventas::calculaprecio(double total_adelanto)
{
    if(total_venta == total_adelanto)
    {
        ui->label_cambiante->setText("Cancelado");
        ui->lineEdit_restante->setText(":-)");
    }
    if(total_venta < total_adelanto)
    {
        ui->label_cambiante->setText("Vuelto :-)");
        ui->lineEdit_restante->setText(QString::number(total_adelanto - total_venta));
    }
    if(total_venta > total_adelanto)
    {
        ui->label_cambiante->setText("Debe");
        ui->lineEdit_restante->setText(QString::number(total_venta - total_adelanto));
    }
    ui->lineEdit_total_cancelado->setText(QString::number(total_adelanto));
}

void uiventas::on_radioButton_Boleta_clicked()
{
    numeroDocumento = QString::number(configuracion.mf_get_serieBoleta().toInt()+1);
    ui->label_numero_documento->setText(numeroDocumento);

}

void uiventas::on_radioButton_Factura_clicked()
{
    numeroDocumento = QString::number(configuracion.mf_get_serieFactura().toInt()+1);
    ui->label_numero_documento->setText(numeroDocumento);
}

void uiventas::on_radioButton_cotizacion_clicked()
{
    numeroDocumento = QString::number(configuracion.mf_get_serieCotizacion().toInt()+1);
    ui->label_numero_documento->setText(numeroDocumento);
}

void uiventas::on_pushButton_guardar_clicked()
{
    //Validaciones Basicas
    if(ui->lineEdit_razonSocial->text().size() == 0)
    {   QMessageBox msgBox;
        msgBox.setText("Inserte Razon Social");
        msgBox.exec();
        return;
    }
    if(count_row == 0)
    {   QMessageBox msgBox;
        msgBox.setText("Inserte un producto");
        msgBox.exec();
        return;
    }



    QString fechaPreventa,fechaCancelacion,serieDocumento,formaPago,codigoTransaccion,
            plazo,montoTotal,fechaEntrega,montoAdelanto,
            idTienda,IdColaborador,tipoDocumento;

    object_Venta venta;

    //TIPO DE DOCUMENTO   FACTURA,1 BOLETA 2, COTIZACION 3

    if(ui->radioButton_Factura->isChecked())
        tipoDocumento = "1";
    if(ui->radioButton_Boleta->isChecked())
        tipoDocumento = "2";
    if(ui->radioButton_cotizacion->isChecked())
        tipoDocumento = "3";
    //if(ui->radioButton_cotizacion->isChecked())
        //Falta Ejecutar la Cotizacio

    bool entregado = true;
    bool anulado = false; //LUEGO SE PODRA ANULAR


    fechaPreventa = ui->dateTimeEdit_fecha_preventa->dateTime().toString(Qt::ISODate);
    fechaCancelacion = ui->dateTimeEdit_entrega->dateTime().toString(Qt::ISODate);    
    serieDocumento = QString::number(Sesion::getUbicacion().second);

    //Forma de pago Contado 0, Credito 1, Ambos 2,  ninguno 3
    if(ui->label_efectivo->text().size() > 0 && ui->label_tarjeta->text().size() > 0)
        formaPago = "2";
    else
    {
        if(ui->label_tarjeta->text().size() > 0)
            formaPago = "1";
        else
        {
            if(ui->label_efectivo->text().size() > 0)
                formaPago = "0";
            else
                formaPago = "3";
        }
    }

    //SETEANDO PENDIENTE DE REGISTRO TARJETA
    if(formaPago == "1" || formaPago == "2")
        venta.mf_set_pendiente("0");
    else
        venta.mf_set_pendiente("1");

    //Plazo Un plazo = 1 Dos Plazo = 2
    if(ui->lineEdit_total_cancelado->text().toDouble() >= ui->lineEdit_total->text().toDouble() )
        plazo = "1";
    else
        plazo = "2";

    montoTotal = ui->lineEdit_total->text();
    fechaEntrega = ui->dateTimeEdit_entrega->dateTime().toString(Qt::ISODate);

    if(ui->lineEdit_total_cancelado->text().toDouble() >= ui->lineEdit_total->text().toDouble() )
        montoAdelanto = ui->lineEdit_total->text();
    else
        montoAdelanto = ui->lineEdit_total_cancelado->text();



    for(int i = 0; i<count_row;i++)
    {
        if(seleccionados_model->item(i,5)->text().toStdString() == "No")
            entregado = false;
    }


    //OBTENIENDO ID TIENDA ID COLABORADOR
    idTienda = QString::number(Sesion::getUbicacion().second);
    IdColaborador = QString::number(Sesion::getIdColaborador());
    //CREANDO OBJETO VENTA

    venta.mf_set_fechaPreVenta(fechaPreventa);
    venta.mf_set_fechaCancelacion(fechaCancelacion);
    venta.mf_set_serieDocumento(serieDocumento);
    venta.mf_set_Tienda_idTienda(idTienda);
    venta.mf_set_formaPago(formaPago);
    venta.mf_set_codigoTransaccion(codigoTransaccion);
    venta.mf_set_plazo(plazo);
    venta.mf_set_montoTotal(montoTotal);
    venta.mf_set_fechaEntrega(fechaEntrega);
    //MONTO ADELANTO EN EFECTIVO
    venta.mf_set_montoAdelanto(ui->label_efectivo->text());
    venta.mf_set_numeroDocumento(numeroDocumento);

    //TIPOS DE ESTADO SON: ENTREGADO, ANULADO,
    if(entregado)
        venta.mf_set_Entregado("1");
    else
        venta.mf_set_Entregado("0");
    if(anulado)
        venta.mf_set_Anulado("1");
    else
        venta.mf_set_Anulado("0");

    venta.mf_set_Cliente_idCliente(idCliente);
    venta.mf_set_Colaborador_idColaborador(IdColaborador);
    venta.mf_set_tipoDocumento(tipoDocumento);

    //COMPORTAMIENTO TRUE = NUEVA VENTA, FALSE ACTUALIZAR VENTA
    if(comportamiento)
    {
        if(!validar_montoEntregado())
        {
            QMessageBox msgBox;
            msgBox.setText("No puede guardar la venta, si el monto entregado es cero");
            msgBox.exec();
            return;
        }
        if(!venta.mf_add())
        {
            QMessageBox msgBox;
            msgBox.setText("Fallo al insertar nueva transaccion, intentelo nuevamente o llame al servicio tecnico.");
            msgBox.exec();
            return;
        }

        //CREANDO OBJETO ENTREGA PRODUCTO E INSERTANDO CADA ENTREGA
        for(int i = 0; i<count_row;i++)
        {
            object_EntregaProducto entregado;
            entregado.mf_set_Cliente_idCliente(idCliente);
            entregado.mf_set_Colaborador_idColaborador(IdColaborador);
            entregado.mf_set_Producto_idProducto(seleccionados_model->item(i,0)->text());
            entregado.mf_set_Venta_idVenta(venta.mf_get_lastIdVenta());
            if(seleccionados_model->item(i,5)->text() == "No")
                entregado.mf_set_estado("0");
            else
                entregado.mf_set_estado("1");
            entregado.mf_set_fecha(fechaPreventa);
            if(!entregado.mf_add())
            {
                QMessageBox msgBox;
                msgBox.setText("Fallo al insertar item de venta en tabla entregado: "+seleccionados_model->item(i,0)->text());
                msgBox.exec();
                return;
            }
        }

        //CREANDO OBJETO VENTA - PRODUCTO REGISTRANDO CADA PRODUCTO VENDIDO

        for(int i = 0; i<count_row;i++)
        {
            object_Venta_has_Producto vendido;
            vendido.mf_set_Venta_idVenta(venta.mf_get_lastIdVenta());
            vendido.mf_set_Producto_idProducto(seleccionados_model->item(i,0)->text());
            vendido.mf_set_cantidad(seleccionados_model->item(i,4)->text());
            vendido.mf_set_precio(seleccionados_model->item(i,2)->text());
            vendido.mf_set_descuento(seleccionados_model->item(i,3)->text());
            vendido.mf_set_descripcion(seleccionados_model->item(i,1)->text());

            if(!vendido.mf_add())
            {
                QMessageBox msgBox;
                msgBox.setText("Fallo al insertar item de venta en tabla Productos Vendidos: "+seleccionados_model->item(i,0)->text());
                msgBox.exec();
                return;
            }
        }

        //AGREGANDO TARJETA SI LA UBIERE
        object_Venta_has_Tarjeta v_tarjeta;
        if(ui->label_tarjeta->text().size() > 0)
        {

              v_tarjeta.mf_set_monto(ui->label_tarjeta->text());
              if(!v_tarjeta.mf_add())
              {
                    QMessageBox msgBox;
                    msgBox.setText("Fallo al guardar tarjeta de credito");
                    msgBox.exec();
                    return;
               }
         }

        //AGREGANDO VENTA A PLAZO
        object_VentaPlazo v_plazo;
        v_plazo.mf_set_Venta_idVenta(venta.mf_get_lastIdVenta());
        v_plazo.mf_set_Venta_has_Tarjeta_idVenta_has_Tarjeta(v_tarjeta.mf_get_lastIdVenta_has_Tarjeta());
        v_plazo.mf_set_Colaborador_idColaborador(IdColaborador);
        v_plazo.mf_set_Tienda_idTienda(idTienda);
        v_plazo.mf_set_montoEfectivo(ui->label_efectivo->text());
        v_plazo.mf_set_montoTarjeta(ui->label_tarjeta->text());
        v_plazo.mf_set_fecha(fechaPreventa);
        v_plazo.mf_set_numeroDocumento(numeroDocumento);
        v_plazo.mf_set_serieDocumento(serieDocumento);
        if(!v_plazo.mf_add())
        {
            QMessageBox msgBox;
            msgBox.setText("Fallo al agregar venta a plazo");
            msgBox.exec();
            return;
        }


        if(!actualizar_configuracion())
        {
            QMessageBox msgBox;
            msgBox.setText("Fallo al actualizar numero de documento");
            msgBox.exec();
            return;
        }

        //PRIMERO VERIFICAMOS QUE EL STOCK
        //ACTUALIZANDO CANTIDADES EN VITRINA Y ALMACEN Y EN PRODUCTOS DE CADA PRODUCTO

        //EL TIPO 1 SIGNIFICA QUE ES UNA LUNA
        //EL TIPO 2 SIGNIFICA QUE ES UNA MONTURA
        //EL TIPO 3 SIGNIFICA QUE ES UNA LENTE CONTACTO
        //EL TIPO 4 SIGNIFICA QUE ES UNA OTROS
        //EL TIPO 5 SIGNIFICA QUE ES UNA TRABAJOS EXTRAS
        //EL TIPO 6 SIGNIFICA QUE ES UNA ACCESORIOS

        /**
         *FUENTE 0 SELECCIONE PRODUCTO
         *FUENTE 1 PRODUCTOS
         *FUENTE 2 VITRINA
         *FUENTE 3 ALMACEN
         **/


        for(int i = 0; i<count_row;i++)
        {
            int tipo = seleccionados_model->item(i,8)->text().toInt();
            int fuente_producto = seleccionados_model->item(i,11)->text().toInt();

            if(tipo == 1 || tipo == 2 || tipo == 3 || tipo == 4 || tipo == 6)
            {
                QString id = seleccionados_model->item(i,0)->text();
                int cant = seleccionados_model->item(i,4)->text().toInt();
                int stock_producto = seleccionados_model->item(i,9)->text().toInt();
                QString precioCompra = seleccionados_model->item(i,10)->text();
                QString descripcion = "Venta con ";
                if(ui->radioButton_Boleta->isChecked())
                     descripcion += "boleta: "+ui->label_numero_documento->text();
                if(ui->radioButton_Factura->isChecked())
                    descripcion += "factura: "+ui->label_numero_documento->text();
                if(ui->radioButton_cotizacion->isChecked())
                    descripcion += "cotizacion: "+ui->label_numero_documento->text();

                producto myProducto;
                myProducto.setIdProducto(id);
                myProducto.setPrecioCompra(precioCompra);


                if(!myProducto.registrarKardex(cant,stock_producto,descripcion,2))
                {
                    QMessageBox msgBox;
                    msgBox.setText("Fallo al registrar en kardex");
                    msgBox.exec();
                    return;
                }

                if(!myProducto.updateStockVenta(cant))
                {
                    QMessageBox msgBox;
                    msgBox.setText("Fallo al actualizar cantidad en producto");
                    msgBox.exec();
                    return;
                }

                if(fuente_producto == 2)
                {
                    object_Producto_has_Vitrina producto_vitrina;
                    QString idVitrina_Producto_old = seleccionados_model->item(i,7)->text();
                    producto_vitrina.mf_set_idProducto_has_Vitrina(idVitrina_Producto_old);
                    producto_vitrina.mf_set_estado("V");
                    QString descripcion = "Cod. ";
                    descripcion += seleccionados_model->item(i,7)->text();
                    descripcion += " Vendido con ";
                    if(ui->radioButton_Boleta->isChecked())
                         descripcion += "boleta: "+ui->label_numero_documento->text();
                    if(ui->radioButton_Factura->isChecked())
                        descripcion += "factura: "+ui->label_numero_documento->text();
                    if(ui->radioButton_cotizacion->isChecked())
                        descripcion += "cotizacion: "+ui->label_numero_documento->text();

                    producto_vitrina.mf_set_comentario(descripcion);
                    if(!producto_vitrina.mf_update_estado())
                    {
                        QMessageBox msgBox;
                        msgBox.setText("No se logro actualizar el estado del producto");
                        msgBox.exec();
                        return;
                     }

                    //DISMINUIMOS LA CANTIDAD DE VITRINA EN PRODUCTOS

                    if(!myProducto.retryFromVitrina())
                    {
                        QMessageBox msgBox;
                        msgBox.setText("Fallo al actualizar cantidad vitrina en producto");
                        msgBox.exec();
                        return;
                    }
                }

                if(fuente_producto == 3)
                {
                    //DISMINUIMOS LA CANTIDAD DE ALMACEN EN PRODUCTOS

                    QString idContenedor = seleccionados_model->item(i,7)->text();
                    object_Contenedor_has_Producto myContenedor;
                    myContenedor.mf_set_Producto_idProducto(id);
                    myContenedor.mf_set_Contenedor_idContenedor(idContenedor);


                    ;
                    if(!myContenedor.mf_remove_cant(cant))
                    {
                        QMessageBox msgBox;
                        msgBox.setText("Fallo al actualizar cantidad producto en contenedor");
                        msgBox.exec();
                        return;
                    }

                    if(!myProducto.retryFromAlmacen(cant))
                    {
                        QMessageBox msgBox;
                        msgBox.setText("Fallo al actualizar cantidad almacen en producto");
                        msgBox.exec();
                        return;
                    }

                }

            }
        }

        //hasta aqui el Kardex



        QMessageBox msgBox;
        msgBox.setText("Venta Guarda.");
        msgBox.setInformativeText("Desea imprimir el comprobante de pago?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        switch (ret) {
          case QMessageBox::Yes:
              // Yes was clicked
                imprimir();
              break;
          case QMessageBox::No:
              // Don't Save was clicked
              break;
          default:
              // should never be reached
              break;
        }

        limpiarInterfazVenta();
        ui->dateTimeEdit_Hasta->setDateTime(QDateTime::currentDateTime());        
    }
     // ACTUALIZAR VENTA
    else
    {        
        venta.mf_set_idVenta(idVentaActual);

        if(!venta.mf_update())
        {
            QMessageBox msgBox;
            msgBox.setText("Fallo al actualizar transaccion, intentelo nuevamente o llame al servicio tecnico.");
            msgBox.exec();
            return;
        }

        //MODIFICANDO LA ENTREGA PRODUCTO
        object_EntregaProducto v_entregado;
        vector<_QSTR> salida = v_entregado.mf_listPk(venta.mf_get_idVenta());
        for(int i=0;i<salida.size();i++)
        {
            if(v_entregado.mf_load(salida[i]))
            {                
                if(seleccionados_model->item(i,5)->text() == "No")
                    v_entregado.mf_set_estado("0");
                else
                    v_entregado.mf_set_estado("1");
                v_entregado.mf_set_fecha(fechaPreventa);
                if(!v_entregado.mf_update())
                {
                    QMessageBox msgBox;
                    msgBox.setText("Fallo al actualizar item de venta en tabla entregado: "+seleccionados_model->item(i,0)->text());
                    msgBox.exec();
                    return;
                }
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setText("No se logro carcar datos");
                msgBox.exec();
                return;
            }

        }
        if(validar_montoEntregado())
        {
            //AGREGANDO TARJETA SI LA UBIERE
            object_Venta_has_Tarjeta v_tarjeta;
            if(ui->label_tarjeta->text().size() >= 0)
            {
                  v_tarjeta.mf_set_monto(ui->lineEdit_tarjeta->text());
                  if(!v_tarjeta.mf_add())
                  {
                        QMessageBox msgBox;
                        msgBox.setText("Fallo al guardar tarjeta de cretido");
                        msgBox.exec();
                        return;
                   }
             }

            //AGREGANDO VENTA A PLAZO
            object_VentaPlazo v_plazo;
            v_plazo.mf_set_Venta_idVenta(idVentaActual);
            v_plazo.mf_set_Venta_has_Tarjeta_idVenta_has_Tarjeta(v_tarjeta.mf_get_lastIdVenta_has_Tarjeta());
            v_plazo.mf_set_Colaborador_idColaborador(IdColaborador);
            v_plazo.mf_set_Tienda_idTienda(idTienda);
            v_plazo.mf_set_montoEfectivo(ui->lineEdit_efectivo->text());
            v_plazo.mf_set_montoTarjeta(ui->lineEdit_tarjeta->text());
            v_plazo.mf_set_fecha(fechaPreventa);
            v_plazo.mf_set_numeroDocumento(numeroDocumento);
            v_plazo.mf_set_serieDocumento(serieDocumento);
            if(!v_plazo.mf_add())
            {
                QMessageBox msgBox;
                msgBox.setText("Fallo al agregar venta a plazo");
                msgBox.exec();
                return;
            }

            //ACTUALIZANDO LA SERIE GENERAL


            if(!actualizar_configuracion())
            {
                QMessageBox msgBox;
                msgBox.setText("Fallo al actualizar numero de documento");
                msgBox.exec();
                return;
            }
        }
        QMessageBox msgBox;
        msgBox.setText("Venta Actualizada.");
        msgBox.exec();
        ui->dateTimeEdit_Hasta->setDateTime(QDateTime::currentDateTime());        
        limpiarInterfazVenta();

    }

}

void uiventas::configTableVentas()
{
    seleccionados_model = new QStandardItemModel;
    ComboBoxDelegate* delegate = new ComboBoxDelegate;
    ui->tableView_Productos->setModel(seleccionados_model);

    //ui->tableView_Productos->setItemDelegateForColumn(5, delegate);
}

bool uiventas::actualizar_configuracion()
{
    //ACTUALIZANDO LA SERIE GENERAL

    if(ui->radioButton_Boleta->isChecked())
        configuracion.mf_set_serieBoleta(QString::number(configuracion.mf_get_serieBoleta().toInt()+1));
    if(ui->radioButton_Factura->isChecked())
        configuracion.mf_set_serieFactura(QString::number(configuracion.mf_get_serieFactura().toInt()+1));
    if(ui->radioButton_cotizacion->isChecked())
        configuracion.mf_set_serieCotizacion(QString::number(configuracion.mf_get_serieCotizacion().toInt()+1));
    return configuracion.mf_update();

}

bool uiventas::validar_montoEntregado()
{
    if(ui->lineEdit_efectivo->text().toDouble() <= 0  && ui->lineEdit_tarjeta->text().toDouble() <= 0 )
        return false;
    else
        return true;
}

void uiventas::imprimir()
{
    impresion myimpresion;
    myimpresion.setNombreTienda(ui->label_nombre_Tienda->text(),tienda_actual.mf_get_razonSocial());
    myimpresion.setDireccionTienda(tienda_actual.mf_get_direccion());
    myimpresion.setRucTienda_Telefono(tienda_actual.mf_get_ruc(),tienda_actual.mf_get_telefono());
    myimpresion.setNumeroTicket_Fecha(ui->label_numero_documento->text(),ui->dateTimeEdit_fecha_preventa->dateTime().toString(Qt::ISODate));

    myimpresion.setNombreCliente(ui->lineEdit_razonSocial->text());
    myimpresion.setRucCliente(ui->lineEdit_ruc->text());
    myimpresion.setDireccionCliente(ui->lineEdit_direccion->text());

    //CREANDO LISTA DE PRODUCTOS VENDIDOS
    QVector<articulo> art;
    for(int i = 0; i<count_row;i++)
    {
        articulo myarticulo;
        myarticulo.set_t_articulo(seleccionados_model->item(i,1)->text());
        myarticulo.set_cantidad((seleccionados_model->item(i,4)->text()));
        myarticulo.set_p_unitario((seleccionados_model->item(i,2)->text()));
        myarticulo.set_importe((seleccionados_model->item(i,6)->text()));
        art.push_back(myarticulo);
    }

    myimpresion.setArticuloVector(&art);


    myimpresion.setSubTotal(QString::number(total_venta));
    myimpresion.setIgv(QString::number(monto_igv));
    myimpresion.setTotal(ui->lineEdit_total->text());
    myimpresion.setEntregaEfectivo(ui->lineEdit_efectivo->text());
    myimpresion.setEntregaTarjeta(ui->lineEdit_tarjeta->text());
    myimpresion.setSaldo(ui->lineEdit_restante->text());
    myimpresion.setFechaEntrega(ui->dateTimeEdit_entrega->dateTime().toString(Qt::ISODate));
    myimpresion.setNombreColaborador(ui->lineEdit_usuario->text());
    myimpresion.setMensajeFinal(tienda_actual.mf_get_mensaje_cliente());
    myimpresion.imprimir();

}



void uiventas::on_tableView_Productos_doubleClicked(const QModelIndex &index)
{
   if(index.column() != 5 && comportamiento)
    {
        double total = ui->tableView_Productos->currentIndex().data().toDouble();
        total_venta -= total;
        seleccionados_model->removeRow(index.row());
        ui->tableView_Productos->hideRow(index.row());

        if(seleccionados_model->rowCount() == 0)
        {
            //Reseteando la Venta
            total_venta = 0;
        }
        count_row--;
        monto_igv = total_venta -(total_venta / ((igv/100)+1));
        ui->lineEdit_total->setText(QString::number(total_venta));
        //Recalculando el precio de venta
        double total_adelanto = ui->lineEdit_tarjeta->text().toDouble() +  ui->lineEdit_efectivo->text().toDouble();
        calculaprecio(total_adelanto);
    }

}



void uiventas::configuracionReportes()
{

    /*
     *Ubicacion de los combos en el vector
     * 0 EMPRESA
     * 1 TIENDA
     * 2 COLABORADOR
     * 3 DOCUMENTO
     * 4 FORMA DE PAGO
     */

    //LLENADO DE FALSE DADO QUE NO HAY SELECCIONADOS


    //LLENANDO LOS COMBOBOX


    ui->comboBox_Empresa->ActualizarItemsReporte(empresa::mostrar());

    ui->comboBox_Colaborador->ActualizarItemsReporte(object_Colaborador::mf_show());


    ui->comboBox_Documento->addItem("---Todos---");
    ui->comboBox_Documento->addItem("Factura");
    ui->comboBox_Documento->addItem("Boleta");


    ui->comboBox_Forma_Pago->addItem("---Todos---");
    ui->comboBox_Forma_Pago->addItem("Contado");
    ui->comboBox_Forma_Pago->addItem("Credito");
    ui->comboBox_Forma_Pago->addItem("Contado y Credito");
    ui->comboBox_Forma_Pago->addItem("Ninguno");

    //CONFIGURANDO FECHA

    ui->dateTimeEdit_Desde->setDate(QDate::currentDate());
    ui->dateTimeEdit_Hasta->setDateTime(QDateTime::currentDateTime());

    ui->comboBox_Empresa->removeItem(0);
    ui->tableView_Reporte_Ventas->setColumnWidth(1,150);
    ui->tableView_Reporte_Ventas->setColumnWidth(3,280);
    calcularReporte();
    //activando busquedas
    flag_reporte = true;

}

void uiventas::on_dateTimeEdit_Desde_dateTimeChanged(const QDateTime &dateTime)
{
    ui->dateTimeEdit_Hasta->setMinimumDateTime(dateTime);
}


void uiventas::calcularReporte()
{


    object_Venta consulta;

    _idTienda = ui->comboBox_Tienda->currentText();
    _idTienda = QString::number(ui->comboBox_Tienda->getId(_idTienda));

    _idColaborador = QString::number(ui->comboBox_Colaborador->getId(ui->comboBox_Colaborador->currentText()));
    _idDocuemento = QString::number(ui->comboBox_Documento->currentIndex());
    _idFormaPago = QString::number(ui->comboBox_Forma_Pago->currentIndex());

    consulta.mf_set_Tienda_idTienda(_idTienda);
    consulta.mf_set_Colaborador_idColaborador(_idColaborador);
    consulta.mf_set_tipoDocumento(_idDocuemento);
    consulta.mf_set_dateFrom(ui->dateTimeEdit_Desde->dateTime().toString(Qt::ISODate));
    consulta.mf_set_dateTo(ui->dateTimeEdit_Hasta->dateTime().toString(Qt::ISODate));
    consulta.mf_set_formaPago(_idFormaPago);    


    ui->tableView_Reporte_Ventas->setModel(consulta.mf_show(0));
    //CALCULANDO INGRESOS
    float total =0;
    for(int i = 0;i<ui->tableView_Reporte_Ventas->model()->rowCount();i++)
    {
        total+= ui->tableView_Reporte_Ventas->model()->index(i,5).data().toFloat();
    }
    ui->label_Resultado->setText(QString::number(total));
}

void uiventas::on_comboBox_Empresa_currentIndexChanged(const QString &arg1)
{
    QString idEmpresa = QString::number(ui->comboBox_Empresa->getId(arg1));
    ui->comboBox_Tienda->ActualizarItemsReporte(c_tienda::mostrar(idEmpresa));
    ui->comboBox_Tienda->removeItem(0);
}



void uiventas::on_comboBox_Empresa_currentIndexChanged(int index)
{
    if(flag_reporte)
        calcularReporte();

}

void uiventas::on_comboBox_Tienda_currentIndexChanged(int index)
{    
    if(flag_reporte)
        calcularReporte();
}

void uiventas::on_comboBox_Documento_currentIndexChanged(int index)
{
    if(flag_reporte)
        calcularReporte();
}

void uiventas::on_comboBox_Forma_Pago_currentIndexChanged(int index)
{    
    if(flag_reporte)
        calcularReporte();
}

void uiventas::on_comboBox_Colaborador_currentIndexChanged(int index)
{
    if(flag_reporte)
        calcularReporte();
}

void uiventas::limpiarInterfazVenta()
{
    ui->radioButton_Boleta->setChecked(TRUE);
    ui->lineEdit_razonSocial->clear();
    ui->lineEdit_ruc->clear();
    ui->lineEdit_direccion->clear();
    ui->lineEdit_codProducto->clear();
    ui->dateTimeEdit_entrega->setDateTime(QDateTime::currentDateTime());
    ui->lineEdit_efectivo->clear();
    ui->lineEdit_tarjeta->clear();
    ui->lineEdit_restante->clear();
    ui->lineEdit_total->clear();
    ui->lineEdit_total_cancelado->clear();    



    seleccionados_model->clear();
    count_row = 0;
    total_venta = 0;
    sub_total = 0;
    monto_igv = 0;
    ui->label_tarjeta->clear();
    ui->label_efectivo->clear();    
    configuracionesIniciaciales();

    habilitar_nuevo();


}

void uiventas::habilitar_editar()
{

    ui->pushButton_buscarCliente->setEnabled(false);
    ui->comboBox_buscar_producto->setEnabled(false);
    ui->lineEdit_codProducto->setEnabled(false);

    ui->radioButton_Boleta->setEnabled(false);
    ui->radioButton_cotizacion->setEnabled(false);
    ui->radioButton_Factura->setEnabled(false);

    ui->pushButton_anular->setEnabled(true);
    ui->dateTimeEdit_entrega->setEnabled(true);
    comportamiento = false;

}

void uiventas::habilitar_nuevo()
{
    ui->pushButton_buscarCliente->setEnabled(true);
    ui->comboBox_buscar_producto->setEnabled(true);
    ui->pushButton_imprimir->setEnabled(true);
    ui->lineEdit_codProducto->setEnabled(true);
    ui->lineEdit_efectivo->setEnabled(true);
    ui->lineEdit_tarjeta->setEnabled(true);
    ui->pushButton_guardar->setEnabled(true);
    ui->tableView_Productos->setEnabled(true);
    ui->dateTimeEdit_entrega->setEnabled(true);

    //LA FECHA DE VENTA NO PUEDE EDITARSE DADO QUE LA BOLETA SE PUDO IMPRIMIR
    ui->dateTimeEdit_fecha_preventa->setEnabled(false);
    ui->pushButton_anular->setEnabled(false);
    ui->radioButton_Boleta->setEnabled(true);
    ui->radioButton_cotizacion->setEnabled(true);
    ui->radioButton_Factura->setEnabled(true);
    comportamiento = true;
}

void uiventas::on_dateTimeEdit_Desde_editingFinished()
{
    if(flag_reporte)
        calcularReporte();
}

void uiventas::on_dateTimeEdit_Hasta_editingFinished()
{
    if(flag_reporte)
        calcularReporte();
}

void uiventas:: on_tableView_Reporte_Ventas_doubleClicked(const QModelIndex &index)
{
    limpiarInterfazVenta();
    object_Venta venta;
    venta.mf_load(ui->tableView_Reporte_Ventas->model()->index(index.row(),0).data().toString());
    //COMPLETANDO DATOS DE LA TABLA VENTAS

    //ui->dateTimeEdit_fecha_preventa->setMinimumDateTime(QDateTime::fromString(venta.mf_get_fechaPreVenta()));
    //ui->dateTimeEdit_fecha_preventa->setDateTime(QDateTime::fromString(venta.mf_get_fechaPreVenta()));
    idVentaActual = venta.mf_get_idVenta();    
    ui->dateTimeEdit_entrega->setMinimumDateTime(QDateTime::fromString(venta.mf_get_fechaEntrega(),Qt::ISODate));
    ui->dateTimeEdit_entrega->setDateTime(QDateTime::fromString(venta.mf_get_fechaEntrega(),Qt::ISODate));    
    ui->lineEdit_restante->setText(QString::number(venta.mf_get_montoTotal().toDouble() - venta.mf_get_montoAdelanto().toDouble()));

    //TIPO DE DOCUMENTO
    if(venta.mf_get_tipoDocumento() == "1")
        ui->radioButton_Factura->click();
    if(venta.mf_get_tipoDocumento() == "2")
        ui->radioButton_Boleta->click();
    if(venta.mf_get_tipoDocumento() == "3")
        ui->radioButton_cotizacion->click();



    object_Tienda v_tienda;
    v_tienda.mf_load(venta.mf_get_Tienda_idTienda());
    ui->label_nombre_Tienda->setText(v_tienda.mf_get_nombre());
    _idTienda = v_tienda.mf_get_idTienda();

    object_Cliente v_cliente;
    v_cliente.mf_load(venta.mf_get_Cliente_idCliente());
    if(v_cliente.mf_get_razonSocial() =="")
        ui->lineEdit_razonSocial->setText(v_cliente.mf_get_nombres() + " " +v_cliente.mf_get_primer_apellido() + " " +v_cliente.mf_get_segundo_apellido());
    else
        ui->lineEdit_razonSocial->setText(v_cliente.mf_get_razonSocial());
    ui->lineEdit_ruc->setText(v_cliente.mf_get_ruc());
    ui->lineEdit_direccion->setText(v_cliente.mf_get_direccion());
    idCliente = v_cliente.mf_get_idCliente();

    object_Colaborador v_colaborador;
    v_colaborador.mf_load(venta.mf_get_Colaborador_idColaborador());
    _idColaborador = v_colaborador.mf_get_idColaborador();

    ui->lineEdit_usuario->setText(v_colaborador.mf_get_nombres() + " " +v_colaborador.mf_get_primer_apellido());
    //TIPO DE DOCUMENTO  0 FACTURA,1 BOLETA, COTIZACION 2
    if(venta.mf_get_tipoDocumento() == "1")
        ui->radioButton_Factura->setChecked(true);
    if(venta.mf_get_tipoDocumento() == "2")
        ui->radioButton_Boleta->setChecked(true);
    ui->tabWidget->setCurrentIndex(1);


    /*********************************************************/
    //LLENANDO PRODUCTOS A LA TABLA.
    object_Venta_has_Producto v_producto;

    vector<_QSTR> salida = v_producto.mf_listPk(venta.mf_get_idVenta());
    for(int i=0;i<salida.size();i++)
    {
        v_producto.mf_load(salida[i]);
        recojeProducto(v_producto.mf_get_Producto_idProducto(),v_producto.mf_get_descripcion(),v_producto.mf_get_precio(),v_producto.mf_get_descuento(),v_producto.mf_get_cantidad().toInt(),"0",0,0,"0");
    }
    salida.clear();

    //LLENANDO SI NO A LA TABLA
    object_EntregaProducto v_entregado;
    salida = v_entregado.mf_listPk(venta.mf_get_idVenta());
    for(int i=0;i<salida.size();i++)
    {
        v_entregado.mf_load(salida[i]);
        if(v_entregado.mf_get_estado() == "1")
            seleccionados_model->item(i,5)->setText("Si");
        else
            seleccionados_model->item(i,5)->setText("No");
    }
    //COMPLETANDO PRECIOS
    object_VentaPlazo v_plazo;
    vector<_QSTR> lista_pk =v_plazo.mf_listPk(venta.mf_get_idVenta());
    double acumulado_efectivo = 0;
    double acumulado_tarjeta = 0;
    for(int i=0;i<lista_pk.size();i++)
    {
        v_plazo.mf_load(lista_pk[i]);
        acumulado_efectivo+= v_plazo.mf_get_montoEfectivo().toDouble();
        acumulado_tarjeta+= v_plazo.mf_get_montoTarjeta().toDouble();
    }
    lista_pk.clear();

    ui->label_efectivo->setText(QString::number(acumulado_efectivo));
    ui->label_tarjeta->setText(QString::number(acumulado_tarjeta));


    //DESABILITANDO INTERFACE SI TODO ESTA CANCELADO

    //GRABANDO ADELANTOS

    efectivo_pasado = ui->label_efectivo->text().toDouble();
    tarjeta_pasado = ui->label_tarjeta->text().toDouble();

    calculaprecio(efectivo_pasado + tarjeta_pasado);

    //ACTIVANDO BOTONES
    ui->pushButton_anular->setEnabled(true);

    comportamiento = false;
    habilitar_editar();
    if(ui->lineEdit_total->text().toDouble() == ui->lineEdit_total_cancelado->text().toDouble())
    {
        ui->lineEdit_tarjeta->setEnabled(false);
        ui->lineEdit_efectivo->setEnabled(false);
    }
    //SETEANDO EL NUMERO DE DOCUMENTO
    ui->label_numero_documento->setText(venta.mf_get_serieDocumento()+"-"+venta.mf_get_numeroDocumento());

    ui->pushButton_imprimir->show();



}

void uiventas::on_pushButton_limpiar_clicked()
{
    limpiarInterfazVenta();
}


void uiventas::on_pushButton_anular_clicked()
{
    //CONSIDERAR QUE LOS PRODUCTOS DEBEN INCREMENTAR EN EL STOCK GENERAL EN LA TABLA PRODUCTOS.
    //
}
void uiventas::on_comboBox_buscar_producto_activated(int index)
{
    fuente = index;
    if(index == 1)
    {
        ui_producto* form=new ui_producto;
        form->setComportamiento(2);
        form->setWindowTitle("Seleccione Producto");
        connect(form,SIGNAL(sentProductoVenta(QString,QString,QString,QString,int,QString,int,int,QString)),this,SLOT(recojeProducto(QString,QString,QString,QString,int,QString,int,int,QString)));
        connect(form,SIGNAL(sentProductoVenta(QString,QString,QString,QString,int,QString,int,int,QString)),form,SLOT(close()));
        form->show();
    }
    if(index == 2)
    {
        ui_tienda* form = new ui_tienda;
        form->setWindowTitle("Seleccione Producto");
        form->setComportamiento(1);
        connect(form,SIGNAL(sentProductoVenta(QString,QString,QString,QString,int,QString,int,int,QString)),this,SLOT(recojeProducto(QString,QString,QString,QString,int,QString,int,int,QString)));
        connect(form,SIGNAL(sentProductoVenta(QString,QString,QString,QString,int,QString,int,int,QString)),form,SLOT(close()));
        form->show();
    }
    if(index == 3)
    {
        ui_almacen* form = new ui_almacen;
        form->setWindowTitle("Seleccione Producto");
        //form->setComportamiento(1);
        connect(form,SIGNAL(sendProductoVenta(QString,QString,QString,QString,int,QString,int,int,QString)),this,SLOT(recojeProducto(QString,QString,QString,QString,int,QString,int,int,QString)));
        connect(form,SIGNAL(sendProductoVenta(QString,QString,QString,QString,int,QString,int,int,QString)),form,SLOT(close()));
        form->show();
    }

}

void uiventas::on_tabWidget_currentChanged(int index)
{
    if(index == 0)
    {

    }
    if(index == 1)
    {
        ui->tabWidget->setCurrentIndex(1);
    }
    if(index == 2)
    {
        object_VentaPlazo vplazo;
        vplazo.mf_set_Venta_idVenta(idVentaActual);
        vplazo.mf_set_Tienda_idTienda(_idTienda);
        vplazo.mf_set_Colaborador_idColaborador(_idColaborador);
        ui->tableView_pagos->setModel(vplazo.mf_show());

        //CALCULANDO TOTALES
        float totalefectivo =0;
        float totaltarjeta =0;
        for(int i = 0;i<ui->tableView_pagos->model()->rowCount();i++)
        {

            totalefectivo+= ui->tableView_pagos->model()->index(i,3).data().toFloat();
            totaltarjeta+= ui->tableView_pagos->model()->index(i,4).data().toFloat();
        }
        ui->lineEdit_pagos_efectivo->setText(QString::number(totalefectivo));
        ui->lineEdit_pagos_tarjeta->setText(QString::number(totaltarjeta));
        ui->lineEdit_pagos_total->setText(QString::number(totalefectivo+totaltarjeta));
    }
}

void uiventas::on_tableView_pagos_clicked(const QModelIndex &index)
{
    index_pagos = index;
    QString tarjeta = ui->tableView_pagos->model()->index(index.row(),4).data().toString();
    if(tarjeta.size() > 0)
        ui->pushButton_registro_tarjeta->setEnabled(true);
    else
        ui->pushButton_registro_tarjeta->setEnabled(false);
}

void uiventas::on_pushButton_registro_tarjeta_clicked()
{
    QString tarjeta = ui->tableView_pagos->model()->index(index_pagos.row(),4).data().toString();
    QString id_venta_tarjeta = ui->tableView_pagos->model()->index(index_pagos.row(),0).data().toString();
    ui_tarjeta *form_tarjeta =  new ui_tarjeta;
    form_tarjeta->set_monto(tarjeta);
    form_tarjeta->set_idVenta(idVentaActual);
    object_Venta_has_Tarjeta v_has_tarjeta;
    v_has_tarjeta.mf_load(id_venta_tarjeta);
    form_tarjeta->load_data(&v_has_tarjeta);
    form_tarjeta->show();

}

void uiventas::on_pushButton_compras_clicked()
{

}

void uiventas::on_pushButton_imprimir_clicked()
{
    imprimir();
}
