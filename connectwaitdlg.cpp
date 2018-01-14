#define WIN32_LEAN_AND_MEAN
#include "connectwaitdlg.h"
#include "ui_connectwaitdlg.h"
#include <QMessageBox>

//ConnThread * ConnectWaitDlg::connthread = nullptr;
ConnectWaitDlg::ConnectWaitDlg(QWidget *parent)
    : QDialog(parent), ui(new Ui::ConnectWaitDlg) {
    ui->setupUi(this);
    /*QPixmap bkgnd("E:\\Projects\\xsy\\images\\backgro.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);*
    this->setPalette(palette);*/
    ui->ipEdit->setFocus();
    connthread = new ConnThread();
    connect(connthread,SIGNAL(sock_error()),this,SLOT(on_sock_error()));
    connect(connthread,SIGNAL(sock_started()),this,SLOT(on_sock_started()));
}
void ConnectWaitDlg::on_sock_started(){

    qDebug("yes!!!!!");
    QMessageBox::information(this, "Title", "Connected");
    accept();
}
void ConnectWaitDlg::on_sock_error(){
    QMessageBox::information(this, "Title", "Socket Error!");
    /*if(connthread != nullptr)
        delete connthread;
    connthread = new ConnThread();*/
}
void ConnectWaitDlg::on_ConnButton_clicked() {
    emit foo();
    if (connthread != nullptr && connthread->isRunning()) {
        QMessageBox::information(this, "Title", "Please Wait!!!");
        return;
    }
    QString ip = ui->ipEdit->text();
    QString port = ui->portEdit->text();
    if(!ip.isEmpty() && !port.isEmpty()){
        connthread->setType(CLIENT);
        connthread->setHost(ip.toStdString());
        connthread->setPort(port.toStdString());
    } else if (!port.isEmpty()) {
        connthread->setPort(port.toStdString());
        connthread->setType(SERVER);
    } else {
        QMessageBox::information(this, "Title", "重新输入");
        ui->ipEdit->clear();
        ui->portEdit->clear();
        ui->ipEdit->setFocus();
        return;
    }
    qDebug("%s : %s : %d",connthread->getHost().c_str(),connthread->getPort().c_str(),connthread->getType());
    connthread->start();

}
ConnectWaitDlg::~ConnectWaitDlg() {
    delete connthread;
    delete ui;
}
