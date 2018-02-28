#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include "game.h"
extern Tile *tile[8][8];
//void MainWindow::setPlayer(int p) { player = p; }
void MainWindow::showinf() { qDebug("hahaha"); }
void MainWindow::setTurnLabel(bool playing){
    ui->label_turn->setText(playing?"It's your turn to play":"It's his turn to play");
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    myscore = hisscore;
    init();
}
void MainWindow::updateScore(){
    char str[100];
    qDebug("myscore: %d,hisscore: %d",myscore,hisscore);
    sprintf(str,"%d:%d",myscore,hisscore);
    ui->label_score->setText(str);
}
void MainWindow::init(){
    //memset(a, -1, sizeof(a));
    mynewgame = hisnewgame = NOT_SET;
    Game::gamestarted = 0;
    Game::gameover = 0;
    updateScore();
}
void MainWindow::on_recv_error(int err){
    if(Game::gameover) return;
    switch (err) {
    case SockData::SOCK_ERROR:
        QMessageBox::critical(this,"Error","Socket Error");
        break;
    case SockData::RECV_ERROR:
        Game::gameover = 1;
        QMessageBox::critical(this,"Error","Error when receving.The other play might be offline");
        emit(stopthread());
        this->close();
        break;
    case SockData::SEND_ERROR:
        Game::gameover = 1;
        QMessageBox::critical(this,"Error","Error when sending.The other play might be offline");
        emit(stopthread());
        this->close();
        break;
    }
    Common::sockthread->closesock();
}
void MainWindow::on_recv_cod(int sr, int sc,int tr,int tc) {
    Game::gamestarted = 1;
    char ch = tile[tr][tc]->pieceName;
    tile[sr][sc]->piece = 0;
    tile[tr][tc]->piece = 1;
    tile[tr][tc]->pieceColor = tile[sr][sc]->pieceColor;
    tile[tr][tc]->pieceName = tile[sr][sc]->pieceName;

    tile[sr][sc]->display(tile[sr][sc]->pieceName);
    tile[tr][tc]->display(tile[sr][sc]->pieceName);

    tile[sr][sc]->tileDisplay();
    tile[tr][tc]->tileDisplay();

    Game::playing = 1;

    setTurnLabel(Game::playing);


    if(ch == 'K'){
        checkmate(0);
    }

}

void MainWindow::checkmate(bool flag){
    setEnabled(false);
    QMessageBox::information(this, "Game", flag?"Checkmate!You Win!":"Checkmate!You Lose", QMessageBox::Ok);
    if(flag){
        myscore++;
    }else
        hisscore++;
    //emit(new_game());
    updateScore();
    post_game();
    on_send_newgame();
}
void MainWindow::newgame(bool f){
    if(hisnewgame == YES && mynewgame == YES){
        //QMessageBox::information(this, "Game Over", "Game Not Over", QMessageBox::Ok);
        Game::turn = !Game::turn;
        init();
        Game::playing = Game::turn;
       // Common::sockthread->setReceiving(!Game::playing);
        if(!Game::playing && !Common::sockthread->isReceiving() && !f && mynewgame == NOT_SET)
             //Common::sockthread->setReceiving(1);
            Common::sockthread->sendMsg(SockData::HELLO,"");
        Game::chessBoard(this, tile,Game::turn);
        update();
        setEnabled(true);
    }else{
        Game::gameover = 1;
        QMessageBox::information(this, "Game Over", "Game Over", QMessageBox::Ok);
        this->close();
    }
}
void MainWindow::on_recv_newgame(bool f){
    hisnewgame = f?YES:NO;
    if(mynewgame==NOT_SET) return;

    newgame(0);
}
void MainWindow::on_recv_throw(){
    checkmate(1);
    //Common::sockthread->sendMsg(SockData::HELLO,"");
}
void MainWindow::on_send_newgame(){
    if(hisnewgame==NOT_SET) return;
    newgame(1);
}

void MainWindow::post_game(){
    int ret = QMessageBox::information(this,"Another Game","Do you want another game?",QMessageBox::Ok,QMessageBox::No);
    if(ret == QMessageBox::Ok){
        Common::sockthread->sendMsg(SockData::NEW_GAME,"YES");
        mynewgame=YES;
        ui->label_turn->setText("Waiting for reply.");

    }else{
        Common::sockthread->sendMsg(SockData::NEW_GAME,"NO");
        mynewgame=NO;
    }

}

MainWindow::~MainWindow() {
    //Common::sockthread->closesock();
    //delete Common::sockthread;
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(!Game::playing) return;
    Common::sockthread->sendMsg(SockData::THROW,"");
    checkmate(0);
}
