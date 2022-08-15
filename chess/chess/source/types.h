#pragma once

//file that holds user-defined types

#include <array>
#include <memory>

class Piece;

struct Piece_coords
{
    int x = 0;
    int y = 0;
};

struct Field
{
    Piece* PiecePtr = nullptr;
    Piece_coords Coords;

    Piece* operator->() { return PiecePtr; }
    const Piece* operator->() const { return PiecePtr; };
    bool is_empty() const { return PiecePtr == nullptr; }
};

enum class Piece_color : bool{WHITE = false, BLACK = true };
inline bool operator!(Piece_color InPieceColor) 
{
    return InPieceColor == static_cast<Piece_color>(false);
}

inline bool operator==(const Piece_coords& lhs, const Piece_coords& rhs)
{
    if(lhs.x==rhs.x && lhs.y==rhs.y)
        return true;

    return false;
}
