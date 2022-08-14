#pragma once

#include "types.h"
#include "piece.h"
#include <iterator>

using namespace std;

class Chess_board
{
    Pieces_board _board; //8x8 array representing the board with pieces
public:
    Chess_board();

    Field& operator[](const Piece_coords &icoords);
    const Field& operator[](const Piece_coords &icoords) const;
    Pieces_board::iterator begin();
    Pieces_board::iterator end();

    Pieces_board &get();
};
