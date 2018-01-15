#include "rpsdlg.h"
#include "ui_rpsdlg.h"
#include"QMessageBox"
#include"QDebug"
bool RPSdlg::getFirst() { return first; }
RPSdlg::RPSdlg(QWidget *parent) : QDialog(parent), ui(new Ui::RPSdlg) {
    ui->setupUi(this);
       waiting = false;
    hisrps = myrps=-1;

}

RPSdlg::~RPSdlg() { delete ui; }

void RPSdlg::judge(){
    qDebug("rps result: me: %dhe: %d",myrps,hisrps);
    if((myrps+1)%3 == hisrps){
        //defeated
        first = 0;
    }else{
        //win
        first = 1;
    }
    qDebug("%d",Common::sockthread->isReceiving());
    //Common::sockthread->sendMsg(SockData::packRPS(myrps));
    accept();
}
void RPSdlg::onGetRPS(int rps){
    qDebug("recvRPS :%d",rps);
    if(rps == myrps){
        QMessageBox::information(this, "Retry", "Retry",
                                  QMessageBox::Ok);
        waiting = false;

        hisrps = myrps=-1;
    }else if(myrps!=-1){
        hisrps = rps;
        judge();
    }else{
        hisrps = rps;
    }
}
void RPSdlg::on_rockbtn_clicked() {

    if(hisrps !=-1){
        judge();
    }
    if(waiting){
        QMessageBox::information(this, "Wait!!!", "Wait!!!",
                                  QMessageBox::Ok);
        return;
    }
    Common::sockthread->sendMsg(SockData::packRPS(ROCK));
    myrps = ROCK;
    waiting = true;
    Common::sockthread->setReceiving(1);
}



void RPSdlg::on_paperbtn_clicked()
{

    if(hisrps !=-1){
        judge();
    }
    if(waiting){
        QMessageBox::information(this, "Wait!!!", "Wait!!!",
                                  QMessageBox::Ok);
        return;
    }

    Common::sockthread->sendMsg(SockData::packRPS(PAPER));
    myrps = PAPER;
    waiting = true;
    Common::sockthread->setReceiving(1);
}

void RPSdlg::on_scissorbtn_clicked()
{
    if(hisrps !=-1){
        judge();
    }
    if(waiting){
        QMessageBox::information(this, "Wait!!!", "Wait!!!",
                                  QMessageBox::Ok);
        return;
    }
    Common::sockthread->sendMsg(SockData::packRPS(SCISSOR));
    myrps = SCISSOR;
    waiting = true;
    Common::sockthread->setReceiving(1);
}
