#include "chess_board.h"


Chess_board::Chess_board()
{

}

Field& Chess_board::operator[](const Piece_coords& icoords)
{
    return _board[icoords.y*8 + icoords.x];
}

const Field& Chess_board::operator[](const Piece_coords& icoords) const
{
    return _board[icoords.y*8 + icoords.x];
}

Pieces_board::iterator Chess_board::begin()
{
    return _board.begin();
}

Pieces_board::iterator Chess_board::end()
{
    return _board.end();
}

Pieces_board &Chess_board::get()
{
    return _board;
}
