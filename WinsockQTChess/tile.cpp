#include "tile.h"
#include "QDebug"
#include"sockdata.h"
#include "validation.h"
#include"common.h"
#include"game.h"
validation *valid = new validation();

extern int count;
extern QWidget *myWidget;
extern Tile *click1;
extern Tile *tile[8][8];

void validate(Tile *temp, int c);
void disOrange();

void Tile::mousePressEvent(QMouseEvent *event) {
    if(!Game::playing) return;
    qDebug("%d %d" , this->row, this->col);
    qDebug("turn: %d", Game::turn);
    validate(this, ++count);
}

void Tile::display(char elem) {
    this->pieceName = elem;

    if (this->pieceColor && this->piece) {
        switch (elem) {
        case 'P':
            this->setPixmap(QPixmap(":/Images/pawn_white.svg"));
            break;
        case 'R':
            this->setPixmap(QPixmap(":/Images/rook_white.svg"));
            break;
        case 'H':
            this->setPixmap(QPixmap(":/Images/knight_white.svg"));
            break;
        case 'K':
            this->setPixmap(QPixmap(":/Images/king_white.svg"));
            break;
        case 'Q':
            this->setPixmap(QPixmap(":/Images/queen_white.svg"));
            break;
        case 'B':
            this->setPixmap(QPixmap(":/Images/bishop_white.svg"));
            break;
        }
    }

    else if (this->piece) {
        switch (elem) {
        case 'P':
            this->setPixmap(QPixmap(":/Images/pawn_black.svg"));
            break;
        case 'R':
            this->setPixmap(QPixmap(":/Images/rook_black.svg"));
            break;
        case 'H':
            this->setPixmap(QPixmap(":/Images/knight_black.svg"));
            break;
        case 'K':
            this->setPixmap(QPixmap(":/Images/king_black.svg"));
            break;
        case 'Q':
            this->setPixmap(QPixmap(":/Images/queen_black.svg"));
            break;
        case 'B':
            this->setPixmap(QPixmap(":/Images/bishop_black.svg"));
            break;
        }
    } else
        this->clear();
}
void Tile::tileDisplay() {

    if (this->tileColor)
        this->setStyleSheet("QLabel {background-color: rgb(120, 120, "
                            "90);}:hover{background-color: rgb(170,85,127);}");
    else
        this->setStyleSheet("QLabel {background-color: rgb(211, 211, "
                            "158);}:hover{background-color: rgb(170,95,127);}");
}
void validate(Tile *temp, int c) {
    int retValue, i;

    if (c == 1) {
        qDebug("c=1,%d %d %d", temp->row, temp->col,temp->pieceColor);
        if (temp->piece && (temp->pieceColor == Game::turn)) {
            // exp[max++]=temp->tileNum;
            retValue = valid->chooser(temp);

            if (retValue) {
                click1 = new Tile();
                temp->setStyleSheet("QLabel {background-color: green;}");
                click1 = temp;
            } else {
                // temp->setStyleSheet("QLabel {background-color: red;}");
                count = 0;
            }
        } else {
            // qDebug()<<"Rascel, clicking anywhere";
            count = 0;
        }
    }

    else {
        qDebug("c=2,%d %d", temp->row, temp->col);
        if (temp->tileNum == click1->tileNum) {
            click1->tileDisplay();
            disOrange();
            max = 0;
            count = 0;
        }

        for (i = 0; i < max; i++) {
            if (temp->tileNum == Exp[i]) {
                char ch = temp->pieceName;
                int color = temp->pieceColor;
                click1->piece = 0;
                temp->piece = 1;
                temp->pieceColor = click1->pieceColor;
                temp->pieceName = click1->pieceName;

                click1->display(click1->pieceName);
                temp->display(click1->pieceName);

                click1->tileDisplay();
                temp->tileDisplay();

                retValue = valid->check(click1);
                disOrange();
                max = 0;

                //turn = (turn + 1) % 2;
                count = 0;
                qDebug("I click %d %d %d %d",temp->row,temp->col,click1->row,click1->col);

                Common::sockthread->sendMsg(SockData::packCod(7-click1->row,7-click1->col,7-temp->row,7-temp->col));
                Game::gamestarted = 1;
                Game::playing = 0;
                Game::m_window->setTurnLabel(Game::playing);

                if(ch == 'K' && color != Game::turn){
                    Game::m_window->checkmate(1);
                }
            }

            else
                count = 1;
        }
    }
}



void disOrange() {
    int i;

    for (i = 0; i < max; i++)
        tile[Exp[i] / 8][Exp[i] % 8]->tileDisplay();
}
