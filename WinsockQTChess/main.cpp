//120.79.129.106
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
    // chessBoard(&w, tile);
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
    // QObject::connect(dlg, SIGNAL(foo()), &w, SLOT(showinf()));
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

    QObject::connect(dlg, SIGNAL(stopthread()), Common::sockthread,
                     SLOT(stopImmediately()));
    QObject::connect(rpsdlg, SIGNAL(stopthread()), Common::sockthread,
                     SLOT(stopImmediately()));
    QObject::connect(&w, SIGNAL(stopthread()), Common::sockthread,
                     SLOT(stopImmediately()));
    QApplication::setQuitOnLastWindowClosed(true);
    if (dlg->exec() == QDialog::Accepted) {
        delete dlg;
        if (rpsdlg->exec() == QDialog::Accepted) {
            if (rpsdlg->getFirst()) {
                // w.setPlaying(1);
                Game::playing = 1;
                // w.setPlayer(0);
                qDebug("play first");
                Common::sockthread->setReceiving(0);
            } else {
                qDebug("play second");
                // w.setPlaying(0);
                Game::playing = 0;
                // w.setPlayer(1);
                Common::sockthread->setReceiving(1);
            }
            // rpsdlg->quit();
            // rpsdlg->wait();

            delete rpsdlg;
            Game::turn = Game::playing;
            qDebug("turn: %d playing %d", Game::turn, Game::playing);
            Game::chessBoard(&w, tile, Game::playing);
            w.setTurnLabel(Game::playing);
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
        exit(0);
        return 0;
    }
}
