#include "king.h"


void King::move(Piece_coords icoords)
{
    if(is_move_correct(icoords))
    {
        _coords = icoords;
        _is_moved = true;
    }
}

bool King::is_move_correct(Piece_coords icoords)
{
    if( (_coords.x+1==icoords.x && _coords.y+1==icoords.y) || (_coords.x-1==icoords.x && _coords.y-1==icoords.y) ||
        (_coords.x+1==icoords.x && _coords.y==icoords.y) || (_coords.x==icoords.x && _coords.y+1==icoords.y) ||
        (_coords.x+1==icoords.x && _coords.y-1==icoords.y) || (_coords.x-1==icoords.x && _coords.y+1==icoords.y) ||
        (_coords.x-1==icoords.x && _coords.y==icoords.y) || (_coords.x==icoords.x && _coords.y-1==icoords.y)
        /*(_coords.x+2==icoords.x && _coords.y==icoords.y) || (_coords.x-2==icoords.x && _coords.y==icoords.y)*/ )
    {
        return true;
    }

    return false;
}

Piece_type King::get_type()
{
    return _type;
}

