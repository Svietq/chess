#include "chess_game.h"

#include "piece.h"

#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

#include "types.h"
#include "graphics.h"
#include "control.h"


constexpr int LINE_0 = 0;
constexpr int LINE_7 = 7;
constexpr int COL_H = 7;
constexpr int COL_G = 6;
constexpr int COL_F = 5;
constexpr int COL_E = 4;
constexpr int COL_D = 3;
constexpr int COL_C = 2;
constexpr int COL_B = 1;
constexpr int COL_A = 0;

template<Piece_color Color>
struct Pieces 
{
    Rook LeftRook = Rook{Color};
    Rook RightRook = Rook{Color};
    Knight LeftKnight = Knight{Color};
    Knight RightKnight = Knight{Color};
    Bishop LeftBishop = Bishop{Color};
    Bishop RightBishop = Bishop{Color};
    Queen QueenPiece = Queen{Color};
    King KingPiece = King{Color};

    std::array<Pawn, 8> Pawns
    {
        Pawn{Color},
        Pawn{Color},
        Pawn{Color},
        Pawn{Color},
        Pawn{Color},
        Pawn{Color},
        Pawn{Color},
        Pawn{Color}
    };
};


void ChessGame::play()
{
    initialize_board();

    while(!has_game_ended())
    {
        Graphics::print_board(_board.get());
        check_possible_moves();
        make_move();

        switch_fields();
        if (_is_moved)
        {
            switch_color();
        }
    }

    Graphics::print_board(_board.get());
}

Field& ChessGame::GetField(Piece_coords InPieceCoords)
{
    return _board[InPieceCoords];
}

const Field& ChessGame::GetField(Piece_coords InPieceCoords) const
{
    return _board[InPieceCoords];
}

bool ChessGame::is_1st_choice_correct()
{
    const bool isEmpty = GetField(_1st_choice).is_empty();

    if (!isEmpty)
    {
        const bool isCurrentColor = (GetField(_1st_choice)->get_color() == _current_color);
        
        return isCurrentColor;
    }

    return false;
}

bool ChessGame::is_2nd_choice_correct()
{
    const bool isEmpty = GetField(_2nd_choice).is_empty();

    if(!isEmpty)
    {
        const bool isCurrentColor = (GetField(_2nd_choice)->get_color() == _current_color);

        return !isCurrentColor;
    }

    return true;
}

void ChessGame::initialize_board()
{
    static Pieces<Piece_color::WHITE> WhitePieces;
    static Pieces<Piece_color::BLACK> BlackPieces;

    for(int i=0; i<8; i++)
    {
        GetField({i, 1}).PiecePtr = &WhitePieces.Pawns[i];
        GetField({i, 6}).PiecePtr = &BlackPieces.Pawns[i];
    }

    GetField({ 0, 0 }).PiecePtr = &WhitePieces.LeftRook;
    GetField({ 7, 0 }).PiecePtr = &WhitePieces.RightRook;
    GetField({ 1, 0 }).PiecePtr = &WhitePieces.LeftKnight;
    GetField({ 6, 0 }).PiecePtr = &WhitePieces.RightKnight;
    GetField({ 2, 0 }).PiecePtr = &WhitePieces.LeftBishop;
    GetField({ 5, 0 }).PiecePtr = &WhitePieces.RightBishop;
    GetField({ 3, 0 }).PiecePtr = &WhitePieces.QueenPiece;
    GetField({ 4, 0 }).PiecePtr = &WhitePieces.KingPiece;

    GetField({ 0, 7 }).PiecePtr = &BlackPieces.LeftRook;
    GetField({ 7, 7 }).PiecePtr = &BlackPieces.RightRook;
    GetField({ 1, 7 }).PiecePtr = &BlackPieces.LeftKnight;
    GetField({ 6, 7 }).PiecePtr = &BlackPieces.RightKnight;
    GetField({ 2, 7 }).PiecePtr = &BlackPieces.LeftBishop;
    GetField({ 5, 7 }).PiecePtr = &BlackPieces.RightBishop;
    GetField({ 3, 7 }).PiecePtr = &BlackPieces.QueenPiece;
    GetField({ 4, 7 }).PiecePtr = &BlackPieces.KingPiece;

    int i = 0;
    int j = 0;

    for(Field &x : _board)
    {
        if (j>7)
        {
            j=0;
            ++i;
        }

        x.Coords = Piece_coords{j,i};
        if (!x.is_empty())
        {
            x->set_coords(x.Coords);
        }
        ++j;
    }
}

