#include "game.h"
MainWindow* Game::m_window=nullptr;
bool Game::gameover=0;
bool Game::gamestarted=0;
void Game::chessBoard(MainWindow *baseWidget, Tile *tile[8][8],bool playing) {
    int i, j, k = 0, hor, ver;

    // Create 64 tiles (allocating memories to the objects of Tile class)
    ver = 125;
    for (i = 0; i < 8; i++) {
        hor = 350;
        for (j = 0; j < 8; j++) {
            if(tile[i][j]==nullptr)
            tile[i][j] = new Tile(baseWidget);
            tile[i][j]->tileColor = (i + j) % 2;
            tile[i][j]->piece = 0;
            tile[i][j]->row = i;
            tile[i][j]->col = j;
            tile[i][j]->tileNum = k++;
            tile[i][j]->clear();

            tile[i][j]->tileDisplay();

            tile[i][j]->setGeometry(hor, ver, 64, 64);
            hor += 64;
        }
        ver += 64;
    }

    // white pawns
    for (j = 0; j < 8; j++) {
        tile[1][j]->piece = 1;
        tile[1][j]->pieceColor = !playing;
        tile[1][j]->display('P');
    }

    // black pawns
    for (j = 0; j < 8; j++) {
        tile[6][j]->piece = 1;
        tile[6][j]->pieceColor = playing;
        tile[6][j]->display('P');
    }

    // white and black remaining elements
    for (j = 0; j < 8; j++) {
        tile[0][j]->piece = 1;
        tile[0][j]->pieceColor = !playing;
        tile[7][j]->piece = 1;
        tile[7][j]->pieceColor = playing;
    }
    tile[0][0]->display('R');
    tile[0][1]->display('H');
    tile[0][2]->display('B');
    tile[0][5]->display('B');
    tile[0][6]->display('H');
    tile[0][7]->display('R');

    tile[7][0]->display('R');
    tile[7][1]->display('H');
    tile[7][2]->display('B');
    tile[7][5]->display('B');
    tile[7][6]->display('H');
    tile[7][7]->display('R');
    if(playing){
        tile[0][3]->display('Q');
        tile[0][4]->display('K');
        tile[7][3]->display('Q');
        tile[7][4]->display('K');
    }else{
        tile[0][3]->display('K');
        tile[0][4]->display('Q');

        tile[7][3]->display('K');
        tile[7][4]->display('Q');
    }
    qDebug("initing");
}
void Game::drawBorder(MainWindow *baseWidget){

    Border *border[4] = {NULL};

    // borderDisplay
    {
        border[0]->outline(baseWidget, 330, 105, 0);
        border[1]->outline(baseWidget, 330, 637, 0);
        border[2]->outline(baseWidget, 330, 125, 1);
        border[2]->outline(baseWidget, 862, 125, 1);
    }
}
