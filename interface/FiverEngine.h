#ifndef _FIVERENGINE_H_
#define _FIVERENGINE_H_

#include "..\\engine\GoBang.h"
#include <iostream>
#include <sstream>
#include <curses.h>
//#include <io.h>


#define ORIGIN_X 40
#define ORIGIN_Y 12

using namespace std;

class FiverEngine{
private:
    GoBang goBang;
    static const int BOARD_X=22;
    static const int BOARD_Y=3;
    int cursor_i;
    int cursor_j;
    ostringstream os;
public:
    FiverEngine():goBang(true), cursor_i(8),cursor_j(8){};

    int begin()
    {

        int ch;

        initscr();
        clear();
        noecho();
        cbreak();	/* Line buffering disabled. pass on everything */

        keypad(stdscr, TRUE);
        mvprintw(0,0, "OOXX alpha 0.1");
        refresh();



//        pair< int, vector<Action> > state;
        int winner=0;
        Action best_a;
//        goBang.move(8, 8);
        while(1)
        {	drawBoard();
            drawCursorIndicator();

            switch(ch = getch())
            {	case KEY_UP:
                    if (++cursor_i>goBang.board.size())
                        --cursor_i;
                    break;
                case KEY_DOWN:
                    if (--cursor_i<1)
                        ++cursor_i;
                    break;
                case KEY_LEFT:
                    if (--cursor_j<1)
                        ++cursor_j;
                    break;
                case KEY_RIGHT:
                    if (++cursor_j>goBang.board.size())
                        --cursor_j;
                    break;
                case 'x':
                    winner=goBang.move(BLACK_STONE, cursor_i, cursor_j, true);
//                        goBang.move(BLACK_STONE, cursor_i, cursor_j, true);
                    break;
                case 'X':
                    winner=goBang.move(BLACK_STONE, cursor_i, cursor_j, true);
//                        goBang.move(BLACK_STONE, cursor_i, cursor_j, true);
                    break;
                case 'o':
                    winner=goBang.move(WHITE_STONE, cursor_i, cursor_j, true);
//                        goBang.move(WHITE_STONE, cursor_i, cursor_j, true);
                    break;
                case 'O':
                    winner=goBang.move(WHITE_STONE, cursor_i, cursor_j, true);
//                        goBang.move(WHITE_STONE, cursor_i, cursor_j, true);

                    break;
                case 10:
                    winner=goBang.move(cursor_i, cursor_j);
//                        goBang.move(cursor_i, cursor_j);
                    break;
                case 8:
                    {
                        winner=goBang.undo();
//                        goBang.undo();
                        if (goBang.last_move().first!=0)
                        {
                            cursor_i=goBang.last_move().first;
                            cursor_j=goBang.last_move().second;
                        }
                        else
                        {
                            cursor_i=8;
                            cursor_j=8;
                        }

                    }

                    break;
                case ',':
                    {
                        winner=goBang.undo();
//                        goBang.undo();
                        if (goBang.last_move().first!=0)
                        {
                            cursor_i=goBang.last_move().first;
                            cursor_j=goBang.last_move().second;
                        }
                        else
                        {
                            cursor_i=8;
                            cursor_j=8;
                        }

                    }

                    break;
                case '<':
                    {
                        winner=goBang.undo();
//                        goBang.undo();
                        if (goBang.last_move().first!=0)
                        {
                            cursor_i=goBang.last_move().first;
                            cursor_j=goBang.last_move().second;
                        }
                        else
                        {
                            cursor_i=8;
                            cursor_j=8;
                        }

                    }

                    break;
                case '.':
                    {
                        winner=goBang.redo();
//                        goBang.redo();
                        if (goBang.last_move().first!=0)
                        {
                            cursor_i=goBang.last_move().first;
                            cursor_j=goBang.last_move().second;
                        }
                        else
                        {
                            cursor_i=8;
                            cursor_j=8;
                        }

                    }
                    break;
                case '>':
                    {
                        winner=goBang.redo();
//                        cout<<winner<<endl;
//                        goBang.redo();
                        if (goBang.last_move().first!=0)
                        {
                            cursor_i=goBang.last_move().first;
                            cursor_j=goBang.last_move().second;
                        }
                        else
                        {
                            cursor_i=8;
                            cursor_j=8;
                        }

                    }
                    break;
                case ' ':
                    {
                        winner=goBang.move(cursor_i, cursor_j);
//                          goBang.move(cursor_i, cursor_j);

//                        mvprintw(24, 6, "Size:%d", goBang.getMovePointSet().size());

//                        mvprintw(24, 0, "%d:%d", best_a.first, best_a.second);
                    }

                    break;
                case 'R':
                    {
                        goBang.reset();
                        winner=0;
                    }
                    break;
                case 'r':
                    {
                        goBang.reset();
                        winner=0;
                    }
                    break;
                case 'A':goBang.board.rotate();
                    break;
                case 'a':goBang.board.rotate();
                    break;
                default:
                    //mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", ch, ch);
                    refresh();
                    break;
            }

//            drawState(state);
            drawCount(goBang.move_count);

//            int winner=goBang.winner;
            switch(winner)
            {
                case EMPTY_POINT: mvprintw(1, 32, "           ");
                break;
                case WHITE_STONE:mvprintw(1, 32, "White Wins!");
                break;
                case BLACK_STONE: mvprintw(1, 32, "Black Wins!");
                break;
                case TIE: mvprintw(1, 32, "Ties.      ");
                break;
                default:mvprintw(1, 32, "           ");
                break;
            }

//            erase();  //解决会字符会缺的bug.. 但是会闪屏
//            refresh();
        }

        clrtoeol();
        refresh();
        endwin();
        return 0;
    };

