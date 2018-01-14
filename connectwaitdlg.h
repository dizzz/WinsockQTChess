
#define WIN32_LEAN_AND_MEAN
#ifndef CONNECTWAITDLG_H
#define CONNECTWAITDLG_H
// #include"sendthread.h"
// #include"recvthread.h"
#include"basesock.h"
// #include"sockthread.h"
#include"connthread.h"
#include <QDialog>
namespace Ui {
class ConnectWaitDlg;
}

class ConnectWaitDlg : public QDialog
{
    Q_OBJECT

public:
    ConnThread *connthread;
    explicit ConnectWaitDlg(QWidget *parent = 0);
    ~ConnectWaitDlg();


private:
    Ui::ConnectWaitDlg *ui;
    int connflag;
    // SendThread sdthread;
    // RecvThread rcthread;
    //SockThread scthread;

signals:
  void foo();
private slots:
    void on_ConnButton_clicked();
    void on_sock_started();
    void on_sock_error();

};

#endif // CONNECTWAITDLG_H
