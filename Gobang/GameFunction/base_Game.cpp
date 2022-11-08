#include "base_Game.h"

/*----------------- base_Game -------------------*/



/* --------- 1. Piece --------- */


Piece::Piece()
{
    color = status_none;
}

int Piece::Piece_colorSet(int input)
{
    if (input != para_default)   // Not default
        color = input;
    return color;
}

int Piece::Piece_xSet(int input)
{
    if (input != para_default)   // Not default
        location_x = input;
    return location_x;
}

int Piece::Piece_ySet(int input)
{
    if (input != para_default)   // Not default
        location_y = input;
    return location_y;
}





/* --------- 2. Board ---------- */

/* Initialize */
Board::Board(int topInterval, int leftInterval, int len, int wid, int inter, int range)
{
    player = color_black; // Black first
    topMargin = topInterval;
    leftMargin = leftInterval;
    length = len;
    width = wid;
    size = inter;
    this->range = range;
    pieces_count = 0;
    win_piece[0] = -1;
    win_piece[1] = -1;
    win_piece[2] = -1;
    pieces = new Piece * [width / size + 1LL];
    for (int i = 0; i <= width / size; i++)
    {
        pieces[i] = new Piece[length / size + 1LL];
        for (int j = 0; j <= length / size; j++)
        {
            //pieces[i][j].Piece_xSet(leftMargin + j * size);
            //pieces[i][j].Piece_ySet(topMargin + i * size);
        }
    }
}

/* Get player */
bool Board::GetPlayer(void)
{
    return player;
}

/* Get topMargin */
int Board::GetTopMargin(void)
{
    return topMargin;
}

/* Get leftMargin */
int Board::GetLeftMargin(void)
{
    return leftMargin;
}

/* Get length */
int Board::GetLength(void)
{
    return length;
}

/* Get width */
int Board::GetWidth(void)
{
    return width;
}

/* Get size */
int Board::GetSize(void)
{
    return size;
}

/* Get range */
int Board::GetRange(void)
{
    return range;
}

/* Get pieces_count */
int Board::GetPiecesCount(void)
{
    return pieces_count;
}

/* Get pieces */
Piece** Board::GetPieces(void)
{
    return pieces;
}


/* Get last_piece */
QStack<QPoint> Board::GetPieceStack(void)
{
    return piece_Stack;
}

/* Get the win piece [record] 0 - method,1 - r, 2 - c (method: -1-none 0-r 1-c 2-dr 3-ur) */
int* Board::GetWinPiece(void)
{
    return win_piece;
}

/* Get color with r and c */
int Board::GetColor(int row, int column)
{
    return pieces[row][column].Piece_colorSet();
}

/* Move */
bool Board::GetMove(int put_row, int put_column)
{
    if (pieces[put_row][put_column].Piece_colorSet() == status_none)
    {
        pieces[put_row][put_column].Piece_colorSet(player);
        player = !player;   // change move
        pieces_count++;
        QPoint point;
        point.setX(put_row);
        point.setY(put_column);
        piece_Stack.push(point);
        return true;
    }
    return false;
}

/* traceback */
void Board::Traceback(void)
{
    QPoint last_point = piece_Stack.top();
    piece_Stack.pop();
    pieces[last_point.x()][last_point.y()].Piece_colorSet(status_none);
    pieces_count--;
    player = !player;   // change move
}

/* Judge if end */
bool Board::obvious_Victory(void)
{
    for (int r = 0; r <= width / size; r++)
        for (int c = 0; c <= length / size; c++)
            if (obvious_Victory_Piece(r, c))
                return true;
    return false;
}

/* Judge if piece(c, r) matches the condition */
bool Board::obvious_Victory_Piece(int r, int c)
{
    if (pieces[r][c].Piece_colorSet() == status_none)
        return false;

    bool connected[4] = { true, true, true, true };

    // row Search
    if (c >= length / size - 3)
        connected[0] = false;
    else
        for (int tmp_c = c + 1; tmp_c < c + 5 && tmp_c <= length / size; tmp_c++)
            if (pieces[r][tmp_c].Piece_colorSet() != pieces[r][c].Piece_colorSet())
            {
                connected[0] = false;
                break;
            }

    // column Search
    if (r >= width / size - 3)
        connected[1] = false;
    else
        for (int tmp_r = r + 1; tmp_r < r + 5 && tmp_r <= width / size; tmp_r++)
            if (pieces[tmp_r][c].Piece_colorSet() != pieces[r][c].Piece_colorSet())
            {
                connected[1] = false;
                break;
            }

    // diagonal1 Search
    if (r >= width / size - 3 || c >= length / size - 3)
        connected[2] = false;
    else
        for (int tmp_r = r + 1, tmp_c = c + 1; tmp_r < r + 5 && tmp_c <= length / size && tmp_r <= width / size;)
            if (pieces[tmp_r++][tmp_c++].Piece_colorSet() != pieces[r][c].Piece_colorSet())
            {
                connected[2] = false;
                break;
            }

    // diagonal2 Search
    if (r <= 3 || c >= length / size - 3)
        connected[3] = false;
    else
        for (int tmp_r = r - 1, tmp_c = c + 1; tmp_c < c + 5 && tmp_c <= length / size && tmp_r >= 0;)
            if (pieces[tmp_r--][tmp_c++].Piece_colorSet() != pieces[r][c].Piece_colorSet())
            {
                connected[3] = false;
                break;
            }

    for(int i = 0; i <= 3; i++)
        if (connected[i])
        {
            win_piece[0] = i;
            win_piece[1] = r;
            win_piece[2] = c;
            return true;
        }
    return false;
}


/* Judge if deadlock */
bool Board::Deadlock(void)
{
    for (int r = 0; r <= width / size; r++)
        for (int c = 0; c <= length / size; c++)
            if(GetColor(r,c) == (int)status_none)
                return false;
    return true;
}
