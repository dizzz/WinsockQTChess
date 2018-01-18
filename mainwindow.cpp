#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>

void MainWindow::setPlaying(int t) {
    playing = t;
    qDebug("set playing %d", playing);
}
int MainWindow::getPlaying() { return playing; }
// void MainWindow::setPlayerType(int t) { playertype = t; }
// int MainWindow::getPlayerType() { return playertype; }
void MainWindow::setPlayer(int p) { player = p; }
void MainWindow::showinf() { qDebug("hahaha"); }
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    /*QPixmap bkgnd("E:\\Projects\\xsy\\images\\backgro.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);*/
    init();
}
void MainWindow::init(){
    memset(a, -1, sizeof(a));
    mynewgame = hisnewgame = NOT_SET;
    //设置玩家颜色 flag为1 先行
    //
    Common::sockthread->start();
    player = 0;
}
void MainWindow::paintEvent(QPaintEvent *) {
    // qDebug("fuck");
    ui->color->setText(player?"Red":"Green");
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
void MainWindow::on_recv_error(){
        QMessageBox::critical(this,"Error","Connection Error");
        Common::sockthread->closesock();
}
void MainWindow::onGetXY(int x, int y) {

    // if(isplaying) return ;
    qDebug("play");
    a[x][y] = 1 - player;
    playing = 1;
    update();
    whowin(x, y);
}
void MainWindow::whowin(int x, int y) {
    bool gameover = 0;
    if (hasRoom() == 0) {
        QMessageBox::information(this, "Draw", "Draw!!!", QMessageBox::Ok);
        gameover = 1;
    }
    int win = isWin(x,y);
    if(win !=-1){
        if (win == player) {
            QMessageBox::information(this, "Win", "You Win!!!",
                                     QMessageBox::Ok);
        }
        else{
            QMessageBox::information(this, "Lose", "You Lose...", QMessageBox::Ok);

          }
        //只要等到两个人都发送玩不玩就可以了

        setEnabled(false);
        gameover = 1;
    }
    if(gameover){
        post_game();
        on_send_newgame();
    }
}
void MainWindow::newgame(){
    if(hisnewgame == YES && mynewgame == YES){
        QMessageBox::information(this, "Game Over", "Game Not Over", QMessageBox::Ok);

    }else{
        QMessageBox::information(this, "Game Over", "Game Over", QMessageBox::Ok);
    }
    //this->close();
}
void MainWindow::on_recv_newgame(bool f){
    hisnewgame = f?YES:NO;
    if(mynewgame==NOT_SET) return;

    newgame();
}
void MainWindow::on_send_newgame(){
    if(hisnewgame==NOT_SET) return;
    newgame();
}

void MainWindow::post_game(){
    int ret = QMessageBox::information(this,"Another Game","Do you want another game?",QMessageBox::Ok,QMessageBox::No);
    if(ret == QMessageBox::Ok){
        Common::sockthread->sendMsg(SockData::NEW_GAME,"YES");
        mynewgame=YES;
    }else{
        Common::sockthread->sendMsg(SockData::NEW_GAME,"NO");
        mynewgame=NO;
    }

}
void MainWindow::mouseReleaseEvent(QMouseEvent *e) {
    qDebug("is it receving? %d", Common::sockthread->isReceiving());
    qDebug("playing %d", playing);
    if (!playing)
        return;
    //ui->label->setVisible(false);
    int x, y;
    if (e->x() >= 200 && e->x() < 440 && e->y() >= 200 && e->y() < 440) {
        x = (e->x() - 200) / 80;
        y = (e->y() - 200) / 80;
        if (a[x][y] == -1) {
            // a[x][y] = player++ % 2 + 1;
            a[x][y] = player;
            qDebug("move: %d %d", x, y);
            Common::sockthread->sendMsg(SockData::packXY(x, y));
            playing = 0;
        }

        whowin(x, y);
    }
    update();
}

int MainWindow::isWin(int x, int y) {
    if (a[x][0] == a[x][1] && a[x][1] == a[x][2])
        return a[x][y];
    if (a[0][y] == a[1][y] && a[1][y] == a[2][y])
        return a[x][y];
    if (x == y && a[0][0] == a[1][1] && a[1][1] == a[2][2])
        return a[x][y];
    if (x == 2 - y && a[0][2] == a[1][1] && a[1][1] == a[2][0])
        return a[x][y];
    return -1;
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
    delete Common::sockthread;
    delete ui;
}
