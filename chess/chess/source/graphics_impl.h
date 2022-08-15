#pragma once

//implementation of the template function print_board() from the class Graphics

#include "types.h"

template<typename T> void Graphics::print_board(std::array<T, 64> &iboard)
{
    //print letters
    gotoxy(0,0);
    for(char c='a'; c!='i'; c++)
    {
        cout.width(2);
        cout << c;
    }

    //print board
    gotoxy(0,1);
    int n=0;
    for(Field &x : iboard)
    {
        if(n>7)
        {
           n=0;
           cout << endl;
        }
        n++;

        cout.width(2);
        if (x.is_empty())
        {
            cout << "-";
            continue;
        }

        char pieceSymbol = 'e';
        if (x->IsA<Pawn>())
        {
            pieceSymbol = 'p';
        }
        else if (x->IsA<Rook>())
        {
            pieceSymbol = 'r';
        }
        else if (x->IsA<Knight>())
        {
            pieceSymbol = 'n';
        }
        else if (x->IsA<Bishop>())
        {
            pieceSymbol = 'b';
        }
        else if (x->IsA<Queen>())
        {
            pieceSymbol = 'q';
        }
        else if (x->IsA<King>())
        {
            pieceSymbol = 'k';
        }

        if (x->get_color() == Piece_color::BLACK)
        {
            pieceSymbol = std::toupper(pieceSymbol);
        }

        std::cout << pieceSymbol;
    }

    //print numbers
    for(int i=1; i<9; i++)
    {
        gotoxy(16,i);
        cout.width(2);
        cout << i << endl;
    }
}
