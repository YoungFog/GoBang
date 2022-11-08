#pragma once
#include <qpoint.h>
#include <qstack.h>

// color Status
constexpr auto color_black = true;
constexpr auto color_white = false;
constexpr auto status_none = -1;

// parameter
constexpr auto para_default = -100;


class Piece
{
public:
    Piece();
    int Piece_colorSet(int value = para_default);
    int Piece_xSet(int value = para_default);
    int Piece_ySet(int value = para_default);

private:
    int color;
    int location_x = 0;
    int location_y = 0;

};


class Board
{
public:
    Board(int topInterval = 190, int leftInterval = 610, int len = 700, int wid = 700, int pieceInterval = 50, int range = 18);
    bool GetPlayer();   // Get player
    int GetTopMargin(); // Get topMargin
    int GetLeftMargin(); // Get leftMargin
    int GetLength(); // Get length
    int GetWidth(); // Get width
    int GetSize(); // Get size
    int GetRange(); // Get range
    int GetPiecesCount();    // get pieces_count
    Piece** GetPieces();  // Get pieces
    QStack<QPoint> GetPieceStack();  // Get piece_Stack
    int* GetWinPiece();  // Get the win piece [record] 0 - method,1 - r, 2 - c (method: -1-none 0-r 1-c 2-dr 3-ur)
    int GetColor(int row, int column); //Get color with r and c
    bool GetMove(int row, int column);   // put piece on board
    void Traceback(void); // traceback
    bool obvious_Victory(); // Judge if end
    bool Deadlock();    // Judge if deadlock
    

protected:
    bool obvious_Victory_Piece(int row, int column); // Judge if piece(x(column), y(row)) matches the condition

private:
    /* Style */
    int topMargin;
    int leftMargin;
    int length;
    int width;
    int size;   // Interval bettween pieces
    int range;  // Allow to click

    /* Member */
    Piece** pieces; // pieces

    /* Status */
    bool player;
    int pieces_count;
    QStack<QPoint> piece_Stack;
    int win_piece[3];   // 0 - method,1 - r, 2 - c (method: -1-none 0-r 1-c 2-dr 3-ur)

};




