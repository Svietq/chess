#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

class Queen  : public Piece
{
    Piece_type _type;

public:
    Queen(Piece_color icolor) : Piece(icolor), _type(Piece_type::QUEEN) {}

    virtual void move(Piece_coords icoords);
    virtual bool is_move_correct(Piece_coords icoords);
    virtual Piece_type get_type();

};

#endif // QUEEN_H