void ChessGame::make_move()
{
    //choose first field:
    do
    {
        _1st_choice = Control::choose_piece();
    }
    while(!is_1st_choice_correct() || !GetField(_1st_choice)->has_moves());

    show_possible_moves();

    //choose second field:
    do
    {
        _2nd_choice = Control::choose_field();
    }
    while(!is_2nd_choice_correct());

    //pawn promotion:
    if(is_pawn_promotion() && GetField(_2nd_choice).is_empty())
    {
        pawn_promotion();
        swap(GetField(_1st_choice), GetField(_2nd_choice));
        GetField(_1st_choice)->set_coords(_1st_choice);
        GetField(_2nd_choice)->set_coords(_2nd_choice);
    }

    //move the piece:
    if (is_move_correct() && !is_pawn_promotion())
    {
        GetField(_1st_choice)->move(_2nd_choice);
    }
}

void ChessGame::switch_fields()
{
    //castling:
    if(is_castling())
    {
        castling();
        _is_moved = true;
        return;
    }

    //moving a piece to a new place
    if(GetField(_2nd_choice).Coords == GetField(_1st_choice)->get_coords())
    {
        Field temp;

        std::swap(GetField(_1st_choice).PiecePtr, GetField(_2nd_choice).PiecePtr);
        GetField(_2nd_choice)->set_coords(_2nd_choice);
        
        //capturing:
        if (!GetField(_1st_choice).is_empty())
        {
            temp = GetField(_1st_choice);
            GetField(_1st_choice).PiecePtr = nullptr;
        }        

        check_possible_moves();//reload moves to check if king is being attacked

        //if there is check then return to previous configuration of the board
        if(is_check())
        {
            swap(GetField(_1st_choice), GetField(_2nd_choice));
            GetField(_1st_choice)->set_coords(_1st_choice);
            GetField(_2nd_choice)->set_coords(_2nd_choice);

            if(!temp.is_empty())
            {
                swap(temp, GetField(_2nd_choice));
                GetField(_2nd_choice)->set_coords(_2nd_choice);
            }
            _is_moved = false;

        }
        else
        {
            _prev_piece = temp.PiecePtr;
            _is_moved = true;
        }
    }
    else
    {
        _is_moved = false;
    }
}

void ChessGame::switch_color()
{
    _current_color = static_cast<Piece_color>(!_current_color);
}

bool ChessGame::is_move_correct()
{
    //check if any piece except knight is not being blocked by other piece:
    if(!GetField(_1st_choice)->IsA<Knight>())
    {
        if(_1st_choice.x==_2nd_choice.x || _1st_choice.y==_2nd_choice.y)
        {
            if(check_vert())
                return false;

            if(check_hor())
                return false;
        }
        else
        {
            if(check_diag())
                return false;
        }

        //check movement for a pawn:
        if(GetField(_1st_choice)->IsA<Pawn>())
        {
            return is_pawn_move_correct();
        }

        return true;
    }

    return true;
}

bool ChessGame::check_vert()
{
    int x1 = _1st_choice.x;
    int y1 = _1st_choice.y;
    int x2 = _2nd_choice.x;
    int y2 = _2nd_choice.y;

    //check downwards:
    for(int i=y1+1; i<y2; i++)
    {
        if( !GetField({x1, i}).is_empty() && x1 == x2)
            return true;
    }

    //check upwards:
    for(int i=y1-1; i>y2; i--)
    {
        if( !GetField({x1,i}).is_empty() && x1 == x2)
            return true;
    }

    return false;
}

bool ChessGame::check_hor()
{
    int x1 = _1st_choice.x;
    int y1 = _1st_choice.y;
    int x2 = _2nd_choice.x;
    int y2 = _2nd_choice.y;

    //from left to right:
    for(int i=x1+1; i<x2; i++)
    {
        if( !GetField({i,y1}).is_empty() && y1==y2)
            return true;
    }

    //from right to left:
    for(int i=x1-1; i>x2; i--)
    {
        if( !GetField({i,y1}).is_empty() && y1==y2)
            return true;
    }

    return false;
}

