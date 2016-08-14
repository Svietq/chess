#ifndef PIECE_H
#define PIECE_H

#include "types.h"

#include <vector>

using namespace std;

class Piece
{

protected:
    Piece_coords _coords; //coordinates of the piece
    Piece_color _color; //color of the piece
    vector<Piece_coords> _moves; //vector which holds possible moves of the piece
    bool _is_moved = false; //variable that tells if the piece was previously moved

public:
    Piece();
    Piece(Piece_color icolor) : _color(icolor) {_is_moved = false;}
    virtual ~Piece();

    virtual void move(Piece_coords icoords)=0; //set new coordinates of the piece
    virtual bool is_move_correct(Piece_coords icoords)=0; //check if move can be done
    virtual Piece_type get_type()=0;
    Piece_color get_color();
    void set_coords(Piece_coords icoords);
    Piece_coords get_coords();
    void load_moves(const vector<Piece_coords> &imoves); //load all moves that can be made by the piece
    vector<Piece_coords> get_moves();
    bool get_status(); //returns _is_moved variable


};

#endif // PIECE_H
