#ifndef KING_H
#define KING_H

#include "piece.h"

class King  : public Piece
{
    Piece_type _type;

public:
    King(Piece_color icolor) : Piece(icolor), _type(Piece_type::KING) {_is_moved = false;}

    virtual void move(Piece_coords icoords);
    virtual bool is_move_correct(Piece_coords icoords);
    virtual Piece_type get_type();
};

#endif // KING_H
