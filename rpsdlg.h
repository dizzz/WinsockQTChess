#ifndef RPSDLG_H
#define RPSDLG_H

#include "common.h"
#include "sockdata.h"
#include <QDialog>
namespace Ui {
class RPSdlg;
}

class RPSdlg : public QDialog {
    Q_OBJECT

  public:
    enum { ROCK, PAPER, SCISSOR };
    void judge();
    bool getFirst();
    explicit RPSdlg(QWidget *parent = 0);
    ~RPSdlg();

  private slots:
    void on_rockbtn_clicked();

    void on_paperbtn_clicked();

    void on_scissorbtn_clicked();
    void onGetRPS(int rps);
    //void onGetReady();
  private:
    Ui::RPSdlg *ui;
    bool waiting;
    int myrps;
    int hisrps;
    bool first;

};

#endif // RPSDLG_H
