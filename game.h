#ifndef GAME_H
#define GAME_H
#include"tile.h"
#include"mainwindow.h"

class Border {
  public:
    Border();
    void outline(QWidget *baseWidget, int xPos, int yPos, int Pos) {
        QLabel *outLabel = new QLabel(baseWidget);

        if (!Pos)
            outLabel->setGeometry(xPos, yPos, 552, 20); // Horizontal Borders

        else
            outLabel->setGeometry(xPos, yPos, 20, 512); // Vertical Borders

        outLabel->setStyleSheet(
            "QLabel { background-color :rgb(170, 170, 127); color : black; }");
    }
};
class Game
{
public:
    static bool gamestarted;
    static bool gameover;
    static MainWindow * m_window;
    static void chessBoard(MainWindow *baseWidget, Tile *tile[8][8],bool playing);
    static void drawBorder(MainWindow *baseWidget);
};

#endif // GAME_H
