#pragma once

#include "piece.h"

class Pawn : public Piece
{
public:
    Pawn(Piece_color icolor) : Piece(icolor) { }
    Pawn(Piece_coords icoords, Piece_color icolor) : Piece(icoords, icolor) {}

    virtual void move(Piece_coords icoords);
    virtual bool is_move_correct(Piece_coords icoords);

};