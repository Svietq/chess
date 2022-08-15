#pragma once

#include "piece.h"

class Knight : public Piece
{
public:
    Knight(Piece_color icolor) : Piece(icolor) {}
    Knight(Piece_coords icoords, Piece_color icolor) : Piece(icoords, icolor) {}

    virtual void move(Piece_coords icoords);
    virtual bool is_move_correct(Piece_coords icoords);

};
