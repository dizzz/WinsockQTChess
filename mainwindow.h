#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <sockthread.h>
#include"sockdata.h"
#include"rpsdlg.h"
#include"common.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    enum{
        NOT_SET,YES,NO
    };

    void startplay();
    //int getPlayerType();
    //oid setPlayerType(int t);
    void whowin(int x,int y);
    void setPlayer(int p);
    void post_game();
    void on_send_newgame();
    void newgame(bool f);
    void init();
    void checkmate(bool flag);
    void setTurnLabel(bool playing);
    void updateScore();
  private:
    Ui::MainWindow *ui;
    //int colorflag;
    //int player;//颜色

    int myscore=0;//得分
    int hisscore=0;
    //int playertype;
    int mynewgame;
    int hisnewgame;
   private slots:
    void showinf();
    void on_recv_cod(int sr,int sc,int tr,int tc);
    void on_recv_error(int err);
    void on_recv_newgame(bool f);
    void on_pushButton_clicked();
    void on_recv_throw();
};

#endif // MAINWINDOW_H
