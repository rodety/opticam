#include "ui_configuracion.h"
#include "ui_ui_configuracion.h"
#include "conexionbd.h"
#include <QSqlQuery>
#include <QDebug>
#include "configurador.h"
#include <configuracion/tusuario.h>
#include <producto/estado.h>
#include <vitrina/ui_agregar_empresa.h>
#include "mainwindow.h"
#include "configuracion/ui_login.h"
#include "configuracion/conexionbd.h"
#include "configuracion/sesion.h"
#include "configuracion/configurador.h"
#include "configuracion/dialoglogin.h"
#include "configuracion/ui_configuracion.h"





ui_configuracion::ui_configuracion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ui_configuracion)
{
    ui->setupUi(this);

    controlbotones(false);

    config = new configurador("config.ini");

    if(config->leerConfiguracion())
    {

        res = config->getConfig();
        loadConfiguration();

    }
    else
    {
        qDebug()<<"No logro abrir el fichero"<<endl;

    }

    ui->comboBox_tipoUsuario->setTipo("tusuario");

    ui->comboBox_Documento->setTipo("documento");

    ui->comboBox_estadoProductos->setTipo("estado");

}

ui_configuracion::~ui_configuracion()
{
    delete ui;
}


void ui_configuracion::on_btn_testConexion_clicked()
{
    QString ip = ui->lineEdit_ipdatabase->text();
    QString db = ui->lineEdit_nameDatabase->text();
    QString user = ui->lineEdit_userDatabase->text();
    QString pass = ui->lineEdit_passwordUser->text();
    QString port = ui->lineEdit_portDatabase->text();

    config->actualizarConfiguracion(ip,db,user,pass,port);
    if(config->conectar_db())
    {
        ui->label_result->setText("Conexion exitosa");
        update_comboBox_Empresa();
        controlbotones(true);

        currentIdEmpresa = res[5];
        currentIdTienda = res[6];
        QString id_Empresa = (res[5]);
        QString id_Tienda =  (res[6]);
        ui->comboBox_empresa->setCurrentIndex(id_Empresa.toInt(0));
        ui->comboBox_tienda->setCurrentIndex(id_Tienda.toInt(0));
        ui->comboBox_tipoUsuario->ActualizarItems(tusuario::mostrar());
        ui->comboBox_Documento->ActualizarItems(documento::mostrarId());
        ui->comboBox_estadoProductos->ActualizarItems(estado::mostrarId());


    }
    else
    {
        ui->label_result->setText("No se logro conectar con la base de datos revise lo datos");
        return;
    }

}


void ui_configuracion::update_comboBox_Empresa()
{
    ui->comboBox_empresa->clear();

    QSqlQuery query;
    query.prepare("SELECT idEmpresa,razonSocial FROM Empresa");
    query.exec();

    int c = 0;

    while(query.next())
    {
        QString idempresa = query.value(0).toString();
        QString raz_social = query.value(1).toString();

        Empresas[raz_social] = idempresa;
        ui->comboBox_empresa->insertItem(c++,raz_social);
    }

}

void ui_configuracion::update_comboBox_Tienda(QString idEmpresa)
{
    ui->comboBox_tienda->clear();

    QSqlQuery query;
    query.prepare("SELECT idTienda,nombre FROM Tienda WHERE Empresa_idEmpresa=?");
    query.bindValue(0,idEmpresa);
    query.exec();

    int c = 0;

    while(query.next())
    {
        QString idtienda = query.value(0).toString();
        QString alias = query.value(1).toString();
        Tiendas[alias] = idtienda;
        ui->comboBox_tienda->insertItem(c++,alias);
    }
}



void ui_configuracion::on_comboBox_empresa_currentIndexChanged(const QString &arg1)
{
    set_currentIdEmpresa(Empresas[arg1]);
    update_comboBox_Tienda(get_currentIdEmpresa());
}

QString ui_configuracion::get_currentIdEmpresa(){return currentIdEmpresa;}
QString ui_configuracion::get_currentIdTienda(){return currentIdTienda;}

void ui_configuracion::set_currentIdEmpresa(QString e){currentIdEmpresa = e;}
void ui_configuracion::set_currentIdTienda(QString t){currentIdTienda = t;}

void ui_configuracion::saveConfiguration()
{
    QSqlQuery query;
    query.prepare("INSERT Configuracion idTienda,nombre FROM Tienda WHERE Empresa_idEmpresa=?");

}

