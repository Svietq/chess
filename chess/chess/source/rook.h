#pragma once

#include "piece.h"

class Rook : public Piece
{
public:
    Rook(Piece_color icolor) : Piece(icolor) {}

    virtual void move(Piece_coords icoords) override;
    virtual bool is_move_correct(Piece_coords icoords) override;

};
