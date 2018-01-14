#include "common.h"
#include "connectwaitdlg.h"
#include "mainwindow.h"
#include "rpsdlg.h"
#include "sockthread.h"
#include <QApplication>
#include"sockdata.h"
#include"QDateTime"
#include"QFile"
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{


    QString text;
    switch(type)
    {
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

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();

}

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    //注册MessageHandler
    qInstallMessageHandler(outputMessage);
    // QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    Common::startThread();
    MainWindow w;
    ConnectWaitDlg dlg; // 建立自己新建的类的对象dlg
    RPSdlg rpsdlg;
    QObject::connect(&dlg, SIGNAL(foo()), &w, SLOT(showinf()));
    QObject::connect(Common::sockthread, SIGNAL(recvXY(int, int)), &w,
                     SLOT(onGetXY(int, int)));
    QObject::connect(Common::sockthread,SIGNAL(recvRPS(int)),&rpsdlg,SLOT(onGetRPS(int)));

    if (dlg.exec() == QDialog::Accepted) {

        if (rpsdlg.exec() == QDialog::Accepted) {
            if (rpsdlg.getFirst()) {
                w.setPlaying(1);
                w.setPlayer(0);
                qDebug("play first");
                Common::sockthread->setReceiving(0);
            } else {
                qDebug("play seconbd");
                w.setPlaying(0);
                w.setPlayer(1);
                Common::sockthread->setReceiving(1);
            }
            w.show();
            return a.exec();
        }
    }else
        return 0;

    /*
    if (dlg.exec() == QDialog::Accepted) {

        if(dlg.connthread->getType()==CLIENT){
            w.setPlaying(1);

            qDebug("play first");
        }else{
            qDebug("play seconbd");
            w.setPlaying(0);
            Common::sockthread->setReceiving(1);
        }
        w.show();
        return a.exec();
    } else
        return 0; //如果没被按下，则不会进入主窗口，整个程序结束运行*/
}
//TODO必须等到双方都准备好再开始
//可不可以在当我收到对方rps成功后，重发上次自己的rps
//A启动并发送rps 但是这时B没有启动，所以没有收到rps，所以A重新发送rps?

