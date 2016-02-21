#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"

#include <QMessageBox>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QDialog>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     //   serial = new QSerialPort("/dev/pts/2");
        serial = new QSerialPort(this);
        settings = new SettingsDialog;

        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
        ui->actionQuit->setEnabled(true);
        ui->actionConfigure->setEnabled(true);

        initActionsConnections();

        connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
                SLOT(handleError(QSerialPort::SerialPortError)));

        if(connOpen())
        qDebug()<<"ok";


}


void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));



}


MainWindow::~MainWindow()
{
    delete ui;
    delete settings;
}

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

       ui->angleCodo->setEnabled(true);
       ui->angleBase->setEnabled(true);
       ui->angleBrazo->setEnabled(true);
       ui->angleWrist->setEnabled(true);
        ui->angleRotate->setEnabled(true);
        ui->angleGrab->setEnabled(true);
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Open error"));
    }
}

void MainWindow::closeSerialPort()
{
    serial->close();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));

    ui->angleBase->setEnabled(false);
    ui->angleBrazo->setEnabled(false);
    ui->angleCodo->setEnabled(false);
    ui->angleWrist->setEnabled(false);
    ui->angleRotate->setEnabled(false);
    ui->angleGrab->setEnabled(false);
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}



void MainWindow::connClose()
{
    mydb.close();
    mydb.removeDatabase(QSqlDatabase::defaultConnection);

}

bool MainWindow::connOpen()
{
     mydb=QSqlDatabase::addDatabase("QSQLITE");

     mydb.setDatabaseName("/home/joel/QTProjects/6DOFarm/Movements.db");

     if(!mydb.open())
     {
        qDebug("Failed to open the database");
     return false;
     }
     else
    {
         qDebug("Connected...");
     return true;
    }
  }



void MainWindow::on_angleBrazo_valueChanged(const QString &arg1)
{

    QByteArray data= arg1.toUtf8();
    qDebug()<<data;
    serial->write("/t");
    serial->write(data);
    serial->write("\\");
}

void MainWindow::on_angleBase_valueChanged(const QString &arg1)
{
    QByteArray data= arg1.toUtf8();
    qDebug()<<data;
    serial->write("/b");
    serial->write(data);
    serial->write("\\");
}

void MainWindow::on_angleCodo_valueChanged(const QString &arg1)
{
    QByteArray data= arg1.toUtf8();
    qDebug()<<data;
    serial->write("/c");
    serial->write(data);
    serial->write("\\");
}

void MainWindow::on_angleWrist_valueChanged(const QString &arg1)
{
    QByteArray data= arg1.toUtf8();
    qDebug()<<data;
    serial->write("/m");
    serial->write(data);
    serial->write("\\");
}

void MainWindow::on_angleRotate_valueChanged(const QString &arg1)
{
    QByteArray data= arg1.toUtf8();
    qDebug()<<data;
    serial->write("/m");
    serial->write(data);
    serial->write("\\");
}

void MainWindow::on_load_clicked()
{
    load();

}

void MainWindow::on_save_clicked()
{

    MainWindow conn;
    int base, brazo, codo, munieca, rotar, pinza,select;
    base=ui->angleBase->value();
    brazo=ui->angleBrazo->value();
    codo=ui->angleCodo->value();
    munieca=ui->angleWrist->value();
    rotar=ui->angleRotate->value();
    pinza=ui->angleGrab->value();
    select=ui->select->value();
    if(!conn.connOpen())
    {
     qDebug()<<"Failed to open the database";
     return;
    }


    QSqlQuery qry;



    qry.prepare("INSERT OR REPLACE INTO angles (Id, base, brazo, codo, munieca, rotar, pinza)"
                "VALUES (:select, :base, :brazo, :codo, :munieca, :rotar, :pinza)");
    qry.bindValue(":select",select);
    qry.bindValue(":base", base);
    qry.bindValue(":brazo", brazo);
    qry.bindValue(":codo", codo);
    qry.bindValue(":munieca", munieca);
    qry.bindValue(":rotar", rotar);
    qry.bindValue(":pinza", pinza);

     if (qry.exec())
     {
         qDebug()<<"ok";

     }
        load();



         conn.connClose();
    return;
}

