#pragma once

//file that holds user-defined types

#include <array>
#include <memory>

class Piece;
using Pieces_board = std::array<std::unique_ptr<Piece>, 64 >;
enum class Piece_color : bool{WHITE = false, BLACK = true };
inline bool operator!(Piece_color InPieceColor) 
{
    return InPieceColor == static_cast<Piece_color>(false);
}
enum class Piece_type : int{EMPTY=0, PAWN=1, KNIGHT=2, BISHOP=3, ROOK=4, QUEEN=5, KING=6 };
struct Piece_coords
{
    int x;
    int y;
};

inline bool operator==(const Piece_coords& lhs, const Piece_coords& rhs)
{
    if(lhs.x==rhs.x && lhs.y==rhs.y)
        return true;

    return false;
}
