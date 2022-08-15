#pragma once

#include "chess_board.h"

class ChessGame
{
public:
    void play();

private:
    void initialize_board();
    bool has_game_ended();
    void check_possible_moves();
    void make_move();
    void switch_fields();
    void switch_color();

    class Chess_board _board;
    struct Piece_coords _1st_choice{0,0};
    struct Piece_coords _2nd_choice{0,0};
    bool is_1st_choice_correct();
    bool is_2nd_choice_correct();
    Piece_color _current_color{Piece_color::WHITE};

    
    
    bool _is_moved = false;
    
    bool is_move_correct();
        bool check_vert();
        bool check_hor();
        bool check_diag();
    bool is_pawn_move_correct();
    bool is_castling();
    void castling();
    
    void show_possible_moves();
    bool is_check();
    void undo_turn();

    class Piece* _prev_piece = nullptr;

    bool is_pawn_promotion();
    void pawn_promotion();

    Field& GetField(Piece_coords InPieceCoords);
    const Field& GetField(Piece_coords InPieceCoords) const;

};
