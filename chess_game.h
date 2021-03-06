#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "chess_board.h"
#include "piece.h"
#include "empty.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"
#include "graphics.h"
#include "control.h"

#include <vector>
#include <array>
#include <algorithm>
#include <iterator>

#define LINE_0 0
#define LINE_7 7
#define COL_H 7
#define COL_G 6
#define COL_F 5
#define COL_E 4
#define COL_D 3
#define COL_C 2
#define COL_B 1
#define COL_A 0

using namespace std;

class Chess_game : public Graphics, public Control
{
private:
    Chess_board _board; //8x8 array representing the board with pieces
    Piece_coords _1st_choice{0,0}; //coordinates of the first chosen field
    Piece_coords _2nd_choice{0,0}; // coordinates of the second chosen field
    bool is_1st_choice_correct(); //check if coordinates of the first choose are correct
    bool is_2nd_choice_correct(); //check if coordinates of the second choose are correct
    Piece_color _current_color{Piece_color::WHITE}; //variable holding value of the current color
    void set_pieces(); //set in board initial position of all pieces

    void make_move(); //choose coordinates of the first and the second field, and then call Piece's function move()
    void switch_fields(); //set piece in it's new position
    bool _is_moved = false; //variable which tells if the movement was made
    void switch_color(); //switch to another color
    bool is_move_correct(); //check if piece is not being blocked by any other piece
        bool check_vert();  // functions called
        bool check_hor();   // by function
        bool check_diag();  // is_move_correct()
    bool is_pawn_move_correct(); //check if pawn's movement is correct
    bool is_castling(); //check if there is a castling
    void castling(); //do the castling
    void check_possible_moves(); //check all moves the all pieces can do
    void show_possible_moves(); //show on the board possible moves of the chosen piece
    bool is_check(); //check if there is check
    bool is_mate(); //check if there is mate
    void undo_turn(); //undo turn
    unique_ptr<Piece> _prev_piece = make_unique<Empty>(); //variable holding piece that was previously taken from the board
    bool is_pawn_promotion(); //check if pawn promotion can be done
    void pawn_promotion(); //do the pawn promotion

public:
    Chess_game();

    void play(); //loop which calls all functions neccessary to play the game

};

#endif // CHESS_GAME_H
