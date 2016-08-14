#ifndef TYPES_H
#define TYPES_H

//file that holds user-defined types

enum class Piece_color : bool{WHITE=0, BLACK=1};
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

#endif // TYPES_H
