#include "bishop.h"

void Bishop::move(Piece_coords icoords)
{
    if(is_move_correct(icoords))
    {
        _coords = icoords;
    }
}

bool Bishop::is_move_correct(Piece_coords icoords)
{
    for(int i=1; i<9; i++)
    {
        if( (_coords.x+i==icoords.x && _coords.y+i==icoords.y) || (_coords.x-i==icoords.x && _coords.y+i==icoords.y) ||
            (_coords.x+i==icoords.x && _coords.y-i==icoords.y) || (_coords.x-i==icoords.x && _coords.y-i==icoords.y) )
        {
            return true;
        }
    }

    return false;
}

Piece_type Bishop::get_type()
{
    return _type;
}
