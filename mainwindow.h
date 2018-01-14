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

    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void startplay();
    //int getPlayerType();
    //oid setPlayerType(int t);
    void setPlayer(int p);
    void setPlaying(int t);
    int getPlaying();
    void whowin(int x,int y);
  private:
    Ui::MainWindow *ui;
    int colorflag;
    int a[3][3];
    int isWin(int, int);
    int f1(int, int);
    int f2(int, int);
    int f3(int, int);
    int f4(int, int);
    bool hasRoom();
    int player;
    int playing;
    //int playertype;

   private slots:
    void showinf();
    void onGetXY(int x,int y);
};

#endif // MAINWINDOW_H
