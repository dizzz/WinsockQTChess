#include "rpsdlg.h"
#include "QDebug"
#include "QMessageBox"
#include "ui_rpsdlg.h"
bool RPSdlg::getFirst() { return first; }
void RPSdlg::_init(){
    init = waiting = false;
    hisrps = myrps = -1;
}
RPSdlg::RPSdlg(QWidget *parent) : QDialog(parent), ui(new Ui::RPSdlg) {
    ui->setupUi(this);
    _init();
}

RPSdlg::~RPSdlg() { delete ui; }

void RPSdlg::judge() {
    qDebug("rps result: me: %dhe: %d", myrps, hisrps);
    if ((myrps + 1) % 3 == hisrps) {
        // defeated
        first = 0;
    } else {
        // win
        first = 1;
    }
    accept();
}
void RPSdlg::onGetRPS(int rps) {
    qDebug("recvRPS :%d", rps);
    if (rps == myrps) {
        QMessageBox::information(this, "Retry", "Draw!!!Please try again",
                                 QMessageBox::Ok);
        ui->label->setText("");
        waiting = false;
        hisrps = myrps = -1;
    } else if (myrps != -1) {
        hisrps = rps;
        judge();
    } else {
        hisrps = rps;
    }
}
void RPSdlg::on_sock_error(){
    this->close();
}
void RPSdlg::on_rockbtn_clicked() {
    if (hisrps != -1) {
        judge();
    }
    if (waiting) {
        QMessageBox::information(this, "Wait!!!", "Wait!!!", QMessageBox::Ok);
        return;
    }
    Common::sockthread->sendMsg(SockData::packRPS(ROCK));
    ui->label->setText("Selected ROCK,waiting for the other player");
    myrps = ROCK;
    waiting = true;
    Common::sockthread->setReceiving(1);
}
void RPSdlg::on_paperbtn_clicked() {

    if (hisrps != -1) {
        judge();
    }
    if (waiting) {
        QMessageBox::information(this, "Wait!!!", "Wait!!!", QMessageBox::Ok);
        return;
    }
    ui->label->setText("Selected PAPER,waiting for the other player");
    Common::sockthread->sendMsg(SockData::packRPS(PAPER));
    myrps = PAPER;
    waiting = true;
    Common::sockthread->setReceiving(1);
}

void RPSdlg::on_scissorbtn_clicked() {
    if (hisrps != -1) {
        judge();
    }
    if (waiting) {
        QMessageBox::information(this, "Wait!!!", "Wait!!!", QMessageBox::Ok);
        return;
    }
    ui->label->setText("Selected SCISSOR,waiting for the other player");
    Common::sockthread->sendMsg(SockData::packRPS(SCISSOR));
    myrps = SCISSOR;
    waiting = true;
    Common::sockthread->setReceiving(1);
}
