#pragma once

#include "piece.h"

class King  : public Piece
{
    Piece_type _type;

public:
    King(Piece_color icolor) : Piece(icolor), _type(Piece_type::KING) {_is_moved = false;}
    King(Piece_coords icoords, Piece_color icolor) : Piece(icoords, icolor), _type(Piece_type::KING) {}

    virtual void move(Piece_coords icoords);
    virtual bool is_move_correct(Piece_coords icoords);
    virtual Piece_type get_type();

};
