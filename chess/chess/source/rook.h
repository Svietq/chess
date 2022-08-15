#pragma once

#include "piece.h"

class Rook : public Piece
{
public:
    Rook(Piece_color icolor) : Piece(icolor) {_is_moved = false;}
    Rook(Piece_coords icoords, Piece_color icolor) : Piece(icoords, icolor) {}

    virtual void move(Piece_coords icoords);
    virtual bool is_move_correct(Piece_coords icoords);

};
