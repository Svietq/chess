#include "chess_game.h"

Chess_game::Chess_game()
{

}

void Chess_game::play()
{
    set_pieces();

    while(!is_mate())
    {
        print_board(_board.get());
        check_possible_moves();
        make_move();

        switch_fields();
        if(_is_moved)
            switch_color();
    }
    print_board(_board.get());
}

bool Chess_game::is_1st_choice_correct()
{
    if(_board[_1st_choice]->is_empty())
        return false;

    if(_board[_1st_choice]->get_color()!=_current_color)
        return false;

    return true;
}

bool Chess_game::is_2nd_choice_correct()
{
    if( !_board[_2nd_choice]->is_empty() && _board[_2nd_choice]->get_color()==_current_color)
    {
        return false;
    }
    else if(_board[_2nd_choice]->is_empty())
    {
        return true;
    }

    return true;
}

void Chess_game::set_pieces()
{
    //set empty fields:
    for(auto &x : _board)
    {
        x = make_unique<Empty>();
    }

    //set pawns:
    for(int i=0; i<8; i++)
    {
        _board[{i,1}] = make_unique<Pawn>(Piece_color::WHITE);
        _board[{i,6}] = make_unique<Pawn>(Piece_color::BLACK);
    }

    //set the rest of pieces:
    {
    Piece_color col = Piece_color::WHITE;
    for(int n=0; n<=7; n+=7)
    {
        _board[{0,n}] = make_unique<Rook>(col);
        _board[{7,n}] = make_unique<Rook>(col);
        _board[{1,n}] = make_unique<Knight>(col);
        _board[{6,n}] = make_unique<Knight>(col);
        _board[{2,n}] = make_unique<Bishop>(col);
        _board[{5,n}] = make_unique<Bishop>(col);
        _board[{3,n}] = make_unique<Queen>(col);
        _board[{4,n}] = make_unique<King>(col);

        col = Piece_color::BLACK;
    }
    }

    //set coords:
    {
    int i=0,j=0;
    for(auto &x : _board)
    {
        if(j>7)
        {
            j=0;
            ++i;
        }
        x->set_coords({j,i});
        ++j;
    }
    }
}

void Chess_game::make_move()
{
    //choose first field:
    do
    {
        _1st_choice = choose_piece();
    }
    while(!is_1st_choice_correct() || _board[_1st_choice]->get_moves().size()==0);

    show_possible_moves();

    //choose second field:
    do
    {
        _2nd_choice = choose_field();
    }
    while(!is_2nd_choice_correct());

    //pawn promotion:
    if(is_pawn_promotion() && _board[_2nd_choice]->is_empty())
    {
        pawn_promotion();
        swap(_board[_1st_choice], _board[_2nd_choice]);
            _board[_1st_choice]->set_coords(_1st_choice);
            _board[_2nd_choice]->set_coords(_2nd_choice);
    }

    //move the piece:
    if(is_move_correct() && !is_pawn_promotion())
        _board[_1st_choice]->move(_2nd_choice);
}

void Chess_game::switch_fields()
{
    //castling:
    if(is_castling())
    {
        castling();
        _is_moved = true;
        return;
    }

    //moving a piece to a new place
    if(_board[_2nd_choice]->get_coords()==_board[_1st_choice]->get_coords())
    {
        unique_ptr<Piece> temp = make_unique<Empty>();

        swap(_board[_1st_choice], _board[_2nd_choice]);
            _board[_1st_choice]->set_coords(_1st_choice);
            _board[_2nd_choice]->set_coords(_2nd_choice);

        if( !_board[_1st_choice]->is_empty())
        {
            swap(temp, _board[_1st_choice]);
                _board[_1st_choice]->set_coords(_1st_choice);
        }        

        check_possible_moves();//reload moves to check if king is being attacked

        //if there is check then return to previous configuration of the board
        if(is_check())
        {
            swap(_board[_1st_choice], _board[_2nd_choice]);
                _board[_1st_choice]->set_coords(_1st_choice);
                _board[_2nd_choice]->set_coords(_2nd_choice);

            if(!temp->is_empty())
            {
                swap(temp, _board[_2nd_choice]);
                    _board[_2nd_choice]->set_coords(_2nd_choice);
            }
            _is_moved = false;

        }
        else
        {
            swap(_prev_piece, temp);
            _is_moved = true;
        }
    }
    else
    {
        _is_moved = false;
    }
}

