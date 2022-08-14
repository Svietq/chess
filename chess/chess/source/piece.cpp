#include "piece.h"

Piece::Piece()
{
    _is_moved = false;
}

Piece::~Piece()
{

}

Piece_color Piece::get_color()
{
    return _color;
}

void Piece::set_coords(const Piece_coords &icoords)
{
    _coords = icoords;
}

Piece_coords Piece::get_coords()
{
    return _coords;
}

void Piece::load_moves(const vector<Piece_coords> &imoves)
{
    _moves = imoves;
}

const std::vector<Piece_coords>& Piece::get_moves() const
{
    return _moves;
}

bool Piece::has_moves() const
{
    return get_moves().size() > 0;
}

bool Piece::get_status()
{
    return _is_moved;
}
