#pragma once

//graphics of the game

#include "console.h"
#include "types.h"
#include "chess_board.h"

#include <vector>
#include <iostream>
#include <array>
#include <memory>

using namespace std;

class Graphics : virtual public Console
{
public:
    Graphics(){}

    template<typename T> 
    static void print_board(array<T, 64> &iboard);
};
#include "graphics_impl.h" //implementation of template function print_board()