void Chess_game::switch_color()
{
    if(_current_color==Piece_color::BLACK)
    {
        _current_color=Piece_color::WHITE;
    }
    else
    {
        _current_color=Piece_color::BLACK;
    }
}

bool Chess_game::is_move_correct()
{
    //check if any piece except knight is not being blocked by other piece:
    if(_board[_1st_choice]->get_type()!=Piece_type::KNIGHT)
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
        if(_board[_1st_choice]->get_type()==Piece_type::PAWN)
        {
            return is_pawn_move_correct();
        }

        return true;
    }

    return true;
}

bool Chess_game::check_vert()
{
    int x1 = _1st_choice.x;
    int y1 = _1st_choice.y;
    int x2 = _2nd_choice.x;
    int y2 = _2nd_choice.y;

    //check downwards:
    for(int i=y1+1; i<y2; i++)
    {
        if( !_board[{x1,i}]->is_empty() && x1==x2)
            return true;
    }

    //check upwards:
    for(int i=y1-1; i>y2; i--)
    {
        if( !_board[{x1,i}]->is_empty() && x1==x2)
            return true;
    }

    return false;
}

bool Chess_game::check_hor()
{
    int x1 = _1st_choice.x;
    int y1 = _1st_choice.y;
    int x2 = _2nd_choice.x;
    int y2 = _2nd_choice.y;

    //from left to right:
    for(int i=x1+1; i<x2; i++)
    {
        if( !_board[{i,y1}]->is_empty() && y1==y2)
            return true;
    }

    //from right to left:
    for(int i=x1-1; i>x2; i--)
    {
        if( !_board[{i,y1}]->is_empty() && y1==y2)
            return true;
    }

    return false;
}

bool Chess_game::check_diag()
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
        if( !_board[{i,j}]->is_empty())
            return true;
    }
    }

    //to up-left:
    {
    int j=y1-1;
    for(int i=x1-1; i>x2 && j>y2; i--,j--)
    {
        if( !_board[{i,j}]->is_empty())
            return true;
    }
    }

    //to bottom-left:
    {
    int j=y1+1;
    for(int i=x1-1; i>x2 && j<y2; i--,j++)
    {
        if( !_board[{i,j}]->is_empty())
            return true;
    }
    }

    //to up-right:
    {
    int j=y1-1;
    for(int i=x1+1; i<x2 && j>y2; i++,j--)
    {
        if( !_board[{i,j}]->is_empty())
            return true;
    }
    }

    return false;
}

bool Chess_game::is_pawn_move_correct()
{
    int x1 = _1st_choice.x;
    int y1 = _1st_choice.y;
    int x2 = _2nd_choice.x;
    int y2 = _2nd_choice.y;

    if(_board[_1st_choice]->get_color()==Piece_color::WHITE)
    {
        if(y2==y1+1 && (x2==x1+1 || x2==x1-1) && _board[_2nd_choice]->is_empty() )
            return false;
        else if(y2==y1+1 && x2==x1 && !_board[_2nd_choice]->is_empty() )
            return false;
        else if(y1==1 && y2==3 && x2==x1 && !_board[_2nd_choice]->is_empty() )
            return false;
    }
    else if(_board[_1st_choice]->get_color()==Piece_color::BLACK)
    {
        if(y2==y1-1 && (x2==x1+1 || x2==x1-1) && _board[_2nd_choice]->is_empty() )
            return false;
        else if(y2==y1-1 && x2==x1 && !_board[_2nd_choice]->is_empty() )
            return false;
        else if(y1==6 && y2==4 && x2==x1 && !_board[_2nd_choice]->is_empty() )
            return false;
    }

    return true;
}

bool Chess_game::is_castling()
{
    int x1 = _1st_choice.x;
    int y1 = _1st_choice.y;
    int x2 = _2nd_choice.x;
    int y2 = _2nd_choice.y;

    if(_board[_1st_choice]->get_type()==Piece_type::KING && _board[_1st_choice]->get_status()==false && x1==COL_E)
    {
        if(y2==y1 && x2==COL_G && _board[{COL_H,y1}]->get_type()==Piece_type::ROOK && _board[{COL_F,y1}]->is_empty()
           && _board[{COL_G,y1}]->is_empty() && _board[{COL_H,y1}]->get_status()==false)
            return true;
        else if(y2==y1 && x2==COL_C && _board[{COL_A,y1}]->get_type()==Piece_type::ROOK && _board[{COL_B,y1}]->is_empty()
             && _board[{COL_C,y1}]->is_empty() && _board[{COL_D,y1}]->is_empty()
             && _board[{COL_A,y1}]->get_status()==false)
            return true;
    }

    return false;
}