bool ChessGame::check_diag()
{
    int x1 = _1st_choice.x;
    int y1 = _1st_choice.y;
    int x2 = _2nd_choice.x;
    int y2 = _2nd_choice.y;

    //to bottom-right:
    {
    int j=y1+1;
    for(int i=x1+1; i<x2 && j<y2; i++,j++)
    {
        if(!GetField({i,j}).is_empty())
            return true;
    }
    }

    //to up-left:
    {
    int j=y1-1;
    for(int i=x1-1; i>x2 && j>y2; i--,j--)
    {
        if(!GetField({i,j}).is_empty())
            return true;
    }
    }

    //to bottom-left:
    {
    int j=y1+1;
    for(int i=x1-1; i>x2 && j<y2; i--,j++)
    {
        if(!GetField({i,j}).is_empty())
            return true;
    }
    }

    //to up-right:
    {
    int j=y1-1;
    for(int i=x1+1; i<x2 && j>y2; i++,j--)
    {
        if(!GetField({i,j}).is_empty())
            return true;
    }
    }

    return false;
}

bool ChessGame::is_pawn_move_correct()
{
    int x1 = _1st_choice.x;
    int y1 = _1st_choice.y;
    int x2 = _2nd_choice.x;
    int y2 = _2nd_choice.y;

    if(GetField(_1st_choice)->get_color()==Piece_color::WHITE)
    {
        if(y2==y1+1 && (x2==x1+1 || x2==x1-1) && GetField(_2nd_choice).is_empty() )
            return false;
        else if(y2==y1+1 && x2==x1 && !GetField(_2nd_choice).is_empty())
            return false;
        else if(y1==1 && y2==3 && x2==x1 && !GetField(_2nd_choice).is_empty())
            return false;
    }
    else if(GetField(_1st_choice)->get_color()==Piece_color::BLACK)
    {
        if(y2==y1-1 && (x2==x1+1 || x2==x1-1) && GetField(_2nd_choice).is_empty())
            return false;
        else if(y2==y1-1 && x2==x1 && !GetField(_2nd_choice).is_empty())
            return false;
        else if(y1==6 && y2==4 && x2==x1 && !GetField(_2nd_choice).is_empty())
            return false;
    }

    return true;
}

bool ChessGame::is_castling()
{
    int x1 = _1st_choice.x;
    int y1 = _1st_choice.y;
    int x2 = _2nd_choice.x;
    int y2 = _2nd_choice.y;

    if(GetField(_1st_choice)->IsA<King>() && GetField(_1st_choice)->get_status()==false && x1==COL_E)
    {
        if(y2==y1 && x2==COL_G && GetField({COL_H,y1})->IsA<Rook>() && GetField({COL_F,y1}).is_empty()
           && GetField({COL_G,y1}).is_empty() && GetField({COL_H,y1})->get_status()==false)
            return true;
        else if(y2==y1 && x2==COL_C && GetField({COL_A,y1})->IsA<Rook>() && GetField({COL_B,y1}).is_empty()
             && GetField({COL_C,y1}).is_empty() && GetField({COL_D,y1}).is_empty()
             && GetField({COL_A,y1})->get_status()==false)
            return true;
    }

    return false;
}

void ChessGame::castling()
{
//    int x1 = _1st_choice.x;
    int y1 = _1st_choice.y;
    int x2 = _2nd_choice.x;
//    int y2 = _second_choose.y;

    if(x2==COL_G)
    {
        swap(GetField(_1st_choice), GetField({COL_G,y1}));
        GetField(_1st_choice)->set_coords(_1st_choice);
        GetField({COL_G,y1})->set_coords({COL_G,y1});

        swap(GetField({COL_H,y1}), GetField({COL_F,y1}));
        GetField({COL_H,y1})->set_coords({COL_H,y1});
        GetField({COL_F,y1})->set_coords({COL_F,y1});
    }
    else if(x2==COL_C)
    {
        swap(GetField(_1st_choice), GetField({COL_C,y1}));
        GetField(_1st_choice)->set_coords(_1st_choice);
        GetField({COL_C,y1})->set_coords({COL_C,y1});

        swap(GetField({COL_A,y1}), GetField({COL_D,y1}));
        GetField({COL_A,y1})->set_coords({COL_A,y1});
        GetField({COL_D,y1})->set_coords({COL_D,y1});
    }

}

