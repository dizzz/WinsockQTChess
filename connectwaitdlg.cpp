#define WIN32_LEAN_AND_MEAN
#include "connectwaitdlg.h"
#include "ui_connectwaitdlg.h"
#include <QMessageBox>

//ConnThread * ConnectWaitDlg::Common::sockthread = nullptr;
ConnectWaitDlg::ConnectWaitDlg(QWidget *parent)
    : QDialog(parent), ui(new Ui::ConnectWaitDlg) {
    ui->setupUi(this);
    /*QPixmap bkgnd("E:\\Projects\\xsy\\images\\backgro.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);*
    this->setPalette(palette);*/
    ui->ipEdit->setFocus();
    //Common::sockthread = new ConnThread();
    //connect(Common::sockthread,SIGNAL(sock_error()),this,SLOT(on_sock_error()));
    //connect(Common::sockthread,SIGNAL(sock_started()),this,SLOT(on_sock_started()));
}
void ConnectWaitDlg::on_sock_started(){
    qDebug("yes!!!!!");
    accept();
}
void ConnectWaitDlg::on_sock_error(){
    //QMessageBox::information(this, "Title", "Socket Error!");
    ui->label_status->setText("");

}
void ConnectWaitDlg::closeEvent(QCloseEvent *event){
    Common::sockthread->closesock();
    //this->close();
}

void ConnectWaitDlg::on_ConnButton_clicked() {
    //emit foo();
    if (Common::sockthread != nullptr && Common::sockthread->isSockRunning()) {
        QMessageBox::information(this, "Title", "Please Wait!!!");
        return;
    }
    QString ip = ui->ipEdit->text();
    QString port = ui->portEdit->text();
    if(!ip.isEmpty() && !port.isEmpty()){
        Common::sockthread->setType(SockThread::CLIENT);
        Common::sockthread->setHost(ip.toStdString());
        Common::sockthread->setPort(port.toStdString());
    } else if (!port.isEmpty()) {
        Common::sockthread->setPort(port.toStdString());
        Common::sockthread->setType(SockThread::SERVER);
    } else {
        QMessageBox::information(this, "Title", "重新输入");
        ui->ipEdit->clear();
        ui->portEdit->clear();
        ui->ipEdit->setFocus();
        return;
    }
    ui->label_status->setText("Connecting");
    //qDebug("%s : %s : %d",Common::sockthread->getHost().c_str(),Common::sockthread->getPort().c_str(),Common::sockthread->getType());
    Common::sockthread->start();
    //Common::sockthread->start();

}
ConnectWaitDlg::~ConnectWaitDlg() {
    //delete Common::sockthread;
    delete ui;
}
