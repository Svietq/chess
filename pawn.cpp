#include "pawn.h"

void Pawn::move(Piece_coords icoords)
{
    if(is_move_correct(icoords))
    {
        _coords = icoords;
        _is_moved = true;

    }
}

bool Pawn::is_move_correct(Piece_coords icoords)
{
    //check if a pawn was moved previously
    if( (_color==Piece_color::WHITE && _coords.y==1) || (_color==Piece_color::BLACK && _coords.y==6))
        _is_moved = false;

    //move one field ahead:
    if( (_coords.y+1==icoords.y && _coords.x==icoords.x && _color==Piece_color::WHITE) ||
        (_coords.y-1==icoords.y && _coords.x==icoords.x && _color==Piece_color::BLACK) )
    {
        return true;
    }//move one field diagonally:
    else if( (_coords.y+1==icoords.y && (_coords.x+1==icoords.x || _coords.x-1==icoords.x) && _color==Piece_color::WHITE) ||
             (_coords.y-1==icoords.y && (_coords.x+1==icoords.x || _coords.x-1==icoords.x) && _color==Piece_color::BLACK) )
    {
        return true;
    }//move two fields ahead:
    else if( (_coords.y+2==icoords.y && _coords.x==icoords.x && _color==Piece_color::WHITE && _is_moved==false) ||
             (_coords.y-2==icoords.y && _coords.x==icoords.x && _color==Piece_color::BLACK && _is_moved==false) )
    {
        return true;
    }

    return false;
}

Piece_type Pawn::get_type()
{
    return _type;
}
