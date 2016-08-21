#include "queen.h"

void Queen::move(Piece_coords icoords)
{
    if(is_move_correct(icoords))
    {
       _coords = icoords;
    }

}

bool Queen::is_move_correct(Piece_coords icoords)
{
    if( (_coords.x==icoords.x && _coords.y!=icoords.y) || (_coords.x!=icoords.x && _coords.y==icoords.y) )
    {
       return true;
    }
    else
    {
        for(int i=1; i<9; i++)
        {
            if( (_coords.x+i==icoords.x && _coords.y+i==icoords.y) || (_coords.x-i==icoords.x && _coords.y+i==icoords.y) ||
                (_coords.x+i==icoords.x && _coords.y-i==icoords.y) || (_coords.x-i==icoords.x && _coords.y-i==icoords.y) )
            {
                return true;
            }
        }
    }

    return false;
}

Piece_type Queen::get_type()
{
    return _type;
}

bool Queen::is_empty()
{
    return false;
}

