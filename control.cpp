#include "control.h"

Control::Control()
{

}

Piece_coords Control::choose_piece()
{
    gotoxy(39,1);
    cout << "                         ";
    gotoxy(39,2);
    cout << "                         ";
    gotoxy(20,1);
    string s="  ";
    while((cout << "Give field's name: " && !(cin >> s)) || s.length()!=2 || (s[0]<97 || s[0]>104) || (s[1]<49 || s[1]>56) )
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        gotoxy(39,1);
        cout << "                         ";
        gotoxy(20,1);
    }

    Piece_coords p{int(s[0])-97,s[1]-49};

    return p;
}

Piece_coords Control::choose_field()
{
    gotoxy(39,2);
    cout << "                         ";
    gotoxy(20,2);
    string s="  ";
    while((cout << "Give field's name: " && !(cin >> s)) || s.length()!=2 || (s[0]<97 || s[0]>104) || (s[1]<49 || s[1]>56) )
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        gotoxy(39,2);
        cout << "                         ";
        gotoxy(20,2);
    }

    Piece_coords p{int(s[0])-97,s[1]-49};

    return p;
}

Piece_type Control::exchange_piece()
{
    gotoxy(20,3);
    cout << "                         ";
    gotoxy(20,3);
    string s="  ";
    while((cout << "Choose piece (Q,B,K,R): " && !(cin >> s)) || s.length()!=1 || (s[0]!=81 && s[0]!=66 && s[0]!=75 && s[0]!=82) )
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        gotoxy(44,3);
        cout << "                         ";
        gotoxy(20,3);
    }

    Piece_type t = Piece_type::EMPTY;

    switch (s[0])
    {
    case 'Q':
        t = Piece_type::QUEEN;
        break;
    case 'B':
        t = Piece_type::BISHOP;
        break;
    case 'K':
        t = Piece_type::KNIGHT;
        break;
    case 'R':
        t = Piece_type::ROOK;
        break;
    default:
        break;
    }

    gotoxy(20,3);
    cout << "                         ";

    return t;

}