    void drawBoard()
    {
        int board_y=BOARD_Y;
        int board_x=BOARD_X;
        mvprintw(board_y, board_x, "   ");

        char gap_mark=' ';

        int last_i=goBang.last_move().first;
        int last_j=goBang.last_move().second;

        for(int j=0;j!=goBang.board.size();j++)
        {
            addch(char('A'+j));addch(' ');
        }

        board_y++;
        for(int i=goBang.board.size();i!=0;--i)
        {

            if (i==cursor_i&&cursor_j==1)
                  gap_mark='[';
            else
                gap_mark=' ';
            os<<setw(2)<<i<<gap_mark;
            mvprintw(board_y, board_x, os.str().c_str());
            os.str("");

            for(int j=1;j!=goBang.board.size()+1;j++)
            {

                if ((((i==4||i==(goBang.board.size()-3))&&(j==4||j==(goBang.board.size()-3)))
                     ||(i==(goBang.board.size()+1)/2&&j==(goBang.board.size()+1)/2))&&((goBang.board.isEmpty(i, j))))
                         os<<STAR_MARK;
                else
                    os<<goBang.board.mark(i, j);

                if ((i==last_i)&&(j==last_j))
                {

                    if (last_j==1)
                    {
                        attron(A_REVERSE);
                        addstr(os.str().c_str());
                    }
                    else
                    {
                        if (i==cursor_i&&j==cursor_j)
                          gap_mark='[';
                        else if ((i==cursor_i)&&(j==cursor_j+1))
                          gap_mark=']';
                        else
                          gap_mark=' ';
                        addch(gap_mark);
                        attron(A_REVERSE);
                        addch(os.str().at(1));
                    }
                    attroff(A_REVERSE);

                }
                else
                    addstr(os.str().c_str());
                os.str("");


                if (i==cursor_i&&j==(cursor_j-1))
                  gap_mark='[';
                else if ((i==cursor_i)&&(j==cursor_j))
                  gap_mark=']';
                else
                  gap_mark=' ';


                os<<gap_mark;
//                addstr(os.str().c_str());
//                os.str("");

            }

              //no last move marker
//            for(int j=1;j!=goBang.board.size()+1;j++)
//            {
//
//
//                if ((((i==4||i==(goBang.board.size()-3))&&(j==4||j==(goBang.board.size()-3)))
//                     ||(i==(goBang.board.size()+1)/2&&j==(goBang.board.size()+1)/2))&&((goBang.board[i][j]).isEmpty()))
//                     os<<STAR_MARK;
//                else
//                {
//                    os<<goBang.board[i][j];
//
//                }
//
//
//                if (i==cursor_i&&j==(cursor_j-1))
//                  gap_mark='[';
//                else if ((i==cursor_i)&&(j==cursor_j))
//                  gap_mark=']';
//                else
//                  gap_mark=' ';
//
//
//                os<<gap_mark;
//
//            }

            os<<i;
            addstr(os.str().c_str());
            os.str("");
            board_y++;
        }

        os<<"   ";
        for(int j=0;j!=goBang.board.size();j++)
            os<<char('A'+j)<<' ';
        mvprintw(board_y, board_x, os.str().c_str());
        os.str("");
        refresh();
    };

    void drawCursorIndicator()
    {
        os<<char(cursor_j-1+'A')<<cursor_i<<' ';
        mvprintw(20, 60, os.str().c_str());
        os.str("");
        refresh();
    }

    void drawState(pair< int, vector<Action> > &s)
    {
        int code=s.first;
        int piece=goBang.last_piece();

        string player_name;
        string state_description;
        switch(piece)
        {
            case 0:
//                return;
                player_name="     ";
            break;
            case 1:
                player_name="Black";
            break;
            case -1:
                player_name="White";
            break;
            default:
                player_name="     ";
            break;
        }
        switch(code)
       {
            case 0:
                {
                    state_description="                        ";
                    player_name="     ";
                }

            break;
            case OPEN_ONE:
                state_description=" has Open One.          ";
            break;
            case SLEEP_ONE:
                state_description=" has Sleep One.         ";
            break;
            case OPEN_TWO:
                state_description=" has Open Two.          ";
            break;
            case SLEEP_TWO:
                state_description=" has Sleep Two.         ";
            break;
            case OPEN_THREE:
                state_description=" has Open Three.        ";
            break;
            case SLEEP_THREE:
                state_description=" has Sleep Three.       ";
            break;
            case SLEEP_FOUR:
                state_description=" has Sleep Four.        ";
            break;
            case OPEN_FOUR:
                state_description=" has Open Four.         ";
            break;
            case FIVE:
                state_description=" is Five.               ";
            break;
            case DOUBLE_THREE:
                state_description=" has Double Three.      ";
            break;
            case FOUR_THREE:
                state_description=" has Four-Three         ";
            break;
            case DOUBLE_FOUR:
                state_description=" has Double Four        ";
            break;
            case F_DOUBLE_THREE:
                state_description=" Forbbiden Double Three!";
            break;
            case F_DOUBLE_FOUR:
                state_description=" Forbbiden Double Four! ";
            break;
            case OVERLINE:
                state_description=" Forbbiden Overline!    ";
            break;
            default:
                //state_description="                        ";
            break;
        }
        mvprintw(21, 25, (player_name+state_description).c_str());
    }

    void drawCount(int count)
    {
        os<<count<<"  ";
        mvprintw(20, 20, os.str().c_str());
        os.str("");
    }

};


#endif
