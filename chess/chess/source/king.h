#pragma once

#include "piece.h"

class King  : public Piece
{
public:
    King(Piece_color icolor) : Piece(icolor) {_is_moved = false;}
    King(Piece_coords icoords, Piece_color icolor) : Piece(icoords, icolor) {}

    virtual void move(Piece_coords icoords);
    virtual bool is_move_correct(Piece_coords icoords);

};
