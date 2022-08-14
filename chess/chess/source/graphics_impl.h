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

        switch(x->get_type())
        {
        case Piece_type::EMPTY:
            cout << "-";
            break;
        case Piece_type::PAWN:
            if(x->get_color()==Piece_color::BLACK)
                cout << "P";
            else cout << "p";
            break;
        case Piece_type::ROOK:
            if(x->get_color()==Piece_color::BLACK)
                cout << "R";
            else cout << "r";
            break;
        case Piece_type::KNIGHT:
            if(x->get_color()==Piece_color::BLACK)
                cout << "K";
            else cout << "k";
            break;
        case Piece_type::BISHOP:
            if(x->get_color()==Piece_color::BLACK)
                cout << "B";
            else cout << "b";
            break;
        case Piece_type::QUEEN:
            if(x->get_color()==Piece_color::BLACK)
                cout << "Q";
            else cout << "q";
            break;
        case Piece_type::KING:
            if(x->get_color()==Piece_color::BLACK)
                cout << "x";
            else cout << "+";
            break;
        }
    }

    //print numbers
    for(int i=1; i<9; i++)
    {
        gotoxy(16,i);
        cout.width(2);
        cout << i << endl;
    }
}
