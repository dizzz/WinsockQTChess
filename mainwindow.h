#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <sockthread.h>
#include"sockdata.h"
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
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void startplay();
    //int getPlayerType();
    //oid setPlayerType(int t);
    void setPlayer(int p);
    void setPlaying(int t);
    int getPlaying();
    void whowin(int x,int y);
    void post_game();
    void on_send_newgame();
    void newgame();
    void init();
  private:
    Ui::MainWindow *ui;
    int colorflag;
    int a[3][3];
    int isWin(int, int);
    bool hasRoom();
    int player;
    int playing;
    //int playertype;
    int mynewgame;
    int hisnewgame;
   private slots:
    void showinf();
    void onGetXY(int x,int y);
    void on_recv_error();
    void on_recv_newgame(bool f);
};

#endif // MAINWINDOW_H