void ui_configuracion::on_btn_saveConfiguration_clicked()
{
    QValidator * validator = new QIntValidator(this);
    ui->lineEdit_IGV->setValidator(validator);
    ui->lineEdit_serieFactura->setValidator(validator);
    ui->lineEdit_boleta->setValidator(validator);
    ui->lineEdit_serieCotizacion->setValidator(validator);


    set_currentIdEmpresa(Empresas[ui->comboBox_empresa->currentText()]);
    set_currentIdTienda(Tiendas[ui->comboBox_tienda->currentText()]);

    QString igv = ui->lineEdit_IGV->text();
    QString serieBoleta = ui->lineEdit_boleta->text();
    QString serieFactura = ui->lineEdit_serieFactura->text();
    QString serieCotizacion = ui->lineEdit_serieCotizacion->text();
    QString codpos = ui->lineEdit_cod_pos->text();
    QSqlQuery query;
    query.prepare("SELECT idConfiguracion FROM Configuracion WHERE Tienda_idTienda = ?");
    query.bindValue(0,currentIdTienda);
    query.exec();

    if(query.next())
    {
        QString id = query.value(0).toString();
        query.prepare("UPDATE Configuracion SET igv = ?,serieBoleta = ?,serieFactura = ?,serieCotizacion = ?, codigoPos = ? WHERE (idConfiguracion = ?)");
        query.bindValue(0,igv);
        query.bindValue(1,serieBoleta);
        query.bindValue(2,serieFactura);
        query.bindValue(3,serieCotizacion);
        query.bindValue(4,codpos);
        query.bindValue(5,id);

    }
    else
    {        
        query.prepare("INSERT INTO Configuracion (Tienda_idTienda,igv,serieBoleta,serieFactura,serieCotizacion) VALUES (?,?,?,?,?)");
        query.bindValue(0,currentIdTienda);
        query.bindValue(1,igv);
        query.bindValue(2,serieBoleta);
        query.bindValue(3,serieFactura);
        query.bindValue(4,serieCotizacion);
        query.bindValue(5,codpos);

    }
    query.exec();

    QString ip = ui->lineEdit_ipdatabase->text();
    QString db = ui->lineEdit_nameDatabase->text();
    QString user = ui->lineEdit_userDatabase->text();
    QString pass = ui->lineEdit_passwordUser->text();
    QString port = ui->lineEdit_portDatabase->text();
    config->guardarConfiguracion(ip,db,user,pass,port,currentIdEmpresa,currentIdTienda);
    QMessageBox msgBox;
    if(ui->lineEdit_IGV->text().size() ==0)
    {
        msgBox.setText("Ingrese el IGV");
        msgBox.exec();
        return;
    }
    if(ui->lineEdit_boleta->text().size() ==0)
    {
        msgBox.setText("Ingrese serie Boleta");
        msgBox.exec();
        return;
    }
    if(ui->lineEdit_serieFactura->text().size() ==0)
    {
        msgBox.setText("Ingrese serie facruta");
        msgBox.exec();
        return;
    }
    if(ui->lineEdit_serieCotizacion->text().size() ==0)
    {
        msgBox.setText("Ingrese serie contizacion");
        msgBox.exec();
        return;
    }
    if(ui->lineEdit_cod_pos->text().size() ==0)
    {
        msgBox.setText("Ingrese el codigo de ma maquina POS");
        msgBox.exec();
        ui->lineEdit_cod_pos->setCursor(this->cursor());
        return;
    }
    //Iniciando MainWindows

    Sesion::Configurar(3,10,6000);
    DialogLogin * log = new DialogLogin;
    log->exec();
    if(log->getOk())
    {
        MainWindow *MAIN_WINDOW;
        MAIN_WINDOW = new MainWindow;
        MAIN_WINDOW->showMaximized();
        //Cargando Datos de ubicacion de la DB
        Sesion::loadUbicacion();
    }
    delete log;
    //Cerrando ventana actual

    this->close();

}

void ui_configuracion::loadConfiguration()
{

    ui->lineEdit_ipdatabase->setText(res[0]);
    ui->lineEdit_nameDatabase->setText(res[1]);
    ui->lineEdit_userDatabase->setText(res[2]);
    ui->lineEdit_passwordUser->setText(res[3]);
    ui->lineEdit_portDatabase->setText(res[4]);

}

void ui_configuracion::on_comboBox_tienda_currentTextChanged(const QString &arg1)
{
    set_currentIdTienda(Tiendas[arg1]);
}

void ui_configuracion::on_comboBox_tienda_currentIndexChanged(const QString &arg1)
{
    set_currentIdTienda(Tiendas[arg1]);
    QSqlQuery query;
    query.prepare("SELECT igv,serieBoleta,serieFactura,serieCotizacion,codigoPos FROM Configuracion WHERE (Tienda_idTienda = ?)");
    query.bindValue(0,currentIdTienda);
    query.exec();
    query.next();
    ui->lineEdit_IGV->setText(query.value(0).toString());
    ui->lineEdit_boleta->setText(query.value(1).toString());
    ui->lineEdit_serieFactura->setText(query.value(2).toString());
    ui->lineEdit_serieCotizacion->setText(query.value(3).toString());
    ui->lineEdit_cod_pos->setText(query.value(4).toString());
}

void ui_configuracion::on_btn_eliminar_ducumento_clicked()
{
    ui->comboBox_Documento->eliminar();
}

void ui_configuracion::on_btn_eliminar_tusuario_clicked()
{
    ui->comboBox_tipoUsuario->eliminar();
}

void ui_configuracion::on_btn_eliminar_estado_clicked()
{
    ui->comboBox_estadoProductos->eliminar();
}

