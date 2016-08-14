#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

class Rook : public Piece
{
    Piece_type _type;

public:
    Rook(Piece_color icolor) : Piece(icolor), _type(Piece_type::ROOK) {_is_moved = false;}

    virtual void move(Piece_coords icoords);
    virtual bool is_move_correct(Piece_coords icoords);
    virtual Piece_type get_type();

};

#endif // ROOK_H
