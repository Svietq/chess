#ifndef EMPTY_H
#define EMPTY_H

//subclass of class piece which represents empty field of the board

#include "piece.h"

class Empty : public Piece
{
    Piece_type _type;
public:
    Empty() : Piece(), _type(Piece_type::EMPTY) {}
    Empty(Piece_coords icoords) : Piece(icoords), _type(Piece_type::EMPTY) {}

    virtual Piece_type get_type();
    virtual void move(Piece_coords icoords) {}
    virtual bool is_move_correct(Piece_coords icoords) { return false; }
    virtual bool is_empty();
};

#endif // EMPTY_H