/*void ui_configuracion::on_btn_script_clicked()
{

    //Inicializando Base de datos con tablas necesarias
    QSqlQuery query;

    query.exec("INSERT INTO `Modulo` VALUES (1,'Modulo General','Para Pruebas')");
    query.exec("INSERT INTO `Documento` VALUES (1,'DNI','DNI'),(2,'Carnet de Extrangeria','CE'),(3,'Pasaporte','PP')");
    query.exec("INSERT INTO `TipoColaborador` VALUES (1,'Administrador'),(2,'Ventas')");
    query.exec("INSERT INTO `Colaborador` VALUES ('Administrador','apellido1','apellido2',1,'admin',1,1,'123456778','',1,'1993-05-15','34235463','','354354','354354',1,'','63a9f0ea7bb98050796b649e85481845',1),('carlos','Siu','Escobedo',1,'sui',1,1,'71631189','teniente rodrigues 1205 miraflores',1,'1995-01-17','956336676','s4ujv_138@hotmail.com','','',1,'Primer Usuario','81dc9bdb52d04dc20036dbd8313ed055',2)");
    query.exec("INSERT INTO `FuncionModulo` VALUES (1,1,'Producto','pro'),(2,1,'Usuario','usu'),(3,1,'Cliente','cli'),(4,1,'Vitrina','vit'),(5,1,'Almacen','alm'),(6,1,'Compras','comp'),(7,1,'Reportes','rep'),(8,1,'Ventas','ven'),(9,1,'Configuracion','con'),(10,1,'Agenda','age')");
    query.exec("INSERT INTO `Permiso` VALUES (1,1,1,1,'2013-07-21'),(2,1,2,1,'2013-07-21'),(3,1,3,1,'2013-07-21'),(4,1,4,1,'2013-07-21'),(5,1,5,1,'2013-07-21'),(6,1,6,1,'2013-07-21'),(7,1,7,1,'2013-07-21'),(8,1,8,1,'2013-07-21'),(9,1,9,1,'2013-07-21'),(10,1,10,1,'2013-07-21'),(11,2,10,1,'2013-09-25'),(12,2,5,1,'2013-09-25'),(13,2,3,1,'2013-09-25'),(14,2,6,1,'2013-09-25'),(15,2,9,0,'2013-09-25'),(16,2,1,1,'2013-09-25'),(17,2,7,0,'2013-09-25'),(18,2,2,0,'2013-09-25'),(19,2,8,1,'2013-09-25'),(20,2,4,1,'2013-09-25')");
    query.exec("INSERT INTO `SiNo` VALUES (0,'No','F'),(1,'Si','M')");
    query.exec("INSERT INTO `Estado` VALUES (1,'Activo'),(2,'Inactivo')");
}
*/
void ui_configuracion::on_comboBox_Documento_currentIndexChanged(const QString &arg1)
{

}

void ui_configuracion::on_comboBox_tipoUsuario_currentIndexChanged(const QString &arg1)
{

}

void ui_configuracion::on_comboBox_estadoProductos_currentIndexChanged(const QString &arg1)
{

}

void ui_configuracion::on_btnAgregar_empresa_clicked()
{
    add_empresa = new ui_agregar_empresa;
    connect(add_empresa,SIGNAL(actualizarParent()),this,SLOT(update_comboBox_Empresa()));
    add_empresa->set_behavior(1);
    add_empresa->setWindowTitle("Agregar Empresa");
    add_empresa->show();

}

void ui_configuracion::controlbotones(bool a)
{
    ui->btn_saveConfiguration->setEnabled(a);
    ui->btn_eliminar_ducumento->setEnabled(a);
    ui->btn_eliminar_estado->setEnabled(a);
    ui->btn_eliminar_tusuario->setEnabled(a);
    ui->btnAgregar_empresa->setEnabled(a);
    ui->btnAgregar_Tienda->setEnabled(a);
    ui->btn_backup->setEnabled(a);

}

void ui_configuracion::on_btnAgregar_Tienda_clicked()
{


    if(currentIdEmpresa.compare("")==0)
    {
        QMessageBox *msgBox =new QMessageBox;
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setWindowIcon(QIcon(":/Icons/abiword.png"));
        msgBox->setWindowTitle("Información");
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->setButtonText(QMessageBox::Ok,"Aceptar");
        msgBox->setText("Debe seleccionarse una Empresa válida!");
        msgBox->exec();
    }
    else
    {

        tienda_agregar = new ui_tienda_agregar;
        connect(tienda_agregar,SIGNAL(actualizarParent(QString)),this,SLOT(update_comboBox_Tienda(QString)));
        tienda_agregar->set_idEmpresa(currentIdEmpresa);
        tienda_agregar->set_idTienda(currentIdTienda);
        tienda_agregar->set_caso(1);
        tienda_agregar->setWindowTitle("Agregar Tienda");
        tienda_agregar->show();
    }

}

void ui_configuracion::on_btn_backup_clicked()
{

}
