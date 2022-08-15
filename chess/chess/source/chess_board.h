#pragma once

#include "types.h"
#include "piece.h"
#include <iterator>

using namespace std;

class ChessBoard
{
public:
    using Board = std::array<Field, 64>;

    Field& operator[](const Piece_coords &icoords);
    const Field& operator[](const Piece_coords &icoords) const;
    Board::iterator begin();
    Board::iterator end();
    Board& get();

private:
    Board _board;
};