void MainWindow::load()
{
    MainWindow conn;
    QSqlQueryModel * modal = new QSqlQueryModel();
    conn.connOpen();

    QSqlQuery* qry=new QSqlQuery(conn.mydb);

    qry->prepare("SELECT * FROM angles ORDER BY select");
    qry->exec();

    qry->prepare("SELECT * FROM angles");
    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);

    conn.connClose();

}

void MainWindow::on_delete_2_clicked()
{
   MainWindow conn;
   int select = ui->select->value();
   if(!conn.connOpen())
    {
     qDebug()<<"Failed to open the database";
     return;
    }

        QSqlQuery qry;
        qry.prepare("DELETE FROM angles WHERE Id = :select");
        qry.bindValue(":select",select);

         if (qry.exec())
         {
             qDebug()<<"delete ok";

         }
         else
          {
            qDebug()<<"not deleted";
          }

    load();

    conn.connClose();

    return;

}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    ui->select->setValue(index.row());
}

void MainWindow::on_run_selected_clicked()
{
    QByteArray base, brazo, codo, munieca, rotar, pinza;
    int indice=ui->select->value();
    QModelIndex index;

    index=ui->tableView->model()->index(indice,1);
    base =ui->tableView->model()->data(index).toByteArray();
    serial->write("/b");
    serial->write(base);
    serial->write("\\");

    index=ui->tableView->model()->index(indice,2);
    brazo =ui->tableView->model()->data(index).toByteArray();
    serial->write("/t");
    serial->write(brazo);
    serial->write("\\");

    index=ui->tableView->model()->index(indice,3);
    codo =ui->tableView->model()->data(index).toByteArray();
    serial->write("/c");
    serial->write(codo);
    serial->write("\\");

    index= ui->tableView->model()->index(indice,4);
    munieca= ui->tableView->model()->data(index).toByteArray();
    serial->write("/m");
    serial->write(munieca);
    serial->write("\\");

    index= ui->tableView->model()->index(indice,5);
    rotar= ui->tableView->model()->data(index).toByteArray();

    index= ui->tableView->model()->index(indice,6);
    pinza= ui->tableView->model()->data(index).toByteArray();

    ui->angleBase->setValue(base.toInt());
    ui->angleBrazo->setValue(brazo.toInt());
    ui->angleCodo->setValue(codo.toInt());
    ui->angleWrist->setValue(munieca.toInt());
    ui->angleRotate->setValue(rotar.toInt());
    ui->angleGrab->setValue(pinza.toInt());


    qDebug()<<base<<" "<<" "<<brazo<<" "<<codo<<" "<<munieca;


}

void MainWindow::on_run_all_clicked()
{
    QEventLoop loop;
    QByteArray base, brazo, codo, munieca, rotar, pinza;
    QModelIndex index;
    int i, count, delay;
    delay=2000;
    count=ui->tableView->model()->rowCount();
    for (i=0;i<count;i++)
    {
    index=ui->tableView->model()->index(i,1);
    base =ui->tableView->model()->data(index).toByteArray();
    serial->write("/b");
    serial->write(base);
    serial->write("\\");

    index=ui->tableView->model()->index(i,2);
    brazo =ui->tableView->model()->data(index).toByteArray();
    serial->write("/t");
    serial->write(brazo);
    serial->write("\\");

    index=ui->tableView->model()->index(i,3);
    codo =ui->tableView->model()->data(index).toByteArray();
    serial->write("/c");
    serial->write(codo);
    serial->write("\\");

    index= ui->tableView->model()->index(i,4);
    munieca= ui->tableView->model()->data(index).toByteArray();
    serial->write("/m");
    serial->write(munieca);
    serial->write("\\");

    index= ui->tableView->model()->index(i,5);
    rotar= ui->tableView->model()->data(index).toByteArray();

    index= ui->tableView->model()->index(i,6);
    pinza= ui->tableView->model()->data(index).toByteArray();


    ui->angleBase->setValue(base.toInt());
    ui->angleBrazo->setValue(brazo.toInt());
    ui->angleCodo->setValue(codo.toInt());
    ui->angleWrist->setValue(munieca.toInt());
    ui->angleRotate->setValue(rotar.toInt());
    ui->angleGrab->setValue(pinza.toInt());

    QTimer::singleShot(delay, &loop, SLOT(quit()));
    loop.exec();
    }

}
