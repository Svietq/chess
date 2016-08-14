#include "chess_board.h"

Chess_board::Chess_board()
{

}

void Chess_board::play()
{
    set_figures();

    while(!is_mate())
    {
        print_board(_board);
        check_possible_moves();
        make_move();

        switch_fields();
        if(_is_moved)
            switch_color();
    }
    print_board(_board);
}

bool Chess_board::is_1st_choose_correct()
{
    int x = _first_choose.x;
    int y = _first_choose.y;

    if(_board[y][x]->get_type()==Piece_type::EMPTY)
        return false;

    if(_board[y][x]->get_color()!=_current_color)
        return false;

    return true;
}

bool Chess_board::is_2nd_choose_correct()
{
    int x = _second_choose.x;
    int y = _second_choose.y;

    if(_board[y][x]->get_type()!=Piece_type::EMPTY && _board[y][x]->get_color()==_current_color)
    {
        return false;
    }
    else if(_board[y][x]->get_type()==Piece_type::EMPTY)
    {
        return true;
    }

    return true;
}

void Chess_board::set_figures()
{
    //set empty fields:
    {
    int i=0;
    int j=0;
    for(array<unique_ptr<Piece>, 8> &v : _board)
    {
       j=0;
       for(unique_ptr<Piece> &x: v)
       {
           x = make_unique<Empty>();
           x->set_coords({j,i});
           j++;
       }
       i++;
    }
    }

    //set pawns:
    for(int i=0; i<8; i++)
    {
        _board[1][i] = make_unique<Pawn>(Piece_color::WHITE);
        _board[1][i]->set_coords({i,1});
        _board[6][i] = make_unique<Pawn>(Piece_color::BLACK);
        _board[6][i]->set_coords({i,6});
    }

    //set the rest of pieces:
    _board[0][0] = make_unique<Rook>(Piece_color::WHITE);
    _board[0][0]->set_coords({0,0});
    _board[0][7] = make_unique<Rook>(Piece_color::WHITE);
    _board[0][7]->set_coords({7,0});
    _board[7][0] = make_unique<Rook>(Piece_color::BLACK);
    _board[7][0]->set_coords({0,7});
    _board[7][7] = make_unique<Rook>(Piece_color::BLACK);
    _board[7][7]->set_coords({7,7});
    _board[0][1] = make_unique<Knight>(Piece_color::WHITE);
    _board[0][1]->set_coords({1,0});
    _board[0][6] = make_unique<Knight>(Piece_color::WHITE);
    _board[0][6]->set_coords({6,0});
    _board[7][1] = make_unique<Knight>(Piece_color::BLACK);
    _board[7][1]->set_coords({1,7});
    _board[7][6] = make_unique<Knight>(Piece_color::BLACK);
    _board[7][6]->set_coords({6,7});
    _board[0][2] = make_unique<Bishop>(Piece_color::WHITE);
    _board[0][2]->set_coords({2,0});
    _board[0][5] = make_unique<Bishop>(Piece_color::WHITE);
    _board[0][5]->set_coords({5,0});
    _board[7][2] = make_unique<Bishop>(Piece_color::BLACK);
    _board[7][2]->set_coords({2,7});
    _board[7][5] = make_unique<Bishop>(Piece_color::BLACK);
    _board[7][5]->set_coords({5,7});
    _board[0][3] = make_unique<Queen>(Piece_color::WHITE);
    _board[0][3]->set_coords({3,0});
    _board[7][3] = make_unique<Queen>(Piece_color::BLACK);
    _board[7][3]->set_coords({3,7});
    _board[0][4] = make_unique<King>(Piece_color::WHITE);
    _board[0][4]->set_coords({4,0});
    _board[7][4] = make_unique<King>(Piece_color::BLACK);
    _board[7][4]->set_coords({4,7});

}