void Chess_game::castling()
{
//    int x1 = _1st_choice.x;
    int y1 = _1st_choice.y;
    int x2 = _2nd_choice.x;
//    int y2 = _second_choose.y;

    if(x2==COL_G)
    {
        swap(_board[_1st_choice], _board[{COL_G,y1}]);
            _board[_1st_choice]->set_coords(_1st_choice);
            _board[{COL_G,y1}]->set_coords({COL_G,y1});
        swap(_board[{COL_H,y1}], _board[{COL_F,y1}]);
            _board[{COL_H,y1}]->set_coords({COL_H,y1});
            _board[{COL_F,y1}]->set_coords({COL_F,y1});
    }
    else if(x2==COL_C)
    {
        swap(_board[_1st_choice], _board[{COL_C,y1}]);
            _board[_1st_choice]->set_coords(_1st_choice);
            _board[{COL_C,y1}]->set_coords({COL_C,y1});
        swap(_board[{COL_A,y1}], _board[{COL_D,y1}]);
            _board[{COL_A,y1}]->set_coords({COL_A,y1});
            _board[{COL_D,y1}]->set_coords({COL_D,y1});
    }

}

void Chess_game::check_possible_moves()
{
    for(auto &x: _board)
    {
        if( !x->is_empty() )
        {
            vector<Piece_coords> moves;

            Piece_color temp_c = _current_color;
            if(x->get_color()!=_current_color)
                switch_color();

            Piece_coords temp_f = _1st_choice;
            Piece_coords temp_s = _2nd_choice;

            _1st_choice = x->get_coords();

            for(const auto &n : _board)
            {
                _2nd_choice = n->get_coords();

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



void Chess_game::show_possible_moves()
{
    vector<Piece_coords> v = _board[_1st_choice]->get_moves();

    for(Piece_coords n : v)
    {
        gotoxy(n.x*2+1,n.y+1);
        cout << "O";
    }
}

bool Chess_game::is_check()
{
    Piece_coords kings_coords{0,0};

    for(const auto &x: _board)
    {
        if(x->get_type()==Piece_type::KING && x->get_color()==_current_color)
             kings_coords = x->get_coords();
    }

    for(const auto &x: _board)
    {
        if(x->get_color()!=_current_color)
        {
            vector<Piece_coords> moves = x->get_moves();

            auto p = find(begin(moves),end(moves),kings_coords);
            if(p != std::end(moves))
                return true;
        }
    }

    return false;
}

bool Chess_game::is_mate()
{
    //try to move each piece of current color, if there are no possible moves then return true

    if(is_check())
    {
        for(const auto &x: _board)
        {
            if( !x->is_empty() && x->get_color()==_current_color)
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

void Chess_game::undo_turn()
{
    swap(_board[_1st_choice], _board[_2nd_choice]);
        _board[_1st_choice]->set_coords(_1st_choice);
        _board[_2nd_choice]->set_coords(_2nd_choice);

    if( !_prev_piece->is_empty())
    {
        swap(_prev_piece, _board[_2nd_choice]);
            _board[_2nd_choice]->set_coords(_2nd_choice);
    }
}

void Chess_game::pawn_promotion()
{
    if(is_pawn_promotion())
    {
        switch(Control::exchange_piece())
        {
        case Piece_type::QUEEN:
            _board[_1st_choice] = make_unique<Queen>(_current_color);
                _board[_1st_choice]->set_coords(_1st_choice);
            break;
        case Piece_type::BISHOP:
            _board[_1st_choice] = make_unique<Bishop>(_current_color);
                _board[_1st_choice]->set_coords(_1st_choice);
            break;
        case Piece_type::KNIGHT:
            _board[_1st_choice] = make_unique<Knight>(_current_color);
                _board[_1st_choice]->set_coords(_1st_choice);
            break;
        case Piece_type::ROOK:
            _board[_1st_choice] = make_unique<Rook>(_current_color);
                _board[_1st_choice]->set_coords(_1st_choice);
            break;
        default:
            break;
        }
    }

}

bool Chess_game::is_pawn_promotion()
{
    if(_board[_1st_choice]->get_type()==Piece_type::PAWN)
    {
        if( (_board[_1st_choice]->get_color()==Piece_color::BLACK && _2nd_choice.y==LINE_0) ||
            (_board[_1st_choice]->get_color()==Piece_color::WHITE && _2nd_choice.y==LINE_7) )
        {
            return true;
        }
    }

    return false;
}
