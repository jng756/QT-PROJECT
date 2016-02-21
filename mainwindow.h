

#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QtCore/QtGlobal>

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>

#include <QtSql>
#include <QFileInfo>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSqlDatabase mydb;
    void connClose();
    bool connOpen();


private slots:
    void openSerialPort();
    void closeSerialPort();
    void handleError(QSerialPort::SerialPortError error);

    void on_angleBrazo_valueChanged(const QString &arg1);

    void on_angleBase_valueChanged(const QString &arg1);
    void on_angleCodo_valueChanged(const QString &arg1);

    void on_angleWrist_valueChanged(const QString &arg1);

    void on_angleRotate_valueChanged(const QString &arg1);

    void on_load_clicked();

    void on_save_clicked();
    void load();
    void on_delete_2_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_run_selected_clicked();

    void on_run_all_clicked();

    void on_angleRotate_valueChanged(int arg1);

private:
    void initActionsConnections();

private:
    Ui::MainWindow *ui;
    SettingsDialog *settings;
    QSerialPort *serial;
};

#endif // MAINWINDOW_H
