#pragma once

#include "piece.h"

class Rook : public Piece
{
    Piece_type _type;

public:
    Rook(Piece_color icolor) : Piece(icolor), _type(Piece_type::ROOK) {_is_moved = false;}
    Rook(Piece_coords icoords, Piece_color icolor) : Piece(icoords, icolor), _type(Piece_type::ROOK) {}

    virtual void move(Piece_coords icoords);
    virtual bool is_move_correct(Piece_coords icoords);
    virtual Piece_type get_type();

};
