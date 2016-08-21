#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

class Pawn : public Piece
{
    Piece_type _type;

public:
    Pawn(Piece_color icolor) : Piece(icolor), _type(Piece_type::PAWN) { }
    Pawn(Piece_coords icoords, Piece_color icolor) : Piece(icoords, icolor), _type(Piece_type::PAWN) {}

    virtual void move(Piece_coords icoords);
    virtual bool is_move_correct(Piece_coords icoords);
    virtual Piece_type get_type();
    virtual bool is_empty();

};

#endif // PAWN_H
