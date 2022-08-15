#pragma once

#include "types.h"

#include <vector>

using namespace std;

class Piece
{
public:
    Piece(Piece_color icolor) : _color(icolor) {}
    virtual ~Piece() {};

    virtual void move(Piece_coords icoords) = 0;
    virtual bool is_move_correct(Piece_coords icoords) = 0;

    Piece_color get_color();
    void set_coords(const Piece_coords &icoords);
    Piece_coords get_coords();
    void load_moves(const vector<Piece_coords> &imoves);
    const vector<Piece_coords>& get_moves() const;
    bool has_moves() const;
    bool get_status();

    template <typename PieceType>
    bool IsA() 
    {
        return dynamic_cast<PieceType*>(this) != nullptr;
    }

protected:
    Piece_coords _coords;
    Piece_color _color;
    vector<Piece_coords> _moves;
    bool _is_moved = false;

};
