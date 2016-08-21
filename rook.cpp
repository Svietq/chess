#include "rook.h"

void Rook::move(Piece_coords icoords)
{
    if( is_move_correct(icoords))
    {
        _coords = icoords;
        _is_moved = true;
    }
}

bool Rook::is_move_correct(Piece_coords icoords)
{
    if( (_coords.x==icoords.x && _coords.y!=icoords.y) || (_coords.x!=icoords.x && _coords.y==icoords.y) )
    {
        return true;
    }

    return false;
}

Piece_type Rook::get_type()
{
    return _type;
}

bool Rook::is_empty()
{
    return false;
}
