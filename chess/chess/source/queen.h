#pragma once

#include "piece.h"

class Queen  : public Piece
{
public:
    Queen(Piece_color icolor) : Piece(icolor) {}
    Queen(Piece_coords icoords, Piece_color icolor) : Piece(icoords, icolor) {}

    virtual void move(Piece_coords icoords);
    virtual bool is_move_correct(Piece_coords icoords);

};