void ChessGame::check_possible_moves()
{
    for(Field &x: _board)
    {
        if(!x.is_empty())
        {
            vector<Piece_coords> moves;

            Piece_color temp_c = _current_color;
            if(x->get_color()!=_current_color)
                switch_color();

            Piece_coords temp_f = _1st_choice;
            Piece_coords temp_s = _2nd_choice;

            _1st_choice = x->get_coords();

            for (const Field &n : _board)
            {
                _2nd_choice = n.Coords;

                if( ( is_2nd_choice_correct() && x->is_move_correct(_2nd_choice) && is_move_correct() && !is_check() ) ||
                    ( is_castling() && !is_check() ) )
                {
                    moves.push_back(_2nd_choice);
                }
            }

            _current_color = temp_c;
            _1st_choice = temp_f;
            _2nd_choice = temp_s;

            x->load_moves(moves);
        }
    }
}



void ChessGame::show_possible_moves()
{
    vector<Piece_coords> v = GetField(_1st_choice)->get_moves();

    for(Piece_coords n : v)
    {
        Console::gotoxy(n.x*2+1,n.y+1);
        cout << "O";
    }
}

bool ChessGame::is_check()
{
    Piece_coords kings_coords{0,0};

    for(Field &x: _board)
    {
        if(!x.is_empty() && x->IsA<King>() && x->get_color()==_current_color)
             kings_coords = x->get_coords();
    }

    for(Field &x: _board)
    {
        if(!x.is_empty() && x->get_color()!=_current_color)
        {
            vector<Piece_coords> moves = x->get_moves();

            auto p = find(begin(moves),end(moves),kings_coords);
            if(p != std::end(moves))
                return true;
        }
    }

    return false;
}

bool ChessGame::has_game_ended()
{
    //try to move each piece of current color, if there are no possible moves then return true

    //TODO: currently only checking for mate, implement other ways of finishing game:
    
    if(is_check())
    {
        for(Field &x: _board)
        {
            if( !x.is_empty() && x->get_color()==_current_color)
            {
                vector<Piece_coords> moves = x->get_moves();

                for(Piece_coords n : moves)
                {
                     _1st_choice = x->get_coords();
                     x->move(n); //change x's coords
                     _2nd_choice = x->get_coords();

                     switch_fields();

                     if(_is_moved)
                     {
                         undo_turn();
                         return false;
                     }

                }
            }
        }

        return true;
    }

    return false;
}

void ChessGame::undo_turn()
{
    std::swap(GetField(_1st_choice), GetField(_2nd_choice));
    GetField(_1st_choice)->set_coords(_1st_choice);
    GetField(_2nd_choice)->set_coords(_2nd_choice);

    if( !_prev_piece)
    {
        _prev_piece = GetField(_2nd_choice).PiecePtr;

        GetField(_2nd_choice)->set_coords(_2nd_choice);
    }
}

void ChessGame::pawn_promotion()
{
    //if(is_pawn_promotion())
    //{
    //    switch(Control::exchange_piece())
    //    {
    //    //case Piece_type::QUEEN:
    //    //    _board[_1st_choice] = make_unique<Queen>(_current_color);
    //    //        _board[_1st_choice]->set_coords(_1st_choice);
    //    //    break;
    //    //case Piece_type::BISHOP:
    //    //    _board[_1st_choice] = make_unique<Bishop>(_current_color);
    //    //        _board[_1st_choice]->set_coords(_1st_choice);
    //    //    break;
    //    //case Piece_type::KNIGHT:
    //    //    _board[_1st_choice] = make_unique<Knight>(_current_color);
    //    //        _board[_1st_choice]->set_coords(_1st_choice);
    //    //    break;
    //    //case Piece_type::ROOK:
    //    //    _board[_1st_choice] = make_unique<Rook>(_current_color);
    //    //        _board[_1st_choice]->set_coords(_1st_choice);
    //    //    break;
    //    //default:
    //    //    break;
    //    }
    //}

}

bool ChessGame::is_pawn_promotion()
{
    if(GetField(_1st_choice)->IsA<Pawn>())
    {
        if( (GetField(_1st_choice)->get_color() == Piece_color::BLACK && _2nd_choice.y == LINE_0) ||
            (GetField(_1st_choice)->get_color() == Piece_color::WHITE && _2nd_choice.y == LINE_7) )
        {
            return true;
        }
    }

    return false;
}
