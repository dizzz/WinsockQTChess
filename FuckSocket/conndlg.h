#ifndef CONNECTWAITDLG_H
#define CONNECTWAITDLG_H

#include<Q>
namespace Ui {
	class ConnectWaitDlg;
}

class ConnectWaitDlg : public QDialog {
	Q_OBJECT

public:
	explicit ConnectWaitDlg(QWidget *parent = 0);
	~ConnectWaitDlg();


	private slots:
	void on_ConnButton_clicked();


private:
	Ui::ConnectWaitDlg *ui;
	int connflag;
};

#endif // CONNECTWAITDLG_H
