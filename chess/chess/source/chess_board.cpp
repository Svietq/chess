#include "chess_board.h"


Field& ChessBoard::operator[](const Piece_coords& icoords)
{
    return _board[icoords.y*8 + icoords.x];
}

const Field& ChessBoard::operator[](const Piece_coords& icoords) const
{
    return _board[icoords.y*8 + icoords.x];
}

ChessBoard::Board::iterator ChessBoard::begin()
{
    return _board.begin();
}

ChessBoard::Board::iterator ChessBoard::end()
{
    return _board.end();
}

ChessBoard::Board &ChessBoard::get()
{
    return _board;
}
