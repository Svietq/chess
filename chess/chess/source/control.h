#pragma once

//controls of the game

#include "types.h"
#include "console.h"

#include <string>
#include <limits>
#include <conio.h>

using namespace std;

class Control
{
public:
    static Piece_coords choose_piece(); //make a choose of the first field where a piece is
    static Piece_coords choose_field(); //make a choose of the second field where a previously chosen piece will be moved
    static Piece_type exchange_piece(); //choose piece that will be exchanged for pawn in pawn promotion

};
