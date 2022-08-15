#pragma once

#include "piece.h"

class Pawn : public Piece
{
public:
    Pawn(Piece_color icolor) : Piece(icolor) { }

    virtual void move(Piece_coords icoords) override;
    virtual bool is_move_correct(Piece_coords icoords) override;

};