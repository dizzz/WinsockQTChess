#include "connectwaitdlg.h"
#include "ui_connectwaitdlg.h"
#include <QMessageBox>


ConnectWaitDlg::ConnectWaitDlg(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ConnectWaitDlg) {
	ui->setupUi(this);
	QPixmap bkgnd("/Users/shuang/chess/images/backgro.png");
	bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
	QPalette palette;
	palette.setBrush(QPalette::Background, bkgnd);
	this->setPalette(palette);
}

void ConnectWaitDlg::on_ConnButton_clicked() {
	if (ui->ipEdit->text() == tr("123") &&
		ui->portEdit->text() == tr("123")) {
		QMessageBox::information(this, "Title", "Connecting");
		//todo。。。连接服务器。。。设置connflag
		connflag = 1;

		if (connflag == 1) {
			accept();//跳转的到mainwindow
		} else {
			QMessageBox::information(this, "Title", "Connecting Fail");
			return;
		}
	} else {
		QMessageBox::information(this, "Title", "重新输入");
		ui->ipEdit->clear();
		ui->portEdit->clear();
		ui->ipEdit->setFocus();

	}

}
ConnectWaitDlg::~ConnectWaitDlg() {
	delete ui;
}
