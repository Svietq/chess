#ifndef GRAPHICS_H
#define GRAPHICS_H

//graphics of the game

#include "console.h"
#include "types.h"

#include <vector>
#include <iostream>
#include <array>
#include <memory>

using namespace std;

class Graphics : virtual public Console
{
public:
    Graphics(){}

    template<typename T> void print_board(const array<array<unique_ptr<T>, 8>, 8> &iboard);
};
#include "graphics_impl.h" //implementation of template function print_board()

#endif // GRAPHICS_H
