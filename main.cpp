#include "QDateTime"
#include "QFile"
#include "QMessageBox"
#include "common.h"
#include "connectwaitdlg.h"
#include "game.h"
#include "mainwindow.h"
#include "rpsdlg.h"
#include "sockdata.h"
#include "sockthread.h"
#include "tile.h"
#include <QApplication>
int count = 0, Exp[60], max = 0;
// int wR, wC, bR, bC;
Tile *click1;

Tile *tile[8][8] = {{NULL}};

/*
void accessories(QWidget *baseWidget) {
    QLabel *player2 = new QLabel(baseWidget);
    QLabel *name2 = new QLabel("Player 2", baseWidget);
    QLabel *time2 = new QLabel("00:00:00", baseWidget);

    QLabel *player1 = new QLabel(baseWidget);
    QLabel *name1 = new QLabel("Player 1", baseWidget);
    QLabel *time1 = new QLabel("00:00:00", baseWidget);

    QLabel *moves = new QLabel(baseWidget);

    name1->setGeometry(125, 610, 80, 20);
    time1->setGeometry(120, 635, 80, 20);
    player1->setGeometry(100, 500, 100, 100);
    player1->setPixmap(QPixmap(":/Images/profile.png"));

    name2->setGeometry(125, 210, 80, 20);
    time2->setGeometry(120, 235, 80, 20);
    player2->setGeometry(100, 100, 100, 100);
    player2->setPixmap(QPixmap(":/Images/profile.png"));

    moves->setGeometry(1000, 105, 250, 550);
    moves->setStyleSheet("QLabel {background-color: white;}");
}
*/

void chessBoard(MainWindow *baseWidget, Tile *tile[8][8]) {
    int i, j, k = 0, hor, ver;

    // Create 64 tiles (allocating memories to the objects of Tile class)
    ver = 125;
    for (i = 0; i < 8; i++) {
        hor = 350;
        for (j = 0; j < 8; j++) {
            tile[i][j] = new Tile(baseWidget);
            tile[i][j]->tileColor = (i + j) % 2;
            tile[i][j]->piece = 0;
            tile[i][j]->row = i;
            tile[i][j]->col = j;
            tile[i][j]->tileNum = k++;
            tile[i][j]->tileDisplay();
            tile[i][j]->setGeometry(hor, ver, 64, 64);
            hor += 64;
        }
        ver += 64;
    }

    // white pawns
    for (j = 0; j < 8; j++) {
        tile[1][j]->piece = 1;
        tile[1][j]->pieceColor = 0;
        tile[1][j]->display('P');
    }

    // black pawns
    for (j = 0; j < 8; j++) {
        tile[6][j]->piece = 1;
        tile[6][j]->pieceColor = 1;
        tile[6][j]->display('P');
    }

    // white and black remaining elements
    for (j = 0; j < 8; j++) {
        tile[0][j]->piece = 1;
        tile[0][j]->pieceColor = 0;
        tile[7][j]->piece = 1;
        tile[7][j]->pieceColor = 1;
    }

    {
        tile[0][0]->display('R');
        tile[0][1]->display('H');
        tile[0][2]->display('B');
        tile[0][3]->display('Q');
        tile[0][4]->display('K');
        tile[0][5]->display('B');
        tile[0][6]->display('H');
        tile[0][7]->display('R');
    }

    {
        tile[7][0]->display('R');
        tile[7][1]->display('H');
        tile[7][2]->display('B');
        tile[7][3]->display('Q');
        tile[7][4]->display('K');
        tile[7][5]->display('B');
        tile[7][6]->display('H');
        tile[7][7]->display('R');
    }
}

void outputMessage(QtMsgType type, const QMessageLogContext &context,
                   const QString &msg) {

    QString text;
    switch (type) {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    QString context_info = QString("File:(%1) Line:(%2)")
                               .arg(QString(context.file))
                               .arg(context.line);
    QString current_date_time =
        QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4")
                          .arg(text)
                          .arg(context_info)
                          .arg(msg)
                          .arg(current_date);

    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    chessBoard(&w, tile);
    // w.setGeometry(100,100,1370,700);
    qInstallMessageHandler(outputMessage);
    Game::drawBorder(&w);
    Game::m_window = &w;
    RPSdlg *rpsdlg = new RPSdlg();
    ConnectWaitDlg *dlg = new ConnectWaitDlg();
    // QObject::connect(&dlg,SIGNAL(c)

    Common::createThread();
    QObject::connect(Common::sockthread, SIGNAL(sock_error(int)), dlg,
                     SLOT(on_sock_error()));
    QObject::connect(Common::sockthread, SIGNAL(sock_error(int)), rpsdlg,
                     SLOT(on_sock_error()));
    QObject::connect(Common::sockthread, SIGNAL(sock_started()), dlg,
                     SLOT(on_sock_started()));
    //QObject::connect(dlg, SIGNAL(foo()), &w, SLOT(showinf()));
    QObject::connect(Common::sockthread, SIGNAL(recv_cod(int, int, int, int)),
                     &w, SLOT(on_recv_cod(int, int, int, int)));
    QObject::connect(Common::sockthread, SIGNAL(recvRPS(int)), rpsdlg,
                     SLOT(onGetRPS(int)));
     QObject::connect(Common::sockthread, SIGNAL(sock_error(int)), &w,
                      SLOT(on_recv_error(int)));
    QObject::connect(Common::sockthread, SIGNAL(recvNewGame(bool)), &w,
                     SLOT(on_recv_newgame(bool)));
    QObject::connect(Common::sockthread, SIGNAL(recvThrow()), &w,
                     SLOT(on_recv_throw()));
    QApplication::setQuitOnLastWindowClosed(true);
    if (dlg->exec() == QDialog::Accepted) {
        delete dlg;
        if (rpsdlg->exec() == QDialog::Accepted) {
            if (rpsdlg->getFirst()) {
                // w.setPlaying(1);
                Common::playing = 1;
                // w.setPlayer(0);
                qDebug("play first");
                Common::sockthread->setReceiving(0);
            } else {
                qDebug("play second");
                // w.setPlaying(0);
                Common::playing = 0;
                // w.setPlayer(1);
                Common::sockthread->setReceiving(1);
            }
            // rpsdlg->quit();
            // rpsdlg->wait();

            delete rpsdlg;
            Common::turn = Common::playing;
            qDebug("turn: %d playing %d", Common::turn, Common::playing);
            Game::chessBoard(&w, tile, Common::playing);
            w.setTurnLabel(Common::playing);
            w.show();

            int ret = a.exec();
            // qDebug("return here");
            exit(0);
            return ret;
        } else {
            exit(0);
            return 0;
        }
    } else {
        // qDebug("not acc");

        exit(0);
        return 0;
    }

    // w.show();
    // return a.exec();
}
