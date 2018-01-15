#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>

void MainWindow::setPlaying(int t) {
    playing = t;
    qDebug("set playing %d",playing);
}
int MainWindow::getPlaying() { return playing; }
//void MainWindow::setPlayerType(int t) { playertype = t; }
//int MainWindow::getPlayerType() { return playertype; }
void MainWindow::setPlayer(int p){player = p;}
void MainWindow::showinf() { qDebug("hahaha"); }
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    /*QPixmap bkgnd("E:\\Projects\\xsy\\images\\backgro.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);*/
    memset(a, -1, sizeof(a));
    //设置玩家颜色 flag为1 先行
    colorflag = 1;
    if (colorflag == 1) {
        ui->Q1->setText("Green");
    } else {
        ui->Q1->setText("Red");
    }
    Common::sockthread->start();
    player = 0;
}
void MainWindow::paintEvent(QPaintEvent *) {
    //qDebug("fuck");
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    int i, j;
    for (i = 0; i < 4; i++) {
        p.drawLine(200, 200 + i * 80, 440, 200 + i * 80);
        p.drawLine(200 + i * 80, 200, 200 + i * 80, 440);
    }

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (a[i][j] == 0) {
                brush.setColor(Qt::green);
                p.setBrush(brush);
                p.drawEllipse(QPoint(160 + (i + 1) * 80, 160 + (j + 1) * 80),
                              30, 30);
            } else if (a[i][j] == 1) {
                brush.setColor(Qt::red);
                p.setBrush(brush);
                p.drawEllipse(QPoint(160 + (i + 1) * 80, 160 + (j + 1) * 80),
                              30, 30);
            }
        }
    }
}
void MainWindow::onGetXY(int x,int y){

    //if(isplaying) return ;
    qDebug("play");
    a[x][y] = 1 - player;
    update();
    playing = 1;
    whowin(x,y);
}
void MainWindow::whowin(int x,int y){
    if (isWin(x, y) == 1) {
        update();
        setEnabled(false);
        QMessageBox::information(this, "Green Win", "Green Win",
                                 QMessageBox::Ok);
    }
    if (isWin(x, y) == 2) {
        update();
        setEnabled(false);
        QMessageBox::information(this, "Red Win", "Red Win",
                                 QMessageBox::Ok);
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e) {
    qDebug("is it receving? %d",Common::sockthread->isReceiving());
    qDebug("playing %d",playing);
    if(!playing)
        return;
    ui->label->setVisible(false);
    int x, y;
    if (e->x() >= 200 && e->x() < 440 && e->y() >= 200 && e->y() < 440) {
        x = (e->x() - 200) / 80;
        y = (e->y() - 200) / 80;
        if (a[x][y] == -1) {
            //a[x][y] = player++ % 2 + 1;
            a[x][y] = player;
            qDebug("move: %d %d",x,y);
            Common::sockthread->sendMsg(SockData::packXY(x,y));
            playing = 0;
        }
        if (hasRoom() == 0) {
            QMessageBox::information(this, "NoRoom", "NoRoom", QMessageBox::Ok);
        }
        whowin(x,y);
    }
    update();
}

int MainWindow::isWin(int x, int y) {
    if (f1(x, y) || f2(x, y) || f3(x, y) || f4(x, y)) {
        if (a[x][y] == 1) {
            return 1;
        }
        if (a[x][y] == 2) {
            return 2;
        }
    }
    return 0;
}

int MainWindow::f1(int x, int y) {
    int i;
    for (i = 0; i < 3; i++) {
        if (y - i >= 0 && y + 2 - i <= 0xF && a[x][y - i] == a[x][y + 1 - i] &&
            a[x][y - i] == a[x][y + 2 - i])
            return 1;
    }
    return 0;
}

int MainWindow::f2(int x, int y) {
    int i;
    for (i = 0; i < 3; i++) {
        if (x - i >= 0 && x + 2 - i <= 0xF && a[x - i][y] == a[x + 1 - i][y] &&
            a[x - i][y] == a[x + 2 - i][y])
            return 1;
    }
    return 0;
}

int MainWindow::f3(int x, int y) {
    int i;
    for (i = 0; i < 3; i++) {
        if (x - i >= 0 && y - i >= 0 && x + 2 - i <= 0xF && y + 2 - i <= 0xF &&
            a[x - i][y - i] == a[x + 1 - i][y + 1 - i] &&
            a[x - i][y - i] == a[x + 2 - i][y + 2 - i])
            return 1;
    }
    return 0;
}

int MainWindow::f4(int x, int y) {
    int i;
    for (i = 0; i < 3; i++) {
        if (x + i <= 0xF && y - i >= 0 && x - 2 + i >= 0 && y + 2 - i <= 0xF &&
            a[x + i][y - i] == a[x - 1 + i][y + 1 - i] &&
            a[x + i][y - i] == a[x - 2 + i][y + 2 - i])
            return 1;
    }
    return 0;
}
bool MainWindow::hasRoom() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (a[i][j] == -1) {
                return true;
            }
        }
    }
    return false;
}

MainWindow::~MainWindow() {
    Common::sockthread->closesock();
    delete ui;
}
