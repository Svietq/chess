#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

class Bishop : public Piece
{
    Piece_type _type;

public:
    Bishop(Piece_color icolor) : Piece(icolor), _type(Piece_type::BISHOP) {}

    virtual void move(Piece_coords icoords);
    virtual bool is_move_correct(Piece_coords icoords);
    virtual Piece_type get_type();
};

#endif // BISHOP_H