void Chess_board::make_move()
{
    //choose first field:
    do
    {
        _first_choose = choose_piece();
    }
    while(!is_1st_choose_correct() || _board[_first_choose.y][_first_choose.x]->get_moves().size()==0);

    show_possible_moves();

    //choose second field:
    do
    {
        _second_choose = choose_field();
    }
    while(!is_2nd_choose_correct());

    int x1 = _first_choose.x;
    int y1 = _first_choose.y;
    int x2 = _second_choose.x;
    int y2 = _second_choose.y;

    //pawn promotion:
    if(is_pawn_promotion() && _board[y2][x2]->get_type()==Piece_type::EMPTY)
    {
        pawn_promotion();
        swap(_board[y1][x1], _board[y2][x2]);
        _board[y1][x1]->set_coords({x1,x2});
        _board[y2][x2]->set_coords({x2,y2});
    }

    //move the piece:
    if(is_move_correct() && !is_pawn_promotion())
        _board[y1][x1]->move(_second_choose);
}

void Chess_board::switch_fields()
{
    //castling:
    if(is_castling())
    {
        castling();
        _is_moved = true;
        return;
    }

    int x1 = _first_choose.x;
    int y1 = _first_choose.y;
    int x2 = _second_choose.x;
    int y2 = _second_choose.y;

    if(_board[y2][x2]->get_coords()==_board[y1][x1]->get_coords())
    {
        unique_ptr<Piece> temp=make_unique<Empty>();

        swap(_board[y1][x1], _board[y2][x2]);
        _board[y1][x1]->set_coords({x1,y1});
        _board[y2][x2]->set_coords({x2,y2});

        if(_board[y2][x2]->get_type()!=Piece_type::EMPTY)
        {
            swap(temp, _board[y1][x1]);
            _board[y1][x1]->set_coords({x1,y1});
        }        

        check_possible_moves();//reload moves to check if king is being attacked

        //if is check then return to previous configuration of board
        if(is_check())
        {
            swap(_board[y1][x1], _board[y2][x2]);
            _board[y1][x1]->set_coords({x1,y1});
            _board[y2][x2]->set_coords({x2,y2});

            if(temp->get_type()!=Piece_type::EMPTY)
            {
                swap(temp, _board[y2][x2]);
                _board[y2][x2]->set_coords({x2,y2});
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

void Chess_board::switch_color()
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

bool Chess_board::is_move_correct()
{
    int x1 = _first_choose.x;
    int y1 = _first_choose.y;
    int x2 = _second_choose.x;
    int y2 = _second_choose.y;

    //check if any piece except knight is not being blocked by other piece:
    if(_board[y1][x1]->get_type()!=Piece_type::KNIGHT)
    {
        if(x1==x2 || y1==y2)
        {
            for(int i=y1+1; i<y2; i++)
            {
                if(_board[i][x1]->get_type()!=Piece_type::EMPTY && x1==x2)
                    return false;
            }

            for(int i=y1-1; i>y2; i--)
            {
                if(_board[i][x1]->get_type()!=Piece_type::EMPTY && x1==x2)
                    return false;
            }

            for(int i=x1+1; i<x2; i++)
            {
                if(_board[y1][i]->get_type()!=Piece_type::EMPTY && y1==y2)
                    return false;
            }

            for(int i=x1-1; i>x2; i--)
            {
                if(_board[y1][i]->get_type()!=Piece_type::EMPTY && y1==y2)
                    return false;
            }
        }
        else
        {
            {
            int j=y1+1;
            for(int i=x1+1; i<x2 && j<y2; i++,j++)
            {
                if(_board[j][i]->get_type()!=Piece_type::EMPTY)
                    return false;
            }
            }

            {
            int j=y1-1;
            for(int i=x1-1; i>x2 && j>y2; i--,j--)
            {
                if(_board[j][i]->get_type()!=Piece_type::EMPTY)
                    return false;
            }
            }

            {
            int j=y1+1;
            for(int i=x1-1; i>x2 && j<y2; i--,j++)
            {
                if(_board[j][i]->get_type()!=Piece_type::EMPTY)
                    return false;
            }
            }

            {
            int j=y1-1;
            for(int i=x1+1; i<x2 && j>y2; i++,j--)
            {
                if(_board[j][i]->get_type()!=Piece_type::EMPTY)
                    return false;
            }
            }
        }

        //check movement for a pawn:
        if(_board[y1][x1]->get_type()==Piece_type::PAWN)
        {
            return is_pawn_move_correct();
        }

    }

    return true;
}

bool Chess_board::is_pawn_move_correct()
{
    int x1 = _first_choose.x;
    int y1 = _first_choose.y;
    int x2 = _second_choose.x;
    int y2 = _second_choose.y;

    if(_board[y1][x1]->get_color()==Piece_color::WHITE)
    {
        if(y2==y1+1 && (x2==x1+1 || x2==x1-1) && _board[y2][x2]->get_type()==Piece_type::EMPTY )
            return false;
        else if(y2==y1+1 && x2==x1 && _board[y2][x2]->get_type()!=Piece_type::EMPTY )
            return false;
        else if(y1==1 && y2==3 && x2==x1 && _board[y2][x2]->get_type()!=Piece_type::EMPTY )
            return false;
    }
    else
    {
        if(y2==y1-1 && (x2==x1+1 || x2==x1-1) && _board[y2][x2]->get_type()==Piece_type::EMPTY )
            return false;
        else if(y2==y1-1 && x2==x1 && _board[y2][x2]->get_type()!=Piece_type::EMPTY )
            return false;
        else if(y1==6 && y2==4 && x2==x1 && _board[y2][x2]->get_type()!=Piece_type::EMPTY )
            return false;
    }

    return true;
}

bool Chess_board::is_castling()
{
    int x1 = _first_choose.x;
    int y1 = _first_choose.y;
    int x2 = _second_choose.x;
    int y2 = _second_choose.y;

    if(_board[y1][x1]->get_type()==Piece_type::KING && _board[y1][x1]->get_status()==false && x1>3 && x1<5)
    {
        if(y2==y1 && x2==x1+2 && _board[y1][x1+3]->get_type()==Piece_type::ROOK && _board[y1][x1+1]->get_type()==Piece_type::EMPTY
           && _board[y1][x1+2]->get_type()==Piece_type::EMPTY && _board[y1][x1+3]->get_status()==false)
            return true;
        else if(y2==y1 && x2==x1-2 && _board[y1][x1-4]->get_type()==Piece_type::ROOK && _board[y1][x1-3]->get_type()==Piece_type::EMPTY
             && _board[y1][x1-2]->get_type()==Piece_type::EMPTY && _board[y1][x1-1]->get_type()==Piece_type::EMPTY
             && _board[y1][x1-4]->get_status()==false)
            return true;
    }

    return false;
}

void Chess_board::castling()
{
    int x1 = _first_choose.x;
    int y1 = _first_choose.y;
    int x2 = _second_choose.x;
//    int y2 = _second_choose.y;

    if(x2==6)
    {
        swap(_board[y1][x1], _board[y1][x1+2]);
            _board[y1][x1]->set_coords({x1,y1});
            _board[y1][x1+2]->set_coords({x1+2,y1});
        swap(_board[y1][x1+3], _board[y1][x1+1]);
            _board[y1][x1+3]->set_coords({x1+3,y1});
            _board[y1][x1+1]->set_coords({x1+1,y1});
    }
    else if(x2==2)
    {
        swap(_board[y1][x1], _board[y1][x1-2]);
            _board[y1][x1]->set_coords({x1,y1});
            _board[y1][x1-2]->set_coords({x1-2,y1});
        swap(_board[y1][x1-4], _board[y1][x1-1]);
            _board[y1][x1-4]->set_coords({x1-4,y1});
            _board[y1][x1-1]->set_coords({x1-1,y1});
    }

}

void Chess_board::check_possible_moves()
{
    for(const array<unique_ptr<Piece>, 8> &v : _board)
    {
       for(const unique_ptr<Piece> &x: v)
       {
           if(x->get_type()!=Piece_type::EMPTY)
           {
               vector<Piece_coords> moves;

               Piece_color temp_c = _current_color;
               if(x->get_color()==Piece_color::BLACK)
               {
                    _current_color = Piece_color::BLACK;
               }
               else
               {
                   _current_color = Piece_color::WHITE;
               }

               Piece_coords temp_f = _first_choose;
               Piece_coords temp_s = _second_choose;

               _first_choose = x->get_coords();
               for(int i=0; i<8; i++)
               {
                   _second_choose.y = i;
                   for(int j=0; j<8; j++)
                   {
                        _second_choose.x = j;
                        if(is_2nd_choose_correct() && x->is_move_correct(_second_choose) && is_move_correct() )
                        {
                            moves.push_back(_second_choose);
                        }
                        else if(is_castling() && !is_check())
                        {
                            moves.push_back(_second_choose);
                        }
                   }
               }

               _current_color = temp_c;
               _first_choose = temp_f;
               _second_choose = temp_s;

               x->load_moves(moves);
           }           
       }
    }
}

void Chess_board::show_possible_moves()
{
    vector<Piece_coords> v = _board[_first_choose.y][_first_choose.x]->get_moves();

    for(Piece_coords n : v)
    {
        gotoxy(n.x*2+1,n.y+1);
        cout << "O";
    }
}

bool Chess_board::is_check()
{
    Piece_coords kings_coords{0,0};

    for(const array<unique_ptr<Piece>, 8> &v : _board)
    {
       for(const unique_ptr<Piece> &x: v)
       {
           if(x->get_type()==Piece_type::KING && x->get_color()==_current_color)
                kings_coords = x->get_coords();
       }
    }

    for(const array<unique_ptr<Piece>, 8> &v : _board)
    {
       for(const unique_ptr<Piece> &x: v)
       {
           if(x->get_color()!=_current_color)
           {
               vector<Piece_coords> moves = x->get_moves();

               for(Piece_coords n : moves)
               {
                    if(n==kings_coords)
                        return true;
               }
           }
       }
    }

    return false;
}

bool Chess_board::is_mate()
{
    if(is_check())
    {
        for(const array<unique_ptr<Piece>, 8> &v : _board)
        {
           for(const unique_ptr<Piece> &x: v)
           {
               if(x->get_type()!=Piece_type::EMPTY && x->get_color()==_current_color)
               {
                   vector<Piece_coords> moves = x->get_moves();

                   for(Piece_coords n : moves)
                   {
                        _first_choose = x->get_coords();
                        x->move(n);
                        _second_choose = x->get_coords();

                        switch_fields();

                        if(_is_moved)
                        {
                            undo_turn();
                            return false;
                        }
                   }
               }
           }
        }

        return true;
    }

    return false;
}

void Chess_board::undo_turn()
{
    int x2 = _first_choose.x;
    int y2 = _first_choose.y;
    int x1 = _second_choose.x;
    int y1 = _second_choose.y;

    unique_ptr<Piece> temp=make_unique<Empty>();

    swap(temp, _prev_piece);

    swap(_board[y1][x1], _board[y2][x2]);
    _board[y1][x1]->set_coords({x1,y1});
    _board[y2][x2]->set_coords({x2,y2});

    if(_board[y2][x2]->get_type()!=Piece_type::EMPTY)
    {
        swap(temp, _board[y1][x1]);
        _board[y1][x1]->set_coords({x1,y1});
    }
}



void Chess_board::pawn_promotion()
{
    int x1 = _first_choose.x;
    int y1 = _first_choose.y;
//    int x2 = _second_choose.x;
//    int y2 = _second_choose.y;

    if(is_pawn_promotion())
    {
        switch(Control::exchange_piece())
        {
        case Piece_type::QUEEN:
            _board[y1][x1] = make_unique<Queen>(_current_color);
            _board[y1][x1]->set_coords({x1,y1});
            break;
        case Piece_type::BISHOP:
            _board[y1][x1] = make_unique<Bishop>(_current_color);
            _board[y1][x1]->set_coords({x1,y1});
            break;
        case Piece_type::KNIGHT:
            _board[y1][x1] = make_unique<Knight>(_current_color);
            _board[y1][x1]->set_coords({x1,y1});
            break;
        case Piece_type::ROOK:
            _board[y1][x1] = make_unique<Rook>(_current_color);
            _board[y1][x1]->set_coords({x1,y1});
            break;
        default:
            break;
        }
    }

}

bool Chess_board::is_pawn_promotion()
{
    int x1 = _first_choose.x;
    int y1 = _first_choose.y;
//    int x2 = _second_choose.x;
    int y2 = _second_choose.y;

    if(_board[y1][x1]->get_type()==Piece_type::PAWN)
    {
        if( (_board[y1][x1]->get_color()==Piece_color::BLACK && y2==0) || (_board[y1][x1]->get_color()==Piece_color::WHITE && y2==7) )
        {
            return true;
        }
    }

    return false;

}
