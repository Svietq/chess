#include "knight.h"

void Knight::move(Piece_coords icoords)
{
    if(is_move_correct(icoords))
    {
        _coords = icoords;
    }
}

bool Knight::is_move_correct(Piece_coords icoords)
{
    if((_coords.x+1==icoords.x && _coords.y+2==icoords.y) || (_coords.x+1==icoords.x && _coords.y-2==icoords.y) ||
       (_coords.x-1==icoords.x && _coords.y+2==icoords.y) || (_coords.x-1==icoords.x && _coords.y-2==icoords.y) ||
       (_coords.x+2==icoords.x && _coords.y+1==icoords.y) || (_coords.x+2==icoords.x && _coords.y-1==icoords.y) ||
       (_coords.x-2==icoords.x && _coords.y+1==icoords.y) || (_coords.x-2==icoords.x && _coords.y-1==icoords.y))
    {
        return true;
    }

    return false;
}

Piece_type Knight::get_type()
{
    return _type;
}
